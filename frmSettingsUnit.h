//---------------------------------------------------------------------------

#ifndef frmSettingsUnitH
#define frmSettingsUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvEdit.hpp"
#include "AdvFileNameEdit.hpp"
#include "AdvPanel.hpp"
#include <Vcl.ExtCtrls.hpp>
#include "AdvGlowButton.hpp"
#include "AdvEdBtn.hpp"
//---------------------------------------------------------------------------
class TfrmSettings : public TForm
{
__published:	// IDE-managed Components
	TAdvPanelStyler *AdvPanelStyler1;
	TAdvPanel *AdvPanel1;
	TAdvFileNameEdit *ebSQLFile;
	TAdvFileNameEdit *ebMyobApp;
	TAdvFileNameEdit *ebMyobData;
	TBevel *Bevel1;
	TAdvGlowButton *btnOK;
	TAdvGlowButton *btnCancel;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ebSQLFileChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSettings(TComponent* Owner);
	bool changed;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSettings *frmSettings;
//---------------------------------------------------------------------------
#endif
