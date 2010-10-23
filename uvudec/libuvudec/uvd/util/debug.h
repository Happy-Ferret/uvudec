/*
UVNet Universal Decompiler (uvudec)
Copyright 2008 John McMaster <JohnDMcMaster@gmail.com>
Licensed under the terms of the LGPL V3 or later, see COPYING for details
*/

#include "uvd/util/error.h"

#ifndef UV_DEBUG_H
#define UV_DEBUG_H

#include <stdio.h>

#define DEBUG_BREAK()			do { printf("DEBUG BREAK (%s:%d)\n", __FILE__, __LINE__); exit(1); } while ( 0 )


//Don't print anything.  A debug statement should not have this, only global level
#define UVD_DEBUG_NONE			0
//A message that should be disabled for production, but is used for current diagnosis
#define UVD_DEBUG_TEMP			1
//Only print high level status messages
#define UVD_DEBUG_PASSES		2
//Main sections being executed
#define UVD_DEBUG_SUMMARY		3
//Something that shouldn't be used anymore...maybe this should be higher level
#define UVD_DEBUG_DEPRECATED	4
//Debug messages from each section
#define UVD_DEBUG_VERBOSE		5
//extern int g_verbose_level;

const char *get_last_func();

/*
It would be cool to set system preferences to compile this selectivly in programs
*/
#define printf_help(format, ...) fprintf(stdout, format, ## __VA_ARGS__)

//#define printf_warn printf_debug
//Or should this be treated as a level 1 error?
#define printf_warn(format, ...) printf("WARNING: " format, ## __VA_ARGS__)
//An error the user should see because they likely caused the issue due to bad input (file, arg, etc)
#define printf_error(format, ...) do { printf("ERROR: " format, ## __VA_ARGS__); fflush(stdout); } while( 0 )
//An internal error
void printf_debug_error(const char *format, ...);

#define printf_deprecated(format, ...) printf_debug_level(UVD_DEBUG_DEPRECATED, format, ## __VA_ARGS__)

/*
uv_assert is only enabled during debugging.
	It is for paranoid error checking
uv_assert_all is always enabled
	It is for checking things like malloc.  Fails on value of false
uv_assert_err is always enabled
	It is like above, except checks for UV_SUCCEDED rather than being false
*/
#define uv_assert_all(x) if( !(x) ) { UV_DEBUG(rc); goto error; }
#define uv_assert_all_ret(x) if( !(x) ) { return UV_DEBUG(rc); }
#define uv_assert_err(x) if( UV_FAILED(x) ) { UV_DEBUG(rc); goto error; }
#define uv_assert_err_ret(x) if( UV_FAILED(x) ) { return UV_DEBUG(UV_ERR_GENERAL); }
#define uv_assert_err_ret_rc(x) if( UV_FAILED(x) ) { return UV_DEBUG(rc); }

//#define NDEBUG
#ifdef NDEBUG 

#define printf_debug(format, ...)
#define printf_debug_level(level, format, ...)
#define uv_assert(x) do{ } while(0)
#define uv_assert_ret(x) do{ } while(0)

/* Disable such messages if not debugging, probably signifigant binary size reduction, speed improvement */
#define UV_DEBUG(x) x
#define UV_ENTER()
#define UV_CHECKED(x)

#else /* ifdef NDEBUG */

#define UVD_DEBUG_TYPE_NONE							0x00000000
//Main engine initializaiton and such
#define UVD_DEBUG_TYPE_GENERAL						0x00000001
//Iteration on instructions and assembly printing
#define UVD_DEBUG_TYPE_ITERATOR						0x00000002
//Config file parsing
#define UVD_DEBUG_TYPE_UVD_ASM_CONFIG				0x00000003
//Actual printing operations of instructions
#define UVD_DEBUG_TYPE_UVD_PRINT					0x00000004
//FLIRT related...will likely break this up into several parts
#define UVD_DEBUG_TYPE_FLIRT						0x00000100
//If your plugin needs to debug, chose something above this
#define UVD_DEBUG_TYPE_PLUGIN_BASE					0x10000000
#define UVD_DEBUG_TYPE_ALL							0xFFFFFFFF

typedef uint32_t uvd_debug_flag_t;
//Return error if type already registered
//argName ex: if "FLIRT" given, --debug-FLIRT will activate this debug flag
uv_err_t UVDRegisterTypePrefix(uvd_debug_flag_t typeFlag, const std::string &argName, const std::string &prefix);
//Get a free flag to register for debugging (and other operations?)
uv_err_t UVDGetTypeFlag(uvd_debug_flag_t *typeFlag);

#define printf_debug(format, ...) printf_debug_level(UVD_DEBUG_VERBOSE, format, ## __VA_ARGS__)
#define printf_debug_type(type, format, ...) printf_debug_macro(UVD_DEBUG_TEMP, type, format, ## __VA_ARGS__)
#define printf_debug_level(level, format, ...) printf_debug_macro(level, UVD_DEBUG_TYPE_ALL, format, ## __VA_ARGS__)
#define printf_debug_macro(level, type, format, ...) printf_debug_core(level, type, __FILE__, __LINE__, __FUNCTION__, format, ## __VA_ARGS__)
void printf_debug_core(uint32_t level, uint32_t type, const char *file, uint32_t line, const char *func, const char *format, ...);

#define uv_assert(x) if( !(x) ) { UV_ERR(rc); goto error; }
#define uv_assert_ret(x) if( !(x) ) { return UV_ERR(UV_ERR_GENERAL); }

uv_err_t uv_err_ret_handler(uv_err_t rc, const char *file, uint32_t line, const char *func);
void uv_enter(const char *file, uint32_t line, const char *func);

#define UV_DEBUG(x) uv_err_ret_handler(x, __FILE__, __LINE__, __FUNCTION__)
//This was an old distinction
#define UV_ERR		UV_DEBUG
#define UV_ENTER() uv_enter(__FILE__, __LINE__, __FUNCTION__)
//Only active on heavily checked debug builds
#define UVD_CHECKED(x)				x

#endif /* ifndef NDEBUG */

uv_err_t UVDDebugInit();
uv_err_t UVDDebugDeinit();
uv_err_t UVDSetDebugFlag(uint32_t flag, uint32_t shouldSet);

#define UVD_BREAK() do { __asm__("INT3"); } while( 0 )
#define UVD_PRINT_STACK() uvd_print_trace(__FILE__, __LINE__, __FUNCTION__)
void uvd_print_trace(const char *file, int line, const char *function);

#endif /* ifndef UV_DEBUG_H */
