//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <System.DateUtils.hpp>
#include "UfrmGroupRates.h"          // this header
//#include "Unit1.h"					 // test program main header file
//#include "classGroup.h"				// class header for Group
#include "uDMUniDac.h"				 // main data module with connection and datasources
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvEdit"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "DBAdvGrid"
#pragma link "AdvDateTimePicker"
#pragma link "AdvDBDateTimePicker"
#pragma link "AdvDBLookupComboBox"
#pragma link "AdvGroupBox"
#pragma link "AdvOfficeButtons"
#pragma link "DBAdvOfficeButtons"
#pragma link "dbmnyed"
#pragma link "MoneyEdit"
#pragma link "DBAdvEd"
#pragma link "DBAdvGlowNavigator"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
TfrmGroupRates *frmGroupRates;
//---------------------------------------------------------------------------
__fastcall TfrmGroupRates::TfrmGroupRates(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmGroupRates::grpTypeClick(TObject *Sender)
{
   /*	switch (grpType->ItemIndex)
	{
		case 0:		// percentage
			ebRate->EditType  = Advedit::etFloat;
			ebRate->MaxFloatValue = 100.0;
			ebRate->MinFloatValue = 0.0;
			break;
		case 1:		// dollar
			ebRate->EditType = Advedit::etMoney;
			ebRate->MaxValue = 100;
			ebRate->MinValue = 0;
			break;
		default:
			;
	}
	*/
}
//---------------------------------------------------------------------------
#pragma region Support Functions
void __fastcall TfrmGroupRates::SetupGrid()
{

}
//---------------------------------------------------------------------------
void __fastcall TfrmGroupRates::SetupFields()
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmGroupRates::ClearInputs()
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmGroupRates::PopulateGroups()
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmGroupRates::CreateLookupField()
{
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma end_region

void __fastcall TfrmGroupRates::gridGetCellColor(TObject *Sender, int ARow, int ACol,
		  TGridDrawState AState, TBrush *ABrush, TFont *AFont)
{
	if ( ARow > 0 )
	{	if (ACol == 3 )
		{	if ( grid->Cells[1][ARow] < Date() )
				// show expired rates in red or dimmed
				AFont->Color = clGray;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmGroupRates::gridGetFloatFormat(TObject *Sender, int ACol, int ARow,
		  bool &IsFloat, UnicodeString &FloatFormat)
{
	return;
	if (ARow > 0)
	{	if ( ACol == grid->ColCount -1 )
		{   if ( IsFloat )
			{	if ( grid->Cells[ACol-1][ARow] == L"Percent" )
					FloatFormat = L"%.3f";
				else
					FloatFormat = L"%.2m";
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmGroupRates::tblAfterScroll(TDataSet *DataSet)
{
	switch ( dmUniDac->tblGroupRates->FieldByName(L"DiscountType")->AsInteger )
	{	case 0:
			ebRate->EditMask = L"0.0%;0;_";
			break;
		case 1:		// money
			ebRate->EditMask = L"$$9.000;0;_";
			break;
		default:
			;
	}
}
//---------------------------------------------------------------------------

