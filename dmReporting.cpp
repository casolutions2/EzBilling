//---------------------------------------------------------------------------


#pragma hdrstop

#include "dmReporting.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma link "frxDBSet"
#pragma link "frxExportPDF"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "Uni"
#pragma link "frxDBSet"
#pragma link "frxExportPDF"

#pragma resource "*.dfm"
TrptModule *rptModule;
//---------------------------------------------------------------------------
__fastcall TrptModule::TrptModule(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------

