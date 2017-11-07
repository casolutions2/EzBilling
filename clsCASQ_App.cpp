//---------------------------------------------------------------------------
#include "CodeSiteLogging.hpp"
#pragma hdrstop

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>

#include "clsCASQ_App.h"
#include "CASQ_FormOps.h"
#include "frmSettingsUnit.h"
//#include "dmUserSec.h"
#include "uDMUniDac.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// for 32 bit
#if defined __WIN32__
	#pragma comment( lib, "CodeSiteExpressPkg.lib" )
#else
	// for 64 bit
	#pragma comment( lib, "CodeSiteExpressPkg.a" )
#endif

// constants
const std::wstring RegistryKey 			= L"Software\\CA Solutions\\EzBilling";
const std::wstring sqlCompanyDetails	= L"SELECT paramValue FROM GlobalParams WHERE paramName IN ( 'CompanyName', 'Initialised' ) ORDER BY paramName;";
//---------------------------------------------------------------------------

__fastcall clsCASQ_App::clsCASQ_App(TForm* frm)
{
	_mainForm 		= frm;
	_dbName 		= L"";
	_myobAppFile	= L"";
	_myobDataFile	= L"";
	_userName		= L"";
	_coName			= L"Unavailable";
	_initialised 	= false;
	_filesDbExist 	= false;
	_filesMyobExist = false;
	Style 			= L"Windows";
	_showInactive 	= true;
    _appPath        = ExtractFilePath(Application->ExeName);
	GetSettings();

}
//---------------------------------------------------------------------------
__fastcall clsCASQ_App::~clsCASQ_App()
{
	SaveSettings();

}
//---------------------------------------------------------------------------
void __fastcall clsCASQ_App::GetSettings()
{
	_dbName = CASQ_FormOps::GetDataFileName();
	CASQ_FormOps::GetMyobFiles(_myobAppFile, _myobDataFile);
	if ( _dbName.length() )
	{	if ( FileExists( _dbName.c_str() ) )
			_filesDbExist = true;
	}
	if ( _myobAppFile.length() && _myobDataFile.length() )
	{	if ( FileExists( _myobAppFile.c_str() ) && FileExists( _myobDataFile.c_str() ) )
			_filesMyobExist = true;
	}
	Style = CASQ_FormOps::GetStringSetting(L"Style");
	if (Style == L"")
		Style = L"Windows";
	_userName = CASQ_FormOps::GetStringSetting(L"UserName");
	_myobUser = CASQ_FormOps::GetStringSetting(L"MyobUser");
	_showInactive = CASQ_FormOps::GetBoolSetting(L"ShowInactive");

}
//---------------------------------------------------------------------------
void __fastcall clsCASQ_App::SaveSettings()
{
	CASQ_FormOps::SaveDataFileName(_dbName);
	CASQ_FormOps::SaveMyobFiles(_myobAppFile, _myobDataFile);
	CASQ_FormOps::SaveStringSetting(L"Style", Style);
	CASQ_FormOps::SaveStringSetting(L"UserName", _userName);
	CASQ_FormOps::SaveStringSetting(L"MyobUser", _myobUser);
	CASQ_FormOps::SaveBoolSetting(L"ShowInactive", _showInactive);
}
//---------------------------------------------------------------------------
bool __fastcall clsCASQ_App::StartAppWithSecurity()
{
	/*TODO review security model */
	bool rtnval = false;
	#if ! defined _DEBUG
		#if defined _USESECURITY
			// open each security table
			for (int i = 0; i < dmSecurity->ComponentCount; i++)
			{	if ( dynamic_cast<TFDTable*>(dmSecurity->Components[i]) != NULL )
					dynamic_cast<TFDTable*>(dmSecurity->Components[i])->Active = true;
			}
			if ( _userName.length() )
				dmSecurity->dlgLogin->UserName = _userName.c_str();

			if ( dmSecurity->dlgLogin->Execute() )
			{	if( dmSecurity->secManager->LoggedIn )
				{   _userName = dmSecurity->secManager->CurrentUser.c_str();
					rtnval = true;
				}
			}
		#else
            rtnval = true;
		#endif
	#else
		rtnval = true;
	#endif
	GetCompanyDetails();
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsCASQ_App::GetCompanyDetails()
{
	if ( ! dmUniDac->uCon->Connected )
	{	if ( ! dmUniDac->ConnectToDb( String( _dbName.c_str() ) ) )
			return 0;
	}
	//TFDQuery* qry = new TFDQuery(_mainForm);
	TUniQuery* qry = new TUniQuery(NULL);
	qry->Connection = dmUniDac->uCon;
	qry->SQL->Text = sqlCompanyDetails.c_str();
	qry->Open();
	if (! qry->Eof )
	{	_coName = qry->Fields->Fields[0]->AsString.c_str();
		qry->Next();
		_initialised  = qry->Fields->Fields[0]->AsInteger;
	}
	qry->Close();
	delete qry;
	return 1;
}
//---------------------------------------------------------------------------
void __fastcall clsCASQ_App::SetInitialised(bool val)
{
	if ( ! dmUniDac->uCon->Connected )
	{	if ( ! dmUniDac->ConnectToDb( String( _dbName.c_str() ) ) )
			return;
	}
	if ( val )
	{	if ( dmUniDac->ExecuteSQL( L"UPDATE GlobalParams SET paramValue=1 WHERE paramName='Initialised';" ) == 1 )
			_initialised = val;
	}
	else
	{	if (dmUniDac->ExecuteSQL( L"UPDATE GlobalParams SET paramValue=0 WHERE paramName='Initialised';" ) == 1 )
			_initialised = val;
	}
}
//---------------------------------------------------------------------------
void __fastcall clsCASQ_App::SetupLogging(String pathname)
{
	std::wstring destFolder;
	String msg(L"");
	int rslt = CreateAppFolder(destFolder);
	if ( rslt == 0 )
	{	TCodeSiteDestination *csdLog = new TCodeSiteDestination(NULL);
		csdLog->LogFile->FilePath = destFolder.c_str();
		csdLog->LogFile->FileName = "EzBilling.ProcessLog.csl";
		csdLog->LogFile->Active = true;
		csdLog->Viewer->Active = true;
		csdLog->LogFile->MaxSize = 1024;
		CodeSiteManager->DefaultDestination = csdLog;
		CodeSite->AddSeparator();
		CodeSite->Send(L"Application starting...");
		CodeSite->Send(csmInfo, L"SQLite: " + String(_dbName.c_str()));
		CodeSite->Send(csmInfo, L"MYOB App: " + String(_myobAppFile.c_str()));
		CodeSite->Send(csmInfo, L"MYOB Data: " + String(_myobDataFile.c_str()));
	}
	else
	{   switch (rslt)
		{	case 1:		// unable to get known folder
				msg = "Unable to get known folder!";
				break;
			case 2:		// unable to create folder
				msg = "Unable to create folder";
				break;
			case 99: 	// unknown exception
				msg = "Unknown exception thrown!";
				break;
		default:
			;
		}
		ShowMessage( "Error: " + msg );
		CodeSiteManager->Enabled = false;
	}
	return;
}
// -------------------------------------------------------------------
int __fastcall clsCASQ_App::CreateAppFolder(std::wstring& appFldr)
{
	bool rtnval = 0;
	std::wstring buf(L"");
	wchar_t* lpbuf = (wchar_t*)buf.c_str();
	try
	{
		if (SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &lpbuf) != S_OK)
		{	// try my docs folder
			if ( SHGetKnownFolderPath( FOLDERID_Documents, 0, NULL, &lpbuf) != S_OK )
				rtnval = 1;		// unable to get known folder
		}
		else
		{   try
			{	appFldr.append(lpbuf);
				appFldr.append(L"\\CA Solutions");
				if (CreateDirectory((wchar_t*) appFldr.c_str(), NULL) ||
					GetLastError() == ERROR_ALREADY_EXISTS)
					rtnval = 0;		// success
				else
					rtnval = 2;		// unable to create folder
			}
			catch (...)
			{
				rtnval = 99;		// unexpected exception
			}
		}
	}
	catch( Exception *ex )
	{
		rtnval = 00;				// unexpected exception
	}
	return rtnval;
}
// -------------------------------------------------------------------
int __fastcall clsCASQ_App::ShowSettings()
{
	int rtnval = 0;
	TfrmSettings* frm = new TfrmSettings(_mainForm);
	frm->ebSQLFile->Text 	= _dbName.c_str();
	frm->ebMyobApp->Text 	= _myobAppFile.c_str();
	frm->ebMyobData->Text 	= _myobDataFile.c_str();
	frm->changed 			= false;
	if ( frm->ShowModal() == mrOk && frm->changed )
	{   _dbName = frm->ebSQLFile->Text.c_str();
		_myobAppFile = frm->ebMyobApp->Text.c_str();
		_myobDataFile = frm->ebMyobData->Text.c_str();
		SaveSettings();
		rtnval = 1;
	}

	delete frm;
	frm = NULL;
	return rtnval;
}
//---------------------------------------------------------------------------
TDateTime __fastcall DateFromUKString(const String& strDate)
{
	if ( ! strDate.Length() ) return 0;

	boost::regex reg("(\\d+)[\/](\\d+)[\/](\\d+)");
	boost::smatch sm;

	std::string txtDte = AnsiString(strDate).c_str();
	boost::regex_search( txtDte, sm, reg );
	TDateTime dte = TDateTime(atoi(sm[3].str().c_str()), atoi(sm[2].str().c_str()), atoi(sm[1].str().c_str()));
	return dte;
}
//---------------------------------------------------------------------------

