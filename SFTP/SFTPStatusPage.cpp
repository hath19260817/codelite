#include "SFTPStatusPage.h"
#include "sftp_worker_thread.h"
#include <wx/menu.h>

SFTPStatusPage::SFTPStatusPage(wxWindow* parent)
    : SFTPStatusPageBase(parent)
{
}

SFTPStatusPage::~SFTPStatusPage()
{
}

void SFTPStatusPage::OnContentMenu(wxDataViewEvent& event)
{
    wxUnusedVar(event);
    CallAfter( &SFTPStatusPage::ShowContextMenu );
}

void SFTPStatusPage::AddLine(SFTPThreadMessage* message)
{
    wxVector<wxVariant> cols;
    cols.push_back( wxDateTime::Now().FormatISOTime() );
    wxBitmap bmp;
    switch (message->GetStatus()) {
    case SFTPThreadMessage::STATUS_ERROR:
        bmp = m_bitmaps.Bitmap("sftp_error");
        break;
    case SFTPThreadMessage::STATUS_OK:
        bmp = m_bitmaps.Bitmap("sftp_ok");
        break;
    default:
        bmp = m_bitmaps.Bitmap("sftp_info");
        break;
    }
    
    wxVariant varBmp;
    varBmp << bmp;
    cols.push_back( varBmp );
    cols.push_back( message->GetAccount());
    cols.push_back( message->GetMessage() );
    m_dvListCtrl->AppendItem( cols );
    wxDELETE(message);
    
    // Ensure that the last item is visible
    wxDataViewItem item = m_dvListCtrl->RowToItem(m_dvListCtrl->GetItemCount()-1);
    m_dvListCtrl->Select( item );
    m_dvListCtrl->EnsureVisible( item );
}

void SFTPStatusPage::ShowContextMenu()
{
    wxMenu menu;
    menu.Append(wxID_CLEAR, _("Clear Log"));
    menu.Enable(wxID_CLEAR, m_dvListCtrl->GetItemCount());
    menu.Connect(wxID_CLEAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SFTPStatusPage::OnClearLog), NULL, this);
    m_dvListCtrl->PopupMenu( &menu );
}

void SFTPStatusPage::OnClearLog(wxCommandEvent& event)
{
    wxUnusedVar(event);
    m_dvListCtrl->DeleteAllItems();
}
