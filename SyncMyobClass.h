//---------------------------------------------------------------------------

#ifndef SyncMyobClassH
#define SyncMyobClassH
//---------------------------------------------------------------------------
#include <string>

#include "clsCustomer.h"

//---------------------------------------------------------------------------
class clsSyncMYOB
{
	public:
				__fastcall clsSyncMYOB(TComponent* Owner, clsColCustomers* ezbCustomers);
				__fastcall ~clsSyncMYOB();

		int 	__fastcall	SyncCustomers(const std::wstring& exeFile, const std::wstring& dbFile);
		int		__fastcall 	SyncSales(const std::wstring& exeFile, const std::wstring& dbFile);
		int 	__fastcall 	SyncBillingRates(const std::wstring& exe, const std::wstring& db);
		int		__fastcall	AddCustomer( clsCustomer* customer, const std::wstring& exe, const std::wstring& db );

	// added 2016-07-11 to allow bulk send of application billing rates to MYOB
		int 	__fastcall 	SyncEzBillingRates( const std::wstring& exe, const std::wstring& db );


	private:
		void 	__fastcall 	SyncCustomerAddress( clsCustomer* ezCustomer );
		bool 	__fastcall 	CheckForHistoricalSales( TDateTime startDate );
		void 	__fastcall 	GetCustomerID( clsCustomer* ezCustomer );

		clsColCustomers* ezbCustomers;

};
#endif
