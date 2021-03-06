/*
UVNet Universal Decompiler (uvudec)
Copyright 2008 John McMaster <JohnDMcMaster@gmail.com>
Licensed under the terms of the LGPL V3 or later, see COPYING for details
*/

#ifndef UVD_LANGUAGE_H
#define UVD_LANGUAGE_H

#include <string>
#include <stdint.h>

/*
Try to convert to various code types
Code should be compilable into the original program, unmodified
A makefile should be specified either as an output file or a comment in a
source file
*/
#define UVD_LANGUAGE_UNKNOWN					0
/*
Machine language
The output of an assembler
*/
#define UVD_LANGUAGE_MACHINE					1
/*
Like what objdump would produce
8048670:       31 ed                   xor    %ebp,%ebp
Will have to see how hard it is to calc mem addresses rather than file
*/
#define UVD_LANGUAGE_ASSEMBLY					2
#define UVD_LANGUAGE_C							3
//C++
#define UVD_LANGUAGE_CXX						3
#define UVD_LANGUAGE_PASCAL						4
#define UVD_LANGUAGE_COBOL						5
#define UVD_LANGUAGE_FORTRAN					6

#define UVD_LANGUAGE_PYTHON						100
#define UVD_LANGUAGE_LUA						101
#define UVD_LANGUAGE_JAVASCRIPT					102

/*
Best suited for things of different types, or an as yet
of now unknown source type
This will go backwards not forwards...if it detects C code it might get confused
*/
#define UVD_LANGUAGE_AUTO						0x0100

/*
Technique used to access an interpreter
*/
//Uninitialized value
#define UVD_LANGUAGE_INTERFACE_UNKNOWN			0
//Execute the interpreter binary and parse the results back
#define UVD_LANGUAGE_INTERFACE_EXEC				1
//Use a supplied local binary API
#define UVD_LANGUAGE_INTERFACE_API				2
//Other possibilities include RPCs such as XML-RPC

/*
class UVDLanguage
{
public:
	static std::string languageToString(uint32_t in);
	static std::string languageInterfaceToString(uint32_t in);
};
*/

#endif

