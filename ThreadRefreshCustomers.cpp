//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop
#include <System.DateUtils.hpp>
#include "ThreadRefreshCustomers.h"
#include "uDMUniDac.h"
#include "SQLStatements.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall RefreshCustomers::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
clsColCustomers* tCustomers = NULL;
String tSQL(L"");

__fastcall RefreshCustomers::RefreshCustomers(bool CreateSuspended, clsColCustomers* customers, const std::wstring sql)
	: TThread(CreateSuspended)
{
	 tCustomers = customers;
	 tSQL = String( sql.c_str() );
}
//---------------------------------------------------------------------------
void __fastcall RefreshCustomers::Execute()
{
	NameThreadForDebugging(System::String(L"CASQRefreshCustomers"));
	//---- Place thread code here ----
	FreeOnTerminate = false;
	try
	{
	if ( tCustomers )
	{	if ( tSQL.Length() )
		{	if ( dmUniDac->OpenSQL(tSQL) )
			{	while ( ! dmUniDac->qry1->Eof )
				{   clsCustomer* cust = tCustomers->NewCustomer();
					cust->dbID 			= dmUniDac->qry1->Fields->Fields[0]->AsInteger;
					if ( dmUniDac->qry1->Fields->Fields[1]->IsNull )
						cust->MyobCardID	= 0;
					else
						cust->MyobCardID 	= dmUniDac->qry1->Fields->Fields[1]->AsInteger;

					cust->AccountNumber = dmUniDac->qry1->Fields->Fields[2]->AsString.c_str();
					cust->CoName		= dmUniDac->qry1->Fields->Fields[3]->AsString.c_str();
					cust->LName 		= dmUniDac->qry1->Fields->Fields[4]->AsString.c_str();
					cust->FName			= dmUniDac->qry1->Fields->Fields[5]->AsString.c_str();
					cust->ABN			= dmUniDac->qry1->Fields->Fields[6]->AsString.c_str();
					cust->Active		= dmUniDac->qry1->Fields->Fields[7]->AsBoolean;
					cust->IsIndividual	= dmUniDac->qry1->Fields->Fields[8]->AsBoolean;
					cust->External 		= dmUniDac->qry1->Fields->Fields[9]->AsBoolean;
					cust->Notes			= dmUniDac->qry1->Fields->Fields[10]->AsString.c_str();
					cust->VolumePricing	= dmUniDac->qry1->FieldByName(L"VolPricingAdjust")->AsBoolean;
					cust->PricingInterest = dmUniDac->qry1->FieldByName(L"VolPricingInterest")->AsBoolean;
					cust->FixedPriceDate = dmUniDac->qry1->FieldByName(L"FixedPricingDate")->AsDateTime;
					dmUniDac->qry1->Next();
				}
				dmUniDac->qry1->Close();
				PopulateBillingRates();
				PopulateLocations();
				PopulateContainers();
			}
		}
	}

	}
	catch (Exception &ex)
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall RefreshCustomers::PopulateLocations()
{
	try
	{
		if ( dmUniDac->OpenSQL( String( sqlGetLocations.c_str() ) ) )
		{   TUniQuery* q = dmUniDac->qry1;
			while ( ! q->Eof )
			{   clsCustomer* cust = tCustomers->FindByDbID( q->FieldByName(L"CustomerID")->AsInteger );
				if ( cust )
				{	clsLocation* loc = cust->AddLocation( q->FieldByName(L"ID")->AsInteger );
					if ( loc )
					{	loc->AddressLines	= q->FieldByName(L"AddressLines")->AsString.c_str();
						loc->IsPrimary 		= q->FieldByName(L"IsPrimary")->AsBoolean;
						loc->City			= q->FieldByName(L"City")->AsString.c_str();
						loc->State			= q->FieldByName(L"State")->AsString.c_str();
						loc->Country		= q->FieldByName(L"Country")->AsString.c_str();
						loc->PostCode		= q->FieldByName(L"PostCode")->AsString.c_str();
						loc->Phone			= q->FieldByName(L"Phone")->AsString.c_str();
						loc->Email			= q->FieldByName(L"Email")->AsString.c_str();
						loc->MobilePhone	= q->FieldByName(L"MobilePhone")->AsString.c_str();
						loc->ContactName	= q->FieldByName(L"ContactName")->AsString.c_str();
						loc->Notes			= q->FieldByName(L"Notes")->AsString.c_str();
						/*
						if ( q->FieldByName(L"Active")->AsString == "A" )
							loc->SetActive( true );
						else
						{	loc->SetActive( false );
							loc->SetInactiveDate( q->FieldByName(L"InactiveDate")->AsDate );
						}
						if ( q->FieldByName(L"PrevCustomer") != dbNULL )
						{   clsCustomer* pcust = tCustomers->FindByDbID( q->FieldByName(L"PrevCustomer")->AsInteger );
							if ( pcust )
							{	loc->PrevCustomer = pcust;
								loc->SetTransferDate( q->FieldByName(L"TransferDate")->AsDate );
							}
						}
						*/
					}
				}
				q->Next();
			}
			q->Close();
		}
	}
	catch( Exception *ex )
	{
		throw ex;
	}

}
//---------------------------------------------------------------------------
void __fastcall RefreshCustomers::PopulateContainers()
{
	try
	{
		if ( dmUniDac->OpenSQL( String( sqlGetContainers.c_str() ) ) )
		{   TUniQuery* q = dmUniDac->qry1;
			while ( ! q->Eof )
			{   clsLocation* location = tCustomers->FindLocationByDbID( q->FieldByName(L"LocationID")->AsInteger );
				if ( location )
				{	clsContainer* tank = location->AddContainer( q->FieldByName(L"ID")->AsInteger );
					if ( tank )
					{	tank->SerialNumber			= q->FieldByName(L"SerialNumber")->AsString.c_str();
						tank->Size	 				= q->FieldByName(L"Size")->AsInteger;
						tank->LastInspectionDate	= q->FieldByName(L"LastInspectionDate")->AsDateTime;
						tank->InspectionPeriod		= q->FieldByName(L"InspectionPeriod")->AsInteger;
						tank->Leased				= q->FieldByName(L"Leased")->AsBoolean;
						tank->ChargeRate		   	= q->FieldByName(L"ChargeRate")->AsFloat;
						tank->Notes					= q->FieldByName(L"Notes")->AsString.c_str();
						tank->ManufacturedDate		= q->FieldByName(L"ManufactureDate")->AsDateTime;
						tank->ChargePeriod			= q->FieldByName(L"ChargePeriod")->AsInteger;
					}
				}
				q->Next();
			}
			q->Close();
		}
	}
	catch( Exception *ex )
	{

	}
}
//---------------------------------------------------------------------------
void __fastcall RefreshCustomers::PopulateBillingRates()
{
	try
	{
		if ( dmUniDac->OpenSQL( String( sqlGetCustomerBillingRate.c_str() ) ) )
		{ 	TUniQuery* q = dmUniDac->qry1;
			while ( ! q->Eof )
			{	clsCustomer* customer = tCustomers->FindByDbID(q->Fields->Fields[0]->AsInteger);
				if ( customer )
				{	customer->BillingRate 	= q->Fields->Fields[1]->AsFloat;
					TDateTime lastUpdate(0);
					String lastdate = q->Fields->Fields[2]->AsString;
					if ( TryISO8601ToDate( q->Fields->Fields[2]->AsString, lastUpdate, false ) )
						customer->SetRateUpdateDate( lastUpdate );
				}
				q->Next();
			}
			q->Close();
		}
	}
	catch ( Exception *ex )
	{
		throw ex;
	}
}
//---------------------------------------------------------------------------

