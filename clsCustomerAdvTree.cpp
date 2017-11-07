//---------------------------------------------------------------------------

#pragma hdrstop
#include "CodeSiteLogging.hpp"

#include "clsCustomerAdvTree.h"
#include "SQLStatements.h"				// SQL statement file for both MYOB and SQLite
#include "uDMUniDac.h"					// data module for UniDac connection to SQLite

//---------------------------------------------------------------------------
#pragma link "AdvTreeView"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma region Construction/Destruction
//---------------------------------------------------------------------------
__fastcall clsCustomerAdvTree::clsCustomerAdvTree( TAdvTreeView* tv )
{
	if ( ! tv )
		throw Exception( "No component passed to constructor " );
	else
		_atv = tv;
}
//---------------------------------------------------------------------------
__fastcall clsCustomerAdvTree::clsCustomerAdvTree( TAdvTreeView* tv, bool onlyActive )
{
	if ( ! tv )
		throw Exception( "No component passed to constructor " );
	else
	{	_atv = tv;
		_activeOnly = onlyActive;
	}
}
//---------------------------------------------------------------------------
__fastcall clsCustomerAdvTree::~clsCustomerAdvTree()
{
	_atv = NULL;
}
//---------------------------------------------------------------------------
#pragma end_region

int __fastcall clsCustomerAdvTree::PopulateTree()
{
	int rtnval(0);
	try
	{	_atv->Nodes->BeginUpdate();
		_atv->ClearNodes();
		_atv->Nodes->EndUpdate();

		rtnval = 1;
	}
	catch( Exception *ex )
	{
		CodeSite->SendException( ex );
		rtnval = -1;
	}
	return rtnval;
}

//---------------------------------------------------------------------------


