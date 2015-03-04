//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 12:48:06 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "d3d_utils.h"
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

	/// convert tycho format to closest d3d format
	TYCHO_DX9_ABI D3DFORMAT tycho_to_d3d(graphics::format fmt)
	{
		switch(fmt.value)
		{
			case format::pixel_argb8888	: return D3DFMT_A8R8G8B8;
			case format::pixel_xrgb8888 : return D3DFMT_X8R8G8B8;
			case format::pixel_xrgb1555	: return D3DFMT_X1R5G5B5;
			case format::pixel_rgb565	: return D3DFMT_R5G6B5;
			case format::pixel_a8       : return D3DFMT_A8;
			case format::depth_16bit	: return D3DFMT_D16;
			case format::depth_24bit	: return D3DFMT_D24X8;
			case format::depth_32bit	: return D3DFMT_D32;
		}
		return D3DFMT_UNKNOWN;
	}
	
	/// convert d3d format to closest tycho format
	TYCHO_DX9_ABI graphics::format d3d_to_tycho(D3DFORMAT fmt)
	{
		switch(fmt)
		{
			case D3DFMT_A8R8G8B8	: return format::pixel_argb8888;
			case D3DFMT_X8R8G8B8	: return format::pixel_xrgb8888;
			case D3DFMT_X1R5G5B5	: return format::pixel_xrgb1555;
			case D3DFMT_R5G6B5		: return format::pixel_rgb565;
			case D3DFMT_D16			: return format::depth_16bit;
			case D3DFMT_D24X8		: return format::depth_24bit;
			case D3DFMT_D32			: return format::depth_32bit;
		}

		return format::unknown;
	}

	/// convert tycho primitive type to d3d format
	TYCHO_DX9_ABI D3DPRIMITIVETYPE tycho_to_d3d(graphics::primitive_type t)
	{
		switch(t)
		{
		    case primitive_type_line_list : return D3DPT_LINELIST;
			case primitive_type_line_strip : return D3DPT_LINESTRIP;
			case primitive_type_tri_list : return D3DPT_TRIANGLELIST;
			case primitive_type_tri_strip : return D3DPT_TRIANGLESTRIP;
		}
		TYCHO_NOT_IMPLEMENTED
		return (D3DPRIMITIVETYPE)0;
	}

	/// convert rgba colour to d3d format
	TYCHO_DX9_ABI D3DCOLOR tycho_to_d3d(core::rgba clr)
	{
		return D3DCOLOR_ARGB(clr.a(), clr.r(), clr.g(), clr.b());
	}

} // end namespace
} // end namespace
} // end namespace
