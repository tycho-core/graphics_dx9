//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 April 2008 10:07:34 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDER_INTERFACE_H_BD411C93_B5DB_45AB_8E07_8E82EC01CB4C_
#define __RENDER_INTERFACE_H_BD411C93_B5DB_45AB_8E07_8E82EC01CB4C_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/dx9_abi.h"
#include "dx9/d3d_types.h"
#include "dx9/index_buffer_allocator.h"
#include "dx9/vertex_buffer_allocator.h"
#include "dx9/vertex_declaration_factory.h"
#include "dx9/resource_tracker.h"
#include "dx9/render_state.h"
#include "dx9/shader/compiler.h"
#include "graphics/render_interface.h"
#include "graphics/adapter.h"
#include "image/forward_decls.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{
//	vertex_buffer_allocator foo;

	/// DirectX 9 render interface
	class TYCHO_DX9_ABI render_interface : public tycho::graphics::render_interface
	{
	public:
		/// constructor
		render_interface(HWND focus_wnd);
		
		/// destructor
		~render_interface();
		
		//////////////////////////////////////////////////////////////////////////
		// Render interface initialisation
		//////////////////////////////////////////////////////////////////////////

		/// get the number of adapters attached to this interface
		virtual int get_num_adapters();
		
		/// \returns detailed information about the i'th adapter
		virtual const adapter* get_adapter(int i);

		/// initialise the renderer on the specified adapter
		virtual bool initialise(int adapter_idx);

		/// release the interface
		virtual bool destroy();

		//////////////////////////////////////////////////////////////////////////
		// Graphics object creation
		//////////////////////////////////////////////////////////////////////////
		
		/// create a new window
		virtual graphics::window_storage* create_window(int width, int height, graphics::format pixel_fmt, graphics::format depth_fmt, void *platform_data);
		
		/// create a new blank texture
		virtual graphics::texture_storage* create_texture(tycho::image::image_base_ptr);

		/// create a texture of the specified size / format and mips.
		virtual graphics::texture_storage* create_texture(graphics::format format, int width, int height, int num_mips);
		
		/// create a dynamic texture of the specified size / format. No mip maps are allowed.
		virtual graphics::texture_storage* create_dynamic_texture(graphics::format format, int width, int height);

		/// create a render target texture of the specified size / format. No mip maps are allowed.
		virtual graphics::render_target_texture_storage* create_render_texture(graphics::format format, int width, int height);
		
		/// get some vertex buffer bytes
		virtual graphics::vertex_buffer_storage::driver_object* create_vertex_buffer(int num_bytes, int alignment, int create_flags);
		
		/// get some index buffer bytes
		virtual graphics::index_buffer_storage::driver_object* create_index_buffer(graphics::index_buffer::format type, int num_indices, int create_flags);
						
		/// create a new render operation object
		virtual render_op* create_render_op();

		//////////////////////////////////////////////////////////////////////////
		// Shaders
		//////////////////////////////////////////////////////////////////////////

		/// built in system shaders
		virtual graphics::shader::shader_base_ptr create_system_shader(system_shader_type);

		/// create a new gamecube shader. this emulates the rendering setup for gamecube
		/// on PC for easy previewing.
		virtual graphics::shader::gamecube_shader_ptr create_gamecube_shader();

		//////////////////////////////////////////////////////////////////////////
		// Scene rendering
		//////////////////////////////////////////////////////////////////////////
		
		/// must be called before rendering to the passed render target
		virtual void begin_render(render_target_ptr);
		
		/// must be called when rendering to the render target is done
		virtual void end_render();
		
		void set_viewport(const graphics::viewport& vp);
		
		/// submit a render operation to the driver
		virtual void render(render_op_ptr);

		//////////////////////////////////////////////////////////////////////////
		// Frame management
		//////////////////////////////////////////////////////////////////////////
		
		/// flush all queued data to the GPU.
		virtual bool flush(); 

		/// flip to front buffer
		virtual bool flip();		

		/// stall until all current rendering operations are complete
		virtual void sync();        
		
		//////////////////////////////////////////////////////////////////////////
		// Debugging
		//////////////////////////////////////////////////////////////////////////
		
		/// marks the beginning of a GPU event.
		virtual void begin_gpu_event(const char* name);
		
		/// marks the end of a GPU event
		virtual void end_gpu_event();
		
	public:
		//////////////////////////////////////////////////////////////////////////
		// Driver internal functions
		//////////////////////////////////////////////////////////////////////////
		d3d_ptr get_d3d() 
			{ return m_d3d; }
			
		IDirect3DDevice9* get_d3d_device()
			{ return m_d3d_device.p; }
			
		int get_adapter_id()
			{ return m_adapter_id; }
			
		vertex_buffer_allocator& get_vertex_buffer_allocator() 
			{ return m_vertex_buffer_allocator; }
			
		index_buffer_allocator& get_index_buffer_allocator() 
			{ return m_index_buffer_allocator; }
			
		shader::compiler& get_shader_compiler()
			{ return m_shader_compiler; }
			
		resource_tracker& get_resource_tracker()
			{ return m_resource_tracker; }
			
	private:
		// internal functions
		void intitialise_adapters();

	private:
		d3d_ptr							m_d3d;
		d3d_device_ptr					m_d3d_device;			
		int								m_adapter_id; ///< adapter we are running on
		HWND							m_focus_hwnd;
		bool							m_initialised;
		resource_tracker			    m_resource_tracker;
		vertex_declaration_factory		m_vertex_decl_factory;
		vertex_buffer_allocator			m_vertex_buffer_allocator;
		index_buffer_allocator			m_index_buffer_allocator;
		shader::compiler				m_shader_compiler;
		std::vector<graphics::adapter>	m_adapters;
		graphics::render_target_ptr		m_cur_render_target;		
		dx9::render_state				m_render_state;
	};

} // end namespace
} // end namespace
} // end namespace

#endif // __RENDER_INTERFACE_H_BD411C93_B5DB_45AB_8E07_8E82EC01CB4C_
