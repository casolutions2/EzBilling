//---------------------------------------------------------------------------

#pragma hdrstop

#include "CodeSiteLogging.hpp"
#include "clsCustomerTree.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall clsCustomerTree::clsCustomerTree( TAdvTreeView* tv, bool ActiveOnly )
{
	_tv 		= tv;
	#if defined (__clang__)
	_con 		= nullptr;
	#else
	_con        = NULL;
	#endif
	_activeOnly = ActiveOnly;
	SetupTree();
	SetupAlphaParents();
}
//---------------------------------------------------------------------------
__fastcall clsCustomerTree::clsCustomerTree( TAdvTreeView* tv, TUniConnection* con, bool ActiveOnly )
{
	_tv 		= tv;
	_con 		= con;
	_activeOnly = ActiveOnly;
	SetupTree();
	SetupAlphaParents();
}
//---------------------------------------------------------------------------
__fastcall clsCustomerTree::~clsCustomerTree()
{
	#if defined( __clang__)
	_tv 	= nullptr;
	_con 	= nullptr;
	#else
	_tv     = NULL;
	_con    = NULL;
	#endif
}
//---------------------------------------------------------------------------
int __fastcall clsCustomerTree::Populate( )
{
	if ( ! _con )
		return -1;
	try
	{	if ( ! _con->Connected )
			_con->Open();
		if ( ! _con->Connected )
			return -1;
		return RefreshData();
	}
	catch( Exception *ex )
	{
		return -1;
	}
}
//---------------------------------------------------------------------------
int __fastcall clsCustomerTree::Populate( TUniConnection* con )
{
	_con = con;
	return Populate();
}
//---------------------------------------------------------------------------
int __fastcall clsCustomerTree::RefreshData()
{
	return 1;
}
//---------------------------------------------------------------------------
void __fastcall clsCustomerTree::SetupTree()
{
	if ( ! _tv )
		return;
	_tv->Nodes->BeginUpdate();
	_tv->ClearNodes();
	_tv->Columns->Clear();
	_tv->Columns->Add()->Text = "Customer Name";
	_tv->Nodes->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall clsCustomerTree::SetupAlphaParents()
{
	if ( ! _tv )
		return;

	_tv->Nodes->BeginUpdate();
	TAdvTreeViewNode* p = _tv->AddNode();
	p->Text[0] = "0-9";
	_tv->AddNode( p );
	p->Extended = true;
	for (int i = 65; i < 91; i++) {
		p = _tv->AddNode();
		p->Text[0] = String( char(i) );
		p->Extended = true;
		_tv->AddNode(p);
		//FillCustomerBranch( p, i );
	}
	_tv->Nodes->EndUpdate();
}
//---------------------------------------------------------------------------
int __fastcall clsCustomerTree::PopulateBranch( TAdvTreeViewNode* ANode )
{
	ANode->Nodes->Clear();
	if ( ! _con )
		return 0;
	int rtnval = 0;
	String sql("");
	TUniQuery* qry = new TUniQuery(NULL);
	try
	{	// get child nodes
		sql = "SELECT CustomerID, DisplayName FROM vwActiveCustomers WHERE DisplayName REGEXP '";
		sql += "[" + ANode->Text[0] + "].*';";
		qry->Connection = _con;
		qry->SQL->Text = sql;
		qry->Open();
		while ( ! qry->Eof )
		{   TAdvTreeViewNode* child = _tv->AddNode( ANode );
			child->Text[0] = qry->Fields->Fields[1]->AsString;
			child->DBKey = qry->Fields->Fields[0]->AsInteger;
			qry->Next();
		}
		rtnval = qry->RecordCount;
		qry->Close();

	}
	catch( Exception *ex )
	{
		CodeSite->SendException ( ex );
		rtnval = -1;
	}
	qry->Close();
	delete qry;
	#if defined (__clang__)
	qry = nullptr;
	#else
	qry = NULL;
	#endif

	return rtnval;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

