//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 7:01:37 PM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "render_state.h"
#include "dx9/render_interface.h"
#include "core/memory.h"
#include "core/debug/assert.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////
namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// constructor
	render_state::render_state(dx9::render_interface* ri) :
		m_ri(ri)
	{
		core::mem_zero(m_states);
	}
	
	/// set all render states to their default values
	void render_state::set_defaults()
	{
	
	}
	
	/// set a render state
	void render_state::set(D3DRENDERSTATETYPE d3d_rs, core::uint32 value)
	{
		TYCHO_ASSERT(value < MaxStates);
		state& s = m_states[d3d_rs];
		if(s.set == false || s.value != value)
		{	
			m_ri->get_d3d_device()->SetRenderState(d3d_rs, value);
			s.value = value;
			s.set = true;
		}
	}


} // end namespace
} // end namespace
} // end namespace
