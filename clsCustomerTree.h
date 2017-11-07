//---------------------------------------------------------------------------

#ifndef clsCustomerTreeH
#define clsCustomerTreeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.DB.hpp>
#include "AdvTreeView.hpp"
#include "AdvTreeViewBase.hpp"
#include "AdvTreeViewData.hpp"
#include "Uni.hpp"
#include "UniProvider.hpp"
#include <Data.DB.hpp>
#include "DBAccess.hpp"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
class clsCustomerTree
{
	public:
		// constructors, destructors and operators
		__fastcall clsCustomerTree( TAdvTreeView* Atv, bool ActiveOnly = true );
		__fastcall clsCustomerTree( TAdvTreeView* Atv, TUniConnection* Acon, bool ActiveOnly = true );

		__fastcall ~clsCustomerTree();

		// properties
		bool    __property ActiveOnly   = {read=_activeOnly, write=_activeOnly };

		// functions
		int     __fastcall Populate();
		int 	__fastcall Populate( TUniConnection* con );
		int     __fastcall RefreshData();
		int     __fastcall PopulateBranch( TAdvTreeViewNode* parent );




	private:
		// variables
		TAdvTreeView*       _tv;
		TUniConnection*     _con;
		bool                _activeOnly;

		// functions
		void    __fastcall  SetupTree();
		void    __fastcall  SetupAlphaParents();
};

//---------------------------------------------------------------------------
#endif
