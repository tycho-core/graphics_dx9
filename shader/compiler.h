//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 12 April 2008 7:57:07 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __COMPILER_H_46F3A3A3_4A3A_49A2_B7D2_3E3F35DF1965_
#define __COMPILER_H_46F3A3A3_4A3A_49A2_B7D2_3E3F35DF1965_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "graphics_dx9/d3d_types.h"

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

	/// HLSL compiler interface
    class TYCHO_DX9_ABI compiler
    {
    public:
		compiler(dx9::render_interface *ri);
		
		d3d_vs_ptr compile_vertex_shader(const char* str, int len, const char* entry_point);
		d3d_ps_ptr compile_pixel_shader(const char* str, int len, const char* entry_point);
		
    private:
		dx9::render_interface* m_ri;
    };

} // end namespace
} // end namespace
} // end namespace
} // end namespace

#endif // __COMPILER_H_46F3A3A3_4A3A_49A2_B7D2_3E3F35DF1965_
