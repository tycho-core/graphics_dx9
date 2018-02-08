//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 2:56:53 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "buffer_pool_allocator.h"
#include "graphics_dx9/render_interface.h"
#include "graphics_dx9/d3d_utils.h"
#include "core/memory.h"
#include "core/string.h"
#include "core/debug/utilities.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{

	buffer_pool_allocator::buffer_pool_allocator(void) :
		m_renderer(0)
	{
	}

	buffer_pool_allocator::~buffer_pool_allocator(void)
	{
		clear();
	}
	
	/// release all allocated buffers
	void buffer_pool_allocator::clear()
	{	
		// free all pools and blocks
		pool_list::iterator it = m_pools.begin();
		pool_list::iterator end = m_pools.end();
		for(;it != end; ++it)
		{
			pool* p = *it;
			
			pool::block_list::iterator bit = p->m_blocks.begin();
			pool::block_list::iterator bend = p->m_blocks.end();
			for(;bit != bend; ++bit)
				delete *bit;
			
			delete p;
		}
		
		// free tags
		tag_list::iterator tit = m_tags.begin();
		tag_list::iterator tend = m_tags.end();
		for(;tit != tend; ++tit)
			delete *tit;
		
		m_pools.clear();
		m_tags.clear();
	}

	void buffer_pool_allocator::initialise(dx9::render_interface *renderer)
	{
		TYCHO_ASSERT(renderer);	    
		m_renderer = renderer;
	}

	/// Allocate some vertex buffer space. 
	buffer_pool_allocator::tag* buffer_pool_allocator::alloc(DWORD usage, D3DPOOL d3dpool, int size, int alignment, int create_flags)
	{
		TYCHO_ASSERT(usage);
		TYCHO_ASSERT(size);
		TYCHO_ASSERT(alignment);
		TYCHO_ASSERT(create_flags);

		// Find pool to put in
		pool *pool = get_pool(usage, d3dpool, create_flags);

		if(!pool) {
			return false;
		}

		// Get tag
		tag *tag = get_space(pool, size, alignment);

		if(!tag) {
			return false;
		}
#if TYCHO_DEBUG		
		tag->m_stack_depth = core::debug::stacktrace(tag->m_callstack, 16);
#endif // TYCH_DEBUG


		return tag;
	}

	/// Free the space used by tag.
	/// \todo Space is just released, need some sort of free list.
	bool buffer_pool_allocator::free(tag *tag)
	{
		TYCHO_ASSERT(tag);

		m_tags.remove(tag);
		core::safe_delete(tag);
		
		return true;
	}

	/// Dumps file and line info about all unallocated tags to the log.
	void buffer_pool_allocator::dump_allocated_tags(void)
	{
#if TYCHO_DEBUG	
		using namespace tycho::core;
		
		if(m_tags.empty())
			return;
			
		debug::write_ln("----------------------------------------------------------------");
		debug::write_ln("| D3D Tag Leak Report                                          |");
		debug::write_ln("----------------------------------------------------------------");
		char name_buf[debug::SymbolStringMaxLength];
		char file_buf[debug::SymbolStringMaxLength];
		const int BufLen = debug::SymbolStringMaxLength * 2 + 32;
		char buf[BufLen];
		int line = 0;
		tag_list::iterator it = m_tags.begin();
		while(it != m_tags.end()) {
			tag *tag = *it;
			TYCHO_ASSERT(tag);
			debug::write_ln("size=%d pool=%d@", tag->m_size, tag->m_pool);
			for(int i = 0; i < tag->m_stack_depth; ++i)
			{
				if(core::debug::convert_address_to_function(tag->m_callstack[i], name_buf, file_buf, &line))
				{
					core::snprintf(buf, BufLen, "\t\t%s(%d) : %s", file_buf, line, name_buf);
					core::debug::write_ln(buf);
				}
			}
			it++;
		}
#endif // TYCHO_DEBUG		
	}

	/// Get an appropriate pool for the data. Creates one if it does not already exist.
	/// \returns Pool or 0 on failure.
	buffer_pool_allocator::pool* buffer_pool_allocator::get_pool(DWORD d3dusage, D3DPOOL d3dpool, int create_flags)
	{
		TYCHO_ASSERT(d3dusage);
		TYCHO_ASSERT(create_flags);

		pool *result = 0;

		// If unique just create a pool and return it
		if(!(create_flags & cf_unique)) 
		{
			// Search for a suitable pool
			size_t num_pools = m_pools.size();

			for(size_t i = 0; i < num_pools; i++) 
			{
				pool *p = m_pools[i];
				
				TYCHO_ASSERT(p);	

				if(!p->m_unique) 
				{
					if(p->m_dx_pool == d3dpool && p->m_dx_usage == d3dusage && create_flags == p->m_create_flags) 
					{
						// Found a pool
						result = p;
						
						break;
					}
				}
			}
		}

		// If we didn't find one create a new one
		if(!result) 
		{
			result = new pool;

			result->m_unique	   = (create_flags & cf_unique) ? true : false;
			result->m_create_flags = create_flags;
			result->m_dx_pool      = d3dpool;
			result->m_dx_usage     = d3dusage;

			// Add to list
			m_pools.push_back(result);
		}

		TYCHO_ASSERT(result);

		return result;
	}

	/// Get an available block from the specified pool. If a suitable block doesn't exist
	///  it will be created. 
	/// \param from_pool		Pool to allocate from
	/// \param size		Length in bytes to allocate.
	/// \param alignment	Alignment boundary to allocate on.
	/// \returns Block or zero on failure.
	buffer_pool_allocator::tag* buffer_pool_allocator::get_space(pool *from_pool, const int size, const int alignment)
	{
		TYCHO_ASSERT(from_pool);
		TYCHO_ASSERT(size);
		TYCHO_ASSERT(alignment);

		block *result_block = 0;
		int	 offset = 0; // Offset into buffer to allocate from.

		// Search existing blocks for a suitable space
		size_t num_blocks = from_pool->m_blocks.size();

		for(size_t i = 0; i < num_blocks; i++) 
		{
			block *b = from_pool->m_blocks[i];

			TYCHO_ASSERT(b);
			TYCHO_ASSERT(b->m_buffer);
			TYCHO_ASSERT(b->m_size);
			TYCHO_ASSERT(b->m_free_space - b->m_size);

			// Get offset into buffer taking alignment into account
			offset = b->m_next_space + (alignment - b->m_next_space % alignment);

			if((b->m_size - offset) >= size) 
			{
				result_block = b;
				break;
			}
		}

		// No available block so create a new one.
		if(!result_block) {
			result_block = new block;

			// Allocate buffer
			int buffer_len = (size > DefaultBlockSize) ? size : DefaultBlockSize;

			TYCHO_ASSERT(buffer_len);

			// If we are creating a non managed pool buffer we need to flush vram of
			// managed resources so we have the best chance of having space. This
			// also lets the resource manager keep track of vram usage.
			if(from_pool->m_dx_pool == D3DPOOL_DEFAULT)
				TYCHO_DXTRY(m_renderer->get_d3d_device()->EvictManagedResources());

			// create a new buffer
			result_block->m_buffer = create_buffer(buffer_len, from_pool->m_dx_usage, from_pool->m_dx_pool);
			m_renderer->get_resource_tracker().track(result_block->m_buffer);			

			TYCHO_ASSERT(result_block->m_buffer);

			// Fill out block description
			result_block->m_size = buffer_len;
			offset		  = 0;

			// Add to list
			from_pool->m_blocks.push_back(result_block);
		}

		// Initialise tag info.
		tag *new_tag = new tag;
		new_tag->m_allocator = this;
		new_tag->m_block  = result_block;
		new_tag->m_pool   = from_pool;
		new_tag->m_offset = offset;
		new_tag->m_size   = size;

		TYCHO_ASSERT((offset % alignment) == 0);
		// Add to tag list
		m_tags.push_back(new_tag);

		// Update block info
		result_block->m_next_space = offset + size;
		result_block->m_free_space = result_block->m_size - result_block->m_next_space;
		
		TYCHO_ASSERT(new_tag);

		return new_tag;
	}


	/// Functor to delete non managed pool when device is lost
	void buffer_pool_allocator::device_lost_cb(buffer_pool_allocator::pool *p, buffer_pool_allocator::block *b)
	{
		TYCHO_ASSERT(b);
		TYCHO_ASSERT(p);

		if(p->m_dx_pool == D3DPOOL_DEFAULT)
			b->m_buffer.Release();
	}

	/// Functor called when restoring buffers after device lost
	void buffer_pool_allocator::device_restored_cb(buffer_pool_allocator::pool *p, buffer_pool_allocator::block *b) 
	{
		TYCHO_ASSERT(b);
		TYCHO_ASSERT(p);

		// only need restore default pool blocks.
		if(p->m_dx_pool == D3DPOOL_DEFAULT)
			b->m_buffer = create_buffer(b->m_size, p->m_dx_usage, p->m_dx_pool);
	}


	/// Called when the d3d device gets lost.
	bool buffer_pool_allocator::invalidate_device_objects()
	{
		iterate_blocks(true);
		return true;
	}

	/// Called after we regain the device.
	bool buffer_pool_allocator::restore_device_objects()
	{
		iterate_blocks(false);
		return true;
	}  

	void buffer_pool_allocator::iterate_blocks(bool device_lost) 
	{
		// Recreate all default pool buffers
		pool_list::iterator pool_it = m_pools.begin();

		while(pool_it != m_pools.end()) 
		{
			pool::block_list::iterator block_it = (*pool_it)->m_blocks.begin();

			while(block_it != (*pool_it)->m_blocks.end()) 
			{
				if(device_lost)
					device_lost_cb(*pool_it, *block_it);
				else
					device_restored_cb(*pool_it, *block_it);

				block_it++;
			}

			pool_it++;
		}
	}

} // end namespace
} // end namespace
} // end namespace
