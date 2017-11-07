//---------------------------------------------------------------------------

#pragma hdrstop
#include <boost/algorithm/string.hpp>
#include <System.Math.hpp>

//---------------------------------------------------------------------------
#include "BillingRateClass.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TVolumeAdjustment::TVolumeAdjustment()
{
	GasCostPrice 			= 0;
	FixedMarkUp 			= 0;
	CostMarkUp				= 0;
	InterestRate			= 0;
	DaysFree				= 0;
	Volume.Median			= 0;
	Volume.Minimum			= 0;
	Adjust.Pct	   			= 0;
	Adjust.Minimum			= 0;
	Adjust.Maximum			= 0;
	Restrict.AbsoluteMin	= 0;
	Restrict.Minimum		= 0;
	Restrict.Maximum		= 0;
	NewRange.AbsoluteMin 	= 0;
	NewRange.Minimum		= 0;
	NewRange.Maximum		= 0;
	AdjustPrice				= 0;
}
//---------------------------------------------------------------------------
__fastcall TVolumeAdjustment::~TVolumeAdjustment()
{

}
//---------------------------------------------------------------------------
__fastcall TCustomerBilling::TCustomerBilling()
{
	OldBillingRate 	 		= 0;
	NewBillingRate			= 0;
	Volume					= 0;
	FixedPricingDate.Val	= 0;
	Identifiers				= L"";
	ReceivableDays			= 0;
	TotalPaidInvoices		= 0;
	DaysToPay				= 0;
	AdjustmentAmount		= 0;
	TheoreticalPrice		= 0;
	InterestComponent		= 0;
	Customer				= NULL;
}
//---------------------------------------------------------------------------
__fastcall TCustomerBilling::~TCustomerBilling()
{

}
//---------------------------------------------------------------------------
bool __fastcall	TCustomerBilling::IsInFixedPricing()
{
	if ( FixedPricingDate.Val == 0) return false;
	if ( Date() > FixedPricingDate )
		return false;
	else
		return true;
}
//---------------------------------------------------------------------------
int	 __fastcall	TCustomerBilling::CalculateDaysToPay()
{
	if ( TotalPaidInvoices == 0 )
		DaysToPay = 0;
	else
		DaysToPay = RoundTo(double(ReceivableDays / TotalPaidInvoices),-1);
	return DaysToPay;
}
//---------------------------------------------------------------------------
bool __fastcall	TCustomerBilling::HasIdentifier(std::wstring identifier)
{
	if ( identifier.length() == 0 ) return false;
	if ( boost::contains(Identifiers, identifier) )
		return true;
	else
		return false;
}
//---------------------------------------------------------------------------
Currency __fastcall TCustomerBilling::CalculateNewBilling()
{
	float rtnval = 0;
	int dtp = CalculateDaysToPay();
	if ( OldBillingRate < VolAdjust.Restrict.AbsoluteMin )
		NewBillingRate = 0;
	else
	{	if ( OldBillingRate > VolAdjust.Restrict.Minimum )
		{	if ( OldBillingRate < VolAdjust.Restrict.Maximum )
			{	NewBillingRate = OldBillingRate + VolAdjust.AdjustPrice;
				if ( NewBillingRate > VolAdjust.NewRange.Minimum )
				{	if ( NewBillingRate < VolAdjust.NewRange.Maximum )
						;	// alls good
					else
						NewBillingRate = VolAdjust.NewRange.Maximum;
				}
				else
					NewBillingRate = VolAdjust.NewRange.Minimum;
			}
		}
	}
	return NewBillingRate;
}
//---------------------------------------------------------------------------
Currency __fastcall TCustomerBilling::CalculateAdjustment()
{
	if ( ! Customer->VolumePricing )			// if ( HasIdentifier(L"G") )
		AdjustmentAmount = 0;
	else
	{	Currency TP = CalculateTP();
		Currency TP2(0.0);
		if ( Customer->PricingInterest )			//if ( HasIdentifier(L"I") )
		{	// interest charge
			TP2 = CalculateTP2( double(TP) );
			TheoreticalPrice = TP2;
		}
		else
		{   // no interest charge
			TP2 = TP;
			TheoreticalPrice = TP;
		}
		AdjustmentAmount = CalculateAdjustmentAmount( double(TP2) );
	}
	return AdjustmentAmount;
}
//---------------------------------------------------------------------------
Currency __fastcall TCustomerBilling::CalculateTP()
{
	if ( VolAdjust.GasCostPrice == 0) return 0.0;
	Currency Vadj(0.0);
	if ( Volume < VolAdjust.Volume.Minimum )
		Vadj = 1.0;
	else
		Vadj = ( VolAdjust.Volume.Median + VolAdjust.Volume.Minimum ) / ( VolAdjust.Volume.Median + Volume );
	return ( VolAdjust.GasCostPrice + VolAdjust.FixedMarkUp + ( Vadj * VolAdjust.CostMarkUp * VolAdjust.GasCostPrice ) );
}
//---------------------------------------------------------------------------
Currency __fastcall TCustomerBilling::CalculateTP2( float TP )
{
	double intcharged(0.0);
	if ( DaysToPay < VolAdjust.DaysFree )
		intcharged = 1.0;
	else
		intcharged = 1.0 + (( DaysToPay - VolAdjust.DaysFree ) / 366 * VolAdjust.InterestRate );
	InterestComponent = TP * ( intcharged - 1 );
	return ( TP * intcharged );
}
//---------------------------------------------------------------------------
Currency __fastcall TCustomerBilling::CalculateAdjustmentAmount(float TP2)
{
	// check fixed pricing date
	if ( FixedPricingDate.Val >= 0.0 )
	{	if ( FixedPricingDate >= Date() )
		{	AdjustmentAmount = 0.0;
			return 0.0;
		}
	}
	else
	{	AdjustmentAmount = 0.0;
		return 0.0;
	}

	Currency aa(0.0);
	bool aaIsNegative = false;

	aa = TP2 - double(NewBillingRate);
	if ( aa < 0.0 )
		aaIsNegative = true;

	if ( abs(double(aa)) < double(VolAdjust.Adjust.Minimum) || aa == 0.0 )
		return 0.0;
	else
	{	aa = aa * double(VolAdjust.Adjust.Pct);
		if ( aa == 0 ) return 0.0;
		if ( abs(double(aa)) < double(VolAdjust.Adjust.Minimum) )
		{   if ( aaIsNegative)
				aa = VolAdjust.Adjust.Minimum * -1;
			else
				aa = VolAdjust.Adjust.Minimum;
		}
		else
		{	if ( abs(double(aa)) > double(VolAdjust.Adjust.Maximum) )
			{	if ( aaIsNegative )
					aa = VolAdjust.Adjust.Maximum * -1;
				else
					aa = VolAdjust.Adjust.Maximum;
			}
		}
	}
	return aa;
}
//---------------------------------------------------------------------------

