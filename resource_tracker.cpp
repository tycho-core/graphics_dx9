//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 19 April 2008 6:14:28 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "resource_tracker.h"
#include "graphics_dx9/program_config_options.h"
#include "core/program_config.h"
#include "core/globals.h"
#include "core/string.h"
#include "core/debug/utilities.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{
	using namespace tycho::core;

	const char* to_string(D3DRESOURCETYPE rt)
	{
		switch(rt)
		{
			case D3DRTYPE_SURFACE		: return "surface";
			case D3DRTYPE_VOLUME		: return "volume";
			case D3DRTYPE_TEXTURE		: return "texture";
			case D3DRTYPE_VOLUMETEXTURE : return "volumn_texture";
			case D3DRTYPE_CUBETEXTURE	: return "cube_texture";
			case D3DRTYPE_VERTEXBUFFER	: return "vertex_buffer";
			case D3DRTYPE_INDEXBUFFER	: return "index_buffer";
		}
	
		return "<unknown>";
	}
	
	
	//////////////////////////////////////////////////////////////////////////////
	// resource_tracker implementation
	//////////////////////////////////////////////////////////////////////////////

	resource_tracker::~resource_tracker()
	{
		dump_allocated();
	}

	void resource_tracker::add(tracked_resource *res)
	{
		m_resources.push_back(res);
	}
	
	void resource_tracker::remove(tracked_resource *res)
	{
		resource_list::iterator it = std::find(m_resources.begin(), m_resources.end(), res);
		
		TYCHO_ASSERT(it != m_resources.end());
		
		if(it != m_resources.end())
		{
			m_resources.erase(it);
		}
	}
	
	void resource_tracker::track(IDirect3DResource9 *res)
	{
		if(dx9::global_options().trace_d3d_resource)
		{
			const int ss = 16;
			core::ptr_type callstack[ss];
			int depth = debug::stacktrace(callstack, ss);
			std::string trace;
			for(int i = 0; i < depth; ++i)
			{
				char name_buf[debug::SymbolStringMaxLength];
				char file_buf[debug::SymbolStringMaxLength];
				const int BufLen = debug::SymbolStringMaxLength * 2 + 32;
				char buf[BufLen];
				int line = 0;
				if(debug::convert_address_to_function(callstack[i], name_buf, file_buf, &line))
				{
					core::snprintf(buf, BufLen, "\t\t%s(%d) : %s\n", file_buf, line, name_buf);
					trace.append(buf);
				}
			}
			
			D3DPOOL pool = D3DPOOL_DEFAULT;
			D3DRESOURCETYPE res_type = res->GetType();

			switch(res_type)
			{
				case D3DRTYPE_SURFACE : {
					D3DSURFACE_DESC desc;
					((IDirect3DSurface9*)res)->GetDesc(&desc);
					pool = desc.Pool;
				} break;

				case D3DRTYPE_VOLUME : {
					D3DVOLUME_DESC desc;
					((IDirect3DVolume9*)res)->GetDesc(&desc);
					pool = desc.Pool;
				} break;

				case D3DRTYPE_TEXTURE : {
					D3DSURFACE_DESC desc;
					((IDirect3DTexture9*)res)->GetLevelDesc(0, &desc);
					pool = desc.Pool;
				} break;

				case D3DRTYPE_VOLUMETEXTURE : {
					D3DVOLUME_DESC desc;
					((IDirect3DVolumeTexture9*)res)->GetLevelDesc(0, &desc);
					pool = desc.Pool;
				} break;
				
				case D3DRTYPE_CUBETEXTURE : {
					D3DSURFACE_DESC desc;
					((IDirect3DCubeTexture9*)res)->GetLevelDesc(0, &desc);
					pool = desc.Pool;
				} break;

				case D3DRTYPE_VERTEXBUFFER : {
					D3DVERTEXBUFFER_DESC desc;
					((IDirect3DVertexBuffer9*)res)->GetDesc(&desc);
					pool = desc.Pool;
				} break;
		
				case D3DRTYPE_INDEXBUFFER : {
					D3DINDEXBUFFER_DESC desc;
					((IDirect3DIndexBuffer9*)res)->GetDesc(&desc);
					pool = desc.Pool;
				} break;
			}

			tracked_resource *tracker = new tracked_resource(this, res_type, trace, pool);			
			res->SetPrivateData(IID_IViewObject, tracker, sizeof(IUnknown), D3DSPD_IUNKNOWN);
		}
	}
		
	void resource_tracker::dump_allocated() const
	{
		if(!m_resources.empty())
		{
			core::debug::write_ln("----------------------------------------------------------------");
			core::debug::write_ln("| D3D Resource Leaks                                           |");			
			core::debug::write_ln("----------------------------------------------------------------");
			for(resource_list::const_iterator it = m_resources.begin(); it != m_resources.end(); ++it)
			{
				core::debug::write_ln("%s :\n%s", to_string((*it)->get_type()), (*it)->get_callstack().c_str());
			}
		}			
	}

	int resource_tracker::dump_default_pool_allocations() const
	{
		int num = 0;

		if(!m_resources.empty())
		{
			for(resource_list::const_iterator it = m_resources.begin(); it != m_resources.end(); ++it)
			{
				if((*it)->get_pool() == D3DPOOL_DEFAULT)
				{
					core::debug::write_ln("%s : %s", to_string((*it)->get_type()), (*it)->get_callstack().c_str());
					++num;
				}
			}
		}			

		return num;
	}

	//////////////////////////////////////////////////////////////////////////////
	// tracked_resource implementation
	//////////////////////////////////////////////////////////////////////////////


	tracked_resource::tracked_resource(resource_tracker *tracker, D3DRESOURCETYPE type, const std::string &stacktrace, D3DPOOL pool) :
		m_tracker(tracker),
		m_ref_count(0),
		m_stacktrace(stacktrace),
		m_type(type),
		m_pool(pool)
	{
		tracker->add(this);
	}
			
	tracked_resource::~tracked_resource()
	{	
		m_tracker->remove(this);
	}
	
	ULONG tracked_resource::AddRef()
	{
		m_ref_count++;
		return m_ref_count;
	}
	
	ULONG tracked_resource::Release()
	{
		m_ref_count--;
		if(!m_ref_count)
		{
			delete this;
			return 0;
		}
		return m_ref_count;
	}

} // end namespace
} // end namespace
} // end namespace

