//---------------------------------------------------------------------------

#ifndef UFormMainH
#define UFormMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.ImageList.hpp>
#include <System.Actions.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <Data.Bind.ObjectScope.hpp>
#include <Data.DB.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Vcl.Bind.Consts.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Themes.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Bind.DBEngExt.hpp>
#include <Vcl.Bind.Editors.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.DBCtrls.hpp>
#include "AdvOfficePager.hpp"
#include "AdvOfficePagerStylers.hpp"
#include "AdvOfficeStatusBar.hpp"
#include "AdvSmoothCalendar.hpp"
#include "AdvAppStyler.hpp"
#include "AdvPanel.hpp"
#include "TaskDialog.hpp"
#include "WUpdate.hpp"
#include "WUpdateWiz.hpp"
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include "W7Bars.hpp"
#include "W7Classes.hpp"
#include "PlannerCal.hpp"
#include "Planner.hpp"
#include "DBPlanner.hpp"
#include "PlannerWaitList.hpp"
#include "AdvOfficeButtons.hpp"
#include "DBPlannerMonthView.hpp"
#include "PlannerMonthView.hpp"
#include "PlanItemEdit.hpp"
#include "AdvUtil.hpp"
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Uni.hpp"
#include "DBAdvGrid.hpp"
#include "DBAdvNavigator.hpp"
#include "AdvCustomTreeView.hpp"
#include "AdvTreeView.hpp"
#include "AdvTreeViewBase.hpp"
#include "AdvTreeViewData.hpp"
#include "MoneyEdit.hpp"
#include <Data.Bind.Controls.hpp>
#include <Vcl.Bind.Navigator.hpp>
#include "AdvSpin.hpp"
#include <Vcl.Imaging.pngimage.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.DBChart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include "AdvSmoothGauge.hpp"
#include <VCLTee.Series.hpp>
#include "AdvSmoothLabel.hpp"
#include "tmsAdvGridExcel.hpp"
#include "AdvOfficeHint.hpp"
//---------------------------------------------------------------------------
#include <string>
#include <vector>
//---------------------------------------------------------------------------
#include "clsCASQ_App.h"            // Application class containing settings etc
#include "BillingRateClass.h"   	// class for volume billing and fixed price adjustment
#include "clsCustomer.h"			// CASQ header for all required classes
//#include "classTCustomerTree.h"		// class for handling customer treeview functions in VirtualTree
//#include "AdvCustomerTV.h"			// unit for handling TMS Advanced Treeview functions
//#include "EzGasClasses.h"
#include "clsCustomerAdvTree.h"		// class for handling customer treeview functions in AdvTreeView
#include "classClientSales.h"		// class for manipulating client sales

//---------------------------------------------------------------------------
//  PlannerItem editor and related classes for specific gas delivery item
#include "classEZB_Booking.h"       // class for holding additional information regarding the gas delivery booking
#include "clsEzBItemEditor.h"       // class for gas delivery booking planner items
#include "formBookingEditor.h"      // form used by EzBItemEditor to collect information re booking
//---------------------------------------------------------------------------

