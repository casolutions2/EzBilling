//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormBillingAdjustment.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvEdit"
#pragma link "AdvLabel"
#pragma link "AdvMoneyEdit"
#pragma link "AdvPanel"
#pragma link "TaskDialog"
#pragma link "W7Bars"
#pragma link "W7Buttons"
#pragma link "W7Classes"
#pragma resource "*.dfm"
TformBillingAdjust *formBillingAdjust;
//---------------------------------------------------------------------------
__fastcall TformBillingAdjust::TformBillingAdjust(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TformBillingAdjust::FormActivate(TObject *Sender)
{
	ClearBillAdjustControls();
}
//---------------------------------------------------------------------------
TVolumeAdjustment __fastcall TformBillingAdjust::PopulateAdjustment()
{
	TVolumeAdjustment va;
	va.GasCostPrice 		= ebCP->FloatValue;
	va.FixedMarkUp 			= ebMFx->FloatValue;
	va.CostMarkUp			= ebMpc->FloatValue/100;
	va.InterestRate	 		= ebIR->FloatValue/100;
	va.DaysFree 			= ebDF->IntValue;
	va.Volume.Median		= ebVMd->FloatValue;
	va.Volume.Minimum		= ebVMn->FloatValue;
	va.Adjust.Pct			= ebApc->FloatValue/100;
	va.Adjust.Minimum		= ebAmin->FloatValue;
	va.Adjust.Maximum		= ebAmax->FloatValue;
	va.Restrict.AbsoluteMin	= ebResetToZero->FloatValue;
	va.Restrict.Minimum		= ebRestrictMin->FloatValue;
	va.Restrict.Maximum		= ebRestrictMax->FloatValue;
	va.NewRange.Minimum		= ebNewMin->FloatValue;
	va.NewRange.Maximum		= ebNewMax->FloatValue;
	va.AdjustPrice			= ebAdjPrice->FloatValue;
	return va;
}
//---------------------------------------------------------------------------
bool __fastcall TformBillingAdjust::CheckEntries()
{
	bool rtnval = false;
	TVolumeAdjustment va = PopulateAdjustment();
	if ( va.Restrict.AbsoluteMin > 0.20 )
	{	td2->Title 			= L"Validate Entries";
		td2->CommonButtons.Clear();
		td2->CommonButtons << cbYes << cbNo;
		td2->Instruction		= L"Unusual entry. \nDo you wish to continue?";
		td2->ExpandedText		= L"You have entered a high 'Reset to Zero' amount.";
		td2->Icon 				=  tiWarning;
		if ( td2->Execute() == cbNo )
			return false;

	}
	// minimum
	if ( va.Restrict.Minimum < va.Restrict.AbsoluteMin )
	{
		td2->Icon = tiError;
		td2->ExpandedText 		= L"You have conflicting values in \n'Adjust existing - Minimum' and \n'Reset to Zero' values. \nPlease correct this issue.";
		td2->Title 				= L"Validate Entries";
		td2->Instruction 		= L"Invalid entry.  Please correct before proceeding.";
		td2->CommonButtons << cbOK ;
		td2->Execute();
		return false;
	}
	// maximum
	if ( va.Restrict.Minimum > va.Restrict.Maximum )
	{
		td2->Icon = tiError;
		td2->ExpandedText 		= L"You have conflicting values in \n'Adjust existing - Minimum' and \n'Adjust existing - Maximum' values. \nPlease correct this issue.";
		td2->Title 				= L"Validate Entries";
		td2->Instruction 		= L"Invalid entry.  Please correct before proceeding.";
		td2->CommonButtons << cbOK ;
		td2->Execute();
		return false;
	}
	// new minimum
	if ( va.NewRange.Minimum > va.NewRange.Maximum )
	{
		td2->Icon = tiError;
		td2->ExpandedText 		= L"You have conflicting values in \n'New billing rate - Minimum' and \n'New billing rate - Maximum' values. \nPlease correct this issue.";
		td2->Title 				= L"Validate Entries";
		td2->Instruction 		= L"Invalid entry.  Please correct before proceeding.";
		td2->CommonButtons << cbOK ;
		td2->Execute();
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------
void __fastcall TformBillingAdjust::btnProceedClick(TObject *Sender)
{
	btnCancel->Enabled 	= false;
	btnPrevious->Enabled 	= false;
	btnProceed->Enabled 	= false;
	Screen->Cursor = crHourGlass;
	try
	{	if ( CheckEntries() )
		{   this->ModalResult = mrOk;
			_returnStatus = 1;
		}
	}
	catch (Exception *ex)
	{
		_returnStatus = -1;
		this->ModalResult = mrCancel;
		Screen->Cursor = crDefault;
		throw ex;
	}
	btnCancel->Caption 	= L"Close";
	btnCancel->Enabled 	= true;
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TformBillingAdjust::btnPreviousClick(TObject *Sender)
{
	if ( ba )
		ba->LoadPrevious();
}
//---------------------------------------------------------------------------
void __fastcall TformBillingAdjust::btnCancelClick(TObject *Sender)
{
	btnCancel->Caption = L"Cancel";
	btnProceed->Enabled = true;
	btnPrevious->Enabled = true;
	_returnStatus = 0;
    this->ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TformBillingAdjust::ClearBillAdjustControls()
{
	for (int p = 0; p < ControlCount; p++)
	{   if ( dynamic_cast<TAdvPanel*>(Controls[p]) != NULL )
		{	for (int i = 0; i < dynamic_cast<TAdvPanel*>(Controls[p])->ControlCount; i++)
			{	if ( dynamic_cast<TAdvEdit*>(dynamic_cast<TAdvPanel*>(Controls[p])->Controls[i]) != NULL )
					dynamic_cast<TAdvEdit*>(dynamic_cast<TAdvPanel*>(Controls[p])->Controls[i])->FloatValue = 0;
			}
		}
	}
}

//---------------------------------------------------------------------------

