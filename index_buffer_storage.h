//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:59:48 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __INDEX_BUFFER_H_18851F4E_4860_444C_83EF_1B870490D5BA_
#define __INDEX_BUFFER_H_18851F4E_4860_444C_83EF_1B870490D5BA_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/dx9_abi.h"
#include "graphics/index_buffer.h"
#include "graphics/index_buffer_storage.h"
#include "dx9/common.h"
#include "dx9/index_buffer_allocator.h"
#include "dx9/buffer_storage.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// dx9 index buffer. this points to part of an existing buffer allocated by the index_buffer_allocator.
    class TYCHO_DX9_ABI index_buffer_storage : 
		public dx9::buffer_storage<IDirect3DIndexBuffer9, index_buffer_allocator, graphics::buffer_storage::driver_object>
    {
    public:
		index_buffer_storage(render_interface *ri, index_buffer_allocator::tag* tag, int lock_flags) :
			buffer_storage(ri, tag, lock_flags) {}
    private:
		void free_tag(render_interface *ri, buffer_pool_allocator::tag *tag);
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __INDEX_BUFFER_H_18851F4E_4860_444C_83EF_1B870490D5BA_
