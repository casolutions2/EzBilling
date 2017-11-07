//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFormScheduleEntry.h"
#include "clsCASQ_App.h"             // class to store application wide variables
#include "CASQ_FormOps.h"			// operations to save form status
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "AdvUtil"
#pragma link "BaseGrid"
#pragma link "DBAdvGrid"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "Uni"
#pragma link "AdvGlowButton"
#pragma link "AdvPanel"
#pragma resource "*.dfm"
TfrmScheduleEntry *frmScheduleEntry;
bool Restored = false;

const String sqlUpdate = "SELECT p.Key, CAST( (CASE Length(CoName) WHEN 0 " \
		"THEN CASE WHEN LName IS NULL " \
				"THEN '' ELSE LName END || ', ' || "\
			"CASE WHEN FName IS NULL  "\
				"THEN '' ELSE FName END "\
		"ELSE CoName END ) AS varchar(120) ) as DisplayName, " \
		"l.AddressLines , p.StartTime, p.Notes, p.EstQty, p.DelQty " \
"FROM Customers c " \
	"INNER JOIN Planner p On p.CustomerID=c.CustomerID " \
	"LEFT JOIN Locations l on p.LocationID=l.ID " \
"WHERE p.Resource > 0 AND p.StartTime > :startDate AND p.EndTime < :endDate " \
"ORDER BY p.StartTime;";

//---------------------------------------------------------------------------
__fastcall TfrmScheduleEntry::TfrmScheduleEntry(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmScheduleEntry::dtpDelDateCloseUp(TObject *Sender)
{
	RefreshData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmScheduleEntry::RefreshData()
{
	TDateTime selDate = dtpDelDate->Date;
	TDateTime endDate = selDate + 1;
	uq->Close();
	uq->Params->Clear();
	uq->SQL->Text = sqlUpdate;
	uq->ParamByName("startDate")->AsDate = selDate;
	uq->ParamByName("endDate")->AsDate = endDate;
	uq->Open();

	gridDelivered->AutoSizeColumns(false, 3 );
	gridDelivered->AutoSizeRows( false, 3 );
}
//---------------------------------------------------------------------------
void __fastcall TfrmScheduleEntry::FormCreate(TObject *Sender)
{
	dtpDelDate->Date = Date();
	RefreshData();

}
//---------------------------------------------------------------------------
void __fastcall TfrmScheduleEntry::FormClose(TObject *Sender, TCloseAction &Action)
{
	// save position and size
	CASQ_FormOps::SavePosition( this );
    Restored = false;
}
//---------------------------------------------------------------------------


void __fastcall TfrmScheduleEntry::FormActivate(TObject *Sender)
{
	if ( ! Restored )
	{	// restore position
		CASQ_FormOps::RestorePosition( this );
		Restored = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmScheduleEntry::btnCloseClick(TObject *Sender)
{
    this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmScheduleEntry::btnInvoiceClick(TObject *Sender)
{
	// send deliveries to MYOB and sales
	// prepare temporary table to hold MYOB information required and field for tick
	// box to indicate if the invoice should be processed or not

}
//---------------------------------------------------------------------------

void __fastcall TfrmScheduleEntry::btnOkClick(TObject *Sender)
{
    this->Close();
}
//---------------------------------------------------------------------------


void __fastcall TfrmScheduleEntry::gridDeliveredCanEditCell(TObject *Sender, int ARow,
          int ACol, bool &CanEdit)
{
	int actCol = 6;
	if ( ACol == actCol )
		CanEdit = true;
	else
		CanEdit = false;
}
//---------------------------------------------------------------------------