class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPopupMenu 			*tvPopup;
	TMenuItem 			*mnuPopupDelete;
	TMenuItem 			*N6;
	TMenuItem 			*mnuPopupNewLocation;
	TMenuItem 			*mnuPopupNewContainer;
	TMenuItem 			*mnuPopupNewCustomer;
	TAdvOfficeStatusBar *sb;
	TAdvOfficePager 	*pgrMain;
	TAdvOfficePage 		*pgBillingAdjust;
	TAdvOfficePage 		*pgSundryGrid;
	TAdvOfficePage 		*pgPlanner;
	TAdvOfficePage 		*pgCustomer;
	TAdvOfficePage 		*pgImportDeliveries;
	TAdvTaskDialog 		*td1;
	TWebUpdate 			*wu;
	TWebUpdateWizard 	*wuWizard;
	TAdvStringGrid 		*baGrid;
	TAdvStringGrid 		*grid;
	TW7ToolBar 			*W7ToolBar1;
	TImageList 			*ImageList1;
	TPlannerWaitList 	*PlannerWaitList1;
	TAdvPanelStyler 	*AdvPanelStyler1;
	TAdvOfficePagerOfficeStyler *AdvOfficePagerOfficeStyler1;
	TPanel 				*pnlPlannerRight;
	TAdvPanel 			*pnlPlannerLeft;
	TAdvPanel 			*pnlCustomerTree;
	TSplitter 			*Splitter1;
	TAdvPanel 			*pnlTVToolBar;
	TFileOpenDialog 	*fo;
	TAdvOfficePage *pgMonthPlanner;
	TPopupMenu *popupPlannerItem;
	TDefaultItemEditor *plannerItemEditorDefault;
	TPlannerCalendar *plannerCal;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *mnuFileLocations;
	TMenuItem *N1;
	TMenuItem *mnuFileUpdate;
	TMenuItem *N2;
	TMenuItem *mnuStyles;
	TMenuItem *N5;
	TMenuItem *mnuFileExit;
	TMenuItem *mnuPlanner;
	TMenuItem *Schedule1;
	TMenuItem *Month1;
	TMenuItem *N8;
	TMenuItem *ScheduleDelivery1;
	TMenuItem *mnuCustomers;
	TMenuItem *mnuCustomerMaintenance;
	TMenuItem *N7;
	TMenuItem *mnuCustomersAddCustomer;
	TMenuItem *Groups1;
	TMenuItem *N3;
	TMenuItem *mnuCustomersBulkDeactivate;
	TMenuItem *mnuCustomersMYOB;
	TMenuItem *mnuCustomersMYOBSyncCustomers;
	TMenuItem *mnuCustomersMYOBSyncBillingRates;
	TMenuItem *mnuPricing;
	TMenuItem *mnuPricingFixedAdjustment;
	TMenuItem *mnuPricingVolumeAdjustment;
	TMenuItem *mnuDeliveries;
	TMenuItem *mnuDeliveriesConfirmSchedule;
	TMenuItem *mnuDeliveriesManualEntry;
	TMenuItem *N4;
	TMenuItem *mnuDeliveriesImport;
	TMenuItem *mnuDeliveriesImportOrigin;
	TMenuItem *mnuDeliveriesImportKleenheat;
	TMenuItem *Help1;
	TMenuItem *mnuHelpContents;
	TMenuItem *mnuHelpHomepage;
	TMenuItem *mnuHelpAbout;
	TActionManager *ActionManager1;
	TAction *actCustomerBulkDeactivate;
	TAction *actHelpAbout;
	TAction *actCustomerMaint;
	TAction *actCustomerGroups;
	TAction *actMaintContainer;
	TAction *actNewLocation;
	TAction *actShowDaySchedule;
	TAction *actSyncMYOB;
	TAction *actNewCustomer;
	TAction *actDeliverySchedule;
	TAction *actPlannerShowMonth;
	TDBPlanner *daySchedule;
	TMenuItem *mnuPlannerOfficeItem;
	TAction *actGenericItem;
	TMenuItem *New1;
	TMenuItem *Delivery1;
	TMenuItem *OfficeItem1;
	TAlarmMessage *AlarmMessage1;
	TDBPlannerMonthView *plannerMonthView;
	TMenuItem *PrintCalendar1;
	TMenuItem *PrintRunSheet1;
	TAction *actRunSheet;
	TMenuItem *Maintain1;
	TMenuItem *Rates1;
	TAction *actGroupRates;
	TAction *actGroupMaintain;
	TMenuItem *N9;
	TMenuItem *Showoutsidehours1;
	TAction *actInvoiceSchedule;
	TAction *actManualInvoice;
	TActionToolBar *toolbarCommon;
	TTimer *tmrsb;
	TMenuItem *mnuPricingHistory;
	TAction *actPricingHistory;
	TMenuItem *mnuPopupInactive;
	TAdvOfficePager *pgrCustomerInfo;
	TAdvOfficePage *pgDBDetails;
	TAdvOfficePage *pgDBLocations;
	TAdvOfficePage *pgDBContainers;
	TAdvOfficePage *pgDBStatistics;
	TDBAdvGrid *gridCustomerLocations;
	TAdvPanel *AdvPanel1;
	TLabel *Label6;
	TSplitter *Splitter2;
	TLabel *Label7;
	TEdit *ebCity;
	TLabel *Label8;
	TComboBox *ddState;
	TLabel *Label9;
	TEdit *ebCountry;
	TLabel *Label14;
	TEdit *ebPostcode;
	TLabel *Label10;
	TEdit *ebPhone1;
	TLabel *Label11;
	TEdit *ebMobile;
	TLabel *Label12;
	TEdit *ebEmail;
	TLabel *Label13;
	TEdit *ebContact;
	TLabel *Label15;
	TMemo *memLocationNotes;
	TMemo *memAddress;
	TBindSourceDB *BindSourceDB1;
	TLinkControlToField *LinkControlToFieldAddressLines;
	TBindingsList *BindingsList1;
	TLinkControlToField *LinkControlToFieldCity;
	TLinkControlToField *LinkControlToFieldContactName;
	TLinkFillControlToField *LinkFillControlToFieldState;
	TLinkControlToField *LinkControlToFieldEmail;
	TLinkControlToField *LinkControlToFieldCountry;
	TLinkControlToField *LinkControlToFieldMobilePhone;
	TLinkControlToField *LinkControlToFieldPhone;
	TLinkControlToField *LinkControlToFieldPostCode;
	TLinkControlToField *LinkControlToFieldNotes;
	TAdvOfficeCheckBox *cbLocationPrimary;
	TLinkPropertyToField *LinkPropertyToFieldChecked;
	TDBAdvNavigator *dbNavLocations;
	TAdvPanel *AdvPanel2;
	TAdvOfficeCheckBox *cbActive;
	TAdvOfficeCheckBox *cbIsIndividual;
	TLabel *Label1;
	TEdit *ebCoName;
	TLabel *Label2;
	TEdit *ebFName;
	TLabel *Label3;
	TEdit *ebLName;
	TLabel *Label4;
	TEdit *ebABN;
	TLabel *Label5;
	TMemo *memNotes;
	TAdvOfficeCheckBox *cbVolAdjust;
	TAdvOfficeCheckBox *cbVolAdjustInterest;
	TAdvOfficeCheckBox *cbFixedPricingDate;
	TDateTimePicker *dtpFixedPricing;
	TLabel *Label17;
	TDBLookupComboBox *ddGroup;
	TBevel *Bevel1;
	TAdvOfficePager *pgrCustomerTrees;
	TAdvOfficePage *pgTMSTree;
	TAdvTreeView *atvCustomers;
	TMenuItem *SyncMYOBBillingRates1;
	TAdvOfficeCheckBox *cbActiveCustomers;
	TBindSourceDB *BindSourceDB2;
	TLinkControlToField *LinkControlToFieldCoName;
	TLinkControlToField *LinkControlToField1;
	TLinkControlToField *LinkControlToField2;
	TLinkPropertyToField *LinkPropertyToFieldChecked2;
	TLinkPropertyToField *LinkPropertyToFieldChecked3;
	TLinkControlToField *LinkControlToField3;
	TLinkControlToField *LinkControlToField4;
	TLinkPropertyToField *LinkPropertyToFieldChecked4;
	TLinkControlToField *LinkControlToFieldFixedPricingDate;
	TLinkPropertyToField *LinkPropertyToFieldChecked5;
	TLinkPropertyToField *LinkPropertyToFieldChecked6;
	TDBAdvGrid *gridBillingRates;
	TLabel *Label18;
	TAdvPanel *pnlCustomerBillingrates;
	TAdvPanel *AdvPanel3;
	TAdvPanel *AdvPanel4;
	TLabel *Label16;
	TDBAdvNavigator *DBAdvNavigator1;
	TDBAdvGrid *gridXtrnlAccts;
	TAdvPanel *AdvPanel5;
	TBevel *Bevel2;
	TLabel *Label19;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label22;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *Label25;
	TImage *imgTack;
	TEdit *ebSerialNumber;
	TDateTimePicker *dtpLastInspect;
	TDateTimePicker *dtpManufactureDate;
	TAdvSpinEdit *seInspectPd;
	TAdvSpinEdit *seSize;
	TDateTimePicker *dtpNextInspect;
	TAdvOfficeCheckBox *cbLeased;
	TPanel *pnlLease;
	TLabel *Label26;
	TLabel *Label27;
	TMoneyEdit *ebChargeRate;
	TAdvSpinEdit *seChargePd;
	TMemo *Memo1;
	TAdvFormStyler *AdvFormStyler1;
	TAdvPanel *AdvPanel6;
	TSplitter *Splitter3;
	TDBAdvGrid *DBAdvGrid2;
	TAdvPanel *AdvPanel7;
	TAdvPanel *AdvPanel8;
	TLabel *Label28;
	TDBAdvGrid *gridCustomerSales;
	TSplitter *Splitter4;
	TAdvPanel *AdvPanel9;
	TMenuItem *Resources1;
	TMenuItem *N10;
	TAction *actResources;
	TButton *btnCustomerTreeContractAll;
	TAdvPanel *AdvPanel10;
	TSplitter *Splitter5;
	TDBChart *dbchartSales;
	TBarSeries *Series1;
	TAdvSmoothLabel *lblTotalSales;
	TAdvSmoothLabel *lblTotalQty;
	TLabel *lblDisplayName;
	TPanel *Panel1;
	TLinkPropertyToField *LinkPropertyToFieldCaption2;
	TComboBox *ddSalesProduct;
	TAdvPanel *AdvPanel11;
	TAdvPanel *AdvPanel12;
	TAdvPanel *AdvPanel13;
	TAdvSmoothGauge *ggeSalesQty;
	TAdvSmoothGauge *ggeSalesDays;
	TAdvGridExcelIO *AdvGridExcelIO1;
	TAdvOfficeHint *AdvOfficeHint1;
	TDBAdvNavigator *DBAdvNavigator2;
	void __fastcall mnuFileExitClick(TObject *Sender);
	void __fastcall mnuFileLocationsClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall mnuPricingFixedAdjustmentClick(TObject *Sender);
	void __fastcall mnuPricingVolumeAdjustmentClick(TObject *Sender);
	void __fastcall baGridGetFloatFormat(TObject *Sender, int ACol, int ARow, bool &IsFloat,
		  UnicodeString &FloatFormat);
	void __fastcall baGridGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState,
		  TBrush *ABrush, TFont *AFont);
	void __fastcall baGridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
		  Advobj::TVAlignment &VAlign);
	void __fastcall mnuFileUpdateClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall actCustomerBulkDeactivateExecute(TObject *Sender);
	void __fastcall mnuStylesItemClick(TObject* Sender);
	void __fastcall actHelpAboutExecute(TObject *Sender);
	void __fastcall actCustomerMaintExecute(TObject *Sender);
	void __fastcall pgCustomerExit(TObject *Sender);
	void __fastcall mnuDeliveriesImportKleenheatClick(TObject *Sender);
	void __fastcall actShowDayScheduleExecute(TObject *Sender);
	void __fastcall actSyncMYOBExecute(TObject *Sender);
	void __fastcall mnuCustomersMYOBSyncBillingRatesClick(TObject *Sender);
	void __fastcall pgBillingAdjustExit(TObject *Sender);
	void __fastcall actNewCustomerExecute(TObject *Sender);
	void __fastcall actDeliveryScheduleExecute(TObject *Sender);
	void __fastcall plannerCalDateChange(TObject *Sender, TDateTime origDate, TDateTime newDate);
	void __fastcall actPlannerShowMonthExecute(TObject *Sender);
	void __fastcall daySchedulePlannerNext(TObject *Sender);
	void __fastcall daySchedulePlannerPrev(TObject *Sender);
	void __fastcall actGenericItemExecute(TObject *Sender);
	void __fastcall pgPlannerExit(TObject *Sender);
	void __fastcall plannerMonthViewDateChange(TObject *Sender, TDateTime origDate,
		  TDateTime newDate);
	void __fastcall PrintCalendar1Click(TObject *Sender);
	void __fastcall actRunSheetExecute(TObject *Sender);
	void __fastcall plannerMonthViewDblClick(TObject *Sender, TDateTime SelDate);
	void __fastcall plannerMonthViewItemDblClick(TObject *Sender, TPlannerItem *Item);
	void __fastcall plannerMonthViewItemPopupPrepare(TObject *Sender, TPopupMenu *PopupMenu,
		  TPlannerItem *Item);
	void __fastcall actGroupRatesExecute(TObject *Sender);
	void __fastcall actGroupMaintainExecute(TObject *Sender);
	void __fastcall Showoutsidehours1Click(TObject *Sender);
	void __fastcall actInvoiceScheduleExecute(TObject *Sender);
	void __fastcall actManualInvoiceExecute(TObject *Sender);
	void __fastcall tmrsbTimer(TObject *Sender);
	void __fastcall actPricingHistoryExecute(TObject *Sender);
	void __fastcall mnuPopupInactiveClick(TObject *Sender);
	void __fastcall daySchedulePlannerMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall dayScheduleItemDelete(TObject *Sender, TPlannerItem *Item);
	void __fastcall atvcBeforeExpandNode(TObject *Sender, TAdvTreeViewVirtualNode *ANode, bool &ACanExpand);
	void __fastcall pgTMSTreeShow(TObject *Sender);
	void __fastcall cbActiveCustomersClick(TObject *Sender);
	void __fastcall pgCustomerEnter(TObject *Sender);
	void __fastcall cbIsIndividualClick(TObject *Sender);
	void __fastcall atvCustomersAfterSelectNode(TObject *Sender, TAdvTreeViewVirtualNode *ANode);
	void __fastcall cbFixedPricingDateClick(TObject *Sender);
	void __fastcall pgPlannerEnter(TObject *Sender);
	void __fastcall pgMonthPlannerEnter(TObject *Sender);
	void __fastcall pgMonthPlannerExit(TObject *Sender);
	void __fastcall actResourcesExecute(TObject *Sender);
	void __fastcall btnCustomerTreeContractAllClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ddSalesProductChange(TObject *Sender);
	void __fastcall dayScheduleItemRightClick(TObject *Sender, TPlannerItem *Item);
	void __fastcall dayScheduleItemDblClick(TObject *Sender, TPlannerItem *Item);
	void __fastcall daySchedulePlannerDblClick(TObject *Sender, int Position, int FromSel,
		  int FromSelPrecise, int ToSel, int ToSelPrecise);
	void __fastcall dayScheduleItemMoving(TObject *Sender, TPlannerItem *Item, int DeltaBegin,
		  int DeltaPos, bool &Allow);
	void __fastcall dayScheduleItemMove(TObject *Sender, TPlannerItem *Item, int FromBegin,
          int FromEnd, int FromPos, int ToBegin, int ToEnd, int ToPos);
	void __fastcall dayScheduleItemBalloon(TObject *Sender, TPlannerItem *APlannerItem,
          UnicodeString &ATitle, UnicodeString &AText, int &AIcon);




