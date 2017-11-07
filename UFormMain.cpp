//---------------------------------------------------------------------------

#include <vcl.h>
#include "CodeSiteLogging.hpp"
#pragma hdrstop

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include <sstream>
#include <System.DateUtils.hpp>
//---------------------------------------------------------------------------
#include "UFormMain.h"
#include "SQLStatements.h"				// SQL statement file for both MYOB and SQLite
#include "frmSettingsUnit.h"			// file location settings
#include "CASQ_FormOps.h"				// store form positions
#include "FormBillingAdjustment.h"		// billing adjustment parameters
#include "dmMYOBUnit.h"					// myob data connection module
#include "ThreadRefreshCustomers.h"		// thread object to refresh customer details from SQLite
#include "SyncMyobClass.h"				// class for synchronising MYOB data
#include "dlgSyncingUnit.h"				// thread for dialog for synchronisation details
#include "ABOUT.h"						// about box
#include "frmDateSelectorUnit.h"		// date selection dialog
#include "classExternalDeliveries.h" 	// class for handling external deliveries and invoicing
#include "classBillingAdjustment.h"		// class for handling billing adjustments
#include "classPlannerEvent.h"			// class for writing events to the planner
#include "dmReporting.h"				// module holding FastReport components
#include "UfrmGroupEntry.h"				// form for entering customer groups
#include "UfrmGroupRates.h"				// form for entering customer group rates
#include "UFormPriceHistory.h"			// price adjustment history form
#include "uDMUniDac.h"					// data module for UniDac connection to SQLite
//#include "frmExternalAccts.h"			// form to show External Accounts location codes
#include "clsCustomerTree.h"			// class for handling AdvTreeView functions for Customer
#include "uFormResources.h"				// form for Planner Resource management
#include "uFormXtrnlDeliveries.h"		// form for showing External deliveries after import
#include "uFormScheduleEntry.h"			// form for entering delivered quantities
#include "dmWebUpdate.h"                // webupdate components
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvOfficeStatusBar"
#pragma link "AdvOfficePager"
#pragma link "TaskDialog"
#pragma link "WUpdate"
#pragma link "WUpdateWiz"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "W7Bars"
#pragma link "W7Classes"
#pragma link "AdvSmoothCalendar"
#pragma link "PlannerCal"
#pragma link "Planner"
#pragma link "DBPlanner"
#pragma link "AdvAppStyler"
#pragma link "AdvPanel"
#pragma link "AdvOfficePagerStylers"
#pragma link "PlannerWaitList"
#pragma link "AdvOfficeButtons"
#pragma link "DBPlannerMonthView"
#pragma link "PlannerMonthView"
#pragma link "PlanItemEdit"
#pragma link "AdvUtil"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "Uni"
#pragma link "DBAdvGrid"
#pragma link "DBAdvNavigator"
#pragma link "AdvCustomTreeView"
#pragma link "AdvTreeView"
#pragma link "AdvTreeViewBase"
#pragma link "AdvTreeViewData"
#pragma link "MoneyEdit"
#pragma link "AdvSpin"

#pragma link "AdvSmoothGauge"
#pragma link "AdvSmoothLabel"
#pragma link "tmsAdvGridExcel"

#ifdef WIN32
	//#pragma link "wininet.lib" 							// required for webupdate components
	//#pragma link "w32inet.lib"                          // required for webupdate components
#else
	//#pragma link "wininet.a"
	//#pragma link "w32inet.a"
#endif

#pragma link "AdvOfficeHint"
#pragma resource "*.dfm"
TfrmMain *frmMain;
const String 		msgReady	= L"<FONT color=\"clBlue\">Ready</FONT>";
const int defTimeSpan		= 15;
// day planner constants
const int 	RowsPerHour 	= 4;
const int 	HourStart		= 4;
const int	HourEnd			= 20;
const int 	ActiveStart		= 6 ;
const int	ActiveEnd		= 18;
// ****

