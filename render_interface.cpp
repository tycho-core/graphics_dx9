//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 April 2008 10:07:34 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "render_interface.h"
#include "dx9/window.h"
#include "dx9/render_target_texture_storage.h"
#include "dx9/vertex_buffer_storage.h"
#include "dx9/index_buffer_storage.h"
#include "dx9/vertex_declaration.h"
#include "dx9/texture.h"
#include "dx9/shader/system_single_clr.h"
#include "dx9/shader/system_ui.h"
#include "dx9/shader/system_ui_string.h"
#include "graphics/adapter.h"
#include "graphics/vertex_format.h"
#include "graphics/vertex_buffer.h"
#include "graphics/render_op.h"
#include "graphics/camera.h"
#include "graphics/texture.h"
#include "graphics/utility.h"
#include "graphics/shader/gamecube_shader.h"
#include "image/image.h"
#include "core/memory.h"
#include "core/hash.h"
#include "core/colour/constants.h"
#include "common.h"
#include "d3d_utils.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
using namespace tycho::graphics;

namespace tycho
{
namespace graphics
{
namespace dx9
{

	//////////////////////////////////////////////////////////////////////////
	// Render interface initialisation
	//////////////////////////////////////////////////////////////////////////

	/// constructor
	render_interface::render_interface(HWND focus_hwnd) :
		m_adapter_id(-1),
		m_focus_hwnd(focus_hwnd),
		m_initialised(false),		
		m_shader_compiler(this),
		m_vertex_decl_factory(this),
		m_render_state(this)
	{
		// create main d3d object
		m_d3d.Attach(::Direct3DCreate9(D3D_SDK_VERSION));
		TYCHO_ASSERT(m_d3d);
		intitialise_adapters();
		TYCHO_ASSERT(m_adapters.size());
	}
	
	/// destructor
	render_interface::~render_interface()
	{
		// unbind resource from the d3d device so they drop their ref count.
		if(m_d3d_device)
		{
			m_cur_render_target = render_target_ptr();
			for(int i = 0; i < 8; ++i)
			{
				m_d3d_device->SetStreamSource(i, 0, 0, 0);
				m_d3d_device->SetTexture(i, 0);
			}
			m_d3d_device->SetIndices(0);
			m_d3d_device->SetPixelShader(0);
			m_d3d_device->SetVertexShader(0);
			m_d3d_device->SetVertexDeclaration(0);
			m_d3d_device->SetDepthStencilSurface(0);
		}

		m_vertex_buffer_allocator.dump_allocated_tags();
		m_index_buffer_allocator.dump_allocated_tags();
		m_vertex_buffer_allocator.clear();	
		m_index_buffer_allocator.clear();		
		
#if TYCHO_DEBUGx
		IDirect3DDevice9* device = m_d3d_device.Detach();
		while(device->Release())
		{}
#endif	
		m_adapters.clear();
		m_d3d_device.Release();
		m_d3d.Release();	
	}
	
	/// get the number of adapters attached to this interface
	int render_interface::get_num_adapters()
	{			
		return (int)m_adapters.size();
	}
	
	/// \returns detailed information about the i'th adapter
	const adapter* render_interface::get_adapter(int i)
	{	
		TYCHO_ASSERT(i < get_num_adapters());
		return &m_adapters[i];
	}

	/// initialise the renderer on the specified adapter
	bool render_interface::initialise(int adapter_idx)
	{
		// create our main device on the adapter, this is used when the game
		// goes full screen and in windowed mode to hang multiple swap chains off
		// so we can render to multiple windows 
		D3DPRESENT_PARAMETERS d3dpp;
		core::mem_zero(d3dpp);
		d3dpp.BackBufferWidth        = 4;
		d3dpp.BackBufferHeight       = 4;
		d3dpp.BackBufferCount        = 1;
		d3dpp.BackBufferFormat       = D3DFMT_UNKNOWN;
		d3dpp.Windowed               = TRUE;
		d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.hDeviceWindow          = m_focus_hwnd;

		IDirect3DDevice9* d3d_device;
		HRESULT hr = get_d3d()->CreateDevice(adapter_idx, 
											 D3DDEVTYPE_HAL, 
											 m_focus_hwnd, 
											 D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING, 
											 &d3dpp, &d3d_device);
		if(FAILED(hr))
			return false;			
		m_d3d_device.Attach(d3d_device);
		m_adapter_id = adapter_idx;
		
		// setup index and vertex buffer allocators
		m_vertex_buffer_allocator.initialise(this);
		m_index_buffer_allocator.initialise(this);
		
		m_initialised = true;			
		return true;
	}

