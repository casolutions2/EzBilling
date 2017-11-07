//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UFormPriceHistory.h"
#include "uDMUniDac.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "DBAdvGrid"
#pragma link "DBAdvNavigator"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
TfrmPriceAdjustHistory *frmPriceAdjustHistory;
//---------------------------------------------------------------------------
__fastcall TfrmPriceAdjustHistory::TfrmPriceAdjustHistory(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TfrmPriceAdjustHistory::btnPrintClick(TObject *Sender)
{
	// print grid
	gridPriceHistory->Print();
}
//---------------------------------------------------------------------------

