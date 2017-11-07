//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmSettingsUnit.h"
//#include "frmMainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvEdit"
#pragma link "AdvFileNameEdit"
#pragma link "AdvPanel"
#pragma link "AdvGlowButton"
#pragma link "AdvEdBtn"
#pragma resource "*.dfm"
TfrmSettings *frmSettings;
//---------------------------------------------------------------------------
__fastcall TfrmSettings::TfrmSettings(TComponent* Owner)
	: TForm(Owner)
{
	FormResize(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::FormResize(TObject *Sender)
{
	Bevel1->Left = 20;
	Bevel1->Width = this->ClientWidth - 40;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::ebSQLFileChange(TObject *Sender)
{
	changed = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::FormCreate(TObject *Sender)
{
	changed = false;
}
//---------------------------------------------------------------------------

