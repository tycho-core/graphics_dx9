//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Tuesday, 1 April 2008 11:45:58 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __D3D_TYPES_H_720C10B3_8224_40CE_9236_E38F10FF144C_
#define __D3D_TYPES_H_720C10B3_8224_40CE_9236_E38F10FF144C_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "common.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	typedef ::CComPtr<IDirect3D9>				d3d_ptr;
	typedef ::CComPtr<IDirect3DTexture9>		d3d_texture_ptr;
	typedef ::CComPtr<IDirect3DCubeTexture9>	d3d_cube_texture_ptr;
	typedef ::CComPtr<IDirect3DSurface9>		d3d_surface_ptr;
	typedef ::CComPtr<IDirect3DVertexBuffer9>	d3d_vb_ptr;
	typedef ::CComPtr<IDirect3DIndexBuffer9>	d3d_ib_ptr;
	typedef ::CComPtr<IDirect3DPixelShader9>    d3d_ps_ptr;
	typedef ::CComPtr<IDirect3DVertexShader9>   d3d_vs_ptr;
	typedef ::CComPtr<IDirect3DSwapChain9>		d3d_swap_chain_ptr;
	typedef ::CComPtr<IDirect3DDevice9>			d3d_device_ptr;
	typedef ::CComPtr<IDirect3DResource9>		d3d_resource_ptr;
	typedef ::CComPtr<IDirect3DVertexDeclaration9> d3d_vd_ptr;
	typedef ::CComPtr<ID3DXBuffer>				d3dx_buffer_ptr;
	typedef ::CComPtr<ID3DXConstantTable>		d3dx_constant_table;
	
} // end namespace
} // end namespace
} // end namespace

#endif // __D3D_TYPES_H_720C10B3_8224_40CE_9236_E38F10FF144C_
