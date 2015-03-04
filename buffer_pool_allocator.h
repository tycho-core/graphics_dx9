//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 2:56:53 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __BUFFER_POOL_ALLOCATOR_H_56FB63DB_F293_497A_9193_72AAF7DAA4D2_
#define __BUFFER_POOL_ALLOCATOR_H_56FB63DB_F293_497A_9193_72AAF7DAA4D2_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/dx9_abi.h"
#include "common.h"
#include <vector>
#include <list>

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
	
	/// this maintains a list of typeless buffers allocating chunks out of them as it needs.
	/// \warning currently it does not have any support for recycling freed blocks so in use 
	/// you should maintain separate buffer pools that have their lifetimes managed .
	/// i.e.global static, level static and dumping thelevel one after each game.
	/// \todo Recycle freed blocks
    class TYCHO_DX9_ABI buffer_pool_allocator
    {	
    public:
		/// VB creation flags.
		enum create_flags {
			cf_invalid = 0,		   ///< Invalid flags.
			cf_none    = (1 << 1), ///< No special requirements.
			cf_unique  = (1 << 2), /**< Specify to guarantee your own whole buffer. This is needed
										if you ever want to lock discard on it. If you do not
										you'll trash any other data in the buffer. */			
			cf_readable = (1 << 3), ///< Specify if you want to read from the buffer.
			cf_sysmem	= (1 << 5), ///< Keep in system memory				   
		};
    
    public:
	    /// block allocation 
	    struct block {
		    d3d_resource_ptr m_buffer;	///< actual vertex buffer.
		    int			m_size,			///< size in bytes.
						m_free_space,	///< space available in block.
						m_next_space;	///< offset to next free space.
	    };

	    /// pool manages buffers of the same usage and storage.
	    struct pool {
		    typedef std::vector<block*> block_list;

		    block_list	   m_blocks;		///< Blocks in this pool
		    bool		   m_unique;		///< Is this a unique buffer?
		    D3DPOOL		   m_dx_pool;		///< D3d pool type
		    DWORD		   m_dx_usage;		///< D3d usage flags.
		    int			   m_create_flags;	///< Flags buffer was created with.
	    };

    public:
	    /// Tag describing a buffer section
	    struct tag {
			buffer_pool_allocator* m_allocator; ///< allocator we were created with
		    pool*	 m_pool;	///< Pool we are in.
		    block*	 m_block;	///< Block we are using.
		    int		 m_size;	///< Size of our block.
		    int		 m_offset;	///< Offset into block we start.
		    bool	 m_isValid; ///< If this is false the device has been reset.
#if TYCHO_DEBUG
			core::ptr_type m_callstack[16]; ///< call stack when this tag was created.		    
			int m_stack_depth;
#endif // TYCHO_DEBUG		
	    };

	    /// Internal configuration
	    enum config {
		    DefaultBlockSize = 65536, ///< Default buffer block size. 64k.
	    };

    public:
		/// default constructor
	    buffer_pool_allocator();
	    
	    /// destructor
	    ~buffer_pool_allocator();

		/// release all allocated buffers
		void clear();
		
		/// initialise, must be called before any other functions.
        void initialise(dx9::render_interface*);
        
        /// allocate some space out of a buffer
	    tag* alloc(DWORD usage, D3DPOOL pool, int size, int alignment, int create_flags);
	    
	    /// free a previously allocated block
	    bool free(tag *);
	    
	    /// Destroy the DX buffers
	    bool destroy_device_objects();
	    
	    /// Invalidate all dx objects, normally in response to a lost device
	    bool invalidate_device_objects();
	    
	    /// Recreate all dx objects, normally in response to a reset device
	    bool restore_device_objects();

		/// Interface function to create a dx buffer, allows most of the code to be reused between 
		/// vertex and index buffers
		virtual d3d_resource_ptr create_buffer(int len, int usage, D3DPOOL pool) = 0;

		/// dump all allocated tags
	    void dump_allocated_tags(void);	    
		
    private:
	    typedef std::vector<pool*> pool_list;
	    typedef std::list<tag*>	 tag_list;

	    pool_list 	m_pools; ///< vertex buffer pools.
	    tag_list	m_tags;	 ///< tags of allocated buffers.
	   
	protected:
	    dx9::render_interface* m_renderer;

    private:
	    pool* get_pool(DWORD usage, D3DPOOL pool, int create_flags);
	    tag*  get_space(pool *pool, const int size, const int alignment);
	    void iterate_blocks(bool device_lost);
    	
	    /// Functor to delete non managed pool when device is lost
	    void device_lost_cb(pool *, block *);
	    void device_restored_cb(pool *, block *);
	};

} // end namespace
} // end namespace
} // end namespace

#endif // __BUFFER_POOL_ALLOCATOR_H_56FB63DB_F293_497A_9193_72AAF7DAA4D2_
