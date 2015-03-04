//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 13 April 2008 2:26:28 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "vertex_declaration_factory.h"
#include "graphics/vertex_format.h"
#include "dx9/vertex_declaration.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// constructor 
	vertex_declaration_factory::vertex_declaration_factory(dx9::render_interface* ri) :
		m_ri(ri)
	{}

	/// get a vertex declaration matching all the passed vertex formats
	vertex_declaration_ptr vertex_declaration_factory::get_d3d_decl(const graphics::vertex_format **fmts, int num_fmts)
	{
		// \todo accumulate hashes of all fmts and see if we already have one to match this set
		
		// get the hash for the components in the format
//		core::uint32 guid = format->get_components_guid();
		
		vertex_declaration_ptr decl = vertex_declaration_ptr(new dx9::vertex_declaration(m_ri));
		bool res = decl->initialise(fmts, num_fmts);
		TYCHO_ASSERT(res);
		return decl;
	}

} // end namespace
} // end namespace
} // end namespace
