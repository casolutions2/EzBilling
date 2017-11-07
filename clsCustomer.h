//---------------------------------------------------------------------------

#ifndef clsCustomerH
#define clsCustomerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <string>
#include <vector>

//---------------------------------------------------------------------------
// forward declarations
class clsCustomer;
class clsLocation;
class clsContainer;
class clsVehicle;
class clsInvoice;
class clsInvoiceLine;
class clsGroup;

//---------------------------------------------------------------------------
// collection class for customers
class clsColCustomers
{
	public:
		__fastcall clsColCustomers();
		__fastcall ~clsColCustomers();

		clsCustomer*	__fastcall 	NewCustomer();
		void			__fastcall	RemoveCustomer(clsCustomer* customer);
		int				__fastcall	Count()	{	return customers->size();	}
		clsCustomer*	__fastcall	FindByMyobID( int id );
		clsCustomer*	__fastcall	FindByDbID( int id );
		clsLocation* 	__fastcall	FindLocationByDbID( int id );
		clsCustomer*	__fastcall	Customer( unsigned int idx );
		clsCustomer*	__fastcall	FindXtrnlAccount( std::wstring xtrnlAccount );
		void			__fastcall	ClearExternals();
		void			__fastcall 	Sort();
		int				__fastcall	Count( unsigned short idx, bool countActiveOnly = true );

	private:
		std::vector<clsCustomer*>* customers;
};
//---------------------------------------------------------------------------
class clsColContainers
{
	public:
		__fastcall clsColContainers();
		__fastcall ~clsColContainers();

		void 			__fastcall 	Add( clsContainer* container );
		clsContainer*	__fastcall	Container(const std::wstring& serialNum);
		clsContainer*	__fastcall	Container( unsigned int idx );
		clsContainer*	__fastcall 	ContainerByID(int dbID);
		int 			__fastcall 	Count() {	return containers->size();	}
		clsContainer*	__fastcall	NewContainer(clsLocation* location);

	private:
		std::vector<clsContainer*>* containers;

		void	__fastcall	DeleteAllContainers();
};
//---------------------------------------------------------------------------
class clsFleet
{
	public:
		__fastcall clsFleet();
		__fastcall ~clsFleet();

	private:
		std::vector<clsVehicle*>* fleet;
};
//---------------------------------------------------------------------------
class clsCustomer
{
	public:
		__fastcall clsCustomer();
		__fastcall ~clsCustomer();

		// variables
		std::wstring	ABN;
		std::wstring	AccountNumber;
		bool			Active;
		bool 			IsIndividual;
		std::wstring 	CoName;
		int 			dbID;
		bool			External;
		std::wstring	FName;
		std::wstring	LName;
		int 			MyobCardID;
		std::wstring	Notes;
		Currency		BillingRate;
		bool			VolumePricing;
		bool			PricingInterest;


		std::vector<clsContainer*> 	Containers;
		std::vector<clsLocation*> 	Locations;
		std::vector<std::wstring> 	ExtAccounts;

		// properties
		TDateTime	__property	DateCreated 	= {read=_dateCreated};
		TDateTime	__property	FixedPriceDate	= {read=_fixedPriceDate, write=_fixedPriceDate};
		TDateTime	__property 	RateLastUpdated	= {read=_rateUpdateDate};
		bool		__property 	NeedsMyobUpdate	= {read=_needsMyobUpdate, write=_needsMyobUpdate};

		// functions
		clsLocation*	__fastcall	AddLocation(int dbid);
		clsLocation*	__fastcall	AddLocation();
		void 			__fastcall	AddLocation(clsLocation* loc);
		void			__fastcall	CheckIfExternal();
		std::wstring	__fastcall	DisplayName();
		clsLocation*	__fastcall	FindLocationByDbID( int dbid );
		clsLocation* 	__fastcall 	PrimaryAddress();
		void			__fastcall	SetFixedPriceDate(std::wstring UKDateString);
		void			__fastcall	SetMyobIdentifiers(std::wstring identifiers);
		void			__fastcall	SetRateUpdateDate( TDateTime UpdateDate);

	private:
		// functions
		std::vector<std::wstring>	__fastcall 	ParseText(const std::wstring txt);
		// variables
		TDateTime		_dateCreated;
		TDateTime		_fixedPriceDate;
		TDateTime		_rateUpdateDate;
		bool			_needsMyobUpdate;
};
//---------------------------------------------------------------------------
class clsLocation
{
	public:
		// constructors
		__fastcall clsLocation();
		__fastcall clsLocation(clsCustomer* customer, int dbid);
		__fastcall ~clsLocation();

