//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UfrmGroupEntry.h"
#include "uDMUniDac.h"			// main data module with connection and data sources
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "DBAdvGrid"
#pragma link "AdvUtil"
#pragma resource "*.dfm"
TfrmGroup *frmGroup;
//---------------------------------------------------------------------------
__fastcall TfrmGroup::TfrmGroup(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
