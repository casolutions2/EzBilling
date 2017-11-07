//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmDateSelectorUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvDateTimePicker"
#pragma resource "*.dfm"
TfrmDateSelector *frmDateSelector;
//---------------------------------------------------------------------------
__fastcall TfrmDateSelector::TfrmDateSelector(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmDateSelector::btnOKClick(TObject *Sender)
{
	this->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDateSelector::btnCancelClick(TObject *Sender)
{
	this->Hide();
}
//---------------------------------------------------------------------------
