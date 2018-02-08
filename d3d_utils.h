//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 12:48:06 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __D3D_UTILS_H_EB546F2B_4071_4E9F_84C4_AF154B985E2E_
#define __D3D_UTILS_H_EB546F2B_4071_4E9F_84C4_AF154B985E2E_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "graphics/format.h"
#include "graphics/render_op.h"
#include "graphics_dx9/common.h"
#include "core/colour/rgba.h"


//////////////////////////////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////////////////////////////

#define TYCHO_DXTRY(_call)  { \
    HRESULT hr;  \
    if(FAILED(hr = _call)) {  \
        /*HILOG( #_call << " : Failed : " << GetD3DErrorString(hr));*/ \
        return false;  \
    }}        
    
//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// convert tycho format to closest d3d format
	TYCHO_DX9_ABI D3DFORMAT tycho_to_d3d(graphics::format);
	
	/// convert d3d format to closest tycho format
	TYCHO_DX9_ABI graphics::format d3d_to_tycho(D3DFORMAT);
	
	/// convert tycho primitive type to d3d format
	TYCHO_DX9_ABI D3DPRIMITIVETYPE tycho_to_d3d(graphics::primitive_type);
	
	/// convert rgba colour to d3d format
	TYCHO_DX9_ABI D3DCOLOR tycho_to_d3d(core::rgba);
	
} // end namespace
} // end namespace
} // end namespace

#endif // __D3D_UTILS_H_EB546F2B_4071_4E9F_84C4_AF154B985E2E_
