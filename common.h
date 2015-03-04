//////////////////////////////////////////////////////////////////////////////
// Tycho Game Library
// Copyright (C) 2008 Martin Slater
// Created : Wednesday, 2 April 2008 12:54:14 AM
//////////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER

#ifndef __COMMON_H_58F4922E_AEFA_4ED3_8572_C5F393B7D070_
#define __COMMON_H_58F4922E_AEFA_4ED3_8572_C5F393B7D070_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dx9/dx9_abi.h"
#include "core/debug/assert.h"
#include "core/pc/windows_common.h"
#include <atlbase.h> // for CComPtr
#if TYCHO_DEBUG
#define D3D_DEBUG_INFO
#endif
#include "d3d9.h"
#include "d3dx9.h"
#include "d3d_types.h"
#include "core/pc/windows_undefs.h"

#endif // __COMMON_H_58F4922E_AEFA_4ED3_8572_C5F393B7D070_
