//---------------------------------------------------------------------------

#ifndef classClientSalesH
#define classClientSalesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.DB.hpp>

#include <vector>
//---------------------------------------------------------------------------
class clsSale
{
	public:
		__fastcall clsSale();
		__fastcall ~clsSale();

		TDateTime 	SaleDate;
		float       Qty;
		float       SaleDollars;
		String      InvoiceNo;
		String      Desc;
		String      Product;

};
class clsClientSales
{
	public:
		__fastcall clsClientSales();
		__fastcall clsClientSales(TDataSource* ADataSource );
		__fastcall ~clsClientSales();

		int			__property  Count = { read = getCount };

		TStrings* 	__fastcall 	ProductList();
		void        __fastcall  Refresh();
		void        __fastcall  Refresh( TDataSource* ADataSource );
		float       __fastcall  TotalQty( String ProdCode = "", TDateTime frmDate = 0 );
		float       __fastcall  MaxQty( String ProdCode = "", TDateTime frmDate = 0 );
		float       __fastcall  AvgQty( String ProdCode = "", TDateTime frmDate = 0 );
		int		    __fastcall  AvgDays( String ProdCode = "", TDateTime frmDate = 0 );
		int         __fastcall  MaxDays( String ProdCode = "", TDateTime frmDate = 0 );
		float       __fastcall  TotalSales( String ProdCode = "", TDateTime frmDate = 0 );
		clsSale*    __fastcall  Item( int idx );

	private:
		TDataSource				*_ds;
		std::vector<clsSale> 	_sales;
		TStrings				*_productList;
		int                     _maxDays;
		float                   _maxQty;
		float                   _totalQty;
		float                   _totalSales;

		void        __fastcall  RefreshProductList();
        int         __fastcall  getCount();


};





//---------------------------------------------------------------------------
#endif
