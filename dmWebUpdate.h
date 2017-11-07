//---------------------------------------------------------------------------

#ifndef dmWebUpdateH
#define dmWebUpdateH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "WUpdate.hpp"
#include "WUpdateLanguagesU.hpp"
#include "WUpdateWiz.hpp"
//---------------------------------------------------------------------------
class TmodWebUpdate : public TDataModule
{
__published:	// IDE-managed Components
	TWebUpdate *WebUpdate1;
	TWebUpdateWizardEnglish *WebUpdateWizardEnglish1;
	TWebUpdateWizard *WebUpdateWizard1;
private:	// User declarations
public:		// User declarations
	__fastcall TmodWebUpdate(TComponent* Owner);
    bool    __fastcall checkForUpdates();
};
//---------------------------------------------------------------------------
extern PACKAGE TmodWebUpdate *modWebUpdate;
//---------------------------------------------------------------------------
#endif
