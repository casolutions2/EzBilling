//---------------------------------------------------------------------------
//#include <CodeSiteLogging.hpp>

#include "CASQ_FormOps.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
void __fastcall CASQ_FormOps::SavePosition(TForm* frm)
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	// store form position
	try
	{
		settings->WriteInteger ( frm->Name, "Top", frm->Top);
		settings->WriteInteger(frm->Name, "Left", frm->Left);
		settings->WriteInteger(frm->Name, "Width", frm->Width);
		settings->WriteInteger(frm->Name, "Height", frm->Height);
		settings->WriteBool(frm->Name, "InitMax", frm->WindowState == wsMaximized);
	}
	catch (Exception* ex)
	{
		//CodeSite->SendException( ex );
	}
	delete settings;
}
//---------------------------------------------------------------------------
void __fastcall CASQ_FormOps::RestorePosition(TForm* frm)
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	try
	{
		frm->Top 	= settings->ReadInteger(frm->Name, "Top", frm->Top);
		frm->Left 	= settings->ReadInteger(frm->Name, "Left", frm->Left);
		frm->Width 	= settings->ReadInteger(frm->Name, "Width", frm->Width);
		frm->Height	= settings->ReadInteger(frm->Name, "Height", frm->Height);
		// window state
		if (settings->ReadBool(frm->Name, "InitMax", frm->WindowState == wsMaximized) )
			frm->WindowState = wsMaximized;
		else
			frm->WindowState = wsNormal;
	}
	catch (Exception* ex)
	{
		//CodeSite->SendException( ex );
	}
	delete settings;
}
//---------------------------------------------------------------------------
std::wstring __fastcall CASQ_FormOps::GetDataFileName()
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	std::wstring rtnval(L"");
	try
	{	rtnval = settings->ReadString(L"Files", L"DataFile", L"").c_str();

	}
	catch (Exception* ex)
	{	//CodeSite->SendException( ex );

	}
	return rtnval;
}
//---------------------------------------------------------------------------
void __fastcall CASQ_FormOps::SaveDataFileName(const std::wstring filename)
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	try
	{	settings->WriteString(L"Files", L"DataFile", filename.c_str());

	}
	catch(Exception* ex)
	{
		//CodeSite->SendException( ex );
	}
	return;
}
//---------------------------------------------------------------------------
void __fastcall CASQ_FormOps::GetMyobFiles(std::wstring& myobApp, std::wstring& myobData)
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	try
	{
		myobApp = settings->ReadString(L"Files", L"MyobApp", L"").c_str();
		myobData = settings->ReadString(L"Files", L"MyobData", L"").c_str();
	}
	catch( Exception *ex )
	{
		//CodeSite->SendException( ex );
	}
	return;
}
//---------------------------------------------------------------------------
void __fastcall CASQ_FormOps::SaveMyobFiles(const std::wstring& myobApp, const std::wstring& myobData)
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	try
	{
		settings->WriteString(L"Files", L"MyobApp", myobApp.c_str());
		settings->WriteString(L"Files", L"MyobData", myobData.c_str());
	}
	catch (Exception *ex)
	{
		//CodeSite->SendException( ex );
	}
	return;
}
//---------------------------------------------------------------------------
TCustomIniFile* __fastcall CASQ_FormOps::OpenIniFileInstance(int idx, const std::wstring appTitle)
{
	switch ( idx )
	{	case 0:		// Registry
			if ( appTitle.length() == 0 )
				return new TRegistryIniFile(String("Software\\CA Solutions\\") + Application->Title);
			else
				return new TRegistryIniFile( "Software\\CA Solutions\\" + String( appTitle.c_str() ) );
		case 1:     // Ini File
			return new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
		case 2:
			// in memory ini file
			return new TMemIniFile(ChangeFileExt(Application->ExeName, ".ini"));
	}
	return NULL;
}
//---------------------------------------------------------------------------
std::wstring __fastcall CASQ_FormOps::GetStringSetting(const std::wstring& setting)
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	std::wstring rtnval(L"");
	try
	{	rtnval = settings->ReadString(L"General", setting.c_str(), L"").c_str();

	}
	catch (Exception* ex)
	{	//CodeSite->SendException( ex );

	}
	return rtnval;
}
//---------------------------------------------------------------------------
void __fastcall CASQ_FormOps::SaveStringSetting(const std::wstring& setting, const std::wstring& value)
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	try
	{
		settings->WriteString(L"General", setting.c_str(), value.c_str());
	}
	catch( Exception *ex )
	{
		//CodeSite->SendException( ex );
	}
	return;
}
//---------------------------------------------------------------------------
bool __fastcall CASQ_FormOps::GetBoolSetting(const std::wstring& setting)
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	bool rtnval = true;
	try
	{	rtnval = settings->ReadBool(L"General", setting.c_str(), true);	}
	catch (Exception* ex)
	{	//CodeSite->SendException( ex );

	}
	return rtnval;
}
//---------------------------------------------------------------------------
void __fastcall CASQ_FormOps::SaveBoolSetting(const std::wstring& setting, const bool& value)
{
	TCustomIniFile* settings = OpenIniFileInstance(0);
	try
	{
		settings->WriteBool(L"General", setting.c_str(), value);
	}
	catch( Exception *ex )
	{
//		CodeSite->SendException( ex );
	}
	return;
}
//---------------------------------------------------------------------------

