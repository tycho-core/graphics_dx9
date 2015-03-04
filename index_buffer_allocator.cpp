//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:03:11 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/index_buffer_allocator.h"
#include "dx9/render_interface.h"
#include "dx9/resource_tracker.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{

	d3d_resource_ptr index_buffer_allocator::create_buffer(int len, int usage, D3DPOOL pool)
	{
		d3d_resource_ptr new_ib;
		IDirect3DIndexBuffer9* tmp;
		if(SUCCEEDED(m_renderer->get_d3d_device()->CreateIndexBuffer(len, usage, D3DFMT_INDEX16, pool, &tmp, NULL)))
		{
			m_renderer->get_resource_tracker().track(tmp);
			new_ib.Attach(tmp);
		}
		return new_ib;	
	}
	
    index_buffer_allocator::tag* index_buffer_allocator::alloc(DWORD usage, D3DPOOL pool, index_buffer::format format, int num_indices, int create_flags)
    {
		const int bytes_per_index = index_buffer::get_num_bytes(format);
		return buffer_pool_allocator::alloc(usage, pool, num_indices * bytes_per_index, bytes_per_index, create_flags);
    }

} // end namespace
} // end namespace
} // end namespace
