//---------------------------------------------------------------------------

#ifndef dmReportingH
#define dmReportingH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

#include "uDMUniDac.h"
#include <Data.DB.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "Uni.hpp"
#include "frxDBSet.hpp"
#include "frxExportPDF.hpp"
#include "frxClass.hpp"

//---------------------------------------------------------------------------
class TrptModule : public TDataModule
{
__published:	// IDE-managed Components
	TUniQuery *qryRunSheet;
	TUniQuery *qryPriceAdjustHistory;
	TfrxDBDataset *frxDBDataset1;
	TfrxPDFExport *frxPDFExport1;
	TfrxReport *frxRunSheet;
	TfrxReport *frxPricingHistory;
	TfrxDBDataset *frxDBDataset2;
	TfrxReport *frxReport1;
private:	// User declarations
public:		// User declarations
	__fastcall TrptModule(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TrptModule *rptModule;
//---------------------------------------------------------------------------
#endif
