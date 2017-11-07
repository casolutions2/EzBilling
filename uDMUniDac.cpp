//---------------------------------------------------------------------------
#pragma hdrstop
#include "uDMUniDac.h"

//---------------------------------------------------------------------------
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>

//---------------------------------------------------------------------------
#include "CodeSiteLogging.hpp"
#include "SQLStatements.h"		// header with all SQL Statements
//#ifdef SYNCCUSTOMERS
	#include "UFormMain.h"			// main form header
//#else
//	#include "Unit1.h"      // main form header for supplementary app to sync customers
//#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma link "DBAccess"
#pragma link "SQLiteUniProvider"
#pragma link "Uni"
#pragma link "UniProvider"
#pragma link "MemDS"
#pragma link "TaskDialog"
#pragma link "DBAccess"
#pragma link "DBPlanner"
#pragma link "DASQLMonitor"
#pragma link "UniSQLMonitor"
#pragma resource "*.dfm"
TdmUniDac *dmUniDac;
//Caption colors
const TColor 	clDelivery		= TColor(0x00ff00);
const TColor 	clDeliveryTo	= TColor(0x00ff66);
const TColor 	clrGeneric 		= TColor(0xeeeeee);
const TColor 	clrGenericTo 	= TColor(0xeecccc);
//---------------------------------------------------------------------------


#pragma region SQLiteFunctions
Variant __fastcall regexp(Variant *InValues, const int InValues_Size)
{
	try
	{	std::wstring strToSearch( InValues[1] );
		std::wstring strRegex( InValues[0] );
		boost::wregex regex( strRegex );
		if ( boost::regex_match( strToSearch, regex ) )
			return 1;
		else
			return 0;
	}
	catch( Exception* ex )
	{
		CodeSite->SendException( ex );
		return 0;
    }
}
//---------------------------------------------------------------------------

#pragma end_region

