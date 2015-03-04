//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 7 July 2008 2:26:13 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDER_TARGET_TEXTURE_STORAGE_H_377C7C84_5906_429C_8218_2E334F753F56_
#define __RENDER_TARGET_TEXTURE_STORAGE_H_377C7C84_5906_429C_8218_2E334F753F56_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/dx9_abi.h"
#include "dx9/forward_decls.h"
#include "dx9/texture.h"
#include "dx9/texture_storage_base.h"
#include "graphics/render_target_texture.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// DirectX render to texture implementation
    class TYCHO_DX9_ABI render_target_texture_storage : 
		public dx9::texture_storage_base,
		public graphics::render_target_texture_storage
    {
    public:
		/// constructor
		render_target_texture_storage(dx9::render_interface* ri, dx9::texture_ptr tex_store);
		
		/// destructor
		~render_target_texture_storage();
		
		/// \name texture_storage interface
		//@{
		virtual bool rt_create(graphics::format format, int width, int height, int num_mips, int create_flags);
		virtual bool rt_update(int x, int y, int width, int height, image::image_base* img);
		virtual bool rt_lock(int mip_level, char** out_buffer, int* out_stride);		
		virtual bool rt_unlock(int mip_level);		
		virtual bool rt_mark_dirty(int x, int y, int width, int height);
		//@}
		
		/// \name render_target_storage interface
		//@{
		virtual void rt_begin_scene();
		virtual void rt_end_scene();
		virtual void rt_clear();
		virtual void rt_present();		
		//@}
		
		/// \returns the direct 3d texture object
		d3d_texture_ptr get_d3d();		
		
	private:
		dx9::render_interface* m_ri;
		dx9::texture_ptr m_tex_store;
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __RENDER_TARGET_TEXTURE_STORAGE_H_377C7C84_5906_429C_8218_2E334F753F56_
