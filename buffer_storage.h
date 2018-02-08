//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Friday, 4 April 2008 12:40:45 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __BUFFER_STORAGE_H_532D42BE_C9D4_4F57_83CD_DFFD43493660_
#define __BUFFER_STORAGE_H_532D42BE_C9D4_4F57_83CD_DFFD43493660_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "graphics/buffer_storage.h"
#include "graphics_dx9/buffer_pool_allocator.h"

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
	
	/// dx9 buffer storage, templatized to support both vertex and index buffers. 
	/// this points to part of an existing buffer allocated by the relevant allocator.
	template<class D3DType, class AllocatorType, class Base>
    class TYCHO_DX9_ABI buffer_storage : public Base
    {
    public:
		typedef D3DType		  d3d_type;
		typedef AllocatorType allocator_type;
		
    public:
		buffer_storage(render_interface *ri, buffer_pool_allocator::tag* tag, int lock_flags) :
			m_ri(ri),
			m_tag(tag),
			m_lock_flags(lock_flags),
			m_locked(false)
		{
			TYCHO_ASSERT(tag);
		}
				
		~buffer_storage()
		{
			if(m_tag)
			{
				m_tag->m_allocator->free(m_tag);
				m_tag = 0;
			}
		}
		
		void* lock()
		{
			TYCHO_ASSERT(!m_locked);
			if(!m_tag)
			{
				TYCHO_ASSERT(m_tag->m_isValid);
				return 0;
			}
			void *result = 0;
			HRESULT hr;
			// allocator only holds d3d_resource_ptrs, we need to up cast to a vertex buffer here
			d3d_type* vb = (d3d_type*)(m_tag->m_block->m_buffer.p);
			if(FAILED(hr = vb->Lock(m_tag->m_offset, m_tag->m_size, &result, m_lock_flags)))
			{
				TYCHO_PANIC("Dx9::Lock() Failed");
				return 0;
			}
			m_locked = true;
			return result;
		}	
		
		void unlock()
		{
			TYCHO_ASSERT(m_locked);
			HRESULT hr;
			// allocator only holds d3d_resource_ptrs, we need to up cast to a vertex buffer here
			d3d_type* vb = (d3d_type*)(m_tag->m_block->m_buffer.p);
			if(FAILED(hr = vb->Unlock())) 
			{
				TYCHO_PANIC("Dx9::Unlock() Failed");
				return;
			} 		
			m_locked = false;
		}

		/// \returns the size of the buffer in bytes
		int size() const
		{
			TYCHO_ASSERT(m_tag);
			return m_tag->m_size;
		}    
		
		/// \returns the buffers offset in it's buffer pool.
		int get_buffer_offset() const
		{
			TYCHO_ASSERT(m_tag);
			return m_tag->m_offset;
		}
		
    private:
		friend dx9::render_interface;
		render_interface*  m_ri;
		typename allocator_type::tag* m_tag;
		int m_lock_flags;
		bool m_locked;
    };
    

} // end namespace
} // end namespace
} // end namespace

#endif // __BUFFER_STORAGE_H_532D42BE_C9D4_4F57_83CD_DFFD43493660_
