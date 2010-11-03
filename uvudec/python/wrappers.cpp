/*
UVNet Universal Decompiler (uvudec)
Copyright 2010 John McMaster <JohnDMcMaster@gmail.com>
Licensed under the terms of the LGPL V3 or later, see COPYING for details
*/

#include "wrappers.h"

void init()
{
	UVD_SWIG_ASSERT_ERR(UVDInit());
}
void deinit()
{
	UVD_SWIG_ASSERT_ERR(UVDDeinit());
}

//static uv_err_t getUVDFromFileName(UVD **uvdOut, const std::string &file);
UVD *uvd::getUVDFromFileName(const char *fileName)
{
	UVD *ret = NULL;
	
	UVD_SWIG_ASSERT_ERR(UVD::getUVDFromFileName(&ret, fileName));
	return ret;
}
