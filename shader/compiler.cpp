//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 12 April 2008 7:57:07 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "compiler.h"
#include "core/debug/assert.h"
#include "graphics_dx9/render_interface.h"
#include "graphics_dx9/program_config_options.h"
#include "core/console.h"
//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{
namespace shader
{

	compiler::compiler(dx9::render_interface *ri) :
		m_ri(ri)
	{}
	
	d3d_vs_ptr compiler::compile_vertex_shader(const char* str, int len, const char* entry_point)
	{
		TYCHO_ASSERT(str);
		TYCHO_ASSERT(len);
		TYCHO_ASSERT(entry_point);
		
		d3dx_buffer_ptr shader;
		d3dx_buffer_ptr error_msgs;
		d3dx_constant_table constant_table;
		DWORD flags = 0;
		if(dx9::global_options().shader_debug)
			flags |= D3DXSHADER_DEBUG;			
		HRESULT hr = D3DXCompileShader(str, len, 0, 0, entry_point, "vs_3_0" , flags, &shader.p, &error_msgs.p, &constant_table.p);		
		if(FAILED(hr))
		{
			if(error_msgs)
			{
				core::console::write_ln(reinterpret_cast<const char*>(error_msgs->GetBufferPointer()));
			}
			return d3d_vs_ptr();
		}
			
		d3d_vs_ptr new_shader;
		if(FAILED(m_ri->get_d3d_device()->CreateVertexShader(static_cast<const DWORD*>(shader->GetBufferPointer()), &new_shader.p)))
			return d3d_vs_ptr();
		return new_shader;					
	}
	
	d3d_ps_ptr compiler::compile_pixel_shader(const char* str, int len, const char* entry_point)
	{
		TYCHO_ASSERT(str);
		TYCHO_ASSERT(len);
		TYCHO_ASSERT(entry_point);
		
		d3dx_buffer_ptr shader;
		d3dx_buffer_ptr error_msgs;
		d3dx_constant_table constant_table;
		DWORD flags = 0;
		if(dx9::global_options().shader_debug)
			flags |= D3DXSHADER_DEBUG;			
		HRESULT hr = D3DXCompileShader(str, len, 0, 0, entry_point, "ps_3_0" , flags, &shader.p, &error_msgs.p, &constant_table.p);
		if(FAILED(hr))
		{
			if(error_msgs)
			{
				core::console::write_ln(reinterpret_cast<const char*>(error_msgs->GetBufferPointer()));
			}		
			return d3d_ps_ptr();
		}
			
		d3d_ps_ptr new_shader;
		if(FAILED(m_ri->get_d3d_device()->CreatePixelShader(static_cast<const DWORD*>(shader->GetBufferPointer()), &new_shader.p)))
			return d3d_ps_ptr();
		return new_shader;					
	}


} // end namespace
} // end namespace
} // end namespace
} // end namespace
