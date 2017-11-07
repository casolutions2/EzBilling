//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "dlgSyncingUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvSmoothProgressBar"
#pragma link "HTMLabel"
#pragma link "AdvLabel"
#pragma resource "*.dfm"
TdlgSync *dlgSync;
//---------------------------------------------------------------------------
__fastcall TdlgSync::TdlgSync(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TdlgSync::UpdateInstruction(String txt)
{
	lblInstruction->Caption = txt;
    this->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TdlgSync::UpdateProgress(std::wstring name, int pos)
{
	pb->Position = pos;
	HTMLabel1->HTMLText->Text = (L"<B><FONT color=\"#FF0000\">" + name + L"...</FONT></B>").c_str() ;
	this->Refresh();
}
//---------------------------------------------------------------------------
