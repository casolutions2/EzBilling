//---------------------------------------------------------------------------

#include <vcl.h>
#include "CodeSiteLogging.hpp"
#pragma hdrstop

#include "uFormXtrnlDeliveries.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "AdvGridPDFIO"
#pragma link "AdvObj"
#pragma link "AdvPDFIO"
#pragma link "AdvUtil"
#pragma link "asgprev"
#pragma link "asgprint"
#pragma link "BaseGrid"
#pragma link "tmsAdvGridExcel"
#pragma link "W7Bars"
#pragma link "W7Buttons"
#pragma link "W7Classes"
#pragma link "TaskDialog"
#pragma link "AdvCustomComponent"
#pragma resource "*.dfm"
TfrmXtrnlDeliveries *frmXtrnlDeliveries;
//---------------------------------------------------------------------------

__fastcall TfrmXtrnlDeliveries::TfrmXtrnlDeliveries(TComponent* Owner)
	: TForm(Owner)
{
	Supplier = ORIGIN;
	_invoicesWritten = 0;
	xd = NULL;
	parentForm = static_cast<TfrmMain*>(Owner);
}
//---------------------------------------------------------------------------
__fastcall TfrmXtrnlDeliveries::TfrmXtrnlDeliveries( TComponent* Owner, clsColCustomers* ACustomers )
	: TForm( Owner )
{
	Supplier = ORIGIN;
	_invoicesWritten = 0;
	xd = NULL;
	parentForm = static_cast<TfrmMain*>(Owner);
	_customers = ACustomers;
}
//---------------------------------------------------------------------------
	void __fastcall TfrmXtrnlDeliveries::btnPrintClick(TObject *Sender)
{   // print
	if ( printSettings->Execute() )
		printPreview->Execute() ;
}
//---------------------------------------------------------------------------
void __fastcall TfrmXtrnlDeliveries::gridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
		  Advobj::TVAlignment &VAlign)
{
	if (ACol == 0 || ARow == 0) {
		HAlign = taCenter;
		VAlign = Advobj::vtaCenter;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmXtrnlDeliveries::SetupGrid()
{
	grid->ColumnHeaders->Add(L"");
	for (int i = 1; i < 20; i++) {
		grid->ColumnHeaders->Add(char(65+i));
	}
	grid->RowHeaders->Add(L"");
	for (int i = 1; i < 100; i++) {
		grid->RowHeaders->Add(i);
	}
}
//---------------------------------------------------------------------------
bool __fastcall TfrmXtrnlDeliveries::LoadFile(String filename)
{
	bool rtnval = false;
	CodeSite->EnterMethod( "TfrmXtrnlDeliveries::LoadFile - " + filename );
	try
	{	if ( CompareText( filename.SubString(filename.Length()-2,3),L"csv") == 0 )
		{	// load csv file
			grid->Delimiter = L',';
			grid->FixedCols = 0;
			grid->LoadFromCSV( filename );
			grid->AutoSizeColumns(false, 4);
			rtnval = true;
		}
		else if ( CompareText( filename.SubString( filename.Length()-2, 3), L"xls") == 0 )
		{	// load xls file
			AdvGridExcelIO1->XLSImport( filename );
	  //		fmt = 1;
			rtnval = true;
		}
		else
			ShowMessage( L"Unrecognised file format!" );
	}
	catch( Exception *ex )
	{   CodeSite->SendException( ex );
		rtnval = false;
		ShowMessage( "Error: " + ex->Message );
	}
	if ( rtnval )
		ValidateCustomers();

	CodeSite->EnterMethod( "TfrmXtrnlDeliveries::LoadFile - " + filename );
	return rtnval;
}
//---------------------------------------------------------------------------
void __fastcall TfrmXtrnlDeliveries::ValidateCustomers()
{
	xd 	= new clsXtrnlDeliveries( _customers, grid );
	if ( xd->ValidateCustomers() )
		ShowMessage( L"Missing Customers" );
}
//---------------------------------------------------------------------------
void __fastcall TfrmXtrnlDeliveries::btnCancelClick(TObject *Sender)
{
	this->ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmXtrnlDeliveries::btnProcessClick(TObject *Sender)
{
	// process the data
	Screen->Cursor = crHourGlass;
	try
	{	xd->xtrnlCompany 		= Supplier;
		xd->MainForm 			= parentForm;
		td1->Clear();
		td1->Title = L"Import Deliveries";
		_invoicesWritten = xd->ProcessInvoices( parentForm->App );
		if ( _invoicesWritten > 0 )
		{	// success
			td1->Icon 			= tiInformation;	//Shield; //tiWarning;  // tiShield tiQuestion tiInformation
			String msg 			= L"The import of deliveries succeded writing " + String( _invoicesWritten ) + L" invoices.";
			td1->Instruction 	= msg;
		}
		else
		{	// failed
			td1->Clear();
			td1->Title = L"Import Deliveries";
			td1->Icon = tiError;	//tiShield; //tiWarning;  // tiShield tiQuestion tiInformation
			td1->Instruction 	= L"The import of deliveries failed.";
		}
		td1->CommonButtons << cbOK;
		td1->Execute();

	}
	catch (Exception *ex )
	{   CodeSite->SendException( ex );
		// what to do probably already advised to user.
	}
	grid->Clear();
	delete xd;
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
