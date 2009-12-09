/*
UVNet Universal Decompiler (uvudec)
Copyright 2008 John McMaster
JohnDMcMaster@gmail.com
Licensed under terms of the three clause BSD license, see LICENSE for details
*/

#ifndef UVD_BINARY_SYMBOL_H
#define UVD_BINARY_SYMBOL_H

#include "uvd_data.h"
#include "uvd_relocation.h"

#include <string>
#include <vector>

/*
A symbol as if found in an object file, not necessarily ELF format
A format easily workable in memory
*/
class UVDBinarySymbol
{
public:
	UVDBinarySymbol();
	virtual ~UVDBinarySymbol();

	void setSymbolName(const std::string &name);
	uv_err_t getSymbolName(std::string &name);
	virtual uv_err_t init();
	
	//FIXME: this is analysis specific...should it be here?
	//If this is a symbol in our currently analyzed data, the address it presides at 
	uv_err_t getSymbolAddress(uint32_t *symbolAddress);
	uv_err_t setSymbolAddress(uint32_t symbolAddress);
	/*
	uv_err_t getOffset(uint32_t *offset);
	*/
	
	//A relocation inside this relocation, if applicable
	//Some symbols are so small this is not practical, such as variables
	//A basic form
	uv_err_t addRelocation(uint32_t relocatableDataOffset, uint32_t relocatableDataSize);
	//Add relocations from other symbol to this one
	//Used for merging analysis data
	uv_err_t addRelocations(const UVDBinarySymbol *otherSymbol);

public:
	//The symbol's (function's/variable's) name
	std::string m_symbolName;
	//The actual data this symbol represents, if its resolved
	//If the symbol is not resolved, data will be NULL
	UVDData *m_data;

	//The relocations contained within this symbol
	//Stuff like where g_debug is used
	//Also stored is a relocatable version of the function
	UVDRelocatableData *m_relocatableData;

	//Computes the symbol address
	UVDRelocatableElement m_symbolAddress;
};

#if 0
Deprecated, see UVDBinaryFunctionInstance
/*
A function is a peice of code with some relocatables in it
If the function is external, it may or may not have a known value
*/
class UVDFunctionBinarySymbol : public UVDBinarySymbol
{
public:
	UVDFunctionBinarySymbol();
	~UVDFunctionBinarySymbol();
};
#endif

/*
A variable is a reserved location to store data
*/
class UVDVariableBinarySymbol : public UVDBinarySymbol
{
public:
	UVDVariableBinarySymbol();
	~UVDVariableBinarySymbol();
};

/*
A label is a location jumped to, ie a relocation based off of IP
Usually they occur only internally to functions and are not referenced outside that function
Exception: BASIC code...yuck
	In this case the decompiler is useless and you are better off reading the assembly.  Good luck! 
*/
class UVDLabelBinarySymbol : public UVDBinarySymbol
{
public:
	UVDLabelBinarySymbol();
	~UVDLabelBinarySymbol();
 };

/*
A symbol of read only data
Their value cannot be changed
ELF notes
	Do not have a true symbol after compilation
	Like anonymous symbols
	These appear in the .bss section
*/
class UVDROMBinarySymbol : public UVDBinarySymbol
{
public:
	UVDROMBinarySymbol();
	~UVDROMBinarySymbol();
};

/*
A collection of symbols belonging to a common analysis
*/
class UVDBinaryFunction;
class UVDAnalyzer;
class UVDBinarySymbolManager
{
public:
	UVDBinarySymbolManager();
	~UVDBinarySymbolManager();

	uv_err_t findSymbol(std::string &name, UVDBinarySymbol **symbol);
	uv_err_t addSymbol(UVDBinarySymbol *symbol);
	//Shortcuts for now
	//These will create the function/label if necessary
	/*
	ie from a call
	functionAddress: the call target of the function
	relocatableDataOffset: the address of the data that contains the function address
	relocatableDataSize: the size of the section storing the function address
		If there is not enough room to store, an error will be thrown
	*/
	uv_err_t addAbsoluteFunctionRelocation(uint32_t functionAddress,
			uint32_t relocatableDataOffset, uint32_t relocatableDataSize);
	//ie from a goto
	uv_err_t addAbsoluteLabelRelocation(uint32_t labelAddress,
			uint32_t relocatableDataOffset, uint32_t relocatableDataSize);

	//Find the function symbol passed in and add all relocations, if any
	uv_err_t collectRelocations(UVDBinaryFunction *function);

private:
	uv_err_t doCollectRelocations(UVDBinaryFunction *function, UVDBinarySymbol *analysisSymbol);

public:
	//Symbol names must be unique
	std::map<std::string, UVDBinarySymbol *> m_symbols;
	//Needed to convert addresses to symbol names
	UVDAnalyzer *m_analyzer;
};

#endif
