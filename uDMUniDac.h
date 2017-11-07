//---------------------------------------------------------------------------

#ifndef uDMUniDacH
#define uDMUniDacH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "SQLiteUniProvider.hpp"
#include "Uni.hpp"
#include "UniProvider.hpp"
#include <Data.DB.hpp>
#include "MemDS.hpp"
#include "TaskDialog.hpp"
#include "DBAccess.hpp"
#include "DBPlanner.hpp"
#include "boost/variant.hpp"

//---------------------------------------------------------------------------
#include "clsCustomer.h"
#include "DASQLMonitor.hpp"
#include "UniSQLMonitor.hpp"
//---------------------------------------------------------------------------
class TdmUniDac : public TDataModule
{
__published:	// IDE-managed Components
	TSQLiteUniProvider *SQLiteUniProvider1;
	TUniConnection *uCon;
	TUniTable *tblLocations;
	TUniDataSource *dsLocations;
	TUniDataSource *dsCustomers;
	TUniTable *tblCustomers;
	TUniDataSource *dsSales;
	TUniTable *tblSales;
	TUniDataSource *dsTanks;
	TUniTable *tblTanks;
	TUniDataSource *dsVehicles;
	TUniTable *tblVehicles;
	TUniDataSource *dsBillingRates;
	TUniTable *tblBillingRates;
	TUniTable *tblXtrnlAccts;
	TDataSource *dsXtrnlAccts;
	TUniQuery *qryXtrnlAccts;
	TUniQuery *qry1;
	TAdvTaskDialog *td;
	TUniTable *tblGroupRates;
	TDataSource *dsGroupRates;
	TUniTable *tblResource;
	TUniTable *tblPlanner;
	TUniDataSource *dsResource;
	TUniDataSource *dsPlanner;
	TDBDaySource *plannerDaySource;
	TUniTable *tblParams;
	TUniQuery *qryluCustomers;
	TUniTable *tblluLocations;
	TDataSource *dsluCustomers;
	TDataSource *dsluLocations;
	TUniTable *tblGroups;
	TDataSource *dsGroups;
	TUniQuery *qryGroupRates;
	TIntegerField *tblCustomersCustomerID;
	TStringField *tblCustomersMYOBID;
	TStringField *tblCustomersAccountNumber;
	TStringField *tblCustomersCoName;
	TStringField *tblCustomersLName;
	TStringField *tblCustomersFName;
	TStringField *tblCustomersABN;
	TBooleanField *tblCustomersActive;
	TBooleanField *tblCustomersIsIndividual;
	TBooleanField *tblCustomersExternal;
	TStringField *tblCustomersNotes;
	TBooleanField *tblCustomersVolPricingAdjust;
	TBooleanField *tblCustomersVolPricingInterest;
	TDateField *tblCustomersFixedPricingDate;
	TIntegerField *tblCustomersGroupID;
	TStringField *tblCustomersDisplayName;
	TDataSource *dsLUXtrnlAccts;
	TUniSQLMonitor *UniSQLMonitor1;
	void __fastcall uConAfterConnect(TObject *Sender);
	void __fastcall tblCustomersCalcFields(TDataSet *DataSet);
	void __fastcall tblCustomersAfterOpen(TDataSet *DataSet);
	void __fastcall tblCustomersAfterClose(TDataSet *DataSet);
	void __fastcall tblCustomersAfterScroll(TDataSet *DataSet);
	void __fastcall plannerDaySourceFieldsToItem(TObject *Sender, TFields *Fields, TPlannerItem *Item);
	void __fastcall plannerDaySourceItemToFields(TObject *Sender, TFields *Fields, TPlannerItem *Item);
	void __fastcall tblPlannerAfterOpen(TDataSet *DataSet);
	void __fastcall tblPlannerAfterClose(TDataSet *DataSet);
	void __fastcall plannerDaySourceSetFilter(TObject *Sender);
	void __fastcall plannerDaySourceItemsRead(TObject *Sender);



private:	// User declarations
	void __fastcall RegisterCASQFunctions();

public:		// User declarations
	__fastcall TdmUniDac(TComponent* Owner);

	bool 	__fastcall 	ConnectToDb( const String& dbname );
	void	__fastcall 	Disconnect();
	int		__fastcall 	ExecuteSQL( const String& sql );
	bool    __fastcall  OpenSQL( const String& sql );

	int		__fastcall	AddCustomer( clsCustomer* customer );
	int 	__fastcall 	InsertCustomerLocations( clsCustomer* customer );
	bool 	__fastcall 	UpdateTable(const String& tablename, const String& fieldname, boost::variant<std::wstring, int, float>& value, const std::wstring& criteria);
	int		__fastcall 	InsertLocation(clsLocation* loc);
	int 	__fastcall 	InsertBillingRate( clsCustomer* customer );
	int 	__fastcall 	InsertSales( std::vector<clsInvoice*>* invoices );
	void 	__fastcall 	ActivatePlanner( bool state = false );
	void    __fastcall  CloseQuery();

	String 	DBFilename;

};
//---------------------------------------------------------------------------
extern PACKAGE TdmUniDac *dmUniDac;
//---------------------------------------------------------------------------
#endif
