///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/wince/checklst.h
// Purpose:     wxCheckListBox class - a listbox with checkable items
// Author:      Wlodzimierz ABX Skiba
// Modified by:
// Created:     30.10.2005
// RCS-ID:      $Id: checklst.h,v 1.2.2.1 2006/02/24 16:04:43 JS Exp $
// Copyright:   (c) Wlodzimierz Skiba
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef   __CHECKLSTCE__H_
#define   __CHECKLSTCE__H_

class WXDLLEXPORT wxCheckListBox : public wxCheckListBoxBase
{
public:
    // ctors
    wxCheckListBox();
    wxCheckListBox(wxWindow *parent, wxWindowID id,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   int nStrings = 0,
                   const wxString choices[] = NULL,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxListBoxNameStr);
    wxCheckListBox(wxWindow *parent, wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& size,
                   const wxArrayString& choices,
                   long style = 0,
                   const wxValidator& validator = wxDefaultValidator,
                   const wxString& name = wxListBoxNameStr);
    virtual ~wxCheckListBox();

    bool Create(wxWindow *parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n = 0, const wxString choices[] = NULL,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxListBoxNameStr);
    bool Create(wxWindow *parent, wxWindowID id,
                const wxPoint& pos,
                const wxSize& size,
                const wxArrayString& choices,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxListBoxNameStr);

    // override base class virtuals
    virtual void Delete(int n);

    // items may be checked
    virtual bool IsChecked(size_t uiIndex) const;
    virtual void Check(size_t uiIndex, bool bCheck = true);

    // public interface derived from wxListBox and lower classes
    virtual void Clear();
    virtual int GetCount() const;
    virtual int GetSelection() const;
    virtual int GetSelections(wxArrayInt& aSelections) const;
    virtual wxString GetString(int n) const;
    virtual bool IsSelected(int n) const;
    virtual void SetString(int n, const wxString& s);

    // Implementation
    virtual bool MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);
protected:

    void OnSize(wxSizeEvent& event);

    // protected interface derived from wxListBox and lower classes
    virtual int DoAppend(const wxString& item);
    virtual void* DoGetItemClientData(int n) const;
    virtual wxClientData* DoGetItemClientObject(int n) const;
    virtual void DoInsertItems(const wxArrayString& items, int pos);
    virtual void DoSetFirstItem(int n);
    virtual void DoSetItemClientData(int n, void* clientData);
    virtual void DoSetItemClientObject(int n, wxClientData* clientData);
    virtual void DoSetItems(const wxArrayString& items, void **clientData);
    virtual void DoSetSelection(int n, bool select);
    // convert our styles to Windows
    virtual WXDWORD MSWGetStyle(long style, WXDWORD *exstyle) const;

private:

    wxArrayPtrVoid m_itemsClientData;

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS_NO_COPY(wxCheckListBox)
};

#endif    //_CHECKLSTCE_H
