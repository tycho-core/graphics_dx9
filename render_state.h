//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Monday, 31 March 2008 7:01:37 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __RENDER_STATE_H_3B9F070F_66FA_46E7_A922_60F53652FCEE_
#define __RENDER_STATE_H_3B9F070F_66FA_46E7_A922_60F53652FCEE_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "graphics_dx9/dx9_abi.h"
#include "graphics_dx9/common.h"

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

	/// dumb wrapper over d3d render state settings to avoid setting duplicates until
	/// we maybe get round to trying to use render state blocks.
    class TYCHO_DX9_ABI render_state
    {
    public:
		/// constructor
		render_state(dx9::render_interface*);
		
		/// set all render states to their default values
		void set_defaults();
		
		/// set a render state
		void set(D3DRENDERSTATETYPE, core::uint32 value);
		
    private:
		static const int MaxStates = 255;
		struct state
		{
			core::uint32  value;
			bool set;
		};
		dx9::render_interface* m_ri;
		state m_states[MaxStates];
    };

} // end namespace
} // end namespace
} // end namespace

#endif // __RENDER_STATE_H_3B9F070F_66FA_46E7_A922_60F53652FCEE_
