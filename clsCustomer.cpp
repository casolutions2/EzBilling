//---------------------------------------------------------------------------
//#include "CodeSiteLogging.hpp"
#pragma hdrstop
#include <System.DateUtils.hpp>
#include "clsCustomer.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TDateTime __fastcall DateFromUKString(const std::wstring& strDate) ;
//---------------------------------------------------------------------------
bool __fastcall sortCustomer(const clsCustomer* lhs, const clsCustomer* rhs)
{	if ( lhs->IsIndividual )
	{	if ( rhs->IsIndividual )
			return lhs->LName < rhs->LName;
		else
			return lhs->LName < rhs->CoName;
	}
	else
	{	if ( rhs->IsIndividual )
			return lhs->CoName < rhs->LName;
		else
			return lhs->CoName < rhs->CoName;
	}
}
//---------------------------------------------------------------------------

#pragma region clsColCustomers
__fastcall clsColCustomers::clsColCustomers()
{
	customers = new std::vector<clsCustomer*>(0);

}
//---------------------------------------------------------------------------
__fastcall clsColCustomers::~clsColCustomers()
{
	if ( customers )
	{	BOOST_FOREACH(clsCustomer* cust, *customers)
			delete cust;
		delete customers;
	}
}
//---------------------------------------------------------------------------
void __fastcall clsColCustomers::Sort()
{
	std::sort( customers->begin(), customers->end(), sortCustomer );
}
//---------------------------------------------------------------------------
clsCustomer* __fastcall clsColCustomers::Customer(unsigned int idx )
{
	if ( customers )
	{	if ( idx < customers->size() )
			return customers->at(idx);
	}
	throw std::out_of_range("Customer index out of range");
}
//---------------------------------------------------------------------------
clsCustomer* __fastcall clsColCustomers::NewCustomer()
{
	clsCustomer* newguy = new clsCustomer();
	customers->push_back(newguy);
	return newguy;
}
//---------------------------------------------------------------------------
void __fastcall clsColCustomers::RemoveCustomer(clsCustomer* customer)
{
	if ( customers->size() )
	{	for (std::vector<clsCustomer*>::iterator it = customers->begin(); it != customers->end(); it++)
		{   if (*it == customer )
			{	customers->erase(it);
            	return;
			}
		}
	}
}
//---------------------------------------------------------------------------
clsCustomer* __fastcall clsColCustomers::FindByMyobID( int id )
{
	if ( id < 1 ) return NULL;
	BOOST_FOREACH( clsCustomer* cus, *customers)
	{	if ( cus->MyobCardID == id )
			return cus;
	}
	return NULL;
}
//---------------------------------------------------------------------------
clsCustomer* __fastcall	clsColCustomers::FindByDbID( int id )
{
	if ( id < 1 ) return NULL;
	BOOST_FOREACH( clsCustomer* cus, *customers)
	{   if ( cus->dbID == id )
			return cus;
	}
	return NULL;
}
//---------------------------------------------------------------------------
clsLocation* __fastcall	clsColCustomers::FindLocationByDbID( int id )
{
	if ( id < 1 ) return NULL;
	BOOST_FOREACH( clsCustomer* cus, *customers )
	{	clsLocation* loc = cus->FindLocationByDbID( id );
		if ( loc )
			return loc;
	}
	return NULL;
}
//---------------------------------------------------------------------------
clsCustomer* __fastcall	clsColCustomers::FindXtrnlAccount( std::wstring xtrnlAccount )
{
	if ( xtrnlAccount.length() == 0 ) return NULL;
	BOOST_FOREACH( clsCustomer* cus, *customers )
	{   if (cus->ExtAccounts.size() )
		{	if ( std::find( cus->ExtAccounts.begin(), cus->ExtAccounts.end(), xtrnlAccount ) != cus->ExtAccounts.end() )
				return cus;
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
void __fastcall clsColCustomers::ClearExternals()
{
	BOOST_FOREACH( clsCustomer* cust, *customers )
		cust->ExtAccounts.clear();
}
//---------------------------------------------------------------------------
int __fastcall clsColCustomers::Count( unsigned short idx, bool countActiveOnly )
{
	if ( ! customers->size() ) return 0;

	int cntr = 0;
	unsigned short asc = 0;
	BOOST_FOREACH( clsCustomer* cus, *customers )
	{   if ( ! ( countActiveOnly && !cus->Active ) )
		{	asc = AnsiString(cus->DisplayName().c_str())[1];
			switch (idx)
			{	case 0:		// 0-9   ascii 48-57
					if ( asc > 47 && asc < 58 )
						cntr++;
					break;
				case 27:	// All Others
					if (asc < 48 || ( asc > 57 && asc < 65 ) || ( asc > 90 && asc < 97 ) || asc > 122 )
						cntr++;
					break;
				default:	// letters ( A - Z ) char( index + 64 )
					if ( asc == idx + 64 || asc == idx + 96 )
						cntr++;

			}
		}
	}
	return cntr;
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region clsFleet
//---------------------------------------------------------------------------
__fastcall clsFleet::clsFleet()
{

}
//---------------------------------------------------------------------------
__fastcall clsFleet::~clsFleet()
{

}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region clsColContainers
//---------------------------------------------------------------------------
__fastcall clsColContainers::clsColContainers()
{
	containers = new std::vector<clsContainer*>();
}
//---------------------------------------------------------------------------
__fastcall clsColContainers::~clsColContainers()
{
	DeleteAllContainers();
	delete containers;
}
//---------------------------------------------------------------------------
void __fastcall clsColContainers::Add( clsContainer* container )
{
	if ( container )
		containers->push_back(container);
}
//---------------------------------------------------------------------------
void __fastcall clsColContainers::DeleteAllContainers()
{
	BOOST_FOREACH( clsContainer* con, *containers)
		delete con;
}
//---------------------------------------------------------------------------
clsContainer* __fastcall clsColContainers::Container(const std::wstring& serialNum)
{
	BOOST_FOREACH( clsContainer* cont, *containers )
	{	if ( cont->SerialNumber == serialNum )
			return cont;
	}
	return NULL;
}
//---------------------------------------------------------------------------
clsContainer* __fastcall clsColContainers::Container(unsigned int idx)
{
	if ( idx > containers->size()-1 ) throw Exception(L"Out of Range");
	return containers->at(idx);

}
//---------------------------------------------------------------------------
clsContainer* __fastcall clsColContainers::ContainerByID(int dbID)
{
	BOOST_FOREACH( clsContainer* cont, *containers )
	{	if ( cont->DbID == dbID )
			return cont;
	}
	return NULL;
}
//---------------------------------------------------------------------------
clsContainer* __fastcall clsColContainers::NewContainer(clsLocation* location)
{
	clsContainer* newcontainer = new clsContainer(location);
	return newcontainer;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma end_region
#pragma region clsCustomer
__fastcall clsCustomer::clsCustomer()
{
	CoName 				= L"";
	_dateCreated 		= Date();
	AccountNumber		= L"";
	Active 				= true;
	IsIndividual 		= false;
	dbID				= 0;
	External			= false;
	FName				= L"";
	LName				= L"";
	MyobCardID			= 0;
	Notes				= L"";
	BillingRate			= 0;
	VolumePricing		= false;
	PricingInterest		= false;
	_needsMyobUpdate 	= false;

}
//---------------------------------------------------------------------------
__fastcall clsCustomer::~clsCustomer()
{

}
//---------------------------------------------------------------------------
void __fastcall clsCustomer::SetRateUpdateDate(TDateTime updateDate)
{
	_rateUpdateDate = updateDate;
}
//---------------------------------------------------------------------------
void __fastcall	clsCustomer::SetFixedPriceDate(std::wstring UKDateString)
{
	if ( UKDateString.length() )
		_fixedPriceDate = DateFromUKString(UKDateString);
}
//---------------------------------------------------------------------------
void __fastcall	clsCustomer::SetMyobIdentifiers(std::wstring identifiers)
{
	if ( boost::contains(identifiers, L"G" ) )
		VolumePricing = true;
	else
		VolumePricing = false;
	if ( boost::contains( identifiers, L"I" ) )
		PricingInterest = true;
	else
		PricingInterest = false;
}
//---------------------------------------------------------------------------
void __fastcall clsCustomer::CheckIfExternal()
{
	if (Notes.length() == 0)
		External = false;
	ExtAccounts = ParseText(Notes);
}
//---------------------------------------------------------------------------
std::vector<std::wstring> __fastcall clsCustomer::ParseText(const std::wstring input)
{
	std::wstring code(L"AC");
	// create search expression
	boost::wregex srch1(code);
	boost::wregex srch2(L"]");
	// create the match variable
	boost::wsmatch m1;
	boost::wsmatch m2;
	// create an iterator
	std::wstring::const_iterator it = input.begin();
	std::wstring::const_iterator end = input.end();
	// accumulator
	std::vector<std::wstring> accounts(0);

	// search
	while ( boost::regex_search(it, end, m1, srch1) )
	{	it = m1[0].second;
		// now find the closing brace
		if ( boost::regex_search( it, end, m2, srch2 ) )
		{	std::wstring acNum(it, m2[0].second - 1);
			accounts.push_back(acNum);
			it = m2[0].second;
		}
	}
	return accounts;
  //[ACEG:123453] some text here; [ACEG:123454] some text here; [ACEG:123455] some text here;
}
//---------------------------------------------------------------------------
clsLocation* __fastcall clsCustomer::AddLocation(int dbID)
{
	clsLocation* loc = new clsLocation(this, dbID);
	if ( ! loc ) return NULL;
	Locations.push_back(loc);
	return loc;
}
//---------------------------------------------------------------------------
clsLocation* __fastcall clsCustomer::AddLocation()
{
	clsLocation* loc = new clsLocation(this, 0);
	if ( ! loc ) {
		return NULL;
	}
	Locations.push_back(loc);
	return loc;
}
//---------------------------------------------------------------------------
void __fastcall	clsCustomer::AddLocation(clsLocation* loc)
{
	if ( loc )
		Locations.push_back(loc);
}
//---------------------------------------------------------------------------
clsLocation* __fastcall	clsCustomer::FindLocationByDbID( int dbid )
{
	if ( Locations.size() )
	{
		BOOST_FOREACH( clsLocation* loc, Locations )
		{	if ( loc->DbID == dbid )
				return loc;
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
clsLocation* __fastcall clsCustomer::PrimaryAddress()
{
	if ( Locations.size() )
	{	BOOST_FOREACH(clsLocation* loc, Locations)
		{	if ( loc->IsPrimary )
				return loc;
		}
		return Locations.at(0);
	}
	else
		return NULL;
}
//---------------------------------------------------------------------------
std::wstring __fastcall	clsCustomer::DisplayName()
{
	if ( CoName.length() )
		return CoName;
	else
		return LName + L", " + FName;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#pragma end_region
#pragma region clsLocation
//---------------------------------------------------------------------------
__fastcall clsLocation::clsLocation()
	:	_customer(NULL), IsPrimary(true), _dbID(0)
{
	Containers = NULL;
	Country = L"Australia";
}
//---------------------------------------------------------------------------
__fastcall clsLocation::clsLocation(clsCustomer* customer, int dbid)
{
	_customer 	= customer;
	_dbID		= dbid;
	Containers 	= new clsColContainers();
}
//---------------------------------------------------------------------------
__fastcall clsLocation::~clsLocation()
{

}
//---------------------------------------------------------------------------
int	__fastcall clsLocation::SetDbID( int id )
{
	if (id > 0)
		_dbID = id;
	return _dbID;
}
//---------------------------------------------------------------------------
int	__fastcall clsLocation::AddContainer(clsContainer* bottle)
{
	if ( bottle )
	{	if ( ! Containers )
			Containers = new clsColContainers();
		Containers->Add( bottle );
		return Containers->Count();

	}
	return 0;
}
//---------------------------------------------------------------------------
clsContainer* __fastcall clsLocation::AddContainer( int dbid )
{
	if ( ! Containers )
		Containers = new clsColContainers();
	clsContainer* bottle = new clsContainer( this );
	bottle->DbID = dbid;
	Containers->Add( bottle );
	return bottle;
}
//---------------------------------------------------------------------------
std::wstring __fastcall clsLocation::GetNodeDisplay()
{
	if ( AddressLines.length() )
		return AddressLines.substr(0, AddressLines.find(L"\n",0));
	else
		return L"Location";
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#pragma end_region
#pragma region clsContainer
//---------------------------------------------------------------------------
__fastcall clsContainer::clsContainer(clsLocation* loc)
	:	ChargePeriod(6),		ChargeRate(0),
		InspectionPeriod(6),	LastInspectionDate(0),
		Leased(false),			ManufacturedDate(0),
		Notes(L""),				SerialNumber(L""),
		Size(0)
{
	DbID = 0;
	_location = loc;
}
//---------------------------------------------------------------------------
__fastcall clsContainer::~clsContainer()
{

}
//---------------------------------------------------------------------------
int __fastcall clsContainer::LocationID()
{
	if (_location)
		return _location->DbID;
	else
		return 0;
}
//---------------------------------------------------------------------------
TDateTime __fastcall clsContainer::SetScheduledInspection(TDateTime dte)
{
	 if ( dte > ManufacturedDate && dte > LastInspectionDate )
	 {	 _scheduledInspection = dte;
		 if ( DayOf(_scheduledInspection) > DayFriday )
			_scheduledInspection = IncDay(_scheduledInspection, DayFriday - DayOf(_scheduledInspection) );
		 return _scheduledInspection;
	 }
	 else
		throw Exception(L"Invalid Date");
}
//---------------------------------------------------------------------------
TDateTime __fastcall clsContainer::CalcNextInspection()
{
	if ( InspectionPeriod > 0 && LastInspectionDate.Val > 0 )
	{	_calcNextInspection = IncMonth(LastInspectionDate, InspectionPeriod);
		if ( DayOf(_calcNextInspection) > DayFriday )
			_calcNextInspection = IncDay(_calcNextInspection, DayFriday - DayOf(_calcNextInspection) );
	}
	return _calcNextInspection;
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region clsVehicle
//---------------------------------------------------------------------------
__fastcall clsVehicle::clsVehicle()
{

}
//---------------------------------------------------------------------------
__fastcall clsVehicle::~clsVehicle()
{

}
//---------------------------------------------------------------------------
#pragma end_region

#pragma region clsInvoice
//---------------------------------------------------------------------------
__fastcall clsInvoice::clsInvoice()
	:	InvNumber(L""), CustomerRef(L""), ImportedToMyob(false)
{
	Date 			= TDateTime().CurrentDate();
	InvoiceLines 	= new std::vector<clsInvoiceLine*>();
	_customer 		= NULL;
}
//---------------------------------------------------------------------------
__fastcall clsInvoice::clsInvoice(clsCustomer* customer)
	:	InvNumber(L""), CustomerRef(L""), ImportedToMyob(false)
{
	Date 			= TDateTime().CurrentDate();
	InvoiceLines 	= new std::vector<clsInvoiceLine*>();
	_customer = customer;
}
//---------------------------------------------------------------------------
__fastcall clsInvoice::~clsInvoice()
{
	DeleteLines();
	delete InvoiceLines;
	_customer = NULL;
}
//---------------------------------------------------------------------------
clsInvoiceLine* __fastcall clsInvoice::AddLine()
{
	clsInvoiceLine* il = new clsInvoiceLine(this);
	InvoiceLines->push_back(il);
	return il;
}
//---------------------------------------------------------------------------
void __fastcall clsInvoice::DeleteLines()
{
	BOOST_FOREACH( clsInvoiceLine* il, *InvoiceLines )
		delete il;

}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region clsInvoiceLine
//---------------------------------------------------------------------------
__fastcall clsInvoiceLine::clsInvoiceLine()
	: ItemNumber(L""), Quantity(0), Description(L""),
		ExTaxPrice(0), IncTaxPrice(0)
{
	_invoice 	= NULL;
	Location 	= NULL;
	Container 	= NULL;
}
//---------------------------------------------------------------------------
__fastcall clsInvoiceLine::clsInvoiceLine(clsInvoice* inv)
	: ItemNumber(L""), Quantity(0), Description(L""),
		ExTaxPrice(0), IncTaxPrice(0)
{
	_invoice 	= inv;
	Location 	= NULL;
	Container 	= NULL;
}
//---------------------------------------------------------------------------
__fastcall clsInvoiceLine::~clsInvoiceLine()
{

}
//---------------------------------------------------------------------------
Currency __fastcall clsInvoiceLine::CalcExTaxPrice()
{
	if ( _invoice->_gstrate )
	   ExTaxPrice = IncTaxPrice / ( ( 100.0 + _invoice->_gstrate ) / 100 );
	else
	   ExTaxPrice = IncTaxPrice;
	return ExTaxPrice;
}
//---------------------------------------------------------------------------
Currency __fastcall clsInvoiceLine::IncTaxAmount()
{
	return Quantity * IncTaxPrice;
}
//---------------------------------------------------------------------------
Currency __fastcall	clsInvoiceLine::ExTaxAmount()
{
	return Quantity * ExTaxPrice;
}
//---------------------------------------------------------------------------
Currency __fastcall	clsInvoiceLine::GSTAmount()
{
	return IncTaxAmount() - ExTaxAmount();
}
//---------------------------------------------------------------------------
#pragma end_region
//---------------------------------------------------------------------------
TDateTime __fastcall DateFromUKString(const std::wstring& strDate)
{
	if ( ! strDate.length() ) return 0;

	boost::wregex reg(L"(\\d+)[\/](\\d+)[\/](\\d+)");
	boost::wsmatch sm;
	TDateTime dte(0);
	//std::string txtDte = AnsiString(strDate).c_str();
	boost::regex_search( strDate, sm, reg );
	try
	{	dte = TDateTime(_wtoi(sm[3].str().c_str()), _wtoi(sm[2].str().c_str()), _wtoi(sm[1].str().c_str()));

	}
	catch (Exception *ex )
	{
		dte.Val = 0;
	}
	return dte;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