//---------------------------------------------------------------------------
__fastcall TdmUniDac::TdmUniDac(TComponent* Owner)
	: TDataModule(Owner)
{
	DBFilename = "";
}
//---------------------------------------------------------------------------
#pragma region ConnectionMethods ****
bool __fastcall TdmUniDac::ConnectToDb( const String& dbFilename )
{
	try
	{	if ( dbFilename.Length() )
		{   if ( DBFilename.Compare( dbFilename ) == 0 )
			{	// check if already connected
				if ( ! uCon->Connected )
					uCon->Connect();
			}
			else
			{	DBFilename = dbFilename;
				DBFilename.Unique();
				uCon->Database = DBFilename;
				uCon->Connect();
			}
		}
		else
		{	if ( ! uCon->Connected )
				uCon->Connect();
		}
	}
	catch( Exception *ex )
	{   //
		throw ex;
	}
	return uCon->Connected;
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::Disconnect()
{
	uCon->Disconnect();
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::tblPlannerAfterOpen(TDataSet *DataSet)
{
	CodeSite->SendMsg( "Planner Opened" );
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::tblPlannerAfterClose(TDataSet *DataSet)
{
	CodeSite->SendMsg( "Planner Closed" );
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::ActivatePlanner( bool state )
{
	if ( state )
		CodeSite->SendMsg( L"Activate Planner objects" );
	else
		CodeSite->SendMsg( L"Deactivate Planner objects" );
	tblPlanner->Active = state;
	tblResource->Active = state;
	plannerDaySource->Active = state;
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region DBEvents
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::uConAfterConnect(TObject *Sender)
{
	CodeSite->SendMsg("Connection open: " + uCon->Database );
	// register UDF's
	RegisterCASQFunctions();
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::RegisterCASQFunctions()
{
	TLiteUtils::RegisterFunction( uCon, "REGEXP", 2, regexp );
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::tblCustomersCalcFields(TDataSet *DataSet)
{
	if ( tblCustomersCoName->Value.Length() )
		tblCustomersDisplayName->Value = tblCustomersCoName->Value;
	else
		tblCustomersDisplayName->Value = tblCustomersLName->Value + ", " + tblCustomersFName->Value;
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::tblCustomersAfterOpen(TDataSet *DataSet)
{
	CodeSite->SendMsg("tblCustomers Opened");
	tblBillingRates->Active = true;
	tblLocations->Active = true;
	tblXtrnlAccts->Active = true;
	tblSales->Active = true;
	tblTanks->Active = true;
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::tblCustomersAfterClose(TDataSet *DataSet)
{
	CodeSite->SendMsg( "tblCustomers Closed" );
	tblBillingRates->Active = false;
	tblLocations->Active = false;
	tblXtrnlAccts->Active = false;
	tblSales->Active = false;
	tblTanks->Active = false;
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::tblCustomersAfterScroll(TDataSet *DataSet)
{
	if ( frmMain )
	{	// check if active and change color of lblDisplayName
		if (tblCustomers->Fields->FieldByName("Active")->AsBoolean )
			frmMain->lblDisplayName->Font->Color = clBlack;
		else
			frmMain->lblDisplayName->Font->Color = clRed;
		// change enabled settings for individual control
		bool val = tblCustomers->Fields->FieldByName("IsIndividual")->AsBoolean;
		frmMain->ebLName->Enabled = val;
		frmMain->Label2->Enabled = val;
		frmMain->ebFName->Enabled = val;
		frmMain->Label3->Enabled = val;
		frmMain->ebCoName->Enabled = !val;
		frmMain->Label1->Enabled = !val;
		// set FixedPricing edit
		if ( (tblCustomers->Fields->FieldByName("FixedPricingDate")->AsDateTime).Val > 0 )
		{   frmMain->cbFixedPricingDate->Checked = true;
			frmMain->dtpFixedPricing->Enabled = true;
		}
		else
		{   frmMain->cbFixedPricingDate->Checked = false;
			frmMain->dtpFixedPricing->Enabled = false;
		}
		// resize grids
		frmMain->gridCustomerSales->AutoSizeColumns( false, 3 );
		frmMain->gridBillingRates->AutoSizeColumns( false, 3 );
		frmMain->gridXtrnlAccts->AutoSizeColumns( false, 3 );
		frmMain->gridCustomerLocations->AutoSizeColumns( false, 3 );
	}
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region GeneralFunctions
//---------------------------------------------------------------------------
int	__fastcall TdmUniDac::ExecuteSQL( const String& sql )
{
	if (!sql.Length()) return 0;
	String tmp( sql.SubString(0,6) );
	tmp = tmp.UpperCase();

	if ( tmp.Compare(L"SELECT") == 0 )
			return 0;
	if ( ! uCon->Connected )
	{   if ( DBFilename.Length() )
		{	if ( ! ConnectToDb( DBFilename ) )
				return 0;
		}
		else
			return 0;
	}
	int rtnval = 0;
	try
	{  	qry1->SQL->Text = sql;
		qry1->ExecSQL();
		rtnval = 1;
	}
	catch(Exception *ex)
	{
		td->Content = (L"SQL statement: " + sql);
		td->Instruction = L"An error has occured in executing the sql statement.";
		td->ExpandedText = ex->Message;
		td->CommonButtons.Clear();
		td->CommonButtons << cbOK;
		td->Execute();
		rtnval = 0;
	}
	return rtnval;
}
//---------------------------------------------------------------------------
bool __fastcall TdmUniDac::OpenSQL( const String& sql )
{
		if ( ! uCon->Connected )
	{	if ( DBFilename.Length() )
		{	if ( ! ConnectToDb( DBFilename ) )
				return false;
		}
		else
			return false;
	}
	qry1->Active = false;
	qry1->SQL->Text = sql;
	qry1->Open();
	return ( qry1->Active ) ;
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::CloseQuery()
{
	qry1->Active = false;
}
//---------------------------------------------------------------------------
#pragma end_region

#pragma region Specific EzBilling functions
//---------------------------------------------------------------------------
int	__fastcall TdmUniDac::AddCustomer( clsCustomer* customer )
{
if (! uCon->Connected )
	{	if ( DBFilename.Length() )
		{	if ( ! ConnectToDb( DBFilename ) )
				return 0;
		}
		else
			return 0;
	}
	int rtnval = 0;
	uCon->StartTransaction();
	try
	{	qry1->Active = false;
		qry1->SQL->Text = sqlInsertCustomer.c_str();
		qry1->Params->Items[0]->DataType 	= Data::Db::ftInteger;
		if ( customer->MyobCardID )
			qry1->Params->Items[0]->Value 		= customer->MyobCardID;
		else
			qry1->Params->Items[0]->Clear();
		qry1->Params->Items[1]->DataType 	= Data::Db::ftString;
		if ( customer->AccountNumber.length() )
			qry1->Params->Items[1]->Value 	= String(customer->AccountNumber.c_str());
		else
			qry1->Params->Items[1]->Clear();
		qry1->Params->Items[2]->DataType 	= Data::Db::ftString;
		qry1->Params->Items[2]->Value 		= String(customer->CoName.c_str());
		qry1->Params->Items[3]->DataType 	= Data::Db::ftString;
		if ( customer->LName.length() )
			qry1->Params->Items[3]->Value 	= String(customer->LName.c_str());
		else
			qry1->Params->Items[3]->Clear();
		qry1->Params->Items[4]->DataType 	= Data::Db::ftString;
		if ( customer->FName.length() )
			qry1->Params->Items[4]->Value 	= String(customer->FName.c_str());
		else
			qry1->Params->Items[4]->Clear();
		qry1->Params->Items[5]->DataType 	= Data::Db::ftString;
		qry1->Params->Items[5]->Value 		= String(customer->ABN.c_str());
		qry1->Params->Items[6]->DataType 	= Data::Db::ftBoolean;
		qry1->Params->Items[6]->Value 		= customer->Active;
		qry1->Params->Items[7]->DataType 	= Data::Db::ftBoolean;
		qry1->Params->Items[7]->Value 		= customer->IsIndividual;
		qry1->Params->Items[8]->DataType 	= Data::Db::ftBoolean;
		qry1->Params->Items[8]->Value 		= customer->External;
		qry1->Params->Items[9]->DataType 	= Data::Db::ftString;
		if ( customer->Notes.length() )
			qry1->Params->Items[9]->Value 	= String(customer->Notes.c_str());
		else
			qry1->Params->Items[9]->Clear();
		qry1->Params->Items[10]->DataType 	= Data::Db::ftBoolean;
		qry1->Params->Items[10]->Value 		= customer->VolumePricing;
		qry1->Params->Items[11]->DataType 	= Data::Db::ftBoolean;
		qry1->Params->Items[11]->Value 		= customer->PricingInterest;
		qry1->Params->Items[12]->DataType	= Data::Db::ftDateTime;
		qry1->Params->Items[12]->Value 		= customer->FixedPriceDate;
		qry1->ExecSQL();
		qry1->Close();
		qry1->Params->Clear();
		qry1->SQL->Text = L"SELECT last_insert_rowid()";
		qry1->Open();
		customer->dbID = qry1->Fields->Fields[0]->AsInteger;
		qry1->Close();

		// update extAccounts
		if ( customer->ExtAccounts.size() )
		{	// update each external account
			qry1->SQL->Text = sqlInsertExternalAccount.c_str();
			qry1->Params->Items[0]->DataType = Data::Db::ftInteger;
			qry1->Params->Items[1]->DataType = Data::Db::ftString;
			qry1->Params->Items[2]->DataType	= Data::Db::ftString;
			qry1->Prepared = true;
			BOOST_FOREACH( std::wstring eac, customer->ExtAccounts)
			{	qry1->Params->Items[0]->Value = customer->dbID;
				qry1->Params->Items[1]->Value = String(eac.substr(0,2).c_str());
				qry1->Params->Items[2]->Value = String(eac.substr(3, eac.length()-3).c_str());
				qry1->ExecSQL();
			}
		}
		InsertBillingRate( customer );
		if ( customer->Locations.size() )
			// update locations
			InsertCustomerLocations( customer );

		uCon->Commit();
		rtnval = customer->dbID;
	}
	catch( Exception* ex )
	{
		uCon->Rollback();
		rtnval = 0;
	}
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall TdmUniDac::InsertCustomerLocations( clsCustomer* customer )
{   //
	return 0;
}
//---------------------------------------------------------------------------
bool __fastcall TdmUniDac::UpdateTable(const String& tablename, const String& fieldname, boost::variant<std::wstring, int, float>& value, const std::wstring& criteria)
{   //
	 return false;
}
//---------------------------------------------------------------------------
int	 __fastcall TdmUniDac::InsertLocation(clsLocation* loc)
{   //
	return 0;
}
//---------------------------------------------------------------------------
int  __fastcall TdmUniDac::InsertBillingRate( clsCustomer* customer )
{   //
	return 0;
}
//---------------------------------------------------------------------------
int  __fastcall TdmUniDac::InsertSales( std::vector<clsInvoice*>* invoices )
{   //
	return 0;
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region plannerDaySource Events ***
void __fastcall TdmUniDac::plannerDaySourceFieldsToItem(TObject *Sender, TFields *Fields,
		  TPlannerItem *Item)
{
	//	if ( frmMain )
	{	// get additional information for bookings
		// if resource = 0 then generic item
		if (Fields->FieldByName(L"Resource")->AsInteger > 1 )
		{	clsEzBooking* gb 		= new clsEzBooking();
			gb->CustomerID 			= Fields->FieldByName(L"CustomerID")->AsInteger;
			gb->LocationID 			= Fields->FieldByName(L"LocationID")->AsInteger;
			gb->EstQty 				= Fields->FieldByName(L"EstQty")->AsInteger;
			gb->ResourceID          = Fields->FieldByName(L"Resource")->AsInteger;
			Item->CaptionBkg 		= clDelivery;
			Item->CaptionBkgTo		= clDeliveryTo;
			Item->OwnsItemObject 	= true;
			Item->ItemObject 		= gb;
		}
		else
		{	//Item->Editor = frmMain->plannerItemEditorDefault;
			Item->CaptionBkg = clrGeneric;
			Item->CaptionBkgTo = clrGenericTo;
		}
		Item->CaptionType = ctTimeText;
		//Item->ItemPos 	= Fields->FieldByName(L"Resource")->AsInteger;      // not required the component will handle this
	}
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::plannerDaySourceItemToFields(TObject *Sender, TFields *Fields,
		  TPlannerItem *Item)
{
	// save additional information for bookings
	//ShowMessage(L"TODO: ItemToFields mapping");
	//Fields->FieldByName(L"Resource")->AsInteger = Item->ItemPos + 1;      // not required the component will handle this
	if ( Item->ItemObject )
	{	clsEzBooking* gb 								= static_cast<clsEzBooking*>(Item->ItemObject);
		Fields->FieldByName(L"CustomerID")->AsInteger 	= gb->CustomerID;
		Fields->FieldByName(L"LocationID")->AsInteger 	= gb->LocationID;
		Fields->FieldByName(L"EstQty")->AsInteger 		= gb->EstQty;
	}
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::plannerDaySourceSetFilter(TObject *Sender)
{
	// set a filter for the day sought
	tblPlanner->Filter = "StartTime > '" + frmMain->plannerCal->Date.FormatString("yyyy-mm-dd") + "' AND EndTime < '" + (frmMain->plannerCal->Date + 1).FormatString("yyyy-mm-dd")  + "'";
	tblPlanner->Filtered = true;
}
//---------------------------------------------------------------------------
void __fastcall TdmUniDac::plannerDaySourceItemsRead(TObject *Sender)
{
	CodeSite->SendMsg( "plannerDaySourceItemsRead" );
	frmMain->UpdateScheduleFooter();
}
//---------------------------------------------------------------------------
#pragma end_region