bool 				startUp 	= false;
clsBillingAdjust*	ba 			= NULL;
clsCustomerTree*    ctv         = NULL;
//---------------------------------------------------------------------------
//Caption colors
const TColor 	clDelivery		= TColor(0x00ff00);
const TColor 	clDeliveryTo	= TColor(0x00ff66);
const TColor 	clrGeneric 		= TColor(0xeeeeee);
const TColor 	clrGenericTo 	= TColor(0xeecccc);
//---------------------------------------------------------------------------
// Footer text
const String 	footerDels( "Number of deliveries: " );
const String 	footerQty( "Estimated Qty: " );
bool        	CustomersPopulated( false );
//---------------------------------------------------------------------------
#pragma region Startup and Shutdown
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	App 			= NULL;
	Customers		= NULL;
	bookingEditor 	= new TEzBItemEditor(this, dmUniDac->uCon );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	App = new clsCASQ_App(this);
	try
	{
		if ( App->Style.length() )
			TStyleManager::TrySetStyle( App->Style.c_str() );
		App->SetupLogging();
		CASQ_FormOps::RestorePosition(this);
		this->Caption = Application->Title;
		// moved to refresh datta *** SetupPager();
		SetupMenu();
		// set tabheight on main pager
		pgrMain->TabSettings->Height = 3;
		RefreshData();
	}
	catch( Exception *ex )
	{   CodeSite->EnterMethod("FormCreate");
		CodeSite->SendError( ex->Message );
		CodeSite->ExitMethod("FormCreate");
	}
	startUp = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormActivate(TObject *Sender)
{
	#if defined _DEBUG
		if ( startUp && App->FilesDbExist )
	#else
		if ( startUp /*&& dmSecurity->secManager->LoggedIn */ &&  App->FilesDbExist )
	#endif
	{   try
		{	if ( CodeSite->Enabled )
			{	if ( CodeSiteManager->DefaultDestination )
				{	TCodeSiteDestination *csdLog = CodeSiteManager->DefaultDestination;
					sb->Panels->Items[3]->Text = "<b>Logging active</b>";
					// add a hint
					sb->Panels->Items[3]->OfficeHint->Title = "Log file:";
					sb->Panels->Items[3]->OfficeHint->Notes->Add( csdLog->LogFile->FilePath + "\\" + csdLog->LogFile->FileName) ;
					// icon
                    //sb->Panels->Items[3]->OfficeHint->Picture = "";
				}
			}
		}
		catch ( Exception *ex )
		{
			// error outputting codesite destination
			sb->Panels->Items[3]->Text = "<font color=\"#FF0000\"><b>Failed to specify Log file</b></font-color>";
		}
		//PopulateCustomers();

		this->Refresh();
		if ( ! App->Initialised )
		{   // ask if want to sync with Myob

			InitialiseWithMyob();
		}
		startUp = false;
	}
	else
	{   sb->Panels->Items[3]->Text = "<b>Logging not active</b>";
		sb->Panels->Items[3]->OfficeHint->Title = "Debugging:";
        sb->Panels->Items[3]->OfficeHint->Notes->Add( "??" );

    }

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormResize(TObject *Sender)
{
	// resize components
	AdvPanel7->ClientHeight = pgDBStatistics->ClientHeight / 2;
	AdvPanel8->ClientWidth = AdvPanel7->ClientWidth / 2;
	AdvPanel13->ClientWidth = AdvPanel11->ClientWidth / 2;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	CASQ_FormOps::SavePosition(this);
	delete App;
	if ( Customers )
		delete Customers;
	if ( ba ) {
		delete ba;
		ba = NULL;
	}
	if ( ctv ) {
		delete ctv;
		ctv = NULL;
	}
	if ( clientSales )
	{   delete clientSales;
		clientSales = NULL;
	}
	if ( bookingEditor ) {
		delete bookingEditor;
		bookingEditor = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::SetupPager()
{
	for (int i = 0; i < pgrMain->AdvPageCount; i++)
		pgrMain->AdvPages[i]->TabVisible = false;
	pgrMain->ActivePage = pgPlanner;

	SetupDaySchedule();
	pgrMain->Visible = true;
	this->plannerCal->Date = Date();
	plannerCalDateChange(NULL, Date(), Date());
	daySchedule->GridTopRow = daySchedule->AbsTimeToCell(EncodeTime(8,0,0,0));
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::SetupDaySchedule()
{
	daySchedule->Display->DisplayUnit = 60/RowsPerHour;
	SwitchDayPlannerDisplay( App->ShowInactive );
	daySchedule->Display->ColorActive = clWindow;
	daySchedule->Display->ColorNonActive = clInactiveCaptionText;
	plannerMonthView->Balloon->Enable = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::SwitchDayPlannerDisplay(const bool showInactive)
{
	if ( showInactive )
	{	daySchedule->Display->DisplayStart = HourStart * RowsPerHour;
		daySchedule->Display->DisplayEnd = ( HourEnd * RowsPerHour ) - 1;
		daySchedule->Display->ActiveStart = ( ActiveStart-HourStart ) * RowsPerHour;
		daySchedule->Display->ActiveEnd = ( ActiveEnd-HourStart ) * RowsPerHour;
	}
	else
	{   daySchedule->Display->DisplayStart = ActiveStart * RowsPerHour;
		daySchedule->Display->DisplayEnd = ( ActiveEnd * RowsPerHour ) - 1;
		daySchedule->Display->ActiveStart = 0;
		daySchedule->Display->ActiveEnd = ( ActiveEnd-ActiveStart ) * RowsPerHour;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::SetupMenu()
{
	// styles
	DynamicArray<String> styleNames = Vcl::Themes::TStyleManager::StyleNames;
	for (int i = 0; i < styleNames.Length; i++)
	{	TMenuItem* subMnu = new TMenuItem(this);
		subMnu->Caption = styleNames[i];
		subMnu->Tag 	= i;
		subMnu->OnClick	= mnuStylesItemClick;
		mnuStyles->Add(subMnu);
	}
	Showoutsidehours1->Checked = App->ShowInactive;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::RefreshData()
{
	DestroyObjects();
	if ( App->DBName.length() )
	{	if ( dmUniDac->ConnectToDb ( App->DBName.c_str() ) )
		{	App->GetCompanyDetails();
			if ( App->StartAppWithSecurity() )
			{   sb->Panels->Items[0]->Text = L"<FONT color=\"#0000FF\">Connected</FONT>";
				sb->Panels->Items[0]->OfficeHint->Title = "Database:";
				sb->Panels->Items[0]->OfficeHint->Notes->Add( App->DBName.c_str() );
				// add an icon
				//sb->Panels->Items[0]->OfficeHint->Picture = "";
				// specify username
				//sb->Panels->Items[3]->Text = dmSecurity->secManager->CurrentUser;
				UnlockApp();
			}
			else
				sb->Panels->Items[3]->Text = L"<B><FONT color=\"#FF0000\">LOCKED</FONT></B>";
			SetupPager();
		}
		else
		{	sb->Panels->Items[0]->Text = L"<B><FONT color=\"#FF00FF\">NOT CONNECTED</B></FONT>";
			sb->Panels->Items[0]->OfficeHint->Title = "Database:";
			sb->Panels->Items[0]->OfficeHint->Notes->Add("Unable to connect to the supplied database");
		}
	}
	else
	{	sb->Panels->Items[0]->Text = L"<B><FONT color=\"#FF00FF\">NOT CONNECTED</B></FONT>";
        	sb->Panels->Items[0]->OfficeHint->Title = "Database:";
			sb->Panels->Items[0]->OfficeHint->Notes->Add("No database file supplied");
			sb->Panels->Items[0]->OfficeHint->Notes->Add("please check your settings");
	}

	return;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::UnlockApp()
{
	mnuCustomers->Enabled = true;
	mnuPricing->Enabled = true;
	mnuDeliveries->Enabled = true;
	mnuPlanner->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::DestroyObjects()
{
	if ( Customers )
	{	delete Customers;
		Customers = NULL;
	}
	if ( ba )
		delete ba;
}
#pragma end_region
#pragma region Menu Items
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuFileExitClick(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuCustomersMYOBSyncBillingRatesClick(TObject *Sender)
{
	// sync billing rate from myob
	clsSyncMYOB* myob = new clsSyncMYOB( this, Customers );
	int rtnval = 0;
	switch ( static_cast<TMenuItem*>(Sender)->Tag )
	{	case 0:		// write MYOB rates to app
			myob->SyncBillingRates(App->MYOBApp, App->MYOBData);
			break;
		case 1:		// write app rates to MYOB
			rtnval = myob->SyncEzBillingRates( App->MYOBApp, App->MYOBData);
			if (! rtnval ) {
				ShowMessage( "No billing rates updated!" );
			}
			break;
		default:
			;
	}

	delete myob;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuFileLocationsClick(TObject *Sender)
{
	if ( App->ShowSettings() == 1 )
		RefreshData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuStylesItemClick(TObject* Sender)
{
	DynamicArray<String> styleNames = Vcl::Themes::TStyleManager::StyleNames;
	App->Style = styleNames[static_cast<TMenuItem*>(Sender)->Tag].c_str();
	TStyleManager::TrySetStyle( App->Style.c_str() );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuFileUpdateClick(TObject *Sender)
{
	// check for updates on web
	modWebUpdate->WebUpdateWizard1->Execute();

}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region BillingAdjustment Grid
void __fastcall TfrmMain::baGridGetFloatFormat(TObject *Sender, int ACol, int ARow,
		  bool &IsFloat, UnicodeString &FloatFormat)
{
	try
	{   if ( ARow > 0 )
		{	switch ( ACol )
			{   case 0:
					FloatFormat = L"%.0d";
					break;
				case 2:
					FloatFormat = L"%.0n";
					break;
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
					if ( IsFloat )
						FloatFormat = L"%0.4f";
					break;
			}
		}
	}
	catch (...)
	{
		;	// ignore
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::baGridGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState,
		  TBrush *ABrush, TFont *AFont)
{
	try
	{	if ( ARow > 0 && ACol > 1 && ACol != 4 )
		{   if ( baGrid->Floats[ACol][ARow] >= 0 )
				AFont->Color = clBlack;
			else
				AFont->Color = clRed;
		}
	}
	catch( Exception *ex )
	{
		;// ignore
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::baGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
		  Advobj::TVAlignment &VAlign)
{
	try
	{	if ( ARow > 0 && ACol == 4 )
			HAlign = taRightJustify;
	}
	catch(Exception *ex)
	{	;	// ignore
	}
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region BillingRateAdjustment
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuPricingFixedAdjustmentClick(TObject *Sender)
{
	if ( ba )
	{	delete ba;
		ba = NULL;
	}
	mnuPricingFixedAdjustment->Enabled 	= false;
	mnuPricingVolumeAdjustment->Enabled = false;
	ba = new clsBillingAdjust(false);
	// dialog confirming fixed price adjustment
	TformBillingAdjust* frm = new TformBillingAdjust(this);
	ba->SetTargetForm( frm );
	try
	{   frm->ba = ba;
		if ( frm->ShowModal() == mrOk )
		{   if ( frm->ReturnStatus == 1 )
			{   // TODO: need to fill customers class to process billing adjustment
				PopulateCustomers();
				while ( ! CustomersPopulated )
				{   CodeSite->SendMsg( "Waiting for PopulateCustomers" );
					Sleep(250);
				}
				if ( ba->Process() )
				{   String msg = "Fixed price adjustment processed. " ;
					clsPlannerEvent event(daySchedule, L"Billing rate", msg );
					pgrMain->ActivePage = pgBillingAdjust;
					pgrMain->Visible 	= true;
				}
				if ( Customers )
				{   delete Customers;
					Customers = NULL;
					CustomersPopulated = false;
				}
			}
		}
	}
	catch( Exception* ex )
	{
		ShowError( "PricingFixedAdjustment\n" + ex->Message );
	}
	if ( ba ) {
		delete ba;
		ba = NULL;
	}
	if ( Customers )
	{   delete Customers;
		Customers = NULL;
		CustomersPopulated = false;
	}

	delete frm;
	frm = NULL;
	Screen->Cursor = crDefault;
	mnuPricingFixedAdjustment->Enabled 	= true;
	mnuPricingVolumeAdjustment->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuPricingVolumeAdjustmentClick(TObject *Sender)
{
	if ( ba )
		delete ba;
	mnuPricingFixedAdjustment->Enabled 	= false;
	mnuPricingVolumeAdjustment->Enabled = false;
	ba = new clsBillingAdjust(true);
	TformBillingAdjust* frm = new TformBillingAdjust(this);
	ba->SetTargetForm( frm );
	try
	{   frm->ba = ba;
		if ( frm->ShowModal() == mrOk )
		{   if ( frm->ReturnStatus == 1 )
			{   // TODO: need to fill customers class to process billing adjustment
				PopulateCustomers();
				while ( ! CustomersPopulated )
				{   CodeSite->SendMsg( "Waiting for PopulateCustomers" );
					Sleep(250);
				}
				if ( ba->Process() )
				{	String msg = "Volume price adjustment processed. " ;
					clsPlannerEvent event(daySchedule, L"Billing rate", msg );
					pgrMain->ActivePage = pgBillingAdjust;
					pgrMain->Visible 	= true;
				}
				if ( Customers )
				{   delete Customers;
					Customers = NULL;
					CustomersPopulated = false;
				}
			}
		}
	}
	catch( Exception* ex )
	{
		ShowError( ex->Message );
	}
	if ( ba ) {
		delete ba;
		ba = NULL;
	}
	if ( Customers )
	{   delete Customers;
		Customers = NULL;
		CustomersPopulated = false;
	}
	delete frm;
	frm = NULL;
	mnuPricingFixedAdjustment->Enabled 	= true;
	mnuPricingVolumeAdjustment->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::pgBillingAdjustExit(TObject *Sender)
{
	if ( ba )
		delete ba;
	mnuPricingFixedAdjustment->Enabled 	= true;
	mnuPricingVolumeAdjustment->Enabled = true;
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region Sundry functions
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ShowError(String errMsg)
{
	td1->Clear();
	td1->Icon = tiError;
	td1->Title = L"An error occured...";
	td1->Instruction = L"An error has occured in the application.  Please see the extended message below.";
	td1->ExpandedText = errMsg;
	td1->CommonButtons << cbOK;
	td1->Execute();
}
//---------------------------------------------------------------------------
#pragma end_region
//---------------------------------------------------------------------------
int __fastcall TfrmMain::PopulateCustomers()
{
	if ( ! App->DBName.length() ) return 0;
	sb->Panels->Items[0]->Text = L"<B>Populating Customers...</B>";
	sb->Refresh();
	if ( Customers )
		delete Customers;

	Customers = new clsColCustomers();
	RefreshCustomers* thread = new RefreshCustomers(true, Customers, sqlGetCustomers );
	try
	{	thread->OnTerminate = PopulateCustomersFinished;
		thread->Start();
		thread->WaitFor();
		thread->Terminate();
	}
	catch( Exception &ex )
	{
		ShowMessage( ex.Message );
	}
	delete thread;
	return 1;
}
//---------------------------------------------------------------------------
void __fastcall	TfrmMain::PopulateCustomersFinished(TObject* Sender)
{
	sb->Panels->Items[0]->Text = msgReady;
	CustomersPopulated = true;
	//sb->Panels->Items[3]->Text = L"Customers: " + String(Customers->Count());
}
//---------------------------------------------------------------------------
int __fastcall TfrmMain::SyncMyobCustomers()
{
	// initial one way
	int rtnval = 0;
	try
	{	clsSyncMYOB* sync = new clsSyncMYOB( this, Customers);
		rtnval = sync->SyncCustomers( App->MYOBApp, App->MYOBData );
		sb->Panels->Items[3]->Text = L"New Customers from MYOB: " + String( rtnval );
		sb->Refresh();
	}
	catch ( Exception *ex )
	{
		CodeSite->SendException( ex );
		rtnval = 0;
	}
	return rtnval;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::InitialiseWithMyob()
{
	CodeSite->EnterMethod(L"TForm1::InitialiseWithMyob");

	try
	{	clsSyncMYOB* sync = new clsSyncMYOB( this, Customers );
		int newCustomers = sync->SyncCustomers( App->MYOBApp, App->MYOBData );
		sb->Panels->Items[3]->Text = L"Customers: " + String(Customers->Count());
		sb->Refresh();
		if ( sync->SyncSales( App->MYOBApp, App->MYOBData ) > 0 )
			// set initialised to 1;
			App->SetInitialised( true );
	}
	catch( Exception *ex)
	{
		CodeSite->SendException( ex );
	}
	CodeSite->ExitMethod(L"TForm1::InitialiseWithMyob");
	return;
}
//---------------------------------------------------------------------------

#pragma region Actions
void __fastcall TfrmMain::actCustomerBulkDeactivateExecute(TObject *Sender)
{
	td1->Instruction 	= L"Deactivate dormant customers.";
	td1->CommonButtons << cbYes << cbNo;
	td1->Title 			= L"Confirm action...";
	td1->Icon 			= tiQuestion;
	td1->Content = L"This action will deactivate any customers that have not had any sales transactions during the previous specified period.  Do you want to continue?";
	if ( td1->Execute() == mrNo )
		return;
	TfrmDateSelector* dlg = new TfrmDateSelector(this);
	dlg->dtpSlctdDate->Date = IncYear(StartOfTheMonth(Date()),-1);
	if ( dlg->ShowModal() == mrCancel )
		return;
	// hide open pages
	pgrMain->Visible = false;
	// check selected date not within 12 months of today
	TDateTime slctdDate = dlg->dtpSlctdDate->Date;
	if ( slctdDate > IncYear(Date(), -1 ) )
	{	td1->CommonButtons.Clear();
		td1->CommonButtons << cbOK;
		td1->Icon = tiWarning;
		td1->Content = L"Sorry, you cannot deactivate customers within 12 months of today.";
		td1->Execute();
		return;
	}
	// execute sql to inactivate customers
	std::wstring sql = boost::replace_all_copy( sqlBulkDeactivateCustomers, L"<<<startdate>>>", slctdDate.FormatString("yyyy-mm-dd").c_str());
	int rslt = dmUniDac->ExecuteSQL( String( sql.c_str() ) );
	td1->Icon = tiInformation;
	td1->CommonButtons.Clear();
	td1->CommonButtons << cbOK;
	td1->Content = String(rslt) + L" customers were marked as inactive.";
	td1->Execute();
	// refresh customers
	PopulateCustomers();
	return;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actHelpAboutExecute(TObject *Sender)
{
	TAboutBox* frm = new TAboutBox(this);
	frm->ShowModal();
	delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actCustomerMaintExecute(TObject *Sender)
{
	pgrMain->ActivePage = pgCustomer;
	actCustomerMaint->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actSyncMYOBExecute(TObject *Sender)
{
	SyncMyobCustomers();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actNewCustomerExecute(TObject *Sender)
{
	// create a new customer
/*	TfrmNewCustomer* frm = new TfrmNewCustomer(this);
	try
	{	if ( frm->ShowModal() == mrOk )
		{   clsCustomer* newCustomer = Customers->NewCustomer();
			newCustomer->NeedsMyobUpdate = true;
			frm->FCustomerInput1->FillCustomer( newCustomer );
			if ( dmMain->AddCustomer( newCustomer ) )
			{	clsSyncMYOB *myob = new clsSyncMYOB( this, Customers );
				if ( myob->AddCustomer( newCustomer, App->MYOBApp, App->MYOBData ) )
				{	newCustomer->NeedsMyobUpdate = false;
					// update local record for myobcardid
					boost::variant<std::wstring, int, float> val(newCustomer->MyobCardID);
					dmMain->UpdateTable(L"Customers",L"MYOBID", val, (L"CustomerID=" + String(newCustomer->dbID)).c_str());
				}
				else
					// failed to update myob
					ShowMessage ( L"Failed to update MYOB" );
				delete myob;
				// sort customers
				Customers->Sort();
				// if treeview showing add to tree
				if ( CustomerTV )
					CustomerTV->Refresh();
			}
		}
	}
	catch ( Exception *ex )
	{
		ShowError( ex->Message );
	}
	delete frm;
*/
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actRunSheetExecute(TObject *Sender)
{
	// print run sheet
	//ShowMessage(L"TODO: Print run sheet");
    String rptFilename = "RunSheet.fr3";
	TfrmDateSelector* frm = new TfrmDateSelector(this);
	frm->dtpSlctdDate->Date = Now() + 1;
	rptModule->qryRunSheet->Active = false;
	if ( frm->ShowModal() == mrOk )
	{   String sql = ReplaceText( sqlRunSheet.c_str(), L"<<<pdate>>>", frm->dtpSlctdDate->Date.FormatString(L"yyyy-mm-dd") );
		rptModule->qryRunSheet->SQL->Text = sql;
		rptModule->qryRunSheet->Active = true;
		//rptModule->frxReport1->PrepareReport(true);
		if ( rptModule->qryRunSheet->RecNo > 0 )
		{   rptModule->frxRunSheet->LoadFromFile(App->Path + rptFilename );
			rptModule->frxRunSheet->ShowReport(true);
		}
		rptModule->qryRunSheet->Active = false;
	}

	delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actGroupRatesExecute(TObject *Sender)
{
	TfrmGroupRates* frm = new TfrmGroupRates(this);
	dmUniDac->tblParams->Active = true;
	dmUniDac->tblGroups->Active = true;
	dmUniDac->qryGroupRates->Active = true;
	frm->ShowModal();
	dmUniDac->tblGroupRates->Active = false;
	dmUniDac->tblGroups->Active = false;
	dmUniDac->tblParams->Active = false;
	delete frm;

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actGroupMaintainExecute(TObject *Sender)
{
	TfrmGroup* frm = new TfrmGroup(this);
	dmUniDac->tblGroups->Active = true;
	frm->ShowModal();
	dmUniDac->tblGroups->Active = false;
	delete frm;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actInvoiceScheduleExecute(TObject *Sender)
{
	// open scheduleentry form
	TfrmScheduleEntry* frm = new TfrmScheduleEntry( this );
	frm->ShowModal();
	delete frm;
	frm = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actManualInvoiceExecute(TObject *Sender)
{
	ShowMessage(L"Coming soon...");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actPricingHistoryExecute(TObject *Sender)
{
	// show pricing history form
	//frmPriceAdjustHistory->ShowModal();
	// show pricing history report
    String rptFilename = "PriceAdjustHistory.fr3";
	rptModule->qryPriceAdjustHistory->Active = true;
	//rptModule->frxReport1->PrepareReport(true);
	if ( rptModule->qryPriceAdjustHistory->RecNo > 0 )
	{   rptModule->frxPricingHistory->LoadFromFile(App->Path + rptFilename );
		rptModule->frxPricingHistory->ShowReport(true);
	}
	rptModule->qryPriceAdjustHistory->Active = false;
}
//---------------------------------------------------------------------------
#pragma end_region
//---------------------------------------------------------------------------
#pragma region Import Deliveries
void __fastcall TfrmMain::mnuDeliveriesImportKleenheatClick(TObject *Sender)
{
	TPCustomer suppliertag( ORIGIN );
	pgrMain->ActivePage = pgImportDeliveries;
	// open file dialog to get file
	switch ( static_cast<TComponent*>(Sender)->Tag )
	{	case 0:		// origin
			fo->Title = L"Select the Origin source file...";
			suppliertag = ORIGIN;
			fo->FileTypeIndex = 0;
			break;
		case 1:
			fo->Title = L"Select Kleenheat source file...";
			fo->FileTypeIndex = 2;
			suppliertag = KLEENHEAT;
			break;
		default:
			;
	}
	if ( fo->Execute() )
	{   PopulateCustomers();
		LoadExternalAccounts(L"OR");
		LoadBillingRates();
	 /* TODO -oCraig -cNewForm : Create form for processing external deliveries */
		TfrmXtrnlDeliveries* frm = new TfrmXtrnlDeliveries(this);
		try
		{   frm->Supplier = suppliertag;
			if( frm->LoadFile( fo->FileName ) )
			{	frm->ShowModal();
				if ( frm->ModalResult == mrOk )
				{	String msg = L"The import of deliveries succeded writing " + String( frm->InvoicesWritten ) + L" invoices.";
					clsPlannerEvent event(daySchedule, L"Delivery Import", msg);
				}
			}
		}
		catch( Exception *ex )
		{   CodeSite->SendException( ex );
			ShowError( ex->Message );
		}
		delete frm; frm = NULL;
	}
	else
		pgrMain->ActivePage= this->pgPlanner;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LoadBillingRates()
{
	// Missing instantiation for function

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::LoadExternalAccounts(std::wstring prefix)
{
	Customers->ClearExternals();
	std::wstring sql( L"" );
	sql = boost::replace_all_copy(sqlGetEAs, L"<<<extparty>>>", prefix );
	if ( dmUniDac->OpenSQL( String( sql.c_str() ) ) )
	{   while ( ! dmUniDac->qry1->Eof )
		{   clsCustomer* customer = Customers->FindByDbID(dmUniDac->qry1->Fields->Fields[0]->AsInteger);
			if ( customer )
				customer->ExtAccounts.push_back(dmUniDac->qry1->Fields->Fields[2]->AsString.c_str());
			dmUniDac->qry1->Next();
		}
		dmUniDac->qry1->Close();
	}
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region Planner
void __fastcall TfrmMain::pgCustomerExit(TObject *Sender)
{
	actCustomerMaint->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actShowDayScheduleExecute(TObject *Sender)
{
	pgrMain->ActivePage = this->pgPlanner;
	dmUniDac->plannerDaySource->Active = true;
	pgrMain->Visible = true;
	int sb = daySchedule->AbsTimeToCell(Now());
	daySchedule->SelectCells(sb, sb+1, 1);
	//daySchedule->GridTopRow( sb );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actPlannerShowMonthExecute(TObject *Sender)
{
	pgrMain->ActivePage = pgMonthPlanner;
	pgrMain->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::plannerCalDateChange(TObject *Sender, TDateTime origDate,
		  TDateTime newDate)
{
	plannerMonthView->Date = newDate;
	dmUniDac->plannerDaySource->Day = newDate;
	//plannerMonthView->Month = MonthOf( newDate );
	String msg( L"<B>" );
	msg += newDate.FormatString("dd MMMM, YYYY") /*DateString()*/ + L" Schedule</B></P>";
	daySchedule->Caption->Title = L"<P align=\"center\">EzBilling - " + msg;
	daySchedule->Caption->Alignment = taCenter;
	UpdateScheduleFooter();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::daySchedulePlannerNext(TObject *Sender)
{
	// change calendar date
	TDateTime origDate = plannerCal->Date;
	plannerCal->Date = plannerCal->Date + 1;
	plannerCalDateChange(Sender, origDate, plannerCal->Date);
	daySchedule->Refresh();
	//UpdateScheduleFooter();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::daySchedulePlannerPrev(TObject *Sender)
{
	// change calendar date
	TDateTime origDate = plannerCal->Date;
	plannerCal->Date = plannerCal->Date - 1;
	plannerCalDateChange(Sender, origDate, plannerCal->Date);
	daySchedule->Refresh();
	//UpdateScheduleFooter();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actGenericItemExecute(TObject *Sender)
{
	// add generic item to planner
	TPlannerItem* pi = daySchedule->CreateItem();
	if ( pgPlanner->Visible )
	{	TDateTime timeStart, timeEnd;
		daySchedule->SelectionToAbsTime( timeStart, timeEnd );
		pi->ItemStartTime = timeStart;
		pi->ItemEndTime = timeEnd;
	}
	else
	{	pi->ItemStartTime = EncodeTime(8,30,0,0);
		pi->ItemEndTime = EncodeTime(8,45,0,0);
	}
	pi->Editor = this->plannerItemEditorDefault;
	pi->ItemPos = 0;
	pi->Tag = 0;
	pi->CaptionType = ctTimeText;
	pi->CaptionBkg = clrGeneric;
	pi->CaptionBkgTo = clrGenericTo;
	pi->Edit();
	if ( pi->Editor->ModalResult == mrOk )
		pi->Update();
	else
		daySchedule->FreeItem(pi);

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::AddAutoItem( String ACaption, String ANote, TDateTime ADateStart, TDateTime ADateEnd )
{
	TPlannerItem* pi = daySchedule->CreateItem();
	if ( ADateStart.Val == 0 )
		ADateStart = Now();
	if ( ADateEnd.Val == 0 )
		ADateEnd = Now();

	pi->ItemStartTime = ADateStart;
	pi->ItemEndTime = ADateEnd;
	pi->ItemPos = 0;
	pi->Tag = 0;
	pi->CaptionType = ctTimeText;
	//pi->CaptionBkg = clrAutoAdd;
	//pi->CaptionBkgTo = clrAutoAddTo;
	// add some text and notes
	pi->CaptionText = ACaption;
	pi->Text->Add( ANote );
	pi->Update();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actDeliveryScheduleExecute(TObject *Sender)
{
	// schedule a delivery
	TPlannerItem* pi        = nullptr;
	clsEzBooking*   gb      = nullptr;
	try
	{   CodeSite->SendMsg( "create delivery item " );
		if ( ! bookingEditor )
			bookingEditor 		= new TEzBItemEditor( this, dmUniDac->uCon );
		pi 						= daySchedule->CreateItem();
		gb 						= new clsEzBooking();
		pi->Editor 				= bookingEditor;
		pi->ItemStartTime 		= GetNearestTimeBlock( Now() + 1 );                  // round to the nearest 15 min block
		TDateTime finish        = IncMinute( pi->ItemStartTime, defTimeSpan );
		pi->ItemEndTime	 		= finish; 				 //defTimeSpan );
		pi->ItemPos 			= 1;
		pi->OwnsItemObject 		= true;
		pi->ItemObject 			= gb;
		CodeSite->SendMsg("Edit booking item ");
		pi->Edit();
		if ( bookingEditor->ModalResult == mrOk)
		{	pi->CaptionText 	= L"Delivery: " + gb->CustomerName;
			pi->CaptionBkg 		= clDelivery;
			pi->CaptionBkgTo	= clDeliveryTo;
			pi->Text->Text 		= gb->Address;
			pi->Text->Add( L"Estimated Qty: " + IntToStr(gb->EstQty) );
			pi->Text->Add( gb->Notes );
			pi->ItemPos 		= static_cast<TDBDaySource*>(daySchedule->ItemSource)->ResourceMap->ResToPosition( gb->ResourceID );
			pi->Update();
		}
		else
			daySchedule->FreeItem(pi);
	}
	catch( Exception *ex )
	{
		CodeSite->EnterMethod("actDeliveryScheduleExecute");
		CodeSite->SendError(ex->Message);
		CodeSite->ExitMethod("actDeliveryScheduleExecute");
	}
	UpdateScheduleFooter();
}
//---------------------------------------------------------------------------
TDateTime __fastcall TfrmMain::GetNearestTimeBlock( TDateTime ATimestamp )
{
	TDate adate = int( ATimestamp );
	int ahour = ( ATimestamp.Val - adate.Val ) * 24 ;
	int aminute = ( ( (ATimestamp.Val - adate.Val) * 24 ) - ahour ) * 60 ;
	int next15 = ceil(aminute/15);
	TDateTime rtnval(adate);
	switch ( next15 ) {
		case 0:
			rtnval = adate + EncodeTime( ahour, 0, 0, 0 );
			break;
		case 1:
			rtnval = adate + EncodeTime( ahour, 15, 0, 0 );
			break;
		case 2:
			rtnval = adate + EncodeTime( ahour, 30, 0, 0 );
			break;
		case 3:
			rtnval = adate + EncodeTime( ahour, 45, 0, 0 );
			break;
		default:
			rtnval = adate + EncodeTime( 10, 0, 0, 0 );
	}
	return rtnval;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::pgPlannerEnter(TObject *Sender)       // this is the day schedule
{
	// set filter for date
	if ( dmUniDac->uCon->Connected )
	{   dmUniDac->plannerDaySourceSetFilter( Sender );
		dmUniDac->ActivatePlanner( true );
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::pgPlannerExit(TObject *Sender)
{
	dmUniDac->ActivatePlanner(false);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::pgMonthPlannerEnter(TObject *Sender)
{
	dmUniDac->ActivatePlanner( true );
	dmUniDac->tblPlanner->Filtered = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::pgMonthPlannerExit(TObject *Sender)
{
	dmUniDac->ActivatePlanner( false );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::plannerMonthViewDateChange(TObject *Sender, TDateTime origDate,
		  TDateTime newDate)
{
	plannerCal->Date = newDate;
	plannerCalDateChange( Sender, origDate, newDate );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::PrintCalendar1Click(TObject *Sender)
{
	plannerMonthView->Print();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::plannerMonthViewDblClick(TObject *Sender, TDateTime SelDate)
{
	//create a new delivery item
	actDeliveryScheduleExecute(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::plannerMonthViewItemDblClick(TObject *Sender, TPlannerItem *Item)

{
	// edit the item
	if ( ! Item->Editor )
	{
		switch ( Item->ItemPos )
		{   case 0:
				Item->Editor = plannerItemEditorDefault;
				break;
			default:
				if ( ! bookingEditor )
					bookingEditor = new TEzBItemEditor( this, dmUniDac->uCon );
				Item->Editor = bookingEditor;
		}
	}
	Item->Edit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::plannerMonthViewItemPopupPrepare(TObject *Sender, TPopupMenu *PopupMenu,
		  TPlannerItem *Item)
{
	// could use this for shortcut menu to decide which item is selected
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::dayScheduleItemDelete(TObject *Sender, TPlannerItem *Item)

{
	if ( MessageDlg("Are you sure you want to delete the booking\n" + Item->CaptionText, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0 ) == mrYes )
		daySchedule->FreeItem( Item );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::dayScheduleItemRightClick(TObject *Sender, TPlannerItem *Item)
{
	ShowMessage("ItemRightClick");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::dayScheduleItemDblClick(TObject *Sender, TPlannerItem *Item)

{
	switch ( Item->ItemPos )
	{   case 0:			// office item
			Item->Editor = this->plannerItemEditorDefault;
			break;

		default:
			if ( ! bookingEditor )
				bookingEditor = new TEzBItemEditor( this, dmUniDac->uCon );
			Item->Editor = bookingEditor;
	}
	Item->Edit();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::UpdateScheduleFooter()
{
	CodeSite->EnterMethod( "UpdateScheduleFooter" );
	// reset footers to "no deliveries"
	for (int i = 1; i < daySchedule->Positions; i++)
		daySchedule->Footer->Captions->Strings[i+1] = "No deliveries";

	String msg(L"");
	float qty(0.0f);
	int	del(0);
	//CodeSite->SendMsg( "daySchedule->Items->Count: " + String( daySchedule->Items->Count) );
	try
	{	for (int i = 1; i < daySchedule->Positions; i++ )
		{   del = 0; qty = 0.0f;
			for ( int it = 0; it < daySchedule->Items->Count; it++ )
			{   TPlannerItem* itm = daySchedule->Items->Items[it];
				if ( itm->ItemPos == i )
				{   clsEzBooking* gb = static_cast<clsEzBooking*>(itm->ItemObject);
					del++;
					qty += gb->EstQty;
					//CodeSite->SendMsg("values: " + String(del) + ", " + FormatFloat("#,###", qty ) );
				}
			}
			if ( del )
			{	msg = footerDels + String( del );
				TVarRec args[1];
				args[0] = (long double)qty;
				msg += Format( "\n\r" + footerQty + " %.0n", args, 1 );
				daySchedule->Footer->Captions->Strings[i+1] = msg;
			}
		}
	}
	catch( Exception *ex )
	{   CodeSite->SendException( ex );
	}
	CodeSite->ExitMethod( "UpdateScheduleFooter" );
}
//---------------------------------------------------------------------------
#pragma end_region

#pragma region Customer Group Operations

void __fastcall TfrmMain::Showoutsidehours1Click(TObject *Sender)
{
	Showoutsidehours1->Checked = ! Showoutsidehours1->Checked;
	App->ShowInactive = Showoutsidehours1->Checked;
	SwitchDayPlannerDisplay( App->ShowInactive );
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tmrsbTimer(TObject *Sender)
{
	tmrsb->Enabled = false;
	sb->Panels->Items[3]->Text = L"";
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuPopupInactiveClick(TObject *Sender)
{
	// Inactivate customer
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::daySchedulePlannerMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if ( Button == mbLeft )	return;
	// select cell at xy
	//TPoint pt = daySchedule->XYToCell( X, Y );
	TPoint aPoint(X, Y);
	TPoint dsPoint = daySchedule->ClientToScreen( aPoint );
	popupPlannerItem->Popup( dsPoint.X, dsPoint.Y + daySchedule->Caption->Height + daySchedule->Header->Height );
	//daySchedule->SelectCells(pt.)
}
//---------------------------------------------------------------------------


#pragma end_region

#pragma region Customer TreeView
void __fastcall TfrmMain::pgTMSTreeShow(TObject *Sender)
{
	RefreshATVC();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::btnCustomerTreeContractAllClick(TObject *Sender)
{
	atvCustomers->CollapseAll();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::pgCustomerEnter(TObject *Sender)
{
	SetupCustomerTree();
	dmUniDac->tblCustomers->Active = true;
}
//---------------------------------------------------------------------------
// setup initial status of tree
void __fastcall TfrmMain::SetupCustomerTree()
{
	if ( ! ctv ) {
		ctv = new clsCustomerTree( atvCustomers, dmUniDac->uCon, cbActiveCustomers->Checked );
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::atvcBeforeExpandNode(TObject *Sender, TAdvTreeViewVirtualNode *ANode, bool &ACanExpand)
{
	// Check how many customers in this group and add child nodes as appropriate
	ctv->PopulateBranch( ANode->Node );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::RefreshATVC()
{
	if ( ! ctv )
		return;
	else
		ctv->RefreshData();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::cbActiveCustomersClick(TObject *Sender)
{
	// rebuild customer tree

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::atvCustomersAfterSelectNode(TObject *Sender, TAdvTreeViewVirtualNode *ANode)
{
	//CodeSite->SendMsg( "AfterSelectNode" );
	if ( ANode->Level == 0 )
		pgrCustomerInfo->Visible = false;
	else
	{   pgrCustomerInfo->Visible = true;
		TLocateOptions opts;
		dmUniDac->tblCustomers->Locate( "CustomerID", ANode->Node->DBKey, opts );
	}
	UpdateDashboard();

}
//---------------------------------------------------------------------------
#pragma end_region



void __fastcall TfrmMain::cbIsIndividualClick(TObject *Sender)
{
	// check if user wants to clear the previuos entries
	// show task dialog to clear entries
	//if ( rsp == YES )
	//{		//if ( cbIsIndividual->Checked )
			//ebCoName->Text = "";
			//else
			//{	ebLName->Text = "";
				//bFName->Text = "";
			//}
	//}

}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::cbFixedPricingDateClick(TObject *Sender)
{
	CodeSite->SendMsg( "cbFixedPricingDateClick" );
	this->dtpFixedPricing->Enabled = cbFixedPricingDate->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::actResourcesExecute(TObject *Sender)
{
	// show interface to add resources
	TfrmResources* frm = new TfrmResources( this );
	frm->ShowModal();
	delete frm;
	#if defined _clang_
		frm = nullptr;
	#else
		frm = NULL;
	#endif

}
//---------------------------------------------------------------------------
#pragma region CustomerStats

//---------------------------------------------------------------------------
void __fastcall TfrmMain::ddSalesProductChange(TObject *Sender)
{
	// update gauges
	try
	{	clientSales->Refresh(dmUniDac->dsSales);
		ggeSalesQty->Value = clientSales->AvgQty( ddSalesProduct->Text );
		ggeSalesDays->Value = clientSales->AvgDays( ddSalesProduct->Text );
		TVarRec args[1];
		args[0] = (long double)clientSales->TotalSales( ddSalesProduct->Text );
		lblTotalSales->Caption->Text = Format( String("Total Sales: %0.2m"), args, 1 );
		args[0] = (long double)clientSales->TotalQty( ddSalesProduct->Text );
		lblTotalQty->Caption->Text = Format( String("Total Quantity: %0.0n"), args, 1 );
		SetSalesBarChart();
	}
	catch ( Exception *ex )
	{
		CodeSite->SendException( ex );
	}
	return;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::UpdateDashboard()
{
	CodeSite->EnterMethod( "TForm1::UpdateDashboard" );
	try
	{	// populate vector with sales
		if ( ! clientSales ) {
			clientSales = new clsClientSales(dmUniDac->dsSales);
		}
		clientSales->Refresh();
		TVarRec args[1];
		args[0] = (long double)clientSales->TotalSales();
		lblTotalSales->Caption->Text = Format( String("Total Sales: %0.2m"), args, 1 );
		args[0] = (long double)clientSales->TotalQty();
		lblTotalQty->Caption->Text = Format( String("Total Quantity: %0.0n"), args, 1 );
		// fill ddSalesProducts
		TStrings* lst = clientSales->ProductList();
		if ( lst )
			ddSalesProduct->Items = lst;

		// to control tick marks to whole numbers must recalc max values as multiplier of 8
		int maxQty = ceil( clientSales->MaxQty() / 8.0 ) * 8;
		ggeSalesQty->MaximumValue = ( maxQty < 8 ? 8 : maxQty );
		int maxDays = ceil( clientSales->MaxDays() / 8.0 ) * 8;
		ggeSalesDays->MaximumValue = ( maxDays < 8 ? 8 : maxDays );
		// set avg values
		ggeSalesQty->Value = clientSales->AvgQty();
		ggeSalesDays->Value = clientSales->AvgDays();
		// update sales chart
		SetSalesBarChart();
	}
	catch( Exception *ex )
	{
		CodeSite->SendException( ex );
	}
	CodeSite->ExitMethod( "TForm1::UpdateDashboard" );
}
//---------------------------------------------------------------------------
#pragma end_region
#pragma region Sales Bar Chart
//---------------------------------------------------------------------------
void __fastcall TfrmMain::SetSalesBarChart ()
{
	if ( ddSalesProduct->ItemIndex == -1 )
		dbchartSales->Title->Caption = "Total Sales by Quantity";
	else
		dbchartSales->Title->Caption =  L"Sales Quantity by Item - " + ddSalesProduct->Text;
	try
	{   TChartSeries* srs = dbchartSales->Series[0];
		srs->Clear();
		for (int i = 0; i < clientSales->Count; i++)
		{   clsSale* itm = clientSales->Item(i);
			if ( ddSalesProduct->ItemIndex == -1 )
				srs->AddXY( itm->SaleDate, itm->Qty );
			else
			{   if (itm->Product == ddSalesProduct->Text )
					srs->AddXY( itm->SaleDate, itm->Qty );
			}
		}
	}
	catch ( Exception *ex )
	{
		CodeSite->SendException( ex );
	}
	return;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma end_region

#pragma region Test planner item events

void __fastcall TfrmMain::daySchedulePlannerDblClick(TObject *Sender, int Position,
		  int FromSel, int FromSelPrecise, int ToSel, int ToSelPrecise)
{
	CodeSite->EnterMethod( "daySchedulePlannerDblClick" );
	TEzBItemEditor* editor 	= nullptr;
	clsEzBooking*   gb      = nullptr;

	try
	{   TPlannerItem* pi = nullptr;
		if ( Position == 0 )
		{   // create a simple item at the selected position
			CodeSite->SendMsg( "Create office item" );
			pi = daySchedule->CreateItem();
			pi->Editor = plannerItemEditorDefault;
			pi->ItemStartTime = FromSelPrecise;
			pi->ItemEndTime = ToSelPrecise;
			pi->ItemPos = Position;
			pi->Edit();
			pi->Update();
		}
		else
		{   // create a new delivery at this point
			try
			{   CodeSite->SendMsg( "create delivery item " );
				if ( ! bookingEditor )
					bookingEditor = new TEzBItemEditor( this, dmUniDac->uCon );
				pi = daySchedule->CreateItem();
				pi->Editor = bookingEditor;
				pi->ItemBeginPrecis		= FromSelPrecise;
				pi->ItemEndPrecis 		= ToSelPrecise;
				pi->ItemPos 			= Position;
				pi->OwnsItemObject 		= true;
				gb = new clsEzBooking();
				pi->ItemObject 			= gb;
				//dmUniDac->qryluCustomers->Active = true;
				//dmUniDac->tblluLocations->Active = false;
				CodeSite->SendMsg("Edit booking item ");
				pi->Edit();
				if ( bookingEditor->ModalResult == mrOk)
				{   pi->CaptionText 	= L"Delivery: " + gb->CustomerName;
					pi->CaptionBkg 		= clDelivery;
					pi->CaptionBkgTo	= clDeliveryTo;
					pi->Text->Text 		= gb->Address;
					pi->Text->Add( L"Estimated Qty: " + IntToStr(gb->EstQty) );
					pi->Text->Add( gb->Notes );
					pi->ItemPos 		= static_cast<TDBDaySource*>(daySchedule->ItemSource)->ResourceMap->ResToPosition( gb->ResourceID );
					pi->Update();
				}
				else
					daySchedule->FreeItem(pi);
			}
			catch( Exception *ex )
			{
				CodeSite->EnterMethod("daySchedulePlannerDblClick");
				CodeSite->SendError(ex->Message);
				CodeSite->ExitMethod("daySchedulePlannerDblClick");
			}
			//dmUniDac->tblluLocations->Active = false;
			//dmUniDac->qryluCustomers->Active = false;
			UpdateScheduleFooter();
		}
	}
	catch ( Exception *ex )
	{
		CodeSite->EnterMethod("daySchedulePlannerDblClick");
		CodeSite->SendException(ex);
		CodeSite->ExitMethod("daySchedulePlannerDblClick");
	}
	CodeSite->ExitMethod( "daySchedulePlannerDblClick" );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::dayScheduleItemMoving(TObject *Sender, TPlannerItem *Item,
		  int DeltaBegin, int DeltaPos, bool &Allow)
{
	#ifdef _DEBUG
	sb->Panels->Items[1]->Text = String(Item->ItemPos) + " -> " + String( DeltaBegin ) + ", " + String(DeltaPos);
	CodeSite->SendMsg( csmInfo, "Moving Item" );
	#endif

	switch ( Item->ItemPos )
	{   case 0:
			if ( DeltaPos != 0 )
				Allow = false;
			break;
		default:
			if ( Item->ItemPos + DeltaPos == 0 )
				Allow = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::dayScheduleItemMove(TObject *Sender, TPlannerItem *Item,
		  int FromBegin, int FromEnd, int FromPos, int ToBegin, int ToEnd,
		  int ToPos)
{
	if ( ToPos > 0 && ( FromPos != ToPos ) )
		UpdateScheduleFooter();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::dayScheduleItemBalloon(TObject *Sender, TPlannerItem *APlannerItem,
		  UnicodeString &ATitle, UnicodeString &AText, int &AIcon)
{
	try
	{	ATitle = APlannerItem->CaptionText;
		if ( APlannerItem->ItemObject ) {
			clsEzBooking *gb = static_cast<clsEzBooking*>(APlannerItem->ItemObject);
			if ( gb ) {
				AText = L"Est. Qty: " + String( gb->EstQty );
			}
		}
		else
		{   if (APlannerItem->Text->Count )
				AText = APlannerItem->Text->Strings[0];
			else
                AText = "No desription available";
		}
	}
	catch ( Exception *ex )
	{
		;
	}
}
//---------------------------------------------------------------------------


#pragma end_region





