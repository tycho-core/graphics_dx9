//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 27 April 2008 7:36:08 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SYSTEM_UI_H_3BE94081_70C9_4D73_9444_3142FB66C43D_
#define __SYSTEM_UI_H_3BE94081_70C9_4D73_9444_3142FB66C43D_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/dx9_abi.h"
#include "graphics/shader/shader.h"
#include "graphics/shader/forward_decls.h"
#include "dx9/common.h"


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

namespace shader
{

	
	/// shader used for rendering basic system user interface
	///		- Parameters
	///			-# proj_matrix     : matrix4x4  : concatenated view * projection matrices
	///			-# world_matrix    : matrix4x4  : world matrix
	///			-# diffuse_texture : texture2d  : diffuse texture
	///         -# obj_clr         : vector4    : object colour
	///
	///		- Vertex Requirements
	///			-# position : vector4
	///			-# texcoord : vector2
	///			-# color    : rgba
    class TYCHO_DX9_ABI system_ui : public graphics::shader::shader_base
    {
    public:
		system_ui(dx9::render_interface *ri);
		~system_ui();
		virtual int get_num_input_semantics() const;
		virtual const vertex_format::semantic_decl* get_input_semantic(int) const;
		virtual const graphics::shader::parameter_buffer_ptr get_shader_parameters() const;
		virtual graphics::shader::parameter_buffer_ptr create_default_table() const;		
		virtual const graphics::shader::parameter_buffer_layout_ptr get_parameter_layout() const;
		virtual void render_setup(graphics::shader::parameter_buffer_ptr);
    
    private:
		dx9::render_interface* m_ri;
		graphics::shader::parameter_buffer_layout_ptr m_layout;    
		d3d_ps_ptr m_pixel_shader;	
		d3d_vs_ptr m_vertex_shader;
    };

} // end namespace
} // end namespace
} // end namespace
} // end namespace

#endif // __SYSTEM_UI_H_3BE94081_70C9_4D73_9444_3142FB66C43D_
