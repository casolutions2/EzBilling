//---------------------------------------------------------------------------

#ifndef dlgSyncingUnitH
#define dlgSyncingUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvSmoothWin8Marquee.hpp"
#include "AdvSmoothProgressBar.hpp"
#include "HTMLabel.hpp"
#include "AdvLabel.hpp"
#include <string>
//---------------------------------------------------------------------------
class TdlgSync : public TForm
{
__published:	// IDE-managed Components
	TAdvLabel *lblInstruction;
	TAdvSmoothProgressBar *pb;
	THTMLabel *HTMLabel1;
private:	// User declarations
public:		// User declarations
	__fastcall TdlgSync(TComponent* Owner);

	void __fastcall UpdateInstruction(String txt);
	void __fastcall UpdateProgress(std::wstring name, int pos);

};
//---------------------------------------------------------------------------
extern PACKAGE TdlgSync *dlgSync;
//---------------------------------------------------------------------------
#endif
