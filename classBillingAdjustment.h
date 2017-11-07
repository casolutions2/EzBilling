//---------------------------------------------------------------------------

#ifndef classBillingAdjustmentH
#define classBillingAdjustmentH
//---------------------------------------------------------------------------

#include <System.hpp>
#include <System.DateUtils.hpp>
#include <string>
#include <vector>
#include "clsCustomer.h"
#include "BillingRateClass.h"

// this class is for processing billing adjustments
enum PriceAdjustType { paFixed, paVolume };
struct clsBillingRecord
{
	public:
		float	CurRate;
		int		Volume;
		float	NewRate;
};

class clsBillingAdjust
{
	public:
		__fastcall clsBillingAdjust(bool useVolumes);
		__fastcall ~clsBillingAdjust();

		int 	__fastcall 	LoadPrevious();
		bool	__fastcall	Process();
		int 	__fastcall 	ProcessAdjustment(TVolumeAdjustment va);
		void 	__fastcall 	ProcessPriceAdjustment(bool volumeAdjust);
		bool 	__fastcall 	UpdateMyobBilling();
		void    __fastcall 	SetTargetForm( TForm* AForm );

		//PriceAdjustType PA;


	private:
		int					__fastcall	CollectCustomerStats(TVolumeAdjustment va);
		int 				__fastcall 	CollectMYOBStats();
		bool 				__fastcall 	ConfirmAdjustment(Currency AdjustPrice, bool useVolumes);
		bool 				__fastcall 	ConfirmUpdateVolumes();
		void 				__fastcall 	DeleteBillingCustomers();
		TCustomerBilling*	__fastcall 	FindBillingCustomer( clsCustomer* ezcustomer );
		int					__fastcall	LoadVolumesFromSQL();
		void				__fastcall	OutputResults();
		void 				__fastcall 	SaveBillingAdjustment(const TVolumeAdjustment& va);
		int 				__fastcall 	UpdateVolumesInMYOB();
		int 				__fastcall 	UpdateSQLVolumes();

		TForm*							_targetForm;
		//clsColCustomers* 				ezCustomers;
		bool							VolumeAdjust;
		std::vector<TCustomerBilling*>*	vCustAdjust;
};

#endif
