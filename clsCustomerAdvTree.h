//---------------------------------------------------------------------------

#ifndef clsCustomerAdvTreeH
#define clsCustomerAdvTreeH
//---------------------------------------------------------------------------
#include "AdvCustomTreeView.hpp"
#include "AdvTreeView.hpp"
#include "AdvTreeViewBase.hpp"
#include "AdvTreeViewData.hpp"

//---------------------------------------------------------------------------
/* This class will hold the events and functions necessary to view
	the items in the AdvTreeView for Customers
*/
class clsCustomerAdvTree
{
	public:
		__fastcall clsCustomerAdvTree( TAdvTreeView* tv );
		__fastcall clsCustomerAdvTree( TAdvTreeView* tv, bool onlyActive );

		__fastcall ~clsCustomerAdvTree();

		int 	__fastcall	PopulateTree();

	private:
		TAdvTreeView*		_atv;
		bool                _activeOnly;


};
#endif
