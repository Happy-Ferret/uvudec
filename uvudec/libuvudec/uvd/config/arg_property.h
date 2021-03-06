/*
UVNet Universal Decompiler (uvudec)
Copyright 2008 John McMaster <JohnDMcMaster@gmail.com>
Licensed under the terms of the LGPL V3 or later, see COPYING for details
*/

#ifndef UVD_ARG_PROPERTY_H
#define UVD_ARG_PROPERTY_H

//Action
#define UVD_PROP_ACTION_HELP					"action.help"
#define UVD_PROP_ACTION_VERSION					"action.version"
//Debug
#define UVD_PROP_DEBUG_IGNORE_ERRORS			"debug.ignore_errors"
#define UVD_PROP_DEBUG_IGNORE_ERRORS_DEFAULT	true
#define UVD_PROP_DEBUG_SUPPRESS_ERRORS			"debug.suppress_errors"
#define UVD_PROP_DEBUG_SUPPRESS_ERRORS_DEFAULT	false
#define UVD_PROP_DEBUG_LEVEL					"debug.level"
#define UVD_PROP_DEBUG_ARGS						"debug.args"
#define UVD_PROP_DEBUG_INIT						"debug.init"
#define UVD_PROP_DEBUG_PLUGIN					"debug.plugin"
#define UVD_PROP_DEBUG_PROCESSING				"debug.processing"
#define UVD_PROP_DEBUG_ANALYSIS					"debug.analysis"
#define UVD_PROP_DEBUG_PRINTING					"debug.printing"
#define UVD_PROP_DEBUG_FILE						"debug.file"
#define UVD_PROP_DEBUG_CURSE					"debug.curse"
#define UVD_PROP_DEBUG_CURSE_DEFAULT			false
//Config
#define UVD_PROP_CONFIG_LANGUAGE				"config.language"
#define UVD_PROP_CONFIG_LANGUAGE_INTERFACE		"config.language_interface"
//Target
#define UVD_PROP_TARGET_ADDRESS_INCLUDE			"target.address_include"
#define UVD_PROP_TARGET_ADDRESS_EXCLUDE			"target.address_exclude"
#define UVD_PROP_TARGET_FILE					"target.file"
#define UVD_PROP_TARGET_ADDRESS					"target.address"
//Analysis
#define UVD_PROP_ANALYSIS_DIR					"analysis.dir"
//FIXME: This was originally added as a hack to make binary to object operations more efficient
//However, this has been made into a separate program, so this should be deprecated
#define UVD_PROP_ANALYSIS_ONLY					"analysis.only"
//Recursive descent, linear sweep, etc
#define UVD_PROP_ANALYSIS_FLOW_TECHNIQUE		"analysis.flow_technique"
//Output
#define UVD_PROP_OUTPUT_OPCODE_USAGE			"output.opcode_usage"
#define UVD_PROP_OUTPUT_JUMPED_ADDRESSES		"output.jumped_addresses"
#define UVD_PROP_OUTPUT_CALLED_ADDRESSES		"output.called_addresses"
#define UVD_PROP_OUTPUT_ADDRESS_COMMENT			"output.address_comment"
#define UVD_PROP_OUTPUT_ADDRESS_LABEL			"output.address_label"
#define UVD_PROP_OUTPUT_VECTOR_COMMENT			"output.vector_comment"
#define UVD_PROP_OUTPUT_VECTOR_COMMENT_DEFAULT	false
#define UVD_PROP_OUTPUT_FILE					"output.file"
#define UVD_PROP_OUTPUT_STRING_TABLE			"output.string_table"
#define UVD_PROP_OUTPUT_STRING_TABLE_DEFAULT	false
//Plugin
#define UVD_PROP_PLUGIN_ACTIVATE_ALL			"plugin.activate_all"
#define UVD_PROP_PLUGIN_ACTIVATE_ALL_DEFAULT	false
#define UVD_PROP_PLUGIN_FILE					"plugin.file"
#define UVD_PROP_PLUGIN_NAME					"plugin.name"
#define UVD_PROP_PLUGIN_APPEND_PATH				"plugin.path.append"
#define UVD_PROP_PLUGIN_PREPEND_PATH			"plugin.path.prepend"


#endif
