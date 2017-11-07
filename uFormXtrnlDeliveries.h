//---------------------------------------------------------------------------

#ifndef uFormXtrnlDeliveriesH
#define uFormXtrnlDeliveriesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvGrid.hpp"
#include "AdvGridPDFIO.hpp"
#include "AdvObj.hpp"
#include "AdvPDFIO.hpp"
#include "AdvUtil.hpp"
#include "asgprev.hpp"
#include "asgprint.hpp"
#include "BaseGrid.hpp"
#include "tmsAdvGridExcel.hpp"
#include "W7Bars.hpp"
#include "W7Buttons.hpp"
#include "W7Classes.hpp"
#include <System.ImageList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include "TaskDialog.hpp"

#include "classExternalDeliveries.h" 	// class for handling external deliveries and invoicing
#include "UFormMain.h"
#include "clsCustomer.h"
#include "AdvCustomComponent.hpp"			// CASQ header for all required classes

//---------------------------------------------------------------------------
class TfrmXtrnlDeliveries : public TForm
{
__published:	// IDE-managed Components
	TImageList *il32;
	TAdvGridPrintSettingsDialog *printSettings;
	TAdvPreviewDialog *printPreview;
	TW7ToolBar *W7ToolBar1;
	TW7ToolButton *btnProcess;
	TW7ToolButton *btnCancel;
	TW7ToolButton *W7ToolButton2;
	TW7ToolButton *btnPrint;
	TAdvStringGrid *grid;
	TAdvTaskDialog *td1;
	TAdvGridPDFIO *AdvGridPDFIO1;
	TAdvGridExcelIO *AdvGridExcelIO1;
	void __fastcall btnPrintClick(TObject *Sender);
	void __fastcall gridGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign,
		  Advobj::TVAlignment &VAlign);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnProcessClick(TObject *Sender);
private:	// User declarations
	void 	__fastcall SetupGrid();
	void    __fastcall ValidateCustomers();

	int     			_invoicesWritten;
	clsXtrnlDeliveries  *xd;
	TfrmMain            *parentForm;
	clsColCustomers 	*_customers;

public:		// User declarations
	__fastcall TfrmXtrnlDeliveries(TComponent* Owner);
	__fastcall  TfrmXtrnlDeliveries( TComponent* Owner, clsColCustomers* ACustomers );
	bool 	__fastcall	LoadFile(String filename);

	int     __property InvoicesWritten	= { read=_invoicesWritten };

	TPCustomer 	Supplier;

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmXtrnlDeliveries *frmXtrnlDeliveries;
//---------------------------------------------------------------------------
#endif
