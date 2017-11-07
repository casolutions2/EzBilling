//---------------------------------------------------------------------------

#ifndef UfrmGroupEntryH
#define UfrmGroupEntryH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include "DBAdvGrid.hpp"
#include <Data.DB.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Mask.hpp>
#include "AdvUtil.hpp"
//---------------------------------------------------------------------------
class TfrmGroup : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TPanel *Panel2;
	TDBAdvGrid *DBAdvGrid1;
	TDBEdit *DBEdit1;
	TDBMemo *DBMemo1;
	TDBNavigator *DBNavigator1;
private:	// User declarations
public:		// User declarations
	__fastcall TfrmGroup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmGroup *frmGroup;
//---------------------------------------------------------------------------
#endif
