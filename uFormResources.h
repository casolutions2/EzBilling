//---------------------------------------------------------------------------

#ifndef uFormResourcesH
#define uFormResourcesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "DBAdvNavigator.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.DBScope.hpp>
#include <Data.Bind.EngExt.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <Vcl.Bind.DBEngExt.hpp>
#include <Vcl.Bind.Editors.hpp>
//---------------------------------------------------------------------------
class TfrmResources : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TEdit *ebResourceName;
	TDBAdvNavigator *dbNavResources;
	TBindSourceDB *BindSourceDB1;
	TLinkControlToField *LinkControlToFieldName;
	TBindingsList *BindingsList1;
	TMemo *MemoDescription;
	TLinkControlToField *LinkControlToFieldDescription;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmResources(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmResources *frmResources;
//---------------------------------------------------------------------------
#endif
