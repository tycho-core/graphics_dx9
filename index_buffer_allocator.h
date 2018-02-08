//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:03:11 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __IB_ALLOCATOR_H_0C5D28D7_CE94_4EB3_B022_2046EE36672C_
#define __IB_ALLOCATOR_H_0C5D28D7_CE94_4EB3_B022_2046EE36672C_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "graphics/index_buffer.h"
#include "graphics_dx9/buffer_pool_allocator.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// index buffer allocator. this maintains few large ib's behind the scenes and fulfills
	/// allocation requests from them.
    class TYCHO_DX9_ABI index_buffer_allocator : public buffer_pool_allocator
    {
    public:
		typedef buffer_pool_allocator::tag tag;

		virtual d3d_resource_ptr create_buffer(int len, int usage, D3DPOOL);
	    tag* alloc(DWORD usage, D3DPOOL pool, index_buffer::format format, int num_indices, int create_flags);
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __IB_ALLOCATOR_H_0C5D28D7_CE94_4EB3_B022_2046EE36672C_
