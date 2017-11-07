//---------------------------------------------------------------------------

#ifndef classExternalDeliveriesH
#define classExternalDeliveriesH
//---------------------------------------------------------------------------
// class for handling import of external deliveries and processing of invoices
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include "tmsAdvGridExcel.hpp"
#include <Vcl.Grids.hpp>
#include <vector>
#include "clsCustomer.h"
#include "clsCASQ_App.h"

#include "UFormMain.h"

enum TPCustomer { ORIGIN, KLEENHEAT };

class clsXtrnlDeliveries
{
	public:
				__fastcall 	clsXtrnlDeliveries();
				__fastcall	clsXtrnlDeliveries(clsColCustomers* customers, TAdvStringGrid* srcGrid);
				__fastcall	~clsXtrnlDeliveries();

		int		__fastcall 	ValidateCustomers();
		int		__fastcall	ProcessInvoices(clsCASQ_App* app);

		TPCustomer 					xtrnlCompany;
		std::vector<clsInvoice*>* 	Invoices;
		TfrmMain* 					MainForm;


	private:
		clsColCustomers* 	ezbCustomers;
		TAdvStringGrid* 	grid;
		int					_gstrate;

		int 	__fastcall 	CreateInvoice(clsCustomer* cust, int row, int colQty, int colDate, int colPO);
		int 	__fastcall	SendInvoicesToMYOB(clsCASQ_App* app);
		int		__fastcall 	GetGSTRate();
		int		__fastcall 	GetLastInvoiceNumber();
		int 	__fastcall	SendInvoicesToSQLite();
		//2016-04-11  to find invoice already created for a given customer
		clsInvoice* __fastcall FindCustomerInvoice( clsCustomer* customer );
};


//---------------------------------------------------------------------------
#endif
