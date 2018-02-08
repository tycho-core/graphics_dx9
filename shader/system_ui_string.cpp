//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 11 August 2008 5:23:22 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "system_ui_string.h"
#include "graphics/texture.h"
#include "graphics/texture_storage.h"
#include "graphics/shader/parameter_buffer_view.h"
#include "graphics_dx9/render_interface.h"
#include "graphics_dx9/texture.h"
#include "graphics_dx9/texture_storage_base.h"
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
#include "hlsl/system_ui_string.h"
	}

	system_ui_string::system_ui_string(dx9::render_interface *ri) :
		m_ri(ri)
	{
		using namespace graphics::shader;
		
		// setup our parameter buffer layout
		m_layout = parameter_buffer_layout_ptr(new parameter_buffer_layout()); 
		m_layout->add_parameter("view_proj", spt_matrix44, spm_none);
		m_layout->add_parameter("world_matrix", spt_matrix44, spm_none);
		m_layout->add_parameter("obj_clr", spt_float4, spm_none);
		m_layout->add_parameter("diffuse_texture", spt_texture, spm_none);
		m_layout->finalise();
		
		// compiler our shaders 
		shader::compiler& compiler = m_ri->get_shader_compiler();
		m_vertex_shader = compiler.compile_vertex_shader(detail::system_ui_string, strlen(detail::system_ui_string), "vs_main");
		m_pixel_shader = compiler.compile_pixel_shader(detail::system_ui_string, strlen(detail::system_ui_string), "ps_main");	
	}
	
	int system_ui_string::get_num_input_semantics() const
	{
		return 3;
	}
	
	const vertex_format::semantic_decl* system_ui_string::get_input_semantic(int i) const
	{
		if(i > 2)
			return 0;
			
		static vertex_format::semantic_decl decls[2] =  {
			vertex_format::make_semantic_decl(vertex_format::s_position, 0, 0, vertex_format::c_xyz),
			vertex_format::make_semantic_decl(vertex_format::s_texcoord, 0, 0, vertex_format::c_xy)
			//vertex_format::make_semantic_decl(vertex_format::s_diffuse, 0, 1, vertex_format::c_rgba) 
			};
			
		return &decls[i];
	
	}
	
	const graphics::shader::parameter_buffer_ptr system_ui_string::get_shader_parameters() const
	{
		return gs::parameter_buffer_ptr();	
	}
	
	graphics::shader::parameter_buffer_ptr system_ui_string::create_default_table() const
	{
		gs::parameter_buffer_ptr pb = gs::parameter_buffer_ptr(new gs::parameter_buffer(m_layout->get_required_size()));
		gs::parameter_buffer_view view(pb, m_layout);
		view.set("obj_clr", math::vector4f(0.5f, 0.0f, 0.0f, 1.0f));
		view.set("diffuse_texture", (graphics::texture*)0);
		view.set("world_matrix", math::matrix4x4f::create_identity());
		return pb;	
	}
	
	const graphics::shader::parameter_buffer_layout_ptr system_ui_string::get_parameter_layout() const
	{
		return m_layout;	
	}
	
	void system_ui_string::render_setup(graphics::shader::parameter_buffer_ptr pb)
	{
		// setup vertex shader
		gs::parameter_buffer_view view(pb, m_layout);
//		m_ri->get_d3d_device()->SetVertexShaderConstantF(0, (float*)view.get_ptr<math::matrix4x4f>("view_proj"), 4);
		m_ri->get_d3d_device()->SetVertexShaderConstantF(0, (float*)view.get_ptr<math::matrix4x4f>("world_matrix"), 4);
		m_ri->get_d3d_device()->SetVertexShaderConstantF(3, (float*)view.get_ptr<math::vector4f>("obj_clr"), 1);
		m_ri->get_d3d_device()->SetVertexShader(m_vertex_shader);
		
		// setup pixel shader
		graphics::texture_base** diffuse_texture = view.get_ptr<graphics::texture_base*>("diffuse_texture");
		if(diffuse_texture && *diffuse_texture)
		{
			graphics::dx9::texture_storage_base* dx9_texture = dynamic_cast<graphics::dx9::texture_storage_base*>((*diffuse_texture)->get_texture_storage().get());			
			m_ri->get_d3d_device()->SetTexture(0, dx9_texture->get_d3d());			
		}
		
		m_ri->get_d3d_device()->SetPixelShader(m_pixel_shader);	
	}

} // end namespace
} // end namespace
} // end namespace
} // end namespace
