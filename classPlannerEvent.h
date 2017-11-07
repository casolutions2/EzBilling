//---------------------------------------------------------------------------

#ifndef classPlannerEventH
#define classPlannerEventH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "DBPlannerMonthView.hpp"
#include "PlanItemEdit.hpp"

class clsPlannerEvent
{
	public:
		__fastcall clsPlannerEvent(TDBPlanner* plnr);
		__fastcall clsPlannerEvent(TDBPlanner* plnr, const String& caption, const String& message);
		
		__fastcall ~clsPlannerEvent();
		void 	__fastcall WriteEvent(const String& caption, const String& msg);

	private:
		TDBPlanner* 	_planner;

};
#endif
