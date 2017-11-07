//---------------------------------------------------------------------------
#include "CodeSiteLogging.hpp"
#pragma hdrstop
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include <string>
#include <sstream>
#include "classExternalDeliveries.h"

#include "dmMYOBUnit.h"				// MYOB data connection module
#include "SQLStatements.h"			// SQL statement file for both MYOB and SQLite
#include "uDMUniDac.h"					// data module for UniDac connection to SQLite

//---------------------------------------------------------------------------
#pragma package(smart_init)

#define OriginCustomerAc 		4
#define OriginCustomerPO		15
#define OriginDeliveryDckt		14
#define OriginDeliveryDate		13
#define OriginQtyDelivered		17

#define KleenheatCustomerAc		15
#define KleenheatCustomerPO		5
#define	KleenheatDeliveryDate   0
#define KleenheatQtyDelivered   1

const std::wstring	GasItemNumber 			= L"04";
const String 		OriginDescription 		= L"Home Delivery of Origin Gas - Delivery Date: ";
const String		KleenheatDescription 	= L"Home Delivery of Kleenheat Gas - Delivery Date: ";

//---------------------------------------------------------------------------
__fastcall clsXtrnlDeliveries::clsXtrnlDeliveries()
{
	grid 			= NULL;
	ezbCustomers 	= NULL;
	_gstrate		= 10;
	Invoices 		= new std::vector<clsInvoice*>();
	MainForm		= NULL;
}
//---------------------------------------------------------------------------
__fastcall clsXtrnlDeliveries::clsXtrnlDeliveries(clsColCustomers* customers, TAdvStringGrid* srcGrid)
{
	grid 			= srcGrid;
	ezbCustomers	= customers;
	_gstrate		= 10;
	Invoices		= new std::vector<clsInvoice*>();
	MainForm		= NULL;
}
//---------------------------------------------------------------------------
__fastcall clsXtrnlDeliveries::~clsXtrnlDeliveries()
{
	if (Invoices)
		delete Invoices;
	MainForm = NULL;
}
//---------------------------------------------------------------------------
int __fastcall clsXtrnlDeliveries::ValidateCustomers()
{
	try
	{   int col = 0;
		int cntr = 0;
		switch ( xtrnlCompany )
		{	case ORIGIN:
				col = OriginCustomerAc;
				break;
			case KLEENHEAT:
				col = KleenheatCustomerAc;
				break;
		}
		if ( grid && ezbCustomers )
		{
			for (int r = 1; r < grid->RowCount; r++)
			{   if ( grid->Cells[col][r].Length() )
				{	clsCustomer* cust = ezbCustomers->FindXtrnlAccount( grid->Cells[col][r].c_str() );
					if ( ! cust )
					{	// highlight customer not found
						grid->Colors[col][r] = clRed;
						grid->FontColors[col][r] = clWhite;
						grid->AddColorComment(col, r,L"Customer not found!", clBlue);
						cntr++;
					}
				}
			}
		}
		if ( cntr )
			return cntr;
		else
			return 0;
	}
	catch (Exception *ex )
	{
		return 0;
	}
}
//---------------------------------------------------------------------------
int	__fastcall clsXtrnlDeliveries::ProcessInvoices(clsCASQ_App* app)
{
	try
	{   int colAc = 0, colQty = 0, colDate=0, colPO=0;
		if ( grid && ezbCustomers )
		{	switch ( xtrnlCompany )
			{   case ORIGIN:
					colAc 	= OriginCustomerAc;
					colQty 	= OriginQtyDelivered;
					colDate = OriginDeliveryDate;
					colPO	= OriginDeliveryDckt;
					break;
				case KLEENHEAT:
					colAc 	= KleenheatCustomerAc;
					colQty	= KleenheatQtyDelivered;
					colDate	= KleenheatDeliveryDate;
					colPO	= KleenheatCustomerPO;
					break;
				default:
					return 0;
			}
			if (MainForm)
			{	MainForm->sb->Panels->Items[2]->Text = "<b>Creating invoices...</b>";
				MainForm->sb->Refresh();
			}

			for (int r = 1; r < grid->RowCount; r++)
			{   if ( grid->Cells[colAc][r].Length() )
				{	clsCustomer* cust = ezbCustomers->FindXtrnlAccount( grid->Cells[colAc][r].c_str() );
					if ( cust )
						CreateInvoice(cust, r, colQty, colDate, colPO);
				}
			}
			int success = SendInvoicesToMYOB(app);
			if ( success == 0 )
				return 0;
			else
			{	SendInvoicesToSQLite();
				return success;
			}
		}
		return 0;
	}
	catch (Exception *ex )
	{
		return 0;
	}
	if ( MainForm )
	{	MainForm->sb->Panels->Items[2]->Text = L"";
		MainForm->sb->Refresh();
	}
}
//---------------------------------------------------------------------------
// PRIVATE FUNCTIONS ########################################################
int __fastcall clsXtrnlDeliveries::CreateInvoice(clsCustomer* cust, int r, int colQty, int colDate, int colPO)
{
	if ( ! cust )
		return 0;
	CodeSite->EnterMethod(L"clsXtrnlDeliveries::CreateInvoice");
	int rtnval = 0;
	try
	{	if (! Invoices)
			Invoices 		= new std::vector<clsInvoice*>();
		clsInvoice* inv = FindCustomerInvoice( cust );
		if ( ! inv )
		{
			inv 	= new clsInvoice(cust);
			Invoices->push_back(inv);
			inv->CustomerRef 	= grid->Cells[colPO][r].c_str();
		}
		clsInvoiceLine* il 	= inv->AddLine();
		if ( cust->Locations.size() == 1 )
		{	il->Location = cust->Locations.at(0);
			if ( cust->Locations.at(0)->Containers->Count() )
				il->Container = cust->Locations.at(0)->Containers->Container(0);
		}
		il->ItemNumber 	= GasItemNumber;
		il->Quantity 	= grid->Floats[colQty][r];
		il->Description = (OriginDescription + grid->Cells[colDate][r]).c_str();
		il->IncTaxPrice = cust->BillingRate;
		rtnval = 1;
	}
	catch( Exception *ex )
	{   CodeSite->SendException( ex );
		rtnval = 0;
	}
	CodeSite->ExitMethod(L"clsXtrnlDeliveries::CreateInvoice");
	return rtnval;
}
//---------------------------------------------------------------------------
clsInvoice* __fastcall clsXtrnlDeliveries::FindCustomerInvoice( clsCustomer* customer )
{
	if ( ! customer ) {
		return NULL;
	}
	BOOST_FOREACH( clsInvoice* inv, *Invoices )
	{	if ( inv->GetCustomer() == customer )
			return inv;
	}
	return NULL;
}
//---------------------------------------------------------------------------
int __fastcall clsXtrnlDeliveries::SendInvoicesToMYOB(clsCASQ_App* app)
{
	int rtnval = 0;
	int SuccessCount = 0;
	try
	{   if ( Invoices && Invoices->size() )
		{	if ( dmMyob->ConnectToDb( app->MYOBApp.c_str(), app->MYOBData.c_str(), true ) )
			{   int nextinvoice = GetLastInvoiceNumber() + 1;
				int taxrate 	= GetGSTRate();
				std::wstring sql(L""), sqlLine(L"");
				dmMyob->conAdo->BeginTrans();
				BOOST_FOREACH( clsInvoice* inv, *Invoices )
				{   if (MainForm) {
						MainForm->sb->Panels->Items[2]->Text = "<b>Writing invoice for</b> " + String(inv->Customer->DisplayName().c_str());
						MainForm->sb->Refresh();
					}
					inv->SetGSTRate( taxrate );
					std::ostringstream sInvNum;
					sInvNum << std::setw(8) << std::setfill( '0' ) << nextinvoice;
					inv->InvNumber = String(sInvNum.str().c_str()).c_str();
					sql = boost::replace_all_copy( sqlInsertInvoice, L"<<<customer.RecordID>>>", String(inv->Customer->MyobCardID).c_str() );
					sql = boost::replace_all_copy( sql, L"<<<invoice.InvoiceNumber>>>", inv->InvNumber.c_str() );
					sql = boost::replace_all_copy( sql, L"<<<invoice.InvoiceDate>>>", inv->Date.FormatString("dd/mm/yyyy").c_str());
					sql = boost::replace_all_copy( sql, L"<<<invoice.CustomerPO>>>", inv->CustomerRef.c_str());
					int lineCounter = 0;
					BOOST_FOREACH( clsInvoiceLine* il, *inv->InvoiceLines )
					{   if (lineCounter)
							sql += sqlInsertInvoiceLine2On;
						sqlLine = boost::replace_all_copy( sqlInsertInvoiceLineAll, L"<<<item.ItemNumber>>>", il->ItemNumber.c_str() );
						sqlLine = boost::replace_all_copy( sqlLine, L"<<<item.Qty>>>", String(il->Quantity).c_str() );
						sqlLine = boost::replace_all_copy( sqlLine, L"<<<item.Description>>>", il->Description.c_str() );
						sqlLine = boost::replace_all_copy( sqlLine, L"<<<item.IncTaxPrice>>>", CurrToStr( il->IncTaxPrice ).c_str() );
						sqlLine = boost::replace_all_copy( sqlLine, L"<<<item.ExTaxPrice>>>", CurrToStr( il->CalcExTaxPrice() ).c_str() );
						sqlLine = boost::replace_all_copy( sqlLine, L"<<<item.IncTaxAmount>>>", CurrToStr( il->IncTaxAmount() ).c_str() );
						sqlLine = boost::replace_all_copy( sqlLine, L"<<<item.ExTaxAmount>>>", CurrToStr( il->ExTaxAmount() ).c_str() );
						sqlLine = boost::replace_all_copy( sqlLine, L"<<<item.GSTAmount>>>", CurrToStr( il->GSTAmount() ).c_str() );
						sql += sqlLine;
						if ( ! lineCounter )
							sql += sqlInsertInvoiceLineEnd1;
						else
							sql += sqlInsertInvoiceLineEndMulti;
						lineCounter++;
					}
					// finish off sql statement
					sql += L");";
					CodeSite->SendMsg( sql.c_str() );
					if ( dmMyob->ExecuteSQL(sql.c_str()))
					{   dmMyob->conAdo->Execute( L"END TRANSACTION" );		//ExecuteSQL(L"END TRANSACTION");
						nextinvoice++;
						inv->ImportedToMyob = true;
						SuccessCount++;
					}
				}
				if ( dmMyob->conAdo->InTransaction )
				{	try
					{   // required to overcome MYOB ODBC errors where defaults are used these errors can be ignored.
						dmMyob->conAdo->CommitTrans();
					}
					catch( Exception *ex )
					{   int errNative = dmMyob->conAdo->Errors->Item[dmMyob->conAdo->Errors->Count-1]->NativeError;
						if ( errNative > 10000)
							throw ex;
					}
				}
				dmMyob->Disconnect();
				rtnval = SuccessCount;
			}
		}
	}
	catch ( Exception *ex )
	{
		CodeSite->SendException( ex );
		dmMyob->conAdo->RollbackTrans();
		dmMyob->Disconnect();
		rtnval = 0;
	}
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsXtrnlDeliveries::SendInvoicesToSQLite()
{
	int rtnval = 0;
	try
	{   if ( Invoices && Invoices->size() )
			dmUniDac->InsertSales( Invoices );
	}
	catch ( Exception *ex )
	{   CodeSite->SendException( ex );
		rtnval = 0;
	}
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsXtrnlDeliveries::GetGSTRate()
{
	int rtnval = 10;
	try
	{	if ( dmUniDac->OpenSQL( String( sqlGetGSTRate.c_str() ) ) )
			rtnval = dmUniDac->qry1->Fields->Fields[0]->AsInteger;
	}
	catch( Exception *ex )
	{
	}
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsXtrnlDeliveries::GetLastInvoiceNumber()
{
	int rtnval = 0;
	if ( dmMyob->OpenSQL( String( sqlLastInvoice.c_str() ) ) )
	{	rtnval = dmMyob->adqry->Fields->Fields[0]->AsInteger;
		dmMyob->adqry->Close();
	}
	return rtnval;

}
//---------------------------------------------------------------------------