		// functions
		int	   			__fastcall	AddContainer(clsContainer* bottle);
		clsContainer*	__fastcall	AddContainer( int dbid );
		clsCustomer*	__fastcall 	Customer()	{	return _customer;	};
		int				__fastcall	SetDbID( int id );

		// properties
		int				__property 	DbID			= { read=_dbID };
		std::wstring	__property 	NodeDisplay		= { read=GetNodeDisplay };
		bool            __property  Active      	= { read=_active };

		// variables
		clsColContainers* 	Containers;
		bool				IsPrimary;
		std::wstring		AddressLines;
		std::wstring		City;
		std::wstring		State;
		std::wstring		Country;
		std::wstring		PostCode;
		std::wstring		Email;
		std::wstring		Phone;
		std::wstring		MobilePhone;
		std::wstring		ContactName;
		std::wstring		Notes;


	private:
		//__fastcall clsLocation(const clsLocation& rhs);

		// variables
		clsCustomer*	_customer;
		int				_dbID;

		bool            _active;

		std::wstring	__fastcall GetNodeDisplay();

};
//---------------------------------------------------------------------------
class clsContainer
{
	public:
		// constuctors
		__fastcall clsContainer(clsLocation* loc);
		__fastcall ~clsContainer();

		// variables
		int 			ChargePeriod;
		float			ChargeRate;
		int				InspectionPeriod;
		TDateTime		LastInspectionDate;
		bool			Leased;
		TDateTime		ManufacturedDate;
		std::wstring 	Notes;
		std::wstring 	SerialNumber;
		float 			Size;
        int 			DbID;

		// property
		TDateTime		__property	ScheduledInspectionDate	= {read=_scheduledInspection};
		
		// functions
		TDateTime		__fastcall 	SetScheduledInspection(TDateTime dte);
		TDateTime		__fastcall	CalcNextInspection();
		int				__fastcall	LocationID();
		clsLocation*	__fastcall 	Location()	{ return _location; }

	private:
		clsLocation* 	_location;
		TDateTime		_calcNextInspection;
		TDateTime		_scheduledInspection;

};
//---------------------------------------------------------------------------
class clsVehicle
{
	public:
		// constructor
		__fastcall clsVehicle();
		__fastcall ~clsVehicle();

		// variables
		float			Capacity;
		int				dbID;
		std::wstring	Facilities;
		std::wstring	Insurer;
		TDateTime		InsuranceExpiry;
		std::wstring	PolicyNumber;
		std::wstring	RegistrationNumber;
		TDateTime		RegistrationExpiry;
		std::wstring	RegistrationAuthority;
		std::wstring	VehicleType;

	private:

};
//---------------------------------------------------------------------------
class clsGroup
{
	public:
		__fastcall clsGroup();
		__fastcall ~clsGroup();

		std::wstring 	Name;
		float 			DiscountRate;
		std::wstring	Description;

	private:
		__fastcall clsGroup(const clsGroup& rhs);
};
//---------------------------------------------------------------------------
class clsInvoice
{   friend clsInvoiceLine;
	public:
		__fastcall clsInvoice(clsCustomer* owner);
		__fastcall clsInvoice();
		__fastcall ~clsInvoice();

		// property
		clsCustomer __property *Customer	=	{read=_customer, write=_customer };

		std::wstring					InvNumber;
		TDateTime						Date;
		std::wstring					CustomerRef;
		bool							ImportedToMyob;
		std::vector<clsInvoiceLine*>* 	InvoiceLines;

		// functions
		clsInvoiceLine*	__fastcall 	AddLine();
		void			__fastcall	SetGSTRate(int taxrate)	{	_gstrate = taxrate; 	};
        // 2016-04-11
		clsCustomer* 	__fastcall  GetCustomer()	{ return _customer; };

	private:
		clsCustomer* 	_customer;
		int				_gstrate;

		void	__fastcall	DeleteLines();

};
//---------------------------------------------------------------------------
class clsInvoiceLine
{   friend clsInvoice;
	public:
		__fastcall clsInvoiceLine();
		__fastcall clsInvoiceLine(clsInvoice* inv);
		__fastcall ~clsInvoiceLine();

		clsLocation*	Location;
		clsContainer*	Container;
		std::wstring 	ItemNumber;
		double			Quantity;
		std::wstring	Description;
		Currency		ExTaxPrice;
		Currency 		IncTaxPrice;

		Currency	__fastcall	CalcExTaxPrice();
		Currency	__fastcall 	IncTaxAmount();
		Currency 	__fastcall	ExTaxAmount();
		Currency	__fastcall	GSTAmount();

	private:
		clsInvoice* 	_invoice;
};
#endif
