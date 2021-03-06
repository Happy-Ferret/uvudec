/*
UVNet Universal Decompiler (uvudec)
Copyright 2010 John McMaster <JohnDMcMaster@gmail.com>
Licensed under the terms of the GPL V3 or later, see COPYING for details
*/

#ifndef UVD_PROJECT_H
#define UVD_PROJECT_H

#include "uvd/core/uvd.h"
#include "uvd/util/types.h"
#include <string>

class UVDBinarySymbol;
class UVDGUIFormat;
class UVDProject
{
public:
	UVDProject();
	~UVDProject();

	uv_err_t doSave();
	uv_err_t init(int argc, char **argv);
	uv_err_t deinit();
	uv_err_t setFileName(const std::string &fileName);
	
	UVDGUIFormat *getFormat();

public:
	UVD *m_uvd;
	std::string m_canonicalProjectFileName;
	//std::string m_canonicalBinaryName;
	//Symbol, usually a function, we are displaying in the workspace
	UVDBinarySymbol *m_symbolInFocus;
};

#endif

