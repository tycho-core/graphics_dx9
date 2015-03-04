//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 7 July 2008 2:26:13 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "render_target_texture_storage.h"
#include "dx9/render_interface.h"
#include "dx9/resource_tracker.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{
	
	render_target_texture_storage::render_target_texture_storage(dx9::render_interface* ri, dx9::texture_ptr tex_store) :
		m_ri(ri),
		m_tex_store(tex_store)
	{}
	

	render_target_texture_storage::~render_target_texture_storage()
	{}

	bool render_target_texture_storage::rt_create(graphics::format format, int width, int height, int num_mips, int create_flags)
	{
		return m_tex_store->rt_create(format, width, height, num_mips, create_flags);
	}
	
	bool render_target_texture_storage::rt_update(int x, int y, int width, int height, image::image_base* img)
	{
		return m_tex_store->rt_update(x, y, width, height, img);
	}
	
	bool render_target_texture_storage::rt_lock(int mip_level, char** out_buffer, int* out_stride)
	{
		return m_tex_store->rt_lock(mip_level, out_buffer, out_stride);
	}
	
	bool render_target_texture_storage::rt_unlock(int mip_level)
	{
		return m_tex_store->rt_unlock(mip_level);
	}
	
	bool render_target_texture_storage::rt_mark_dirty(int x, int y, int width, int height)
	{
		return m_tex_store->rt_mark_dirty(x, y, width, height);
	}
	
	void render_target_texture_storage::rt_begin_scene()
	{
		d3d_texture_ptr d3d_tex = m_tex_store->get_d3d();
		IDirect3DSurface9* surface;
		if(FAILED(d3d_tex->GetSurfaceLevel(0, &surface)))
			return;
 		m_ri->get_resource_tracker().track(surface);			
		m_ri->get_d3d_device()->SetRenderTarget(0, surface);
		surface->Release();
		m_ri->get_d3d_device()->BeginScene();	
	}
	
	void render_target_texture_storage::rt_end_scene()
	{
		m_ri->get_d3d_device()->EndScene();	
	}
	
	void render_target_texture_storage::rt_clear()
	{
		d3d_device_ptr device = m_ri->get_d3d_device();
		device->Clear(0, 0, D3DCLEAR_TARGET /*| D3DCLEAR_ZBUFFER*/, 0, 1.0f, 0);					
	}
	
	void render_target_texture_storage::rt_present()
	{
		TYCHO_ASSERT_MSG(0, "rt_present not valid on render target textures");
	}

	d3d_texture_ptr render_target_texture_storage::get_d3d()
	{
		return m_tex_store->get_d3d();
	}
		
} // end namespace
} // end namespace
} // end namespace
