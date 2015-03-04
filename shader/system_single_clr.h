//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 12 April 2008 4:27:21 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __SYSTEM_SINGLE_CLR_H_DF1B0AFB_7266_421C_80C7_A942D42F733C_
#define __SYSTEM_SINGLE_CLR_H_DF1B0AFB_7266_421C_80C7_A942D42F733C_

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

	/// Single colour system shader 
	///		- Parameters
	///			-# view_proj_matrix  : matrix4x4  : concatenated view * projection matrices
	///			-# world_matrix      : matrix4x4  : world matrix
	///			-# light_dir         : vector4	  : light direction
	///			-# light_clr         : vector4	  : light colour
	///         -# obj_clr	         : vector4    : object colour
	///
	///		- Vertex Requirements
	///			-# position : vector4
	///			-# color    : rgba
    class TYCHO_DX9_ABI system_single_clr : public graphics::shader::shader_base
    {
    public:
		system_single_clr(dx9::render_interface *ri);
		~system_single_clr();
				
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

#endif // __SYSTEM_SINGLE_CLR_H_DF1B0AFB_7266_421C_80C7_A942D42F733C_
