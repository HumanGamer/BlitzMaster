#pragma once

#include "FunctionCall.h"


class WatchCtrl : public wxPanel  
{
public:

	WatchCtrl( wxWindow* parent, bool EditableRow = false );
	virtual ~WatchCtrl();

   void SetFunctionCall( FunctionCall* Call );
	void UpdateWatch();

	void AddVar( const wxString& Name );
	void DeleteAt( int Row );

	void ClearAll();

protected:

	void _ToggleExpanded( int Row );

	void _AddVar( WatchVar* Var, bool EditableVar = false );
	void _AddVars( const WatchVarArray& Vars );

	int _InsertVars( int Row, const WatchVarArray& Vars );

	friend class HeaderCtrl;
	friend class ListCtrl;

	bool			m_EditableRow;
	bool			m_ShowMetatables;

	HeaderCtrl*		m_HeaderCtrl;
	ListCtrl*		m_ListCtrl;
	wxBoxSizer*		m_Sizer; 

	wxArrayInt			m_Indent;
	WatchVarArray		m_Vars;

	FunctionCall*	m_Call;

	DECLARE_EVENT_TABLE()
};
