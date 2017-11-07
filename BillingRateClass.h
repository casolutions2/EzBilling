//---------------------------------------------------------------------------
#ifndef BillingRateClassH
#define BillingRateClassH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <string>
#include "MYOB_Classes.h"
#include "clsCustomer.h"		// CASQ header for all class objects
//---------------------------------------------------------------------------
class TVolumeAdjustment
{
	public:
		struct 	   	TVolume
		{	double		Median;
			double		Minimum;
		};

		struct 		TAdjust
		{	double 		Pct;
			Currency	Minimum;
			Currency	Maximum;
		};

		struct 		TRestrict
		{   Currency	AbsoluteMin;
			Currency	Minimum;
			Currency	Maximum;
		};

		Currency	GasCostPrice;
		Currency	FixedMarkUp;
		Currency	CostMarkUp;
		double		InterestRate;
		int			DaysFree;
		TVolume		Volume;
		TAdjust		Adjust;
		TRestrict	Restrict;
		TRestrict	NewRange;
		Currency	AdjustPrice;
		bool 		VolumeAdjust;			// use to specify whether this is a volume adjustment or fixed adjustment

				__fastcall TVolumeAdjustment();
				__fastcall ~TVolumeAdjustment();

	private:

};

class TCustomerBilling
{
	public:
		Currency 			OldBillingRate;
		Currency			NewBillingRate;
		float				Volume;
		TDateTime			FixedPricingDate;
		std::wstring 		Identifiers;
		int					ReceivableDays;
		Currency			TotalPaidInvoices;
		int					DaysToPay;
		Currency			AdjustmentAmount;
		Currency			TheoreticalPrice;
		double				InterestComponent;
		clsCustomer*		Customer;
		TVolumeAdjustment	VolAdjust;


		__fastcall 			TCustomerBilling();
		__fastcall			~TCustomerBilling();

		bool		__fastcall	IsInFixedPricing();
		Currency	__fastcall	CalculateNewBilling();
		Currency	__fastcall 	CalculateAdjustment();
		Currency 	__fastcall	NewGasPrice() {return NewBillingRate + AdjustmentAmount; };

	private:

		Currency	__fastcall	CalculateTP();
		Currency	__fastcall	CalculateTP2(float TP);
		Currency	__fastcall	CalculateAdjustmentAmount(float TP2);
		bool		__fastcall	HasIdentifier(std::wstring identifier);
		int			__fastcall	CalculateDaysToPay();

};

#endif
