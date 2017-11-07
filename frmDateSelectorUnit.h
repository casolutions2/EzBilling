//---------------------------------------------------------------------------

#ifndef frmDateSelectorUnitH
#define frmDateSelectorUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvDateTimePicker.hpp"
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmDateSelector : public TForm
{
__published:	// IDE-managed Components
	TAdvDateTimePicker *dtpSlctdDate;
	TLabel *Label1;
	TButton *btnOK;
	TButton *btnCancel;
	void __fastcall btnOKClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmDateSelector(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDateSelector *frmDateSelector;
//---------------------------------------------------------------------------
#endif
