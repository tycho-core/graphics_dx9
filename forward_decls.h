//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 13 April 2008 2:29:41 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __FORWARD_DECLS_H_69AAE37F_14B4_4896_A2D8_1E7E08792EE3_
#define __FORWARD_DECLS_H_69AAE37F_14B4_4896_A2D8_1E7E08792EE3_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "core/shared_ptr.h"
#include "core/intrusive_ptr.h"

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
	
    TYCHO_DECLARE_INTRUSIVE_PTR(TYCHO_DX9_ABI, texture)
    TYCHO_DECLARE_SHARED_PTR(TYCHO_DX9_ABI, vertex_declaration)

} // end namespace
} // end namespace
} // end namespace

#endif // __FORWARD_DECLS_H_69AAE37F_14B4_4896_A2D8_1E7E08792EE3_
