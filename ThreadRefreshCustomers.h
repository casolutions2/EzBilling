//---------------------------------------------------------------------------

#ifndef ThreadRefreshCustomersH
#define ThreadRefreshCustomersH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <string>
#include "clsCustomer.h"
//---------------------------------------------------------------------------
class RefreshCustomers : public TThread
{
protected:
	void __fastcall Execute();
public:
	__fastcall RefreshCustomers(bool CreateSuspended, clsColCustomers* customers, const std::wstring sql);
private:
	void 	__fastcall PopulateLocations();
	void 	__fastcall PopulateContainers();
	void 	__fastcall PopulateBillingRates();

};
//---------------------------------------------------------------------------
#endif
