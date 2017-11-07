//---------------------------------------------------------------------------

#ifndef uFormScheduleEntryH
#define uFormScheduleEntryH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "AdvUtil.hpp"
#include "BaseGrid.hpp"
#include "DBAdvGrid.hpp"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
#include "uDMUniDac.h"
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Uni.hpp"
#include <Data.DB.hpp>
#include "AdvGlowButton.hpp"
#include "AdvPanel.hpp"
//---------------------------------------------------------------------------
class TfrmScheduleEntry : public TForm
{
__published:	// IDE-managed Components
	TDateTimePicker *dtpDelDate;
	TDBAdvGrid *gridDelivered;
	TLabel *Label1;
	TUniQuery *uq;
	TDataSource *DataSource1;
	TAdvPanel *AdvPanel1;
	TAdvPanel *AdvPanel2;
	TAdvGlowButton *btnInvoice;
	TAdvGlowButton *btnOk;
	void __fastcall dtpDelDateCloseUp(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall btnInvoiceClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall gridDeliveredCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit);


private:	// User declarations
    void __fastcall RefreshData();
public:		// User declarations
	__fastcall TfrmScheduleEntry(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmScheduleEntry *frmScheduleEntry;
//---------------------------------------------------------------------------
#endif
