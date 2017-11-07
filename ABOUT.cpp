//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"
//--------------------------------------------------------------------- 
#pragma link "AdvOfficeImage"
#pragma link "ExeInfo"
#pragma link "HTMLabel"
#pragma resource "*.dfm"
TAboutBox *AboutBox;
//--------------------------------------------------------------------- 
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{

}
//---------------------------------------------------------------------
void __fastcall TAboutBox::FormActivate(TObject *Sender)
{
	// center controls
	this->ProductName->Left = (this->ClientWidth / 2 ) - ( ProductName->Width / 2 );
	this->Version->Caption = ExeInfo1->ProductVersion;
	this->Version->AutoSize = true;
	this->Version->Left = ( this->ClientWidth / 2 ) - ( Version->Width / 2 );
	this->OKButton->Left = ( this->ClientWidth / 2 ) - ( OKButton->Width / 2 );
}
//---------------------------------------------------------------------------


