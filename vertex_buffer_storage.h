//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:59:41 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __VERTEX_BUFFER_H_6119EAC6_9762_4BDA_84D5_5AADD5F52D8E_
#define __VERTEX_BUFFER_H_6119EAC6_9762_4BDA_84D5_5AADD5F52D8E_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "vertex_buffer_allocator.h"
#include "graphics/vertex_buffer_storage.h"
#include "graphics_dx9/buffer_storage.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// dx9 vertex buffer. this points to part of an existing buffer
	/// allocated by the vertex_buffer_allocator.
    class TYCHO_DX9_ABI vertex_buffer_storage : 
		public dx9::buffer_storage<IDirect3DVertexBuffer9, vertex_buffer_allocator, graphics::buffer_storage::driver_object>
    {
    public:
		vertex_buffer_storage(render_interface *ri, buffer_pool_allocator::tag* tag, int lock_flags) :
			buffer_storage(ri, tag, lock_flags) {}
    
    private:
		void free_tag(render_interface *ri, buffer_pool_allocator::tag *tag);
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __VERTEX_BUFFER_H_6119EAC6_9762_4BDA_84D5_5AADD5F52D8E_
