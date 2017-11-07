//---------------------------------------------------------------------------

#ifndef ThreadSyncDialogH
#define ThreadSyncDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TthreadSyncDialog : public TThread
{
protected:
	void __fastcall Execute();
	void __fastcall	OnTerminate();
public:
	__fastcall TthreadSyncDialog(bool CreateSuspended);

	String dlginstruction;
};
//---------------------------------------------------------------------------
#endif
