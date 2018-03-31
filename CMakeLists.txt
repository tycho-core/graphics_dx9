cmake_minimum_required (VERSION 2.8)

set(link_libs "")
if(ty_platform MATCHES "pc")
	include_directories("${CMAKE_SOURCE_DIR}/3rdparty/pc/dx9/include")
	link_directories("${CMAKE_SOURCE_DIR}/3rdparty/pc/dx9/lib")
	set(link_libs "d3dx9;d3d9;dxguid")

	# embedded shaders for dx9 renderer
	set(shader_dir "${CMAKE_CURRENT_SOURCE_DIR}/shader/hlsl")
	tycho_stringify_command("${shader_dir}/system_single_clr.hlsl" "${shader_dir}/system_single_clr.h")
	tycho_stringify_command("${shader_dir}/system_ui.hlsl" "${shader_dir}/system_ui.h")
	tycho_stringify_command("${shader_dir}/system_ui_string.hlsl" "${shader_dir}/system_ui_string.h")

	tycho_add_library(dx9 "tygraphics;${link_libs}" "libs")
endif()



