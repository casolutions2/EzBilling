//---------------------------------------------------------------------------

#pragma hdrstop

#include "classClientSales.h"
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <numeric>
#include "CodeSiteLogging.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
class clsSale;
class clsClientSales;

#pragma region Vector functions/functor object
float __fastcall sumqty( float rslt, clsSale sale )
{
	return rslt + sale.Qty;
}
//---------------------------------------------------------------------------
class TSumQty
{
	public:
		TSumQty( String AProductCode ) : _pc( AProductCode ) {};
		float operator() ( float rslt, clsSale ASale )
			{	if ( ASale.Product == _pc )
					return rslt + ASale.Qty;
				else
					return rslt;
			};
	private:
		String _pc;

};
class TSumSales
{
	public:
		TSumSales( String AProductCode ) : _pc( AProductCode ) {};
		float operator() ( float rslt, clsSale ASale )
			{	if ( ASale.Product == _pc )
					return rslt + ASale.SaleDollars ;
				else
					return rslt;
			};
	private:
		String _pc;
};
class TCountProduct
{
	public:
		TCountProduct( String AProductCode ) : _pc ( AProductCode ) {};
		int operator() ( int rslt, clsSale ASale )
		{   if ( ASale.Product == _pc )
				return rslt + 1;
			else
				return rslt;
		};
	private:
		String _pc;
};

