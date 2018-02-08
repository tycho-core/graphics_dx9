//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 1:03:05 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __VB_ALLOCATOR_H_F2032A32_BD27_4838_81C0_5F306544B4CB_
#define __VB_ALLOCATOR_H_F2032A32_BD27_4838_81C0_5F306544B4CB_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "buffer_pool_allocator.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// vertex buffer allocator. this maintains few large vb's behind the scenes and fulfills
	/// allocation requests from them.
    class TYCHO_DX9_ABI vertex_buffer_allocator : public buffer_pool_allocator
    {
    public:
		typedef buffer_pool_allocator::tag tag;
	private:
		virtual d3d_resource_ptr create_buffer(int len, int usage, D3DPOOL);
    };
    
} // end namespace
} // end namespace
} // end namespace

#endif // __VB_ALLOCATOR_H_F2032A32_BD27_4838_81C0_5F306544B4CB_
