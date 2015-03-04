//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Sunday, 27 April 2008 6:11:13 PM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __PROGRAM_CONFIG_OPTIONS_H_6804F175_7BA3_4BA3_BED5_17D08ADD6D8F_
#define __PROGRAM_CONFIG_OPTIONS_H_6804F175_7BA3_4BA3_BED5_17D08ADD6D8F_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/dx9_abi.h"
#include "core/program_config.h"
#include "core/globals.h"

//////////////////////////////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////////////////////////////

namespace tycho
{
namespace graphics
{
namespace dx9
{

	/// dx9 command line options
	struct program_config_options
	{
		program_config_options() : 
			trace_d3d_resource(false), 
			shader_debug(false)
		{
			typedef tycho::core::program_config pc;
			static pc::option_pair options[] = {
				pc::opt("check_leaks", 0, "Enable resource leak tracking", "1", &trace_d3d_resource),
				pc::opt("shader_debug", 0, "Enable dx shader debugging when compiling ", "0", &shader_debug),
				pc::null_opt()
			};						
			tycho::core::globals::get_config()->process_option_group("dx9", "DirectX 9 renderer options", options);				
		}
		bool trace_d3d_resource;
		bool shader_debug;
	};
	const program_config_options& global_options();

} // end namespace
} // end namespace
} // end namespace

#endif // __PROGRAM_CONFIG_OPTIONS_H_6804F175_7BA3_4BA3_BED5_17D08ADD6D8F_
