//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 13 April 2008 1:43:30 AM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "vertex_declaration.h"
#include "graphics/vertex_format.h"
#include "graphics_dx9/d3d_types.h"
#include "graphics_dx9/render_interface.h"
#include "core/memory.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{
	int tycho_to_d3d(vertex_format::semantic_type t)
	{
		switch(t)
		{
			case vertex_format::s_position : return D3DDECLUSAGE_POSITION;
			case vertex_format::s_texcoord : return D3DDECLUSAGE_TEXCOORD;
			case vertex_format::s_tangent  : return D3DDECLUSAGE_TANGENT;
			case vertex_format::s_normal   : return D3DDECLUSAGE_NORMAL;
			case vertex_format::s_binormal : return D3DDECLUSAGE_BINORMAL;
			case vertex_format::s_diffuse  : return D3DDECLUSAGE_COLOR;
			case vertex_format::s_specular : return D3DDECLUSAGE_COLOR;
			case vertex_format::s_bone_indices : return D3DDECLUSAGE_BLENDINDICES;
			case vertex_format::s_bone_weights : return D3DDECLUSAGE_BLENDWEIGHT;

			//case s_ambient_occlusion
		}
		
		TYCHO_ASSERT(!"Invalid semantic type");
		return -1;
	}
	
	/// constructor
	vertex_declaration::vertex_declaration(dx9::render_interface *ri) :
		m_ri(ri)
	{}
	
	bool vertex_declaration::initialise(const graphics::vertex_format **fmts, int num_fmts)
	{	
		// grab some stack space to store the d3d elements
		int num_semantics = 0;
		for(int i = 0; i < num_fmts; ++i)
			num_semantics += fmts[i]->get_num_semantics();		
		D3DVERTEXELEMENT9 *d3d_elements = reinterpret_cast<D3DVERTEXELEMENT9*>(TYCHO_ALLOCA((num_semantics + 1) * sizeof(D3DVERTEXELEMENT9)));
		
		// terminate element array
		core::mem_zero(d3d_elements[num_semantics]);
		d3d_elements[num_semantics].Stream = 0xFF;
		d3d_elements[num_semantics].Type   = D3DDECLTYPE_UNUSED;
		D3DVERTEXELEMENT9* cur_element = d3d_elements;
		// convert all streams
		for(int stream = 0; stream < num_fmts; ++stream)
		{
			const graphics::vertex_format* format = fmts[stream];
			// convert all tycho vertex components into d3d ones
			for(int s = 0; s < num_semantics; ++s, ++cur_element)
			{
				const vertex_format::semantic_decl&  sdecl = format->get_semantic(s);
				const vertex_format::component_decl& decl = format->get_component(s);
				int offset = format->get_component_offset(s);
				int type = -1;
				cur_element->Stream = static_cast<WORD>(stream);
				switch(decl.type)
				{
					case vertex_format::ct_float			: type = D3DDECLTYPE_FLOAT1 + (decl.size - 1); break;					
					case vertex_format::ct_clr				: type = D3DDECLTYPE_D3DCOLOR; break;
					case vertex_format::ct_double			: type = D3DDECLTYPE_UNUSED; break;					
					case vertex_format::ct_int8				: type = D3DDECLTYPE_UNUSED; break;
					case vertex_format::ct_int16			: 
					case vertex_format::ct_int32			: 
					case vertex_format::ct_uint8			: 
					case vertex_format::ct_uint16			: 
					case vertex_format::ct_uint32			: 
					case vertex_format::ct_fixed32_16_16	: 
					case vertex_format::ct_fixed16_8_8		: 
					case vertex_format::ct_normalized_int8	: 
					case vertex_format::ct_normalized_int16	: 
					case vertex_format::ct_normalized_int32	: 
					case vertex_format::ct_hdr_clr			: 
					default									: TYCHO_ASSERT(!"implemented"); return false;
				}
				
				TYCHO_ASSERT(type  >= 0);
				TYCHO_ASSERT(offset >= 0);
				
				cur_element->Offset = static_cast<WORD>(offset);
				cur_element->Type   = static_cast<BYTE>(type);
				cur_element->Usage  = static_cast<BYTE>(tycho_to_d3d(sdecl.type));
				cur_element->UsageIndex = 0;
				cur_element->Method = D3DDECLMETHOD_DEFAULT;
			}
		}
				
		IDirect3DVertexDeclaration9* out_decl;
		HRESULT hr = m_ri->get_d3d_device()->CreateVertexDeclaration(d3d_elements, &out_decl);
		if(FAILED(hr))
			return false;
			
		m_d3d_vd.Attach(out_decl);
		return true;
	}
		
	
	/// \returns the d3d vertex declaration
	d3d_vd_ptr vertex_declaration::get_d3d()
	{
		return m_d3d_vd;
	}


} // end namespace
} // end namespace
} // end namespace
