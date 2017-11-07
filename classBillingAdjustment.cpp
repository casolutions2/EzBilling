//---------------------------------------------------------------------------
#include <vcl.h>
#include "CodeSiteLogging.hpp"
#pragma hdrstop
#include <Windows.h>				// for messagebox function
#include <System.SysUtils.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <time.h>
#include "classBillingAdjustment.h"
#include "FormBillingAdjustment.h"
#include "dmMYOBUnit.h"				// myob data connection module
#include "SQLStatements.h"			// SQL statement file for both MYOB and SQLite
#include "uDMUniDac.h"					// data module for UniDac connection to SQLite
#include "UFormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
const String msgReady = L"<FONT color=\"clBlue\">Ready</FONT>";

//---------------------------------------------------------------------------
__fastcall clsBillingAdjust::clsBillingAdjust(bool useVolumes)
{
	VolumeAdjust	= useVolumes;
//	ezCustomers 	= NULL;
	vCustAdjust		= NULL;
	_targetForm     = NULL;
}
//---------------------------------------------------------------------------
__fastcall clsBillingAdjust::~clsBillingAdjust()
{
	if ( vCustAdjust )
	{	DeleteBillingCustomers();
		delete vCustAdjust;   vCustAdjust = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall clsBillingAdjust::SetTargetForm( TForm* AForm )
{
	_targetForm = AForm;
}
//---------------------------------------------------------------------------
int __fastcall clsBillingAdjust::LoadPrevious()
{
	// load previous billing figures
	std::wstring sql = L"SELECT * FROM LastBillingAdjustment;";
	if( ! _targetForm ) return 0;

	if ( dmUniDac->OpenSQL( String( sql.c_str() ) ) )
	{	TformBillingAdjust* form = static_cast<TformBillingAdjust*>(_targetForm);
		if ( ! dmUniDac->qry1->Eof )
		{	form->ebCP->FloatValue 				= dmUniDac->qry1->FieldByName(L"GasCostPrice")->AsFloat;
			form->ebMpc->FloatValue 			= dmUniDac->qry1->FieldByName(L"MarkUpOnCost")->AsFloat*100;
			form->ebAmax->FloatValue			= dmUniDac->qry1->FieldByName(L"ChangeMax")->AsFloat;
			form->ebAmin->FloatValue			= dmUniDac->qry1->FieldByName(L"ChangeMin")->AsFloat;
			form->ebApc->FloatValue				= dmUniDac->qry1->FieldByName(L"PctDifToAdj")->AsFloat*100;
			form->ebAdjPrice->FloatValue		= dmUniDac->qry1->FieldByName(L"AdjustedValue")->AsFloat;
			form->ebDF->FloatValue				= dmUniDac->qry1->FieldByName(L"DaysFree")->AsInteger;
			form->ebIR->FloatValue				= dmUniDac->qry1->FieldByName(L"APR")->AsFloat*100;
			form->ebLastAdjCustomerCount->IntValue	= dmUniDac->qry1->FieldByName(L"AdjustedCustomers")->AsInteger;
			form->ebLastAdjDate->Text			= dmUniDac->qry1->FieldByName(L"AdjustedTimeStamp")->AsDateTime.FormatString("dd/mm/yyyy HH:mm");
			form->ebLastAdjValue->Text			= dmUniDac->qry1->FieldByName(L"AdjustedValue")->AsFloat;
			form->ebMFx->FloatValue				= dmUniDac->qry1->FieldByName(L"FixedMarkUp")->AsFloat;
			form->ebNewMax->FloatValue			= dmUniDac->qry1->FieldByName(L"NewBillingMaximum")->AsFloat;
			form->ebNewMin->FloatValue			= dmUniDac->qry1->FieldByName(L"NewBillingMinimum")->AsFloat;
			form->ebResetToZero->FloatValue		= dmUniDac->qry1->FieldByName(L"ResetToZero")->AsFloat;
			form->ebRestrictMax->FloatValue		= dmUniDac->qry1->FieldByName(L"AdjustExistingMaximum")->AsFloat;
			form->ebRestrictMin->FloatValue		= dmUniDac->qry1->FieldByName(L"AdjustExistingMinimum")->AsFloat;
			form->ebVMd->IntValue				= dmUniDac->qry1->FieldByName(L"VolumeMedian")->AsInteger;
			form->ebVMn->IntValue				= dmUniDac->qry1->FieldByName(L"VolumeMinimum")->AsInteger;
		}
		dmUniDac->CloseQuery();
		return 1;
	}
	else
		return 0;
}
//---------------------------------------------------------------------------
bool __fastcall clsBillingAdjust::Process( )
{
	TVolumeAdjustment va = static_cast<TformBillingAdjust*>(_targetForm)->PopulateAdjustment();
	va.VolumeAdjust = VolumeAdjust;
	if (! ConfirmAdjustment(va.AdjustPrice, va.VolumeAdjust) ) return false;
	if (ProcessAdjustment( va ) > 0)
		return true;
	else
		return false;
}
//---------------------------------------------------------------------------
bool __fastcall clsBillingAdjust::ConfirmAdjustment(Currency AdjustPrice, bool useVolumes)
{
	bool rtnval = false;
	if (! _targetForm ) return false;
	TAdvTaskDialog* td = static_cast<TformBillingAdjust*>(_targetForm)->td2;
	td->Clear();
	td->Title = L"Fixed Price Adjustment";
	td->Icon = tiShield; //tiWarning;  // tiShield tiQuestion tiInformation
	td->Instruction 	= L"You are about to modify billing rates! \nDo you wish to continue?";
	td->CommonButtons << cbYes << cbNo;
	String msg(L"You are about to ");
	if ( useVolumes  )
		msg += L"adjust all qualifying prices by a calculated amount based on VOLUME.";
	else
	{	msg += AdjustPrice < 0 ? L" DECREASE " : L" INCREASE ";
		msg += L" all qualifying prices by " + CurrToStrF(AdjustPrice, ffCurrency, 4 ) + L".";
	}
	td->ExpandedText = msg;
	if( td->Execute() == mrYes )
		rtnval = true;
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsBillingAdjust::ProcessAdjustment(TVolumeAdjustment va)
{
	int rtnval = 0;
	try
	{   // create vector of customerBilling records
		vCustAdjust = new std::vector<TCustomerBilling*>(0);
		// collect stats from SQL
		if ( CollectCustomerStats(va)== 0 )
			return 0;
		if ( va.VolumeAdjust )
		{	UpdateSQLVolumes();
			LoadVolumesFromSQL();
		}
		if ( frmMain->App->FilesMyobExist )
			// collect Stats from MYOB
			CollectMYOBStats();

		BOOST_FOREACH( TCustomerBilling* cb, *vCustAdjust )
		{ 	cb->CalculateNewBilling();
			if ( va.VolumeAdjust )
				cb->CalculateAdjustment();
			// update sqlite CustomerBillingRates
			cb->Customer->BillingRate = cb->NewGasPrice();
			dmUniDac->InsertBillingRate( cb->Customer );
		}
		OutputResults();

		SaveBillingAdjustment(va);
		if ( frmMain->App->FilesMyobExist )
			// update MYOB
			UpdateMyobBilling( );

		rtnval = vCustAdjust->size();

	}
	catch( Exception *ex )
	{	CodeSite->SendException( ex );
		ShowMessage( L"An error occured\n" + ex->Message );
		rtnval = 0;
	}
	DeleteBillingCustomers();
	delete vCustAdjust;
	vCustAdjust = NULL;
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsBillingAdjust::CollectCustomerStats(TVolumeAdjustment va)
{
	if ( ! vCustAdjust )
		vCustAdjust = new std::vector<TCustomerBilling*> (0);
	// update billing rates
	for (int i = 0; i < frmMain->Customers->Count(); i++)
	{	clsCustomer* ezcustomer = frmMain->Customers->Customer(i);
		if ( ezcustomer->BillingRate > va.Adjust.Minimum )
		{	// can process
			TCustomerBilling* cb 	= new TCustomerBilling();
			cb->Customer			= ezcustomer;
			cb->VolAdjust			= va;
			cb->OldBillingRate 		= ezcustomer->BillingRate;
			//cb->Volume 				= ??;
			cb->FixedPricingDate	= ezcustomer->FixedPriceDate;
			vCustAdjust->push_back(cb);
		}
	}
	return vCustAdjust->size();
}
//---------------------------------------------------------------------------
int __fastcall clsBillingAdjust::CollectMYOBStats()
{
	// connect to myob
	if ( ! dmMyob->ConnectToDb(frmMain->App->MYOBApp.c_str(), frmMain->App->MYOBData.c_str()) ) return 0;
	{	frmMain->sb->Panels->Items[0]->Text = L"<B>Fetching MYOB records...</B>"; frmMain->sb->Refresh();
		if ( dmMyob->OpenSQL(sqlBillingAdjust.c_str()) )	//		->adqry->SQL->Text = sqlBillingAdjust.c_str();
		{	if ( ! dmMyob->adqry->Eof )
			{	frmMain->sb->Panels->Items[0]->Text = L"<B>Processing adjustment...</B>";   frmMain->sb->Refresh();
				do
				{	clsCustomer* ezcustomer = frmMain->Customers->FindByMyobID(dmMyob->adqry->Fields->Fields[0]->AsInteger);
					if ( ezcustomer )
					{	TCustomerBilling* cb = FindBillingCustomer( ezcustomer );
						if ( cb )
						{	cb->ReceivableDays		= dmMyob->adqry->Fields->Fields[2]->AsInteger;
							cb->TotalPaidInvoices	= dmMyob->adqry->Fields->Fields[3]->AsFloat;
						}
					}
					dmMyob->adqry->Next();
				} while ( ! dmMyob->adqry->Eof );
			}
		}
		dmMyob->Disconnect();
	}
	return vCustAdjust->size();
}

//---------------------------------------------------------------------------
TCustomerBilling* __fastcall clsBillingAdjust::FindBillingCustomer( clsCustomer* ezcustomer )
{
	BOOST_FOREACH( TCustomerBilling* cb, *vCustAdjust )
	{	if ( cb->Customer == ezcustomer )
			return cb;
	}
	return NULL;
}
//---------------------------------------------------------------------------
void __fastcall clsBillingAdjust::OutputResults()
{
	TAdvStringGrid* baGrid = frmMain->baGrid;
	if ( vCustAdjust->size() )
	{	frmMain->sb->Panels->Items[0]->Text = L"<B>Updating MYOB records...</B>"; frmMain->sb->Refresh();
		// output records to grid
		baGrid->RowCount = vCustAdjust->size() + 1;
		int r = 1;
		//baGrid->BeginUpdate();
		BOOST_FOREACH( TCustomerBilling *cst, *vCustAdjust )
		{   baGrid->Cells[0][r] 	= r;
			baGrid->Cells[1][r] 	= cst->Customer->DisplayName().c_str();
			baGrid->Floats[2][r]	= cst->Volume;
			baGrid->Ints[3][r] 	= cst->DaysToPay;
			if ( cst->FixedPricingDate.Val > 0)
				baGrid->Cells[4][r] 	= cst->FixedPricingDate.FormatString("dd/mm/yyyy");
			baGrid->Floats[5][r] 	= cst->OldBillingRate;
			baGrid->Floats[6][r] 	= cst->NewBillingRate;
			baGrid->Floats[7][r] 	= cst->TheoreticalPrice;
			baGrid->Floats[8][r] 	= cst->InterestComponent;
			baGrid->Floats[9][r] 	= cst->TheoreticalPrice - cst->NewBillingRate;
			baGrid->Floats[10][r] 	= cst->AdjustmentAmount;
			baGrid->Floats[11][r]	= cst->NewGasPrice();
			r++;
		}
		//baGrid->EndUpdate();
		baGrid->Visible = true;
	}
}
//---------------------------------------------------------------------------
int __fastcall clsBillingAdjust::LoadVolumesFromSQL()
{
	if ( dmUniDac->OpenSQL( String( sqlGetCustomerVolumes.c_str() ) ) )
	{	while ( ! dmUniDac->qry1->Eof )
		{   TCustomerBilling* cb = FindBillingCustomer( frmMain->Customers->FindByDbID(dmUniDac->qry1->Fields->Fields[0]->AsInteger) );
			if ( cb )
				cb->Volume = dmUniDac->qry1->Fields->Fields[1]->AsFloat;
			dmUniDac->qry1->Next();
		}
	}
	return 1;
}
//---------------------------------------------------------------------------
int __fastcall clsBillingAdjust::UpdateSQLVolumes()
{
	// retrieve sales volumes from SQLite
	TDateTime startDate = StartOfTheMonth( IncYear( IncMonth( Date(), -1 ), -1 ) );
	TDateTime endDate = EndOfTheMonth( IncMonth( Date(), -1 ) );
	std::wstring sql(sqlSQLUpdateSalesVolumes);
	sql = boost::replace_all_copy(sql, L"<<<startdate>>>", startDate.FormatString("yyyy-mm-dd").c_str());
	sql = boost::replace_all_copy(sql, L"<<<enddate>>>", endDate.FormatString("yyyy-mm-dd").c_str());
	CodeSite->Send(0, String(sql.c_str()));
	if ( dmUniDac->ExecuteSQL( String( sql.c_str() ) ) )
		return 1;
	else
		return 0;
}
//---------------------------------------------------------------------------
bool __fastcall clsBillingAdjust::ConfirmUpdateVolumes()
{   TAdvTaskDialog* td1 = frmMain->td1;
	td1->Clear();
	td1->Icon = tiQuestion;
	td1->Title = L"Confirmation...";
	td1->Instruction = L"Do you want to update sales volumes?";
	td1->Content = "This process will update the MYOB customer records for the sales volumes in the previous 12 months.";
	td1->CommonButtons << cbYes << cbNo;
	return ( td1->Execute() == mrYes );
}
//---------------------------------------------------------------------------
void __fastcall clsBillingAdjust::SaveBillingAdjustment(const TVolumeAdjustment& va)
{
	TAdvTaskDialog* td1 = frmMain->td1;
	td1->Clear();
	td1->Icon = tiQuestion;
	td1->Instruction = L"Do you want to save this billing adjustment?";
	td1->CustomButtons->Add(L"Yes please");
	td1->CustomButtons->Add(L"No thank you");
	td1->DefaultButton = 100;
	td1->Options << doCommandLinks;
	if ( td1->Execute() == 101 )
		return;

	std::wstring sql = L"INSERT INTO BillingAdjustments VALUES (:p1, :p2, :p3, :p4, :p5, :p6, :p7, :p8, :p9, :p10, :p11, :p12, :p13, :p14, :p15, :p16, CURRENT_TIMESTAMP, :p17, :p18);";
	try
	{	dmUniDac->qry1->SQL->Text = sql.c_str();
	   //	dmUniDac->qry1->Prepare();
		Currency cy; float cyd = double(cy);
		dmUniDac->qry1->ParamByName(L"p1")->AsFloat 	= RoundTo( double( va.GasCostPrice ), -4 );
		dmUniDac->qry1->ParamByName(L"p2")->AsFloat 	= va.CostMarkUp;
		dmUniDac->qry1->ParamByName(L"p3")->AsFloat 	= RoundTo( double( va.FixedMarkUp ), -4 );
		dmUniDac->qry1->ParamByName(L"p4")->AsFloat 	= va.Adjust.Pct;
		dmUniDac->qry1->ParamByName(L"p5")->AsFloat 	= RoundTo( double( va.Adjust.Minimum), -4 );
		dmUniDac->qry1->ParamByName(L"p6")->AsFloat 	= RoundTo( double( va.Adjust.Maximum ), -4 );
		dmUniDac->qry1->ParamByName(L"p7")->AsInteger	= va.Volume.Median;
		dmUniDac->qry1->ParamByName(L"p8")->AsInteger	= va.Volume.Minimum;
		dmUniDac->qry1->ParamByName(L"p9")->AsFloat 	= RoundTo( double( va.InterestRate ), -4 );
		dmUniDac->qry1->ParamByName(L"p10")->AsInteger	= va.DaysFree;
		dmUniDac->qry1->ParamByName(L"p11")->AsFloat 	= RoundTo( double( va.Restrict.AbsoluteMin ), -4 );
		dmUniDac->qry1->ParamByName(L"p12")->AsFloat 	= RoundTo( double( va.Restrict.Minimum ), -4 );
		dmUniDac->qry1->ParamByName(L"p13")->AsFloat 	= RoundTo( double( va.Restrict.Maximum ), -4 );
		dmUniDac->qry1->ParamByName(L"p14")->AsFloat 	= RoundTo( double( va.NewRange.Minimum ), -4 );
		dmUniDac->qry1->ParamByName(L"p15")->AsFloat 	= RoundTo( double( va.NewRange.Maximum ), -4 );
		dmUniDac->qry1->ParamByName(L"p16")->AsFloat 	= RoundTo( double( va.AdjustPrice ),-4 );
		dmUniDac->qry1->ParamByName(L"p17")->AsFloat 	= RoundTo( double( va.AdjustPrice ),-4 );
		dmUniDac->qry1->ParamByName(L"p18")->AsInteger = vCustAdjust->size();
		dmUniDac->qry1->ExecSQL();
	}
	catch (Exception *ex )
	{
		CodeSite->SendException( ex );
		frmMain->ShowError( ex->Message );
	}
	dmUniDac->qry1->Active = false;
    return;
}
//---------------------------------------------------------------------------
void __fastcall clsBillingAdjust::DeleteBillingCustomers()
{
	if ( vCustAdjust )
	{	BOOST_FOREACH(TCustomerBilling* cst, *vCustAdjust)
			delete cst;
	}
}
//---------------------------------------------------------------------------
bool __fastcall clsBillingAdjust::UpdateMyobBilling()
{
	CodeSite->EnterMethod( "clsBillingAdjust::UpdateMyobBilling" );
	bool rtnval = false;
	Screen->Cursor = crHourGlass;
	if ( ! dmMyob->ConnectToDb( frmMain->App->MYOBApp.c_str(), frmMain->App->MYOBData.c_str(), true ) )
	{   CodeSite->SendMsg( "Cannot connect to MYOB" );
		CodeSite->ExitMethod( "clsBillingAdjust::UpdateMyobBilling" );
		Screen->Cursor = crDefault;
		return rtnval;
	}
	std::wstring sql( sqlUpdateBillingRate );
	frmMain->sb->Panels->Items[0]->Text = L"<B>Updating MYOB billing rates...</B>";
	frmMain->sb->Panels->Items[1]->Style = psProgress;
	frmMain->sb->Panels->Items[1]->Progress->Max = vCustAdjust->size();
	frmMain->sb->Panels->Items[1]->Progress->Prefix = L"Updating record  ";
	frmMain->sb->Refresh();
	try
	{	dmMyob->adqry2->Close();
		dmMyob->conAdo->Execute( L"IDENTIFYBY RECORDID" );
		dmMyob->conAdo->Execute( L"UPDATE DUPLICATES" );
		dmMyob->conAdo->BeginTrans();
		dmMyob->adqry2->SQL->Text = sql.c_str();
		dmMyob->adqry2->Parameters->ParamByName(L"p1")->DataType = Data::Db::ftInteger;
		dmMyob->adqry2->Parameters->ParamByName(L"p2")->DataType = ftFloat;
		dmMyob->adqry2->Prepared = true;
		int i = 0;
		BOOST_FOREACH( TCustomerBilling* cust, *vCustAdjust )
		{	frmMain->sb->Panels->Items[1]->Progress->Position = ++i;
			dmMyob->adqry2->Parameters->ParamByName(L"p1")->Value = cust->Customer->MyobCardID;
			dmMyob->adqry2->Parameters->ParamByName(L"p2")->Value = cust->NewGasPrice();
			dmMyob->adqry2->ExecSQL();
			dmMyob->conAdo->Execute(L"END TRANSACTION");
			//dmMyob->adqry->Next();
			frmMain->sb->Refresh();
		}
		frmMain->sb->Panels->Items[1]->Style = psHTML;
		frmMain->sb->Panels->Items[1]->Text = L"<B>Committing updates...</B>";
		frmMain->sb->Refresh();
		dmMyob->conAdo->CommitTrans();
		dmMyob->Disconnect();
		rtnval = true;
	}
	catch( Exception *ex )
	{
		dmMyob->conAdo->RollbackTrans();
		MessageBoxEx( frmMain->Handle, ex->Message.c_str(), L"ERROR - Update Billing Rates", MB_OK ,0 );
		CodeSite->SendException( ex );
		rtnval = false;
		Screen->Cursor = crDefault;
		//throw ex;
	}
	frmMain->sb->Panels->Items[1]->Style = psHTML;
	frmMain->sb->Panels->Items[0]->Text = L"<FONT color=\"clBlue\">Ready</FONT>";
	Screen->Cursor = crDefault;
	CodeSite->ExitMethod( "clsBillingAdjust::UpdateMyobBilling" );
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsBillingAdjust::UpdateVolumesInMYOB()
{
	#pragma region Comments
	// this module reads from MYOB the sales volumes for the past 12 months
	// in then writes these volumes to CustomField3 for futher manipulation during
	// Volume price adjustment
	#pragma end_region
	if ( ! dmMyob->ConnectToDb( frmMain->App->MYOBApp.c_str(), frmMain->App->MYOBData.c_str(), true ) )
		return 0;
	TDateTime startDate = StartOfTheMonth( IncYear( IncMonth( Date(), -1 ), -1 ) );
	TDateTime endDate = EndOfTheMonth( IncMonth( Date(), -1 ) );
	#if defined _DEBUG
	startDate 	= TDateTime(2010,7,1);
	endDate 	= TDateTime(2011,7,31);
	#endif
	Screen->Cursor = crHourGlass;
	TAdvOfficeStatusBar* sb = frmMain->sb;
	try
	{
		std::wstring sql(sqlSalesAnalysis);
		sql = boost::replace_all_copy(sql, L"<<<startdate>>>", startDate.FormatString("yyyy-mm-dd").c_str());
		sql = boost::replace_all_copy(sql, L"<<<enddate>>>", endDate.FormatString("yyyy-mm-dd").c_str());
		sql = boost::replace_all_copy(sql, L"<<<itemnumbers>>>", L"'04','04k','04o'");
		CodeSite->Send( sql.c_str() );
		sb->Panels->Items[0]->Text = "<B>Fetching data from MYOB...</B>";
		sb->Refresh();
		if ( dmMyob->OpenSQL( sql.c_str() ) )
		{   int r = 1;	frmMain->grid->ColCount = dmMyob->adqry->FieldCount+1;
			for (int c = 0; c < dmMyob->adqry->Fields->Count; c++)
				frmMain->grid->Cells[c+1][0] = dmMyob->adqry->Fields->Fields[c]->FieldName;
			while (! dmMyob->adqry->Eof )
			{   frmMain->grid->Cells[0][r] = r;
				for (int c = 0; c < dmMyob->adqry->Fields->Count; c++)
				{	switch ( dmMyob->adqry->Fields->Fields[c]->DataType )
					{	case ftFloat:
						case Data::Db::ftInteger:
						case ftSmallint:
						case ftWord:
						case Data::Db::ftCurrency:
						case ftLargeint:
						case Data::Db::ftSingle:
							frmMain->grid->Floats[c+1][r] = dmMyob->adqry->Fields->Fields[c]->AsFloat;

						default:
							frmMain->grid->Cells[c+1][r] = dmMyob->adqry->Fields->Fields[c]->AsString;
					}
				}
				if ( frmMain->grid->RowCount < ++r )
					frmMain->grid->RowCount++;
				dmMyob->adqry->Next();
			}
			frmMain->grid->AutoSizeColumns(false, 4);
			frmMain->pgrMain->ActivePage = frmMain->pgSundryGrid;
			frmMain->pgrMain->Visible = true;
			// update the value in customer field
			{  	dmMyob->adqry->First();
				sb->Panels->Items[0]->Text = L"<B>Updating purchased volumes in MYOB...</B>";
				sb->Panels->Items[1]->Style = psProgress;
				sb->Refresh();
				dmMyob->conAdo->Execute(L"IDENTIFYBY RECORDID");	//ExecuteSQL(L"IDENTIFYBY RECORDID");
				dmMyob->conAdo->Execute(L"UPDATE DUPLICATES");		//ExecuteSQL(L"UPDATE DUPLICATES");
				dmMyob->conAdo->BeginTrans();
				try
				{	dmMyob->adqry2->SQL->Text = sqlInsertVolumes.c_str();
					dmMyob->adqry2->Parameters->ParamByName(L"pRecID")->DataType = Data::Db::ftInteger;
					dmMyob->adqry2->Parameters->ParamByName(L"pVolume")->DataType = Data::Db::ftString;
					dmMyob->adqry2->Prepared = true;
					sb->Panels->Items[1]->Progress->Max = dmMyob->adqry->RecordCount;
					sb->Panels->Items[1]->Progress->Prefix = L"Updating records  ";
					while ( ! dmMyob->adqry->Eof )
					{   sb->Panels->Items[1]->Progress->Position = dmMyob->adqry->RecNo;
						dmMyob->adqry2->Parameters->ParamByName(L"pRecID")->Value = dmMyob->adqry->Fields->Fields[0]->AsInteger;
						dmMyob->adqry2->Parameters->ParamByName(L"pVolume")->Value = dmMyob->adqry->Fields->Fields[3]->AsFloat;
						dmMyob->adqry2->ExecSQL();
						dmMyob->adqry->Next();
						sb->Refresh();
					}
					sb->Panels->Items[1]->Style = psHTML;
					sb->Panels->Items[1]->Text = L"<B>Committing updates...</B>";
					sb->Refresh();
					dmMyob->conAdo->CommitTrans();
				}
				catch (Exception *ex)
				{	CodeSite->SendException( ex );
					dmMyob->conAdo->RollbackTrans();
				}
			}
			dmMyob->adqry->Close();
			dmMyob->Disconnect();
			sb->Panels->Items[0]->Text = msgReady;
			sb->Panels->Items[1]->Text = L"";
		}
	}
	catch ( Exception * ex)
	{
		CodeSite->SendException( ex );
	}
	Screen->Cursor = crDefault;
	return 1;
}
//---------------------------------------------------------------------------

