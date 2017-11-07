//---------------------------------------------------------------------------

#ifndef FormBillingAdjustmentH
#define FormBillingAdjustmentH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
#include "BillingRateClass.h"
#include "classBillingAdjustment.h"		// class for handling billing adjustments

//---------------------------------------------------------------------------
#include "AdvEdit.hpp"
#include "AdvLabel.hpp"
#include "AdvMoneyEdit.hpp"
#include "AdvPanel.hpp"
#include "TaskDialog.hpp"
#include "W7Bars.hpp"
#include "W7Buttons.hpp"
#include "W7Classes.hpp"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TformBillingAdjust : public TForm
{
__published:	// IDE-managed Components
	TW7ToolBar *toolbar;
	TW7ToolButton *btnCancel;
	TW7ToolButton *btnPrevious;
	TW7ToolButton *W7ToolButton1;
	TW7ToolButton *btnProceed;
	TW7ToolButton *W7ToolButton3;
	TW7ToolButton *W7ToolButton5;
	TAdvPanel *AdvPanel1;
	TAdvLabel *AdvLabel1;
	TAdvLabel *AdvLabel2;
	TAdvLabel *AdvLabel3;
	TAdvLabel *AdvLabel5;
	TAdvLabel *AdvLabel6;
	TAdvLabel *AdvLabel7;
	TAdvLabel *AdvLabel8;
	TBevel *Bevel1;
	TBevel *Bevel2;
	TAdvLabel *AdvLabel9;
	TAdvLabel *AdvLabel10;
	TAdvLabel *AdvLabel4;
	TAdvMoneyEdit *ebCP;
	TAdvMoneyEdit *ebMFx;
	TAdvEdit *ebApc;
	TAdvMoneyEdit *ebAmin;
	TAdvMoneyEdit *ebAmax;
	TAdvEdit *ebVMd;
	TAdvEdit *ebVMn;
	TAdvEdit *ebIR;
	TAdvEdit *ebDF;
	TAdvEdit *ebMpc;
	TAdvPanel *AdvPanel2;
	TAdvLabel *AdvLabel12;
	TAdvLabel *AdvLabel13;
	TAdvLabel *AdvLabel14;
	TAdvEdit *ebLastAdjDate;
	TAdvEdit *ebLastAdjValue;
	TAdvEdit *ebLastAdjCustomerCount;
	TAdvPanel *AdvPanel3;
	TAdvLabel *AdvLabel11;
	TAdvLabel *AdvLabel15;
	TAdvLabel *AdvLabel16;
	TAdvLabel *AdvLabel17;
	TAdvLabel *AdvLabel19;
	TAdvLabel *AdvLabel20;
	TAdvLabel *AdvLabel18;
	TAdvLabel *AdvLabel21;
	TBevel *Bevel3;
	TAdvMoneyEdit *ebResetToZero;
	TAdvEdit *ebAdjPrice;
	TAdvMoneyEdit *ebRestrictMin;
	TAdvMoneyEdit *ebRestrictMax;
	TAdvMoneyEdit *ebNewMin;
	TAdvMoneyEdit *ebNewMax;
	TAdvTaskDialog *td2;
	void __fastcall btnProceedClick(TObject *Sender);
	void __fastcall btnPreviousClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);

private:	// User declarations
	// functions
	bool        		__fastcall CheckEntries();
	void 				__fastcall ClearBillAdjustControls();

	int     _returnStatus;

public:		// User declarations
	__fastcall TformBillingAdjust(TComponent* Owner);

	// functions
	TVolumeAdjustment   __fastcall PopulateAdjustment();

	// properties
	int __property ReturnStatus = {read=_returnStatus };

	// variables
	clsBillingAdjust*	ba;
};
//---------------------------------------------------------------------------
extern PACKAGE TformBillingAdjust *formBillingAdjust;
//---------------------------------------------------------------------------
#endif
