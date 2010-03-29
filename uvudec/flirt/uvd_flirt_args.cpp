/*
UVNet Universal Decompiler (uvudec)
Copyright 2008 John McMaster <JohnDMcMaster@gmail.com>
Licensed under terms of the three clause BSD license, see LICENSE for details
*/

#include "uvd_config.h"
#include "uvd_flirt_args.h"
#include "uvd_flirt_args_property.h"
#include "uvd_arg_property.h"

static uv_err_t argParser(const UVDArgConfig *argConfig, std::vector<std::string> argumentArguments);

uv_err_t initFLIRTSharedConfig()
{
	uv_assert_ret(g_config);

	g_config->m_configArgs.push_back(new UVDArgConfig(UVD_PROP_FLIRT_MIN_SIGNATURE_LENGTH, 0, "sig-min-length", "minimum (unrelocatable) signature length, default 4 bytes", 1, argParser, true));
	g_config->m_configArgs.push_back(new UVDArgConfig(UVD_PROP_FLIRT_MAX_SIGNATURE_LENGTH, 0, "sig-max-length", "maximum signature length, default 0x8000 bytes", 1, argParser, true));
	g_config->m_configArgs.push_back(new UVDArgConfig(UVD_PROP_TARGET_FILE, 0, "input", "Object library (ELF, OMF, etc)", 1, argParser, false));
	g_config->m_configArgs.push_back(new UVDArgConfig(UVD_PROP_OUTPUT_FILE, 0, "output", "pat file (default: stdout)", 1, argParser, false));

	return UV_ERR_OK;	
}

static uv_err_t argParser(const UVDArgConfig *argConfig, std::vector<std::string> argumentArguments)
{
	UVDConfig *config = NULL;
	UVDConfigFLIRT *flirtConfig = NULL;
	//If present
	std::string firstArg;
	uint32_t firstArgNum = 0;
	
	config = g_config;
	uv_assert_ret(config);
	flirtConfig = &config->m_flirt;
	uv_assert_ret(flirtConfig);
	uv_assert_ret(config->m_argv);

	uv_assert_ret(argConfig);

	if( !argumentArguments.empty() )
	{
		firstArg = argumentArguments[0];
		firstArgNum = strtol(firstArg.c_str(), NULL, 0);
	}

	if( argConfig->m_propertyForm == UVD_PROP_FLIRT_MIN_SIGNATURE_LENGTH )
	{
		uv_assert_ret(!argumentArguments.empty());
		flirtConfig->m_signatureLengthMin = firstArgNum;
	}
	else if( argConfig->m_propertyForm == UVD_PROP_FLIRT_MAX_SIGNATURE_LENGTH )
	{
		uv_assert_ret(!argumentArguments.empty());
		flirtConfig->m_signatureLengthMax = firstArgNum;
	}
	//All of these have mult input files and one output file
	else if( argConfig->m_propertyForm == UVD_PROP_TARGET_FILE )
	{
		uv_assert_ret(!argumentArguments.empty());
		flirtConfig->m_targetFiles.push_back(firstArg);
	}
	else if( argConfig->m_propertyForm == UVD_PROP_OUTPUT_FILE )
	{
		uv_assert_ret(!argumentArguments.empty());
		flirtConfig->m_outputFile = firstArg;
	}
	//Unknown
	else
	{
		//return UV_DEBUG(argParserDefault(argConfig, argumentArguments));
		return UV_DEBUG(UV_ERR_GENERAL);
	}

	return UV_ERR_OK;
}