	/// release the interface
	bool render_interface::destroy()
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// Graphics object creation
	//////////////////////////////////////////////////////////////////////////


	//------------------------------------------------------------------------------------------
		
	graphics::window_storage* render_interface::create_window(int width, int height, graphics::format pixel_fmt, graphics::format depth_fmt, void *platform_data)
	{
		return new dx9::window(width, height, pixel_fmt, depth_fmt, this, (HWND)platform_data);
	}
	
	graphics::texture_storage* render_interface::create_texture(tycho::image::image_base_ptr img)
	{
		dx9::texture* new_tex = new dx9::texture(this);
		if(!new_tex->rt_create(img))
			return 0;
		return new_tex;
	}

	graphics::texture_storage* render_interface::create_texture(graphics::format format, int width, int height, int num_mips)
	{
		dx9::texture* new_tex = new dx9::texture(this);
		if(!new_tex->rt_create(format, width, height, num_mips, texture_storage::create_immutable))
			return 0;
		return new_tex;		
	}
		
	graphics::texture_storage* render_interface::create_dynamic_texture(graphics::format format, int width, int height)
	{
		dx9::texture* new_tex = new dx9::texture(this);
		if(!new_tex->rt_create(format, width, height, 1, texture_storage::create_dynamic))
			return 0;
		return new_tex;			
	}

	graphics::render_target_texture_storage* render_interface::create_render_texture(graphics::format format, int width, int height)
	{
		dx9::texture_ptr new_tex(new dx9::texture(this));
		if(!new_tex->rt_create(format, width, height, 1, texture_storage::create_render_target))
			return 0;		
		return new dx9::render_target_texture_storage(this, new_tex);			
	}
	
	/// get some vertex buffer bytes
	/// \todo  static only atm
	graphics::vertex_buffer_storage::driver_object* render_interface::create_vertex_buffer(int num_bytes, int alignment, int create_flags)
	{
		int lock_flags = 0;
		int usage;
		D3DPOOL pool = D3DPOOL_FORCE_DWORD;
		if((create_flags & vertex_buffer::cf_dynamic) > 0)
		{
			lock_flags	= D3DLOCK_NOSYSLOCK | D3DLOCK_NOOVERWRITE;
	        usage		= D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;        
			pool		= D3DPOOL_DEFAULT;
		}
		else 
		{
			lock_flags	= D3DLOCK_NOSYSLOCK;
			usage		= D3DUSAGE_WRITEONLY;
			pool		= D3DPOOL_MANAGED;
		}         			
		vertex_buffer_allocator::tag *tag = m_vertex_buffer_allocator.alloc(usage, pool, num_bytes, alignment, vertex_buffer_allocator::cf_none);		
		TYCHO_ASSERT(tag);
		return new dx9::vertex_buffer_storage(this, tag, lock_flags);
	}

	
	/// get some index buffer bytes
	graphics::index_buffer_storage::driver_object* render_interface::create_index_buffer(graphics::index_buffer::format format, int num_indices, int create_flags)
	{
		int lock_flags = 0;
		int usage;
		D3DPOOL pool = D3DPOOL_FORCE_DWORD;
		if((create_flags & vertex_buffer::cf_dynamic) > 0)
		{
			lock_flags	= D3DLOCK_NOSYSLOCK | D3DLOCK_NOOVERWRITE;
	        usage		= D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;        
			pool		= D3DPOOL_DEFAULT;
		}
		else 
		{
			lock_flags	= D3DLOCK_NOSYSLOCK;
			usage		= D3DUSAGE_WRITEONLY;
			pool		= D3DPOOL_MANAGED;
		}         		
		
		vertex_buffer_allocator::tag *tag = m_index_buffer_allocator.alloc(usage, pool, format, num_indices, index_buffer_allocator::cf_none);		
		TYCHO_ASSERT(tag);
		return new dx9::index_buffer_storage(this, tag, lock_flags);
	}
						
