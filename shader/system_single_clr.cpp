//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 12 April 2008 4:27:21 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "system_single_clr.h"
#include "graphics/shader/parameter_buffer_layout.h"
#include "graphics/shader/parameter_buffer_view.h"
#include "graphics/shader/parameter_buffer.h"
#include "graphics_dx9/render_interface.h"
#include "graphics_dx9/shader/compiler.h"
#include "math/matrix4x4.h"
#include "math/vector4.h"

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
	namespace gs = graphics::shader;
	
	namespace detail
	{
#include "hlsl/system_single_clr.h"
	}
	
	system_single_clr::system_single_clr(dx9::render_interface *ri) :
		m_ri(ri)
	{
		using namespace graphics::shader;
		
		// setup our parameter buffer layout
		m_layout = parameter_buffer_layout_ptr(new parameter_buffer_layout()); 
		m_layout->add_parameter("view_proj", spt_matrix44, spm_none);
		m_layout->add_parameter("light_dir", spt_float4, spm_none);
		m_layout->add_parameter("light_clr", spt_float4, spm_none);
		m_layout->add_parameter("obj_clr", spt_float4, spm_none);
		m_layout->add_parameter("world_matrix", spt_matrix44, spm_none);
		m_layout->finalise();
		
		// compiler our shaders 
		shader::compiler& compiler = m_ri->get_shader_compiler();
		m_vertex_shader = compiler.compile_vertex_shader(detail::system_single_clr, strlen(detail::system_single_clr), "vs_main");
		m_pixel_shader = compiler.compile_pixel_shader(detail::system_single_clr, strlen(detail::system_single_clr), "ps_main");
	}

	system_single_clr::~system_single_clr()
	{
	}
	
	int system_single_clr::get_num_input_semantics() const
	{
		return 2;
	}
	
	const vertex_format::semantic_decl* system_single_clr::get_input_semantic(int i) const
	{
		if(i > 1)
			return 0;
			
		static vertex_format::semantic_decl decls[2] =  {
			vertex_format::make_semantic_decl(vertex_format::s_position, 0, 0, vertex_format::c_xyz),
			vertex_format::make_semantic_decl(vertex_format::s_diffuse, 0, 1, vertex_format::c_rgba) 
			};
			
		return &decls[i];
	}
	
	/// \todo why did we want this as well as create_default_table? or are these the configuration parameters to control compilation?
	const gs::parameter_buffer_ptr system_single_clr::get_shader_parameters() const
	{
		return gs::parameter_buffer_ptr();
	}
	
	gs::parameter_buffer_ptr system_single_clr::create_default_table() const
	{
		gs::parameter_buffer_ptr pb = gs::parameter_buffer_ptr(new gs::parameter_buffer(m_layout->get_required_size()));
		gs::parameter_buffer_view view(pb, m_layout);
		view.set<math::vector4f>("light_dir", math::vector4f(-1, -2, 1, 0).normalized());
		view.set<math::vector4f>("light_clr", math::vector4f(0.7f, 0.5f, 0.5f, 1.0f));
		view.set<math::vector4f>("obj_clr", math::vector4f(0.5f, 0.0f, 0.0f, 1.0f));
		return pb;
	}
	
	const gs::parameter_buffer_layout_ptr system_single_clr::get_parameter_layout() const
	{
		return m_layout;
	}
	
	void system_single_clr::render_setup(gs::parameter_buffer_ptr pb)
	{		
		gs::parameter_buffer_view view(pb, m_layout);
		m_ri->get_d3d_device()->SetVertexShaderConstantF(0, (float*)view.get_ptr<math::matrix4x4f>("view_proj"), 4);
		m_ri->get_d3d_device()->SetVertexShaderConstantF(4, (float*)view.get_ptr<math::matrix4x4f>("world_matrix"), 4);
		m_ri->get_d3d_device()->SetVertexShaderConstantF(8, (float*)view.get_ptr<math::vector4f>("light_dir"), 1);
		m_ri->get_d3d_device()->SetVertexShaderConstantF(9, (float*)view.get_ptr<math::vector4f>("light_clr"), 1);
		m_ri->get_d3d_device()->SetVertexShaderConstantF(10, (float*)view.get_ptr<math::vector4f>("obj_clr"), 1);
		m_ri->get_d3d_device()->SetVertexShader(m_vertex_shader);
		m_ri->get_d3d_device()->SetPixelShader(m_pixel_shader);
	}


} // end namespace
} // end namespace
} // end namespace
} // end namespace
