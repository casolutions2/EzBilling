//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFormResources.h"
#include "uDMUniDac.h"					// data module for UniDac connection to SQLite

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAdvNavigator"
#pragma resource "*.dfm"
TfrmResources *frmResources;
//---------------------------------------------------------------------------
__fastcall TfrmResources::TfrmResources(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmResources::FormCreate(TObject *Sender)
{
	dmUniDac->tblResource->Active = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmResources::FormClose(TObject *Sender, TCloseAction &Action)
{
	dmUniDac->tblResource->Active = false;
}
//---------------------------------------------------------------------------