private:	// User declarations
	void 		__fastcall 	SetupPager();
	void 		__fastcall 	ClearBillAdjustControls();
	void		__fastcall	DestroyObjects();
	int			__fastcall	PopulateCustomers();
	void		__fastcall	PopulateCustomersFinished(TObject* Sender);
	void 		__fastcall 	InitialiseWithMyob();
	void 		__fastcall 	UnlockApp();
	void 		__fastcall  SetupMenu();
	int 		__fastcall 	SyncMyobCustomers();
	void 		__fastcall 	LoadExternalAccounts(std::wstring prefix);
	void 		__fastcall 	LoadBillingRates();
	void 		__fastcall 	SetupDaySchedule();
	void		__fastcall	SwitchDayPlannerDisplay(const bool showInactive);
	void 		__fastcall 	RefreshATVC();
	void    	__fastcall  SetupCustomerTree();
	void    	__fastcall  UpdateDashboard();
	void    	__fastcall	SetSalesBarChart();
	void    	__fastcall  AddAutoItem( String ACaption, String ANote, TDateTime ADateStart = 0, TDateTime ADateEnd = 0 /* add some params here */ );
	TDateTime   __fastcall  GetNearestTimeBlock( TDateTime ATimeStamp );


public:		// User declarations
				__fastcall 	TfrmMain(TComponent* Owner);
	void 		__fastcall	RefreshData();
	void 		__fastcall 	ShowError(String errMsg);
	void    	__fastcall  UpdateScheduleFooter();


	clsCASQ_App			*App;
	clsColCustomers		*Customers;
	clsCustomerAdvTree	*ATV;
//	TCustomerTree		*CustomerTV;
	clsClientSales      *clientSales;
	TEzBItemEditor		*bookingEditor;

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
class TCustomerObject : public TObject
{
	public:
		clsCustomer* Customer;

};
#endif
