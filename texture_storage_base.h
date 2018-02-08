//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 11 August 2008 2:38:51 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __TEXTURE_STORAGE_BASE_H_97CBB5CB_B1EE_4AA5_955F_26C828EACCD0_
#define __TEXTURE_STORAGE_BASE_H_97CBB5CB_B1EE_4AA5_955F_26C828EACCD0_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "graphics_dx9/common.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// base class of all dx9 texture storage implementations
    class TYCHO_DX9_ABI texture_storage_base
    {
    public:
		virtual ~texture_storage_base() {}
		
		/// \returns the direct 3d texture object
		virtual d3d_texture_ptr get_d3d() = 0;
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __TEXTURE_STORAGE_BASE_H_97CBB5CB_B1EE_4AA5_955F_26C828EACCD0_
