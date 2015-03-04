//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 13 April 2008 2:26:28 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __VERTEX_DECLARATION_FACTORY_H_D2447CDF_C4B6_440A_8B7D_9DEB47F6A6B1_
#define __VERTEX_DECLARATION_FACTORY_H_D2447CDF_C4B6_440A_8B7D_9DEB47F6A6B1_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/dx9_abi.h"
#include "dx9/forward_decls.h"
#include "graphics/forward_decls.h"
#include <map>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// the vertex declaration factory is responsible for creating vertex declarations,
	/// the declaration return should not be modified as it will weed out duplicates and
	/// return previously created ones if it matches the requested vertex format.
    class TYCHO_DX9_ABI vertex_declaration_factory
    {
    public:
		/// constructor 
		vertex_declaration_factory(dx9::render_interface*);
				
		/// get a vertex declaration matching all the passed vertex formats
		vertex_declaration_ptr get_d3d_decl(const graphics::vertex_format **, int num_fmts);
		    
    private:
		typedef std::map<core::int32, vertex_declaration_ptr> decl_map;
		dx9::render_interface* m_ri;
		decl_map			   m_decls;
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __VERTEX_DECLARATION_FACTORY_H_D2447CDF_C4B6_440A_8B7D_9DEB47F6A6B1_
