//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 27 April 2008 7:58:58 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "texture.h"
#include "dx9/render_interface.h"
#include "dx9/d3d_utils.h"
#include "image/types.h"
#include "image/image.h"
#include "image/canvas.h"
#include "image/image_rgba.h"
#include "image/image_rgb24.h"
#include "image/image_rgba32.h"
#include "core/memory.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_DX9_ABI, texture)

	namespace detail
	{
		
		struct scoped_lock_rect
		{
			scoped_lock_rect(int mip_, d3d_texture_ptr tex_, RECT* lock_rect = 0) :
				m_mip(mip_),
				m_tex(tex_)
			{
				if(SUCCEEDED(m_tex->LockRect(m_mip, &m_rect, lock_rect, 0)))
					m_ok = true;
				else 
					m_ok = false;				
			}
			
			~scoped_lock_rect()
			{
				if(m_ok)
					m_tex->UnlockRect(m_mip);
			}
			
			operator bool()
			{
				return m_ok;
			}
			
			const D3DLOCKED_RECT& get_rect() const 
				{ return m_rect; }
				
		private:
			D3DLOCKED_RECT m_rect;
			d3d_texture_ptr m_tex;
			int m_mip;
			bool m_ok;
		};
	
		/// get the image::pixel_layout for the passed d3d format
		image::image_rgba::pixel_layout* d3d_to_pixel_layout(D3DFORMAT fmt)
		{
			switch(fmt)
			{
				case D3DFMT_A8R8G8B8	: return &image::image_rgba::pixel_layout_bgra8888;
				case D3DFMT_X8R8G8B8	: return &image::image_rgba::pixel_layout_bgrx8888;
				case D3DFMT_X1R5G5B5	: return &image::image_rgba::pixel_layout_bgrx5551;
				case D3DFMT_R5G6B5		: return &image::image_rgba::pixel_layout_bgr565;
				case D3DFMT_A8			: return &image::image_rgba::pixel_layout_a8;
			};
			return 0;
		}
	
	} // end namespace
	
	using namespace tycho::image;

	/// constructor
	texture::texture(dx9::render_interface* ri) :
		m_ri(ri),
		m_width(0),
		m_height(0),
		m_num_mips(0)
	{}
	
	
	texture::~texture()
	{
		
	}
	
	/// create a blank texture
	bool texture::rt_create(graphics::format format, int width, int height, int num_mips, int create_flags)
	{
		TYCHO_ASSERT(create_flags != create_invalid);
		
		D3DFORMAT d3d_fmt = tycho_to_d3d(format);
		if(d3d_fmt == D3DFMT_UNKNOWN)
			return false;
			
		D3DPOOL d3d_pool = D3DPOOL_MANAGED;
		int     d3d_usage = 0;
		if(create_flags & create_dynamic)
		{
			TYCHO_ASSERT(num_mips == 1);
			d3d_pool = D3DPOOL_DEFAULT;
			d3d_usage = D3DUSAGE_DYNAMIC;
			
			// create a system memory texture to update from
			if(FAILED(m_ri->get_d3d_device()->CreateTexture(width, 
															height, 
															1, 
															0,
															d3d_fmt, 
															D3DPOOL_SYSTEMMEM, 
															&m_sysmem_ptr.p, NULL)))
			{
				return false;
			}			
			m_ri->get_resource_tracker().track(m_sysmem_ptr.p);		
		}
		else if(create_flags & create_render_target)
		{
			TYCHO_ASSERT(num_mips == 1);
			d3d_pool = D3DPOOL_DEFAULT;
			d3d_usage = D3DUSAGE_RENDERTARGET;		
		}
					
		if(FAILED(m_ri->get_d3d_device()->CreateTexture(width, 
														height, 
														num_mips, 
														d3d_usage,
														d3d_fmt, 
														d3d_pool, 
														&m_d3d_ptr.p, NULL)))
		{
			return false;
		}			
		m_ri->get_resource_tracker().track(m_d3d_ptr.p);		
		m_width = width;
		m_height = height;
		m_num_mips = num_mips;
		m_img_view = image_base_ptr(new image_rgba32(*detail::d3d_to_pixel_layout(d3d_fmt)));
		return true;
	}
	
	/// create a texture from an image object
	bool texture::rt_create(tycho::image::image_base_ptr img)
	{
		using namespace tycho::image;
		
		image_format fmt = img->get_image_format();
		D3DFORMAT d3d_fmt = D3DFMT_UNKNOWN;
		image_base_ptr dst_image;
		switch(fmt)
		{
			case image_format_rgba24 : 
			case image_format_rgba32 : {
				d3d_fmt = D3DFMT_A8R8G8B8; 
				dst_image = image_base_ptr(new image_rgba32(image_rgba::pixel_layout_bgra8888)); 
			} break;
			
			default : TYCHO_NOT_IMPLEMENTED;
		}	
		if(FAILED(m_ri->get_d3d_device()->CreateTexture(img->get_width(), 
														img->get_height(), 
														img->get_num_mips(), 
														0,
														d3d_fmt, 
														D3DPOOL_MANAGED, 
														&m_d3d_ptr.p, NULL)))
		{
			return false;
		}
		m_ri->get_resource_tracker().track(m_d3d_ptr.p);		
		
		// copy the image contents to the texture surface
		for(int m = 0; m < img->get_num_mips(); ++m)
		{
			detail::scoped_lock_rect lr(m, m_d3d_ptr);
			if(!lr)
				return false;				
			D3DLOCKED_RECT rect = lr.get_rect();
			tycho::image::canvas src_c;
			if(!img->get_mip_level(m, &src_c))
				return false;

			// create a view over the d3d surface so we can copy the image pixels to it
			// with conversion if necessary
			dst_image->create_view(src_c.get_width(), src_c.get_height(), 1, (core::uint8*)rect.pBits);
			canvas dst_c;
			if(!dst_image->get_mip_level(0, &dst_c))
				return false;			
			dst_image->copy(0, 0, 0, 0, src_c.get_width(), src_c.get_height(), src_c, dst_c);
		}			
								
		m_width = img->get_width();
		m_height = img->get_height();
		m_num_mips = img->get_num_mips();
		m_img_view = dst_image;
																
		return true;
	}
	
	bool texture::rt_update(int x, int y, int width, int height, image::image_base* img)
	{
		RECT lock_rect;
		lock_rect.left = x;
		lock_rect.right = x + width;
		lock_rect.top = y;
		lock_rect.bottom = y + height;
		detail::scoped_lock_rect lr(0, m_d3d_ptr, &lock_rect);
		if(!lr)
			return false;				
		D3DLOCKED_RECT rect = lr.get_rect();
		tycho::image::canvas src_c;
		if(!img->get_mip_level(0, &src_c))
			return false;
		// create a view over the d3d surface so we can copy the image pixels to it
		// with conversion if necessary
		m_img_view->create_view(m_width, m_height, 1, (core::uint8*)rect.pBits);
		canvas dst_c;
		if(!m_img_view->get_mip_level(0, &dst_c))
			return false;			
		return m_img_view->copy(x, y, 0, 0, width, height, src_c, dst_c);		
	}
	
	d3d_texture_ptr texture::get_d3d()
	{
		return m_d3d_ptr;
	}

	//////////////////////////////////////////////////////////////////////////////////
	// Render thread functions
	//////////////////////////////////////////////////////////////////////////////////
	
	bool texture::rt_lock(int mip_level, char** out_buffer, int* out_stride)
	{
		TYCHO_ASSERT(m_sysmem_ptr);
		D3DLOCKED_RECT d3d_rect;
		if(FAILED(m_sysmem_ptr->LockRect(mip_level, &d3d_rect, 0, D3DLOCK_NO_DIRTY_UPDATE)))
			return false;
		*out_buffer = (char*)d3d_rect.pBits;
		*out_stride = d3d_rect.Pitch;
		return true;		
	}

	bool texture::rt_mark_dirty(int x, int y, int width, int height)
	{
		RECT d3d_rect;
		d3d_rect.left = x;
		d3d_rect.right = x + width;
		d3d_rect.top = y;
		d3d_rect.bottom = y + height;
		if(FAILED(m_sysmem_ptr->AddDirtyRect(&d3d_rect)))
			return false;
		return true;
	}
	
	bool texture::rt_unlock(int mip_level)
	{
		if(FAILED(m_sysmem_ptr->UnlockRect(mip_level)))
			return false;
		if(FAILED(m_ri->get_d3d_device()->UpdateTexture(m_sysmem_ptr, m_d3d_ptr)))
			return false;
		return true;
	}

} // end namespace
} // end namespace
} // end namespace
