//---------------------------------------------------------------------------


#pragma hdrstop

#include "dmWebUpdate.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma link "WUpdate"
#pragma link "WUpdateLanguagesU"
#pragma link "WUpdateWiz"
#pragma resource "*.dfm"
TmodWebUpdate *modWebUpdate;
//---------------------------------------------------------------------------
__fastcall TmodWebUpdate::TmodWebUpdate(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
bool __fastcall TmodWebUpdate::checkForUpdates()
{
	return WebUpdate1->NewVersionAvailable();
}
//---------------------------------------------------------------------------