	/// create a new render operation object
	render_op* render_interface::create_render_op()
	{
		return new render_op();
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Shaders
	//////////////////////////////////////////////////////////////////////////

	/// built in system shaders, these use hardcoded vertex + pixel shaders 
	/// so they can be used with only the renderer initialised. 
	graphics::shader::shader_base_ptr render_interface::create_system_shader(system_shader_type type)
	{
		switch(type)
		{
			case system_shader_single_clr : return graphics::shader::shader_base_ptr(new dx9::shader::system_single_clr(this));
			case system_shader_ui : return graphics::shader::shader_base_ptr(new dx9::shader::system_ui(this));
			case system_shader_ui_string : return graphics::shader::shader_base_ptr(new dx9::shader::system_ui_string(this));
		};
		
		return graphics::shader::shader_base_ptr();
	}

	/// create a new gamecube shader. this emulates the rendering setup for gamecube
	/// on PC for easy previewing.
	graphics::shader::gamecube_shader_ptr render_interface::create_gamecube_shader()
	{
		return new graphics::shader::gamecube_shader();
	}
	
	
	//////////////////////////////////////////////////////////////////////////
	// Scene rendering
	//////////////////////////////////////////////////////////////////////////
	
	/// must be called before rendering to the passed render target
	void render_interface::begin_render(render_target_ptr rt)
	{
		// setup the render target
		rt->get_render_target_storage()->rt_begin_scene();
		m_cur_render_target = rt;
	}	
	
	void render_interface::set_viewport(const graphics::viewport& vp)
	{
		TYCHO_ASSERT(m_cur_render_target);		
		D3DVIEWPORT9 d3d_vp;
		render_target_ptr rt = m_cur_render_target;
		d3d_vp.X = static_cast<int>(math::floor(vp.get_origin().x() * rt->get_width()));
		d3d_vp.Y = static_cast<int>(math::floor(vp.get_origin().y() * rt->get_height()));
		d3d_vp.Width  = static_cast<int>(math::floor(vp.get_size().x() * rt->get_width()));
		d3d_vp.Height = static_cast<int>(math::floor(vp.get_size().y() * rt->get_height()));
		d3d_vp.MinZ = vp.get_min_depth();
		d3d_vp.MaxZ = vp.get_max_depth();
		get_d3d_device()->SetViewport(&d3d_vp);	
	}
	
	/// must be called when rendering to the render target is done
	void render_interface::end_render()
	{
		TYCHO_ASSERT(m_cur_render_target);		
		m_cur_render_target->get_render_target_storage()->rt_end_scene();
		m_cur_render_target = 0;
	}
	
	/// submit a render operation to the driver
	void render_interface::render(render_op_ptr op)
	{
		TYCHO_ASSERT(m_cur_render_target);
		TYCHO_ASSERT(op->shader);	
		
		int num_vbs = 0;
		for(; num_vbs < render_op::MaxVertexBuffers && op->vertex_buffers[num_vbs]; ++num_vbs)
			;

		int num_ibs = 0;
		for(; num_ibs < render_op::MaxIndexBuffers && op->index_buffers[num_ibs]; ++num_ibs)
			;
					
		// create a d3d vertex declaration for this call
		const graphics::vertex_format **fmt_list = (const graphics::vertex_format **)TYCHO_ALLOCA(sizeof(graphics::vertex_format*) * num_vbs);
		for(int i = 0; i < num_vbs; ++i)
			fmt_list[i] = op->vertex_buffers[i]->get_format();
		vertex_declaration_ptr vertex_decl = m_vertex_decl_factory.get_d3d_decl(fmt_list, num_vbs);
		TYCHO_ASSERT(vertex_decl);
		
		
		// configure vertex unit
		get_d3d_device()->SetVertexDeclaration(vertex_decl->get_d3d());
		for(int i = 0; i < num_vbs; ++i)
		{
			vertex_buffer_ptr vb = op->vertex_buffers[i];
			graphics::vertex_buffer_storage::driver_object* store = vb->get_storage()->get_driver_object().get();
			dx9::vertex_buffer_storage* dx_store = (dx9::vertex_buffer_storage*)store;
			int stride = vb->get_format()->size();
 			int offset = vb->get_buffer_offset();
			get_d3d_device()->SetStreamSource(i, (IDirect3DVertexBuffer9*)dx_store->m_tag->m_block->m_buffer.p, offset + (op->first_vertex * stride), stride);
		}
				
		int ib_offset = 0;
		if(op->index_buffers[0])
		{
			graphics::index_buffer_storage::driver_object* store = op->index_buffers[0]->get_storage()->get_driver_object().get();
			dx9::index_buffer_storage* dx_store = (dx9::index_buffer_storage*)store;
			ib_offset = dx_store->get_buffer_offset() / 2;
			get_d3d_device()->SetIndices(reinterpret_cast<IDirect3DIndexBuffer9*>(dx_store->m_tag->m_block->m_buffer.p));
		}
		else
		{
			get_d3d_device()->SetIndices(0);
		}
		
		// configure vertex / pixel shaders
		op->shader->render_setup(op->shader_params);
		
		// configure pixel unit
		bool alpha_blend_enable = true;
		m_render_state.set(D3DRS_CULLMODE, D3DCULL_CCW);
		m_render_state.set(D3DRS_ZENABLE, D3DZB_TRUE);
		m_render_state.set(D3DRS_ZWRITEENABLE, TRUE);
		m_render_state.set(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		if(alpha_blend_enable)
		{
			m_render_state.set(D3DRS_ALPHABLENDENABLE, TRUE);
			m_render_state.set(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_render_state.set(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_render_state.set(D3DRS_ALPHATESTENABLE, TRUE);
			m_render_state.set(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			m_render_state.set(D3DRS_ALPHAREF, 0);
		}
		else
		{
			m_render_state.set(D3DRS_ALPHABLENDENABLE, FALSE);		
			m_render_state.set(D3DRS_ALPHATESTENABLE, FALSE);
		}
		
		// render the sucker
		int num_primitives = graphics::get_num_primitives(op->num_indices, op->primitive);
		get_d3d_device()->DrawIndexedPrimitive(tycho_to_d3d(op->primitive), 0, 0, op->num_vertices, ib_offset + op->first_index, num_primitives);
	}

	//////////////////////////////////////////////////////////////////////////
	// Frame management
	//////////////////////////////////////////////////////////////////////////
	
	/// flush all queued data to the GPU.
	bool render_interface::flush()
	{
		return true;
	}

	/// flip to front buffer
	bool render_interface::flip()
	{
		return true;
	}

	/// stall until all current rendering operations are complete
	void render_interface::sync()
	{
	
	}        

	
	void render_interface::intitialise_adapters()
	{
		m_adapters.clear();
		int num_adapters = m_d3d->GetAdapterCount();
		graphics::format formats[] =
			{
				graphics::format::pixel_argb8888,
				graphics::format::pixel_rgb565,
				graphics::format::pixel_xrgb1555,
				graphics::format::pixel_xrgb8888,
				graphics::format::unknown
			};
		
		for(int i = 0; i < num_adapters; ++i)
		{
			D3DADAPTER_IDENTIFIER9 adapter_id;
			
			m_d3d->GetAdapterIdentifier(i, 0, &adapter_id);
			
			const graphics::format *cur_format = &formats[0];		
			adapter::formats formats;		
			while(*cur_format != graphics::format::unknown)
			{
				D3DFORMAT d3d_format = dx9::tycho_to_d3d(*cur_format);
				int num_modes = m_d3d->GetAdapterModeCount(i, d3d_format);					 
				adapter::modes modes;
				modes.reserve(num_modes);
				for(int mode = 0; mode < num_modes; ++mode)
				{
					D3DDISPLAYMODE d3d_mode;						
					m_d3d->EnumAdapterModes(i, d3d_format, mode, &d3d_mode);
					adapter::mode new_mode;
					new_mode.width = d3d_mode.Width;
					new_mode.height = d3d_mode.Height;
					new_mode.refresh_rate = d3d_mode.RefreshRate;
					modes.push_back(new_mode);				
				}
				
				if(modes.size())						
					formats.push_back(adapter::format(*cur_format, modes));				
				
				++cur_format;
			}
			m_adapters.push_back(graphics::adapter("", formats));		
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Debugging
	//////////////////////////////////////////////////////////////////////////
	
	/// marks the beginning of a GPU event.
	void render_interface::begin_gpu_event(const char* name)
	{
		using namespace core;
		
		// create random colour based on hash of the name so it is the same for all calls 
		// with the same name
		static rgba clr_table[] = {
			colours::red,
			colours::green,
			colours::blue,
			colours::lightblue,
			colours::lightgreen,
			colours::yellow
		};
		static int num_clrs = sizeof(clr_table) / sizeof(rgba);
		int clr = core::hash(name) % num_clrs;		
		D3DCOLOR d3d_clr = tycho_to_d3d(clr_table[clr]);		
		wchar_t wname[2048];
		mbstowcs(wname, name, strlen(name)+1);
		int res = D3DPERF_BeginEvent(d3d_clr, wname);
		TYCHO_ASSERT(res >= 0);		
	}
	
	/// marks the end of a GPU event
	void render_interface::end_gpu_event()
	{
		D3DPERF_EndEvent();
	}

} // end namespace
} // end namespace
} // end namespace
