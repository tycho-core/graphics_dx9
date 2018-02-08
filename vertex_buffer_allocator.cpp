//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:03:05 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/vertex_buffer_allocator.h"
#include "graphics_dx9/d3d_utils.h"
#include "graphics_dx9/render_interface.h"
#include "graphics_dx9/resource_tracker.h"
#include "core/memory.h"
#include "core/debug/utilities.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{

	d3d_resource_ptr vertex_buffer_allocator::create_buffer(int len, int usage, D3DPOOL pool)
	{
		d3d_resource_ptr new_vb;
		IDirect3DVertexBuffer9* tmp;
		if(SUCCEEDED(m_renderer->get_d3d_device()->CreateVertexBuffer(len, usage, 0, pool, &tmp, NULL)))
		{
			m_renderer->get_resource_tracker().track(tmp);		
			new_vb.Attach(tmp);
		}
		return new_vb;
	}

} // end namespace
} // end namespace
} // end namespace