#pragma region class Sale
//---------------------------------------------------------------------------
__fastcall clsSale::clsSale()
{
	return;
}
//---------------------------------------------------------------------------
__fastcall clsSale::~clsSale()
{
	return;
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region class ClientSales
//---------------------------------------------------------------------------
__fastcall clsClientSales::clsClientSales()
{
	// constructor
	_ds = NULL;
	_productList 	= NULL;
	_maxDays		= 0;;
	_maxQty			= 0.0;
	_totalQty		= 0.0;
	_totalSales		= 0.0;

}
//---------------------------------------------------------------------------
__fastcall clsClientSales::clsClientSales(TDataSource* ADataSource )
{
	// constructor with datasource
	_ds 			= ADataSource;
	_productList 	= NULL;
	_maxDays		= 0;;
	_maxQty			= 0.0;
	_totalQty		= 0.0;
	_totalSales		= 0.0;

}
//---------------------------------------------------------------------------
__fastcall clsClientSales::~clsClientSales()
{
	// destructor
	if ( _productList )
		delete _productList;
	if ( _ds )
		_ds = NULL;

}

//---------------------------------------------------------------------------
TStrings* __fastcall clsClientSales::ProductList()
{
	RefreshProductList();
	return _productList;
}
//---------------------------------------------------------------------------
void __fastcall clsClientSales::Refresh()
{
	CodeSite->EnterMethod( "clsClientSales::Refresh" );
	// refresh data from ds;
	try
	{	if ( ! _ds )
			goto exitFunction;
		_sales.clear();
		_maxDays = 0;
		_maxQty = 0.0;
		_totalQty = 0.0;
		_totalSales = 0.0;
		TDateTime lastSaleDate(0);
		_ds->DataSet->First();
		while ( ! _ds->DataSet->Eof )
		{   clsSale s;
			s.SaleDate 		= _ds->DataSet->Fields->FieldByName("FillDate")->AsDateTime;
			s.Qty       	= _ds->DataSet->Fields->FieldByName("Quantity")->AsFloat;
			s.SaleDollars   = _ds->DataSet->Fields->FieldByName("ExclTotal")->AsFloat;
			s.InvoiceNo     = _ds->DataSet->Fields->FieldByName("MyobInvNumber")->AsString;
			s.Desc          = _ds->DataSet->Fields->FieldByName("Notes")->AsString;
			s.Product       = _ds->DataSet->Fields->FieldByName("MyobItemID")->AsString;
			_sales.push_back( s );
			_totalQty += s.Qty;
			_totalSales += s.SaleDollars;
			if ( s.Qty > _maxQty )
				_maxQty = s.Qty;
			if ( ( lastSaleDate.Val - s.SaleDate.Val ) > _maxDays )
				_maxDays = lastSaleDate.Val - s.SaleDate.Val;
			lastSaleDate = s.SaleDate;
			_ds->DataSet->Next();
		}
		OutputDebugString( ("no of sales " + String( _sales.size() ) ).c_str() );
		OutputDebugString( ("_maxQty " + String( _maxQty ) ).c_str() );
		OutputDebugString( ("_maxDays " + String( _maxDays ) ).c_str() );
		_ds->DataSet->First();
	}
	catch ( Exception *ex )
	{
		CodeSite->SendException( ex );
	}
	exitFunction:
	CodeSite->ExitMethod( "clsClientSales::Refresh" );
}
//---------------------------------------------------------------------------
void __fastcall clsClientSales::Refresh( TDataSource* ADataSource )
{
	_ds = ADataSource;
	Refresh();
}
//---------------------------------------------------------------------------
void __fastcall clsClientSales::RefreshProductList()
{
	CodeSite->EnterMethod( "clsClientSales::RefreshProductList" );
	try
	{	if ( ! _ds )
			goto exitFunction;
		if ( ! _productList ) {
			_productList = new TStringList(false);
		}
		_productList->Clear();

		BOOST_FOREACH( clsSale s, _sales )
		{
			if ( _productList->IndexOf( s.Product ) == -1 )
				_productList->Add( s.Product );
		}
	}
	catch( Exception *ex )
	{
		// do I need to do anything here??
		CodeSite->SendException( ex );
	}
	exitFunction:
	CodeSite->ExitMethod( "clsClientSales::RefreshProductList" );
	return;
}
//---------------------------------------------------------------------------
float __fastcall clsClientSales::TotalQty( String AProdCode, TDateTime frmDate )
{
	CodeSite->EnterMethod( "clsClientSales::TotalQty" );
	float rtnval(0.0);
	try
	{	if ( frmDate.Val > 0 )
		{
		   //	return std::accumulate( _sales.begin(), _sales.end(), 0.0, []( float rslt, clsSale sale ){ return rslt + sale.Qty; } );
		   rtnval = 1.0;
		}
		else
		{	if ( AProdCode == "" )
				rtnval = std::accumulate( _sales.begin(), _sales.end(), 0.0, sumqty ); //[]( float rslt, clsSale sale ){ return rslt + sale.Qty; } );
			else
			{	// calculate total for particular product code
				TSumQty sumqty( AProdCode );
				rtnval = std::accumulate( _sales.begin(), _sales.end(), 0.0, sumqty );
			}
		}
	}
	catch( Exception *ex )
	{   CodeSite->SendException( ex );
		// do some error thing here??
		rtnval = 0.0;
	}
	CodeSite->ExitMethod( "clsClientSales::TotalQty" );
	return rtnval;
}
//---------------------------------------------------------------------------
float __fastcall clsClientSales::TotalSales( String ProdCode, TDateTime frmDate )
{
	CodeSite->EnterMethod( "clsClientSales::TotalSales" );
	float rtnval(0.0);
	try
	{	if ( frmDate.Val > 0 )
		{

		}
		else
		{	if ( ProdCode == "" )
				rtnval = _totalSales;
			else
			{   TSumSales sumsales( ProdCode );
				rtnval = std::accumulate( _sales.begin(), _sales.end(), 0.0, sumsales );
			}
		}
	}
	catch( Exception *ex )
	{   CodeSite->SendException( ex );
		// do some error thing here??
		rtnval = 0.0;
	}
	CodeSite->ExitMethod( "clsClientSales::TotalSales" );
	return rtnval;
}
//---------------------------------------------------------------------------
float __fastcall clsClientSales::MaxQty( String AProdCode, TDateTime frmDate )
{
	if ( AProdCode == "" )
		return _maxQty;
	// calculate maxqty for product code
	else
        return 0;
}
//---------------------------------------------------------------------------
float __fastcall clsClientSales::AvgQty( String AProdCode, TDateTime frmDate )
{
	CodeSite->EnterMethod( "clsClientSales::AvgQty" );
	float rtnval(0.0);
	try
	{ 	if ( _sales.size() == 0 )
			rtnval = 0.0;
		else
		{	if ( AProdCode == "" )
				rtnval = _totalQty / _sales.size();
			else
			{
				float 	subtotal = TotalQty( AProdCode );
				TCountProduct prodCount( AProdCode );
				int 	subcount = std::accumulate( _sales.begin(), _sales.end(), 0, prodCount );
				if ( subcount > 0 )
					rtnval = subtotal / subcount;
			}
		}

	}
	catch( Exception *ex )
	{   CodeSite->SendException( ex );
		// do some error thing here??
		rtnval = 0.0;
	}
	CodeSite->ExitMethod( "clsClientSales::AvgQty" );
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsClientSales::AvgDays( String AProdCode, TDateTime frmDate )
{
	CodeSite->EnterMethod( "clsClientSales::AvgDays" );
	int rtnval(0);
	try
	{	if ( _sales.size() == 0 )
			rtnval = 0;
		else
		{	int cntr(0);
			int days(0);
			TDateTime lastSale(0);
			BOOST_FOREACH( clsSale s, _sales )
			{	if ( s.SaleDate >= frmDate && ( AProdCode == "" || s.Product == AProdCode ) )
				{   cntr++;
					if ( lastSale.Val > 0)
						days += abs( s.SaleDate.Val - lastSale.Val );
					lastSale = s.SaleDate;
				}
			}
			if ( cntr > 1 )
				rtnval = days / ( cntr - 1 );       // - 1 due to last recorded sale not being calculated
			else
				rtnval = 0;
		}
	}
	catch ( Exception *ex )
	{   CodeSite->SendException( ex );
		// do some error handling here ??
		rtnval = 0;
	}
	CodeSite->ExitMethod( "clsClientSales::AvgDays" );
	return rtnval;
}
//---------------------------------------------------------------------------
int __fastcall clsClientSales::MaxDays( String AProdCode, TDateTime frmDate )
{
	if ( AProdCode == "" )
		return _maxDays;
	else
		return 0;
}
//---------------------------------------------------------------------------
clsSale* __fastcall clsClientSales::Item( int idx )
{
	if ( idx < 0 || idx >= _sales.size() )
		return NULL;

	return &_sales.at( idx );

}
//---------------------------------------------------------------------------
int __fastcall clsClientSales::getCount( )
{
	return _sales.size();
}
#pragma end_region
//---------------------------------------------------------------------------
