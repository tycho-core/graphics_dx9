//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Saturday, 19 April 2008 6:14:27 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RESOURCE_TRACKER_H_9399AB24_89B0_4057_ADEA_D8727D2B8B05_
#define __RESOURCE_TRACKER_H_9399AB24_89B0_4057_ADEA_D8727D2B8B05_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "graphics_dx9/common.h"
#include <vector>
#include <string>

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	class tracked_resource;

	class resource_tracker
	{
	public:
		~resource_tracker();
		
		void add(tracked_resource *);
		void remove(tracked_resource *);		
		void dump_allocated() const;			
		int  dump_default_pool_allocations() const;
		void track(IDirect3DResource9 *);

	private:
		typedef std::vector<tracked_resource*> resource_list;		
		resource_list m_resources;
	};

	/// Used with d3d resources to track allocation and freeing so
	/// we can report unreleased resources
	class tracked_resource : public IUnknown
	{	
	public:
		tracked_resource(resource_tracker *tracker, D3DRESOURCETYPE, const std::string &context, D3DPOOL pool);
		~tracked_resource();
		
		STDMETHOD(QueryInterface)(REFIID, void**) { return E_NOTIMPL; }
		STDMETHOD_(ULONG, AddRef)(THIS);
		STDMETHOD_(ULONG, Release)(THIS);
		
		D3DRESOURCETYPE get_type() const { return m_type; }
		const std::string& get_callstack() const { return m_stacktrace; }
		D3DPOOL get_pool() const { return m_pool; }

	private:
		resource_tracker*		m_tracker;
		std::string				m_stacktrace;
		D3DRESOURCETYPE			m_type;
		ULONG					m_ref_count;
		D3DPOOL					m_pool;
	};
		
} // end namespace
} // end namespace
} // end namespace

#endif // __RESOURCE_TRACKER_H_9399AB24_89B0_4057_ADEA_D8727D2B8B05_
