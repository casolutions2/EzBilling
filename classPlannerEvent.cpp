//---------------------------------------------------------------------------
#include <vcl.h>
#include "CodeSiteLogging.hpp"
#pragma hdrstop

#include "classPlannerEvent.h"
#include "UFormMain.h"		// main form with planner


//---------------------------------------------------------------------------
#pragma package(smart_init)
//Caption colors
const TColor 	clrGeneric 		= TColor(0xff00cc);
const TColor	clrGenericTo 	= TColor(0xff00ff);
const TColor 	clrAutoAdd 		= TColor(0xCFF0EA);
const TColor 	clrAutoAddTo 	= TColor(0x8CC0B1);

//---------------------------------------------------------------------------
__fastcall clsPlannerEvent::clsPlannerEvent(TDBPlanner* plnr)
{
	if ( ! plnr )
		throw Exception(L"No planner passed in" );
	_planner = plnr;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall clsPlannerEvent::clsPlannerEvent(TDBPlanner* plnr, const String& caption, const String& message)
{
	if ( ! plnr )
		throw Exception(L"No planner passed in" );
	_planner = plnr;
	WriteEvent(caption, message);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall clsPlannerEvent::~clsPlannerEvent()
{

}
//---------------------------------------------------------------------------
void __fastcall clsPlannerEvent::WriteEvent( const String& caption, const String& msg)
{
	TPlannerItem* pi = _planner->CreateItem();
	pi->ItemStartTime = Now()-(5/60/24);
	pi->ItemEndTime = Now();
	pi->CaptionText = caption;
	pi->CaptionType = Planner::ctText;
	pi->Text->Text = msg;
	pi->ItemPos = 0;
	pi->Tag = 0;
	pi->CaptionBkg = clrAutoAdd;
	pi->CaptionBkgTo = clrAutoAddTo;
	pi->Update();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
