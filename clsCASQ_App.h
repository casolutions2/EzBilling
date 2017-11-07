//---------------------------------------------------------------------------
#ifndef clsCASQ_AppH
#define clsCASQ_AppH
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include <string>
#include <Classes.hpp>
//---------------------------------------------------------------------------
class clsCASQ_App
{
	public:
		// constructors
		__fastcall clsCASQ_App(TForm* mainForm);
		__fastcall ~clsCASQ_App();

		// variables
		std::wstring	Style;

		// properties
		std::wstring	__property DBName		= {read=_dbName};
		std::wstring	__property UserName		= {read=_userName};
		std::wstring	__property MYOBApp		= {read=_myobAppFile};
		std::wstring	__property MYOBData		= {read=_myobDataFile};
		std::wstring	__property CompanyName	= {read=_coName};
		bool			__property Initialised	= {read=_initialised};
		bool			__property FilesDbExist	= {read=_filesDbExist};
		bool            __property FilesMyobExist = {read=_filesMyobExist};
		std::wstring	__property MYOBUser		= {read=_myobUser, write=_myobUser};
		bool			__property ShowInactive	= {read=_showInactive, write=_showInactive};
        String          __property Path         = {read=_appPath};

		// functions
		int 		__fastcall 	ShowSettings();
		TForm*		__fastcall	MainForm() { return _mainForm;	};
		bool		__fastcall	StartAppWithSecurity();
		void 		__fastcall 	SetupLogging(String pathname = L"");
		int 		__fastcall 	GetCompanyDetails();
		void 		__fastcall	SetInitialised(bool val);
		TDateTime 	__fastcall 	DateFromUKString(const String&);

	private:
		// constructors
		__fastcall clsCASQ_App();
		__fastcall clsCASQ_App(clsCASQ_App& rhs);

		// variables
		std::wstring 	_dbName;
		TForm* 			_mainForm;
		std::wstring	_myobAppFile;
		std::wstring	_myobDataFile;
		std::wstring	_myobUser;
		std::wstring	_userName;
		std::wstring	_coName;
		bool			_initialised;
		bool			_filesMyobExist;
		bool            _filesDbExist;
		bool			_showInactive;
        String          _appPath;

		// functions
		void 	__fastcall GetSettings();
		void 	__fastcall SaveSettings();
		int 	__fastcall CreateAppFolder(std::wstring& appFldr);

};
#endif
