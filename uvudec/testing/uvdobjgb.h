/*
UVNet Universal Decompiler (uvudec)
Copyright 2010 John McMaster <JohnDMcMaster@gmail.com>
Licensed under the terms of the LGPL V3 or later, see COPYING for details
*/

#ifndef UVD_TESTING_UVDOBJGB_H
#define UVD_TESTING_UVDOBJGB_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "testing/object.h"
#include "plugin/uvdgb/object.h"
#include "plugin/uvdgb/plugin.h"

class UVDGBUnitTest : public UVDTestingObjectFixture
{
public:
	void setUp();
	
	//From the plugin engine
	UVDObjgbPlugin *getPlugin();
	//From the runtime
	UVDGBObject *getObject();
	
public:
	CPPUNIT_TEST_SUITE(UVDGBUnitTest);
	CPPUNIT_TEST(loadsCorrectObjectTest);
	CPPUNIT_TEST(isNintendoLogoTest);
	CPPUNIT_TEST(getTitleTest);
	CPPUNIT_TEST(getManufacturerCodeTest);
	CPPUNIT_TEST(getCGBFlagsTest);
	CPPUNIT_TEST(getNewLicenseeCodeTest);
	CPPUNIT_TEST(getSGBFlagsTest);
	CPPUNIT_TEST(isSGBEnabledTest);
	CPPUNIT_TEST(getCartridgeTypeTest);
	CPPUNIT_TEST(getROMSizeTest);
	CPPUNIT_TEST(getROMSizeRawTest);
	CPPUNIT_TEST(getSaveRAMSizeTest);
	CPPUNIT_TEST(getSaveRAMSizeRawTest);
	CPPUNIT_TEST(getDestinationCodeTest);
	CPPUNIT_TEST(getOldLicenseeCodeTest);
	CPPUNIT_TEST(getMaskROMVersioNumberTest);
	CPPUNIT_TEST(getHeaderChecksumTest);
	CPPUNIT_TEST(computeHeaderChecksumTest);
	CPPUNIT_TEST(isHeaderChecksumValidTest);
	CPPUNIT_TEST(getGlobalChecksumTest);
	CPPUNIT_TEST(computeGlobalChecksumTest);
	CPPUNIT_TEST(isGlobalChecksumValidTest);
	CPPUNIT_TEST_SUITE_END();

protected:
	/*
	Did we return a UVDGBObject upon trying to load the gameboy object?
	*/
	void loadsCorrectObjectTest(void);

	/*
	Cartridges are suppose to include the Nintendo logo bitmap
	*/
	void isNintendoLogoTest(void);

	/*
	Matches expected title?
	*/
	void getTitleTest(void);

	void getManufacturerCodeTest(void);

	void getCGBFlagsTest(void);

	void getNewLicenseeCodeTest(void);

	void getSGBFlagsTest(void);

	void isSGBEnabledTest(void);

	void getCartridgeTypeTest(void);

	void getROMSizeTest(void);

	void getROMSizeRawTest(void);

	void getSaveRAMSizeTest(void);

	void getSaveRAMSizeRawTest(void);

	void getDestinationCodeTest(void);

	void getOldLicenseeCodeTest(void);

	void getMaskROMVersioNumberTest(void);

	void getHeaderChecksumTest(void);

	void computeHeaderChecksumTest(void);

	void isHeaderChecksumValidTest(void);

	void getGlobalChecksumTest(void);

	void computeGlobalChecksumTest(void);

	void isGlobalChecksumValidTest(void);
};

#endif

