//---------------------------------------------------------------------------
#include "CodeSiteLogging.hpp"

#pragma hdrstop

#include "TaskDialog.hpp"
#include <SysUtils.hpp>
#include <System.DateUtils.hpp>
#include <boost/algorithm/string.hpp>
#include "SyncMyobClass.h"
#include "dmMYOBUnit.h"        	// data module for MYOB (ado connection)

#include "uDMUniDac.h"			// data module for SQLite

#include "SQLStatements.h"		// header with all sql statements
#include "dlgSyncingUnit.h"	// thread for dialog for synchronisation details

//---------------------------------------------------------------------------
#pragma package(smart_init)

// for 32 bit
#if defined __WIN32__
	#pragma comment( lib, "CodeSiteExpressPkg.lib" )
#else
	// for 64 bit
	#pragma comment( lib, "CodeSiteExpressPkg.a" )
#endif

TComponent* MainForm = NULL;
//---------------------------------------------------------------------------
__fastcall clsSyncMYOB::clsSyncMYOB(TComponent* Owner, clsColCustomers* Customers)
{
	//MainForm = Owner;
	if ( ! Customers )
		throw Exception("Null class passed");
	ezbCustomers = Customers;

}
//---------------------------------------------------------------------------
__fastcall clsSyncMYOB::~clsSyncMYOB()
{

}
//---------------------------------------------------------------------------
int __fastcall	clsSyncMYOB::SyncCustomers(const std::wstring& exe, const std::wstring& db )
{
	int newCustomers=0;
	TdlgSync* dlg = new TdlgSync(NULL);
	dlg->UpdateInstruction( L"Synchronising MYOB Customers. \nPlease wait..." );
	dlg->UpdateProgress(L"Retrieving records from MYOB...",0);
	dlg->Show();
	dlg->Refresh();
	try
	{	// retrieve MYOB Customers
		if ( dmMyob->ConnectToDb(exe.c_str(), db.c_str(), false) )
		{   if ( dmMyob->OpenSQL( sqlSyncCustomers.c_str() ) )
			{   clsCustomer* ezbCustomer = NULL;
				TADOQuery* q = dmMyob->adqry;
				while ( ! q->Eof )
				{   ezbCustomer = ezbCustomers->FindByMyobID( q->Fields->Fields[0]->AsInteger );
					if ( ! ezbCustomer )
					{	// add myob customer to ezbCustomers
						ezbCustomer 					= ezbCustomers->NewCustomer();
						ezbCustomer->MyobCardID 		= q->Fields->Fields[0]->AsInteger;
						if ( q->Fields->Fields[4]->AsString == L"Y" )
						{		// is individual
							ezbCustomer->LName 			= q->Fields->Fields[2]->AsString.c_str();
							ezbCustomer->FName 			= q->Fields->Fields[3]->AsString.c_str();
						}
						else
							ezbCustomer->CoName 		= q->Fields->Fields[1]->AsString.c_str();
						ezbCustomer->IsIndividual		= q->Fields->Fields[4]->AsBoolean;
						ezbCustomer->Active 			= true;
						ezbCustomer->Notes 				= q->Fields->Fields[6]->AsString.c_str();
						ezbCustomer->ABN 				= q->Fields->Fields[8]->AsString.c_str();
						ezbCustomer->BillingRate 		= q->Fields->Fields[10]->AsFloat;
						ezbCustomer->SetFixedPriceDate(q->Fields->Fields[12]->AsString.c_str());
						ezbCustomer->SetMyobIdentifiers(q->Fields->Fields[13]->AsString.c_str());
						ezbCustomer->CheckIfExternal();
						newCustomers++;
					}
					q->Next();
					ezbCustomer 						= NULL;
				}
				q->Close();
				dlg->pb->Style = Advsmoothprogressbar::pbstNormal;
				dlg->pb->Maximum = newCustomers;
				if ( newCustomers )
				{	// and save to database
					int p = 0;
					for (int i = 0; i < ezbCustomers->Count(); i++)
					{	clsCustomer* ezCust = ezbCustomers->Customer(i);
						if ( ezCust->dbID == 0 )
						{	p++;
							SyncCustomerAddress( ezCust );
							dlg->UpdateProgress( ezCust->DisplayName(), p);
						   //	MainForm->sb->Panels->Items[3]->Text = String(i);
							dlg->Refresh();
							dmUniDac->AddCustomer(ezCust);
						}
					}
				}
			}

		}
	}
	catch (Exception* ex )
	{   CodeSite->SendException( ex );
		newCustomers = 0;
	}
	dlg->Close();
	delete dlg;
	return newCustomers;
}
//---------------------------------------------------------------------------
int __fastcall clsSyncMYOB::SyncSales(const std::wstring& exe, const std::wstring& db)
{
	//TDateTime startDate = IncYear(StartOfTheMonth(Date()),-2);
	// for old file
	int rtnval = 0;
	TDateTime startDate(2013,1,1);
	if ( ! CheckForHistoricalSales(startDate) )
		return 0;

	TdlgSync* dlg = new TdlgSync( NULL );
	dlg->Show();
	dlg->UpdateInstruction( L"Retrieving historical MYOB sales. \nPlease wait..." );
	dlg->Refresh();
	try
	{	if ( dmMyob->ConnectToDb(exe.c_str(), db.c_str(), false) )
		{   std::wstring sql = boost::replace_all_copy( sqlSyncSales, L"<<<startdate>>>", startDate.FormatString("yyyy-mm-dd").c_str() );
			sql = boost::replace_all_copy( sql, L"<<<enddate>>>", Date().FormatString("yyyy-mm-dd").c_str() );
			sql = boost::replace_all_copy( sql, L"<<<itemnumber>>>", L"0%");
			CodeSite->Send( csmCheckPoint, String(sql.c_str()) );
			if ( dmMyob->OpenSQL( sql.c_str() ) )
			{	TADOQuery* q = dmMyob->adqry;
				dlg->pb->Style = Advsmoothprogressbar::pbstNormal;
				dlg->pb->Maximum = q->RecordCount;
				dmUniDac->qry1->Active = false;
				dmUniDac->qry1->SQL->Text = sqlInsertMyobSale.c_str();
				dmUniDac->qry1->Params->Items[0]->DataType = ftInteger;
				dmUniDac->qry1->Params->Items[1]->DataType = ftDate;
				dmUniDac->qry1->Params->Items[2]->DataType = ftFloat;
				dmUniDac->qry1->Params->Items[3]->DataType = ftFloat;
				dmUniDac->qry1->Params->Items[4]->DataType = ftString;
				dmUniDac->qry1->Params->Items[5]->DataType = ftString;
				dmUniDac->qry1->Params->Items[6]->DataType = ftString;

				dmUniDac->qry1->Prepared = true;
				while ( ! q->Eof )
				{	// fields 0-Myob customer id, 1-Item Number, 2-Item Name, 3-Tax Exc.Total, 4-Qty, 5-invdate
					clsCustomer* ezCustomer						= ezbCustomers->FindByMyobID( q->Fields->Fields[0]->AsInteger );
					if ( ezCustomer )
					{	dmUniDac->qry1->Params->Items[0]->Value 	= ezCustomer->dbID;
						dmUniDac->qry1->Params->Items[1]->Value 	= q->Fields->Fields[1]->AsDateTime;
						dmUniDac->qry1->Params->Items[2]->Value 	= q->Fields->Fields[2]->AsFloat;
						dmUniDac->qry1->Params->Items[3]->Value 	= q->Fields->Fields[3]->AsFloat;
						dmUniDac->qry1->Params->Items[4]->Value 	= q->Fields->Fields[4]->AsString;
						dmUniDac->qry1->Params->Items[5]->Value 	= q->Fields->Fields[5]->AsString;
						dmUniDac->qry1->Params->Items[6]->Value 	= q->Fields->Fields[6]->AsString;
						dmUniDac->qry1->ExecSQL();
						ezCustomer = NULL;
					}
					q->Next();
					dlg->pb->Position = q->RecNo;
					dlg->pb->Refresh();
				}
				q->Close();
				dmUniDac->qry1->Active = false;
				rtnval = 1;
			}
		}
	}
	catch ( Exception* ex)
	{	CodeSite->SendException( ex );
		rtnval = 0;
	}
	dlg->Close();
	delete dlg;
	return rtnval;
}
//---------------------------------------------------------------------------
bool __fastcall clsSyncMYOB::CheckForHistoricalSales( TDateTime startDate )
{
	bool rtnval = false;
	std::wstring sqlDelete( L"DELETE FROM Sales WHERE FillDate >='");
	sqlDelete += startDate.FormatString("yyyy-mm-dd").c_str();
	sqlDelete += L"' AND FillDate < '";
	sqlDelete += Date().FormatString("yyyy-mm-dd").c_str();
	sqlDelete += L"';";
	std::wstring sql( L"SELECT COUNT(ID) FROM Sales WHERE FillDate >= '");
	sql += startDate.FormatString("yyyy-mm-dd").c_str();
	sql += L"' AND FillDate < '";
	sql += Date().FormatString("yyyy-mm-dd").c_str();
	sql += L"';";
	try
	{	if ( dmUniDac->OpenSQL( String( sql.c_str() ) ) )
		{	if ( dmUniDac->qry1->Fields->Fields[0]->AsInteger > 0 )
			{	TAdvTaskDialog *td = new TAdvTaskDialog(MainForm);;
				td->CommonButtons << cbYes << cbNo;
				td->DefaultButton = cbNo;
				td->Icon = tiQuestion;
				td->Title = L"Confrim action...";
				String msg(L"There are ");
				msg += String(dmUniDac->qry1->Fields->Fields[0]->AsInteger);
				msg += L" records existing in the Sales database from " + startDate.FormatString("dd/mm/yyyy");
				msg += L" to today. \nDo you want to continue?";
				td->Instruction = msg;
				td->Content = L"If you continue these historical records will be deleted!";
				if( td->Execute() == mrYes )
				{	dmUniDac->qry1->Close();
					dmUniDac->ExecuteSQL( sqlDelete.c_str() );
					rtnval = true;
                }
				delete td;
			}
			else
				rtnval = true;
		}
	}
	catch( Exception *ex )
	{
		CodeSite->SendException( ex );
		rtnval = false;
	}
	return rtnval;
}
//---------------------------------------------------------------------------
void __fastcall clsSyncMYOB::SyncCustomerAddress( clsCustomer* ezCust )
{
	// get address locations from MYOB
	// address1 is used for billing
	// address2 is used for shipping
	if ( ! ezCust ) return;

	try
	{	// retrieve MYOB address
		std::wstring sql = boost::replace_all_copy( sqlSyncCustomerAddress, L"<<<customerid>>>", String(ezCust->MyobCardID).c_str() );
		if ( dmMyob->OpenSQL( sql.c_str() ) )
		{   TADOQuery* q = dmMyob->adqry;
			while ( ! q->Eof )
			{   // add myob address record
				clsLocation* loc = ezCust->AddLocation();
				if (q->Fields->Fields[0]->AsInteger == 1 )
					loc->IsPrimary = true;
				else
					loc->IsPrimary = false;
				loc->AddressLines 	= q->Fields->Fields[1]->AsString.c_str();
				if ( q->Fields->Fields[2]->AsString.Length() )
					loc->AddressLines += (L"\n" + q->Fields->Fields[2]->AsString).c_str();
				if ( q->Fields->Fields[3]->AsString.Length() )
					loc->AddressLines += (L"\n" + q->Fields->Fields[3]->AsString).c_str();
				if ( q->Fields->Fields[4]->AsString.Length() )
					loc->AddressLines += (L"\n" + q->Fields->Fields[4]->AsString).c_str();
				loc->City 			= q->Fields->Fields[5]->AsString.c_str();
				loc->State 			= q->Fields->Fields[6]->AsString.c_str();
				loc->PostCode 		= q->Fields->Fields[7]->AsString.c_str();
				loc->Country		= q->Fields->Fields[8]->AsString.c_str();

				if (q->Fields->Fields[9]->AsString.SubString(0, 2) == L"04" )
				{	loc->MobilePhone	= q->Fields->Fields[9]->AsString.c_str();
					loc->Phone 			= q->Fields->Fields[10]->AsString.c_str();
				}
				else
				{	loc->Phone			= q->Fields->Fields[9]->AsString.c_str();
					loc->MobilePhone 	= q->Fields->Fields[10]->AsString.c_str();
				}

				loc->Email			= q->Fields->Fields[11]->AsString.c_str();
				loc->ContactName	= q->Fields->Fields[12]->AsString.c_str();
				//ezCust->Locations.push_back(loc);
				dmMyob->adqry->Next();
			}
			q->Close();
		}
	}
	catch (Exception* ex )
	{
		CodeSite->SendException( ex );
	}
	return;
}
//---------------------------------------------------------------------------
int __fastcall clsSyncMYOB::SyncBillingRates(const std::wstring& exe, const std::wstring& db)
{
	TdlgSync* dlg = new TdlgSync(MainForm);
	dlg->Show();
	dlg->UpdateInstruction( L"Retrieving MYOB customer billing rates. \nPlease wait..." );
	dlg->Refresh();
	int rtnval = 0;
	try
	{	if ( dmMyob->ConnectToDb(exe.c_str(), db.c_str(), false) )
		{   if ( dmMyob->OpenSQL( sqlSyncBillingRates.c_str() ) )
			{	TADOQuery* q = dmMyob->adqry;
				dlg->pb->Style = Advsmoothprogressbar::pbstNormal;
				dlg->pb->Maximum = q->RecordCount;
				dmUniDac->qry1->Active = false;
				dmUniDac->qry1->SQL->Text = sqlInsertBillingRate.c_str();
				dmUniDac->qry1->Params->Items[0]->DataType = ftInteger;
				dmUniDac->qry1->Params->Items[1]->DataType = ftDate;
				dmUniDac->qry1->Params->Items[2]->DataType = ftFloat;
				dmUniDac->qry1->Prepared = true;
				while ( ! q->Eof )
				{	clsCustomer* ezCustomer						= ezbCustomers->FindByMyobID( q->Fields->Fields[0]->AsInteger );
					if ( ezCustomer )
					{   dmUniDac->qry1->Params->Items[0]->Value 	= ezCustomer->dbID;
						dmUniDac->qry1->Params->Items[1]->Value 	= Date();
						dmUniDac->qry1->Params->Items[2]->Value 	= q->Fields->Fields[1]->AsFloat;
						dmUniDac->qry1->ExecSQL();
						ezCustomer = NULL;
					}
					q->Next();
					dlg->pb->Position = q->RecNo;
					dlg->pb->Refresh();
				}
				q->Close();
				dmUniDac->qry1->Active = false;
				rtnval = 1;
			}
		}
	}
	catch ( Exception* ex)
	{	CodeSite->SendException( ex );
		rtnval = 0;
	}
	dlg->Close();
	delete dlg;
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsSyncMYOB::SyncEzBillingRates(const std::wstring& exe, const std::wstring& db)
{
	CodeSite->EnterMethod( "clsSyncMYOB::SyncEzBillingRates" );
	TdlgSync* dlg = new TdlgSync(MainForm);
	dlg->Show();
	dlg->UpdateInstruction( L"Updating MYOB customer billing rates. \nPlease wait..." );
	dlg->Refresh();
	int rtnval = 0;
	std::wstring sql( sqlUpdateBillingRate );

	try
	{	if ( dmMyob->ConnectToDb(exe.c_str(), db.c_str(), true) )
		{   dlg->pb->Style = Advsmoothprogressbar::pbstNormal;
			dlg->pb->Maximum = ezbCustomers->Count();
			dmMyob->adqry2->Close();
			dmMyob->conAdo->Execute( L"IDENTIFYBY RECORDID" );
			dmMyob->conAdo->Execute( L"UPDATE DUPLICATES" );
			dmMyob->conAdo->BeginTrans();
			dmMyob->adqry2->SQL->Text = sql.c_str();
			dmMyob->adqry2->Parameters->ParamByName(L"p1")->DataType = Data::Db::ftInteger;
			dmMyob->adqry2->Parameters->ParamByName(L"p2")->DataType = ftFloat;
			dmMyob->adqry2->Prepared = true;
			// for each customer
			for (int i = 0; i < ezbCustomers->Count(); i++)
			{   clsCustomer* cust = ezbCustomers->Customer( i );
				if ( cust )
				{	if ( cust->Active )
					{   if ( cust->MyobCardID > 0 )
						{	dmMyob->adqry2->Parameters->ParamByName(L"p1")->Value = cust->MyobCardID;
							dmMyob->adqry2->Parameters->ParamByName(L"p2")->Value = cust->BillingRate;
							try
							{   dmMyob->adqry2->ExecSQL();
								rtnval++;
							}
							catch ( Exception *ex )
							{   CodeSite->SendException( ex );
								CodeSite->SendMsg( String(cust->DisplayName().c_str()) + cust->dbID );
							}
							dmMyob->conAdo->Execute(L"END TRANSACTION");
						}
						else
							CodeSite->SendMsg( "Customer MYOB Card ID blank: " + String(cust->DisplayName().c_str()) );
					}
				}
				if ( i%25) {
					dlg->UpdateProgress( cust->DisplayName(), i );
				}
				// loop
			}
			dlg->UpdateProgress( L"Committing changes...", ezbCustomers->Count() );
			try
			{	dmMyob->conAdo->CommitTrans();	}
			catch( Exception *ex )
			{	CodeSite->SendException( ex );
			}
			dmMyob->Disconnect();
		}
	}
	catch ( Exception* ex)
	{	CodeSite->SendException( ex );
		rtnval = 0;
	}
	dlg->Close();
	delete dlg;
	CodeSite->SendMsg( "Customers updated: " + String(rtnval) );
	CodeSite->ExitMethod( "clsSyncMYOB::SyncEzBillingRates" );
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsSyncMYOB::AddCustomer( clsCustomer* ezCustomer, const std::wstring& exe, const std::wstring& db )
{
	if ( ! ezCustomer ) return 0;
	int rslt = 0;
	try
	{
		if ( dmMyob->ConnectToDb( exe.c_str(), db.c_str(), true ) )
		{   TADOQuery* q = dmMyob->adqry;
			q->Active = false;
			dmMyob->conAdo->Execute(sqlMyobDuplicates.c_str());
			dmMyob->conAdo->BeginTrans();
			q->SQL->Text = sqlMyobInsertCustomer.c_str();
			for (int i = 0; i < 16; i++)
				q->Parameters->Items[i]->DataType = ftString;
			q->Parameters->Items[0]->Value = L"EZB" + String(ezCustomer->dbID);
			if ( ezCustomer->IsIndividual )
			{	q->Parameters->Items[1]->Value = String(ezCustomer->LName.c_str());
				q->Parameters->Items[2]->Value = String(ezCustomer->FName.c_str());
			}
			else
			{   q->Parameters->Items[1]->Value = String(ezCustomer->CoName.c_str());
				q->Parameters->Items[2]->Value = L"";
			}
			q->Parameters->Items[4]->Value = String(ezCustomer->ABN.c_str());
			q->Parameters->Items[3]->Value = L"Y";
			q->Parameters->Items[5]->Value = String(ezCustomer->Notes.c_str());
			// billing rate
			q->Parameters->Items[6]->DataType = ftFloat;
			q->Parameters->Items[6]->Value = ezCustomer->BillingRate;
			clsLocation* loc = ezCustomer->PrimaryAddress();
			if (loc)
			{	q->Parameters->Items[7]->Value = String(ezCustomer->PrimaryAddress()->AddressLines.c_str());
				q->Parameters->Items[8]->Value = String(ezCustomer->PrimaryAddress()->City.c_str());
				q->Parameters->Items[9]->Value = String(ezCustomer->PrimaryAddress()->State.c_str());
				q->Parameters->Items[10]->Value = String(ezCustomer->PrimaryAddress()->PostCode.c_str());
				q->Parameters->Items[11]->Value = String(ezCustomer->PrimaryAddress()->Country.c_str());
				q->Parameters->Items[12]->Value = String(ezCustomer->PrimaryAddress()->Phone.c_str());
				q->Parameters->Items[13]->Value = String(ezCustomer->PrimaryAddress()->MobilePhone.c_str());
				q->Parameters->Items[14]->Value = String(ezCustomer->PrimaryAddress()->Email.c_str());
				q->Parameters->Items[15]->Value = String(ezCustomer->PrimaryAddress()->ContactName.c_str());
			}
			else
			{   for (int l = 7; l < 16; l++)
					q->Parameters->Items[l]->Value = L"";
			}
			rslt = q->ExecSQL();
			dmMyob->conAdo->Execute(L"END TRANSACTION");
			dmMyob->conAdo->CommitTrans();
			q->Close();
			if ( rslt )
				GetCustomerID( ezCustomer );

		}
	}
	catch( Exception *ex)
	{   CodeSite->SendException( ex );
		dmMyob->conAdo->RollbackTrans();
		dmMyob->Disconnect();
		throw ex;
	}
    dmMyob->Disconnect();
	return rslt;
}
//---------------------------------------------------------------------------
void __fastcall clsSyncMYOB::GetCustomerID( clsCustomer* ezCustomer )
{
	std::wstring cardID(L"EZB");
	cardID.append( String(ezCustomer->dbID).c_str() );
	std::wstring sql = boost::replace_all_copy( sqlMyobGetCustomerID, L"<<<:p1>>>", cardID );
	if ( dmMyob->OpenSQL( sql.c_str() ) )
	{	if ( ! dmMyob->adqry->Eof )
			ezCustomer->MyobCardID = dmMyob->adqry->Fields->Fields[0]->AsInteger;
	}
	dmMyob->adqry->Close();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

