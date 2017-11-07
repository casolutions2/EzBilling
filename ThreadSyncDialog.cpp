//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ThreadSyncDialog.h"
#include "dlgSyncingUnit.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TthreadSyncDialog::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
TdlgSync* dlg = NULL;

__fastcall TthreadSyncDialog::TthreadSyncDialog(bool CreateSuspended)
	: TThread(CreateSuspended)
{

}
//---------------------------------------------------------------------------
void __fastcall TthreadSyncDialog::Execute()
{
	NameThreadForDebugging(System::String(L"CASQ_threadSyncDialog"));
	//---- Place thread code here ----
    dlg = new TdlgSync(NULL);
	dlg->UpdateInstruction(dlginstruction);
	dlg->Show();
}
//---------------------------------------------------------------------------
void __fastcall TthreadSyncDialog::OnTerminate()
{
	dlg->Close();
	delete dlg;
}