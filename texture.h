//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 27 April 2008 7:58:57 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __TEXTURE_H_6B487438_0EA1_45C2_911C_42040B2854AB_
#define __TEXTURE_H_6B487438_0EA1_45C2_911C_42040B2854AB_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "graphics_dx9/common.h"
#include "graphics/texture_storage.h"
#include "graphics/format.h"
#include "graphics_dx9/texture_storage_base.h"
#include "image/forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	class render_interface;
	
	/// dx9 texture resource 
    class TYCHO_DX9_ABI texture : 
		public dx9::texture_storage_base,
		public graphics::texture_storage
    {
    public:
		/// constructor
		texture(dx9::render_interface*);
		
		/// destructor
		~texture();
		
		/// \name graphics::texture interface
		//@{
		virtual bool rt_create(graphics::format format, int width, int height, int num_mips, int create_flags);
		virtual bool rt_create(tycho::image::image_base_ptr);
		virtual bool rt_update(int x, int y, int width, int height, image::image_base* img);
		virtual bool rt_lock(int mip_level, char** out_buffer, int* out_stride);		
		virtual bool rt_mark_dirty(int x, int y, int width, int height);
		virtual bool rt_unlock(int mip_level);		
		//@}
		
		/// \returns the direct 3d texture object
		virtual d3d_texture_ptr get_d3d();
				
    private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_DX9_ABI, texture)

		dx9::render_interface* m_ri;
		d3d_texture_ptr  m_d3d_ptr;
		d3d_texture_ptr  m_sysmem_ptr;		///< system memory surface used if the texture is dynamic
		image::image_base_ptr m_img_view;	///< used when updating the texture.
		int m_width;
		int m_height;
		int m_num_mips;
    };

    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_DX9_ABI, texture)

} // end namespace
} // end namespace
} // end namespace

#endif // __TEXTURE_H_6B487438_0EA1_45C2_911C_42040B2854AB_
