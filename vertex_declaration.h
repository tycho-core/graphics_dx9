//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 13 April 2008 1:43:29 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __VERTEX_DECLARATION_H_7D99325D_0451_4581_A0A0_37C71DC63EAF_
#define __VERTEX_DECLARATION_H_7D99325D_0451_4581_A0A0_37C71DC63EAF_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "graphics_dx9/d3d_types.h"
#include "graphics/forward_decls.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
	class vertex_format;
	
namespace dx9
{
	class render_interface;

	/// dx9 vertex declaration. This is constructed from a \ref graphics::vertex_format
    class TYCHO_DX9_ABI vertex_declaration
    {
    public:
		/// constructor
		vertex_declaration(dx9::render_interface *ri);
		
		/// create the declaration from a tycho vertex format
		bool vertex_declaration::initialise(const graphics::vertex_format **fmts, int count);
		
		/// \returns the d3d vertex declaration
		d3d_vd_ptr get_d3d();
		
    private:
		d3d_vd_ptr m_d3d_vd;
		dx9::render_interface* m_ri;
    };
    
    TYCHO_DECLARE_SHARED_PTR(TYCHO_DX9_ABI, vertex_declaration)

} // end namespace
} // end namespace
} // end namespace

#endif // __VERTEX_DECLARATION_H_7D99325D_0451_4581_A0A0_37C71DC63EAF_
