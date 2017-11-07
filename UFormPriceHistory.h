//---------------------------------------------------------------------------

#ifndef UFormPriceHistoryH
#define UFormPriceHistoryH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include "DBAdvGrid.hpp"
#include <Data.DB.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Grids.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <Vcl.Bind.DBEngExt.hpp>
#include <Vcl.Bind.Editors.hpp>
#include <Vcl.ComCtrls.hpp>
#include "DBAdvNavigator.hpp"
#include <Vcl.Buttons.hpp>
#include "AdvUtil.hpp"
//---------------------------------------------------------------------------
class TfrmPriceAdjustHistory : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlTop;
	TPanel *pnlBottom;
	TSplitter *Splitter1;
	TDBAdvGrid *gridPriceHistory;
	TDataSource *dsPriceHistory;
	TBindSourceDB *BindSourceDB1;
	TBindingsList *BindingsList1;
	TDateTimePicker *DateTimePickerAdjustedTimeStamp;
	TLinkControlToField *LinkControlToFieldAdjustedTimeStamp2;
	TEdit *EditGasCostPrice;
	TLinkControlToField *LinkControlToFieldGasCostPrice2;
	TEdit *EditAdjustPriceBy;
	TLinkControlToField *LinkControlToFieldAdjustPriceBy;
	TEdit *EditAdjustedCustomers;
	TLinkControlToField *LinkControlToFieldAdjustedCustomers;
	TEdit *EditMarkUpOnCost;
	TLinkControlToField *LinkControlToFieldMarkUpOnCost;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TDBAdvNavigator *DBAdvNavigator1;
	TSpeedButton *btnPrint;
	void __fastcall btnPrintClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPriceAdjustHistory(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPriceAdjustHistory *frmPriceAdjustHistory;
//---------------------------------------------------------------------------
#endif
