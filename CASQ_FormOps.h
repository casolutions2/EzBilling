//---------------------------------------------------------------------------
#ifndef CASQ_FormOpsH
#define CASQ_FormOpsH
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <IniFiles.hpp>
#include <Registry.hpp>
#include <string>
//---------------------------------------------------------------------------
namespace CASQ_FormOps
{
	void 			__fastcall 	SavePosition(TForm* frm);
	void 			__fastcall 	RestorePosition(TForm* frm);
	std::wstring	__fastcall 	GetDataFileName();
	void 			__fastcall 	SaveDataFileName(const std::wstring filename);
	TCustomIniFile*	__fastcall 	OpenIniFileInstance(int idx, const std::wstring appTitle = L"");
	void			__fastcall 	GetMyobFiles(std::wstring& myobApp, std::wstring& myobData);
	void			__fastcall 	SaveMyobFiles(const std::wstring& myobApp, const std::wstring& myobData);
	std::wstring	__fastcall	GetStringSetting(const std::wstring& setting);
	void			__fastcall	SaveStringSetting(const std::wstring& setting, const std::wstring& value);
	bool			__fastcall 	GetBoolSetting(const std::wstring& setting);
	void			__fastcall 	SaveBoolSetting(const std::wstring& setting, const bool& value);
}
#endif
