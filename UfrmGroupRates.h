//---------------------------------------------------------------------------

#ifndef UfrmGroupRatesH
#define UfrmGroupRatesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvEdit.hpp"
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "DBAdvGrid.hpp"
#include <Data.DB.hpp>
#include <Vcl.Graphics.hpp>
#include "AdvDateTimePicker.hpp"
#include "AdvDBDateTimePicker.hpp"
#include "AdvDBLookupComboBox.hpp"
#include "AdvGroupBox.hpp"
#include "AdvOfficeButtons.hpp"
#include "DBAdvOfficeButtons.hpp"
#include "dbmnyed.hpp"
#include "MoneyEdit.hpp"
#include <Vcl.DBCtrls.hpp>
#include "DBAdvEd.hpp"
#include <Vcl.Mask.hpp>
#include "DBAdvGlowNavigator.hpp"
#include "AdvUtil.hpp"
//---------------------------------------------------------------------------
class TfrmGroupRates : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TAdvDBDateTimePicker *dtpEffective;
	TAdvDBDateTimePicker *dtpEnd;
	TDBAdvOfficeRadioGroup *DBAdvOfficeRadioGroup1;
	TDBLookupComboBox *DBLookupComboBox1;
	TDBAdvGrid *grid;
	TDBAdvMaskEdit *ebRate;
	TDBAdvGlowNavigator *DBAdvGlowNavigator1;
	void __fastcall grpTypeClick(TObject *Sender);
	void __fastcall gridGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState,
          TBrush *ABrush, TFont *AFont);
	void __fastcall gridGetFloatFormat(TObject *Sender, int ACol, int ARow, bool &IsFloat,
          UnicodeString &FloatFormat);
	void __fastcall tblAfterScroll(TDataSet *DataSet);
private:	// User declarations
	void __fastcall SetupGrid();
	void __fastcall ClearInputs();
	void __fastcall PopulateGroups();
	void __fastcall CreateLookupField();
	void __fastcall SetupFields();
public:		// User declarations
	__fastcall TfrmGroupRates(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmGroupRates *frmGroupRates;
//---------------------------------------------------------------------------
class TDBObject : public TObject
{
	public:
		__fastcall TDBObject(int _id, String _name) { dbID = _id; Name = _name; };

		int dbID;
		String Name;


};
#endif
