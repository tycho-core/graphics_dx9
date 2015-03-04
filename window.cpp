//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 10:18:11 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "window.h"
#include "core/memory.h"
#include "render_interface.h"
#include "d3d_utils.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{
	TYCHO_INTRUSIVE_PTR_FUNC_IMPL(TYCHO_DX9_ABI, window)

	/// constructor
	window::window(int width, int height, graphics::format pixel_format, graphics::format depth_format, dx9::render_interface *ri, HWND owner_hwnd) :
		m_render_interface(ri),
		m_owner_hwnd(owner_hwnd),
		m_width(-1),
		m_height(-1)
	{
		d3d_ptr d3d = m_render_interface->get_d3d();
		d3d_device_ptr d3d_device = m_render_interface->get_d3d_device();
		
		TYCHO_ASSERT(d3d);
		TYCHO_ASSERT(d3d_device);
		
		// initialise non-exclusive mode window
		D3DPRESENT_PARAMETERS pp;
		core::mem_zero(pp);
		pp.BackBufferCount = 2;
		pp.BackBufferWidth  = width;
		pp.BackBufferHeight = height;
		pp.hDeviceWindow = m_owner_hwnd;
		pp.Windowed = TRUE;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		
		// get the current desktop display mode
		D3DDISPLAYMODE cur_mode;
		core::mem_zero(cur_mode);
		if(FAILED(d3d->GetAdapterDisplayMode(m_render_interface->get_adapter_id(), &cur_mode)))
		{
			/// \todo  error
			return;
		}		
		pp.BackBufferFormat = cur_mode.Format;
		
		// reset the device into this mode
		if(FAILED(m_render_interface->get_d3d_device()->Reset(&pp)))
		{
			/// \todo  error
			return;
		}
		
		// create our additional swap chain
		IDirect3DSwapChain9* swap_chain = 0;
		if(FAILED(d3d_device->CreateAdditionalSwapChain(&pp, &swap_chain)))
		{
			/// \todo  error
			return;
		}
		m_swap_chain.Attach(swap_chain);
		
		// setup the depth buffer
		if(depth_format != format::unknown)
		{
			// see if the format the user asked for is ok, if not then go rummaging for
			// one that will work
			if(FAILED(d3d->CheckDeviceFormat(m_render_interface->get_adapter_id(),
											 D3DDEVTYPE_HAL, 
											 tycho_to_d3d(pixel_format),
											 D3DUSAGE_DEPTHSTENCIL, 
											 D3DRTYPE_SURFACE,
											 tycho_to_d3d(depth_format))))
			{
				// no good so try and find one that does work			
				graphics::format formats[] = {
					format::depth_32bit,
					format::depth_24bit,
					format::depth_16bit,
					format::unknown };

				// Find a compatible z buffer.
				const graphics::format *cur = formats;

				while(*cur != format::unknown) 
				{
					D3DFORMAT d3d_fmt = tycho_to_d3d(*cur);
					if(SUCCEEDED(d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
						pp.BackBufferFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, d3d_fmt)))
					{
						if(SUCCEEDED(d3d->CheckDepthStencilMatch(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
							pp.BackBufferFormat, pp.BackBufferFormat, d3d_fmt)))
						{
							break;
						}
					} 
					++cur;
				}		
				
				if(*cur == format::unknown)
				{
					// no matching ones, give up.
					/// \todo  error
					return;
				}
				depth_format = *cur;
			}
			IDirect3DSurface9* depth_surface;
			if(FAILED(d3d_device->CreateDepthStencilSurface(pp.BackBufferWidth, pp.BackBufferHeight, 
				tycho_to_d3d(depth_format), D3DMULTISAMPLE_NONE, 0, TRUE, &depth_surface, NULL)))
			{
				/// \todo  error
				return;	
			}
			m_render_interface->get_resource_tracker().track(depth_surface);
			m_depth_surface.Attach(depth_surface);
		}		
		m_width = width;
		m_height = height;
	}
	
	/// destructor
	window::~window()
	{
	
	}		
	
	/// called before rendering starts
	void window::rt_begin_scene()
	{
		d3d_device_ptr device = m_render_interface->get_d3d_device();
		IDirect3DSurface9 *buffer = 0;
		/// \todo  check success
		m_swap_chain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &buffer);
		device->SetRenderTarget(0, buffer);
		buffer->Release();
		if(m_depth_surface)
			device->SetDepthStencilSurface(m_depth_surface);
		else
			device->SetDepthStencilSurface(0);		
		device->BeginScene();
	}
	
	/// called after rendering has completed
	void window::rt_end_scene()
	{
		d3d_device_ptr device = m_render_interface->get_d3d_device();
		device->EndScene();
		
		// unbind render and depth targets to highlight errors
//		device->SetRenderTarget(0, 0);
		device->SetDepthStencilSurface(0);			
	}
	
	/// clear the render target
	void window::rt_clear()
	{
		d3d_device_ptr device = m_render_interface->get_d3d_device();		
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);			
	}	
	
	/// present the target to a output device, only valid for window render targets.
	void window::rt_present()
	{
		m_swap_chain->Present(0, 0, 0, 0, 0);
	}
	
	/// \returns the width of the render target
	int window::get_width() const
	{
		return m_width;	
	}	
	
	/// \returns the height of the render target
	int window::get_height() const
	{
		return m_height;	
	}	

} // end namespace
} // end namespace
} // end namespace

