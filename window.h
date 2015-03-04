//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 10:18:11 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __WINDOW_H_CF8E5185_42A6_48C0_954F_6630F408DDF5_
#define __WINDOW_H_CF8E5185_42A6_48C0_954F_6630F408DDF5_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/dx9_abi.h"
#include "graphics/window.h"
#include "graphics/format.h"
#include "common.h"

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
	
	/// dx9 window implementation
	/// \todo  add support for exclusive mode
    class TYCHO_DX9_ABI window : public graphics::window_storage
    {
    public:
		/// constructor
		window(int width, int height, graphics::format pixel_format, graphics::format depth_format, dx9::render_interface *ri, HWND owner_hwnd);
		
		/// destructor
		~window();	
		
		/// called before rendering starts
		virtual void rt_begin_scene();
		
		/// called after rendering has completed
		virtual void rt_end_scene();
		
		/// clear the render target
		virtual void rt_clear();
		
		/// present the target to a output device, only valid for window render targets.
		virtual void rt_present();
		
		/// \returns the width of the render target
		virtual int get_width() const;
		
		/// \returns the height of the render target
		virtual int get_height() const;	        
		
	private:
		TYCHO_INTRUSIVE_PTR_FRIEND_DECLS(TYCHO_DX9_ABI, window)
		
		HWND					m_owner_hwnd;
		d3d_swap_chain_ptr		m_swap_chain;
		d3d_surface_ptr			m_depth_surface;
		dx9::render_interface*	m_render_interface;
		int						m_width;
		int						m_height;
    };
    
    TYCHO_INTRUSIVE_PTR_FUNC_DECLS(TYCHO_DX9_ABI, window)

} // end namespace
} // end namespace
} // end namespace

#endif // __WINDOW_H_CF8E5185_42A6_48C0_954F_6630F408DDF5_
