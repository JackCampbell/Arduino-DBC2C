#include "NodePanel.h"
#include "stubs/NodePanelUi.h"
#include "stubs/NodeDialogUi.h"
#include "stubs/NodeSelectDialogUi.h"
#include "stubs/NodeListPanelUi.h"
#include "Utils.h"
#ifdef _WIN32
#pragma hdrstop
#endif

NodeListValidator::NodeListValidator( wxArrayString *nodes, const wxDBCFile *file ) : wxValidator() {
	this->nodes = nodes;
	this->file = file;
}

NodeListValidator::~NodeListValidator() {
	file = nullptr;
	nodes = nullptr;
}

bool NodeListValidator::Validate( wxWindow *parent ) {
	return true;
}

bool NodeListValidator::TransferFromWindow() {
	try {
		NodeListPanel *panel = dynamic_cast<NodeListPanel *>( m_validatorWindow );
		*nodes = panel->GetNodeList();
	} catch( ... ) {
		wxFAIL_MSG( _T("NodeListValidator works only with NodeListPanel") );
		return false;
	}
	return true;
}

bool NodeListValidator::TransferToWindow() {
	try {
		NodeListPanel *panel = dynamic_cast<NodeListPanel *>( m_validatorWindow );
		panel->SetNodeList( *nodes, file );
	} catch( ... ) {
		wxFAIL_MSG( _T("NodeListValidator works only with NodeListPanel") );
		return false;
	}
	return true;
}






NodeSelectDialog::NodeSelectDialog( wxWindow *parent, const wxDBCFile *file, const wxArrayString &names, wxWindowID id ) : wxDialog( parent, id, "Select Node", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	ui = new Ui::NodeSelectDialog();
	ui->Setup( this );
	ui->node->Set( names );
	ui->node->SetValidator( wxGenericValidator( &name ) );
	this->file = file;
}

void NodeSelectDialog::OnChangeNode( wxCommandEvent &event ) {
	wxString name = ui->node->GetStringSelection();
	wxString comment = wxEmptyString;
	wxDBCNode *node = file->FindNode( name );
	if( node ) {
		comment = node->GetComment();
	}
	ui->comment->SetLabel( comment );
}

void NodeSelectDialog::InitDialog() {
	TransferDataToWindow();
	wxDialog::InitDialog();
}

bool NodeSelectDialog::Validate() {
	TransferDataFromWindow();
	if( name.empty() ) {
		wxMessageBox( "Node name is empty!", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void NodeSelectDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK && !Validate() ) {
		return;
	}
	wxDialog::EndModal( retCode );
}











NodeListPanel::NodeListPanel( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) : wxPanel( parent, id, pos, size, style ) {
	file = nullptr;
	ui = new Ui::NodeListPanel();
	ui->Setup( this );
	Bind( wxEVT_TOOL, &NodeListPanel::OnAddNode, this, wxID_ADD_ITEM );
	Bind( wxEVT_TOOL, &NodeListPanel::OnRemoveNode, this, wxID_REM_ITEM );
}

NodeListPanel::~NodeListPanel() {
	file = nullptr;
}

void NodeListPanel::SetNodeList( wxArrayString &nodes, const wxDBCFile *file ) {
	this->file = file;
	this->nodes = nodes;
	UpdateList();
}

void NodeListPanel::UpdateList() {
	ui->list->Freeze();
	ui->list->DeleteAllItems();
	for( int i = 0; i < nodes.size(); i++ ) {
		const wxDBCNode *node = file->FindNode( nodes[i] );
		if( !node ) {
			continue;
		}
		int count = ui->list->GetItemCount();
		long index = ui->list->InsertItem( count, node->GetName() );
		ui->list->SetItem( index, 1, node->GetComment() );
	}
	ListCtrlAutoColumn(ui->list);
	ui->list->Thaw();
}

wxArrayString NodeListPanel::GetNodeNames() const {
	wxArrayString names;
	for( int i = 0; i < file->NodeCount(); i++ ) {
		const wxString &name = file->GetNode( i )->GetName();
		if( nodes.Index( name ) == wxNOT_FOUND ) {
			names.push_back( name );
		}
	}
	return names;
}

void NodeListPanel::OnAddNode( wxCommandEvent &event ) {
	wxArrayString names = GetNodeNames();
	NodeSelectDialog dialog( this, file, names );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	wxString name = dialog.GetNodeName();
	if( nodes.Index( name ) == wxNOT_FOUND ) {
		nodes.push_back( name );
	}
	UpdateList();
}

void NodeListPanel::OnRemoveNode( wxCommandEvent &event ) {
	int index = ui->list->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( index == -1 ) {
		return;
	}
	const wxString name = ui->list->GetItemText( index );
	const wxDBCNode *node = file->FindNode( name );
	if( !node ) {
		return;
	}
	wxString message = wxString::Format( "Are you sure?\n%s [%s]\n", name, node->GetComment() );
	if( wxMessageBox( message, "Remove", wxYES_NO | wxICON_EXCLAMATION, this ) != wxYES ) {
		return;
	}
	nodes.Remove( name );
	UpdateList();
}









NodeDialog::NodeDialog( wxWindow *parent, const wxDBCFile *file, wxWindowID id ) : wxDialog( parent, id, "Node", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	this->file = file;
	ui = new Ui::NodeDialog();
	ui->Setup( this );
	ui->name->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &name ) );
	ui->comment->SetValidator( wxTextValidator( wxFILTER_NONE, &comment ) );
	ui->attrs->SetValidator( AttrMapValidator( &attrs, OBJ_TYPE_NODE, file ) );
	ui->name->SetFocus();
}

NodeDialog::~NodeDialog() {
	file = nullptr;
}

void NodeDialog::InitDialog() {
	TransferDataToWindow();
	wxDialog::InitDialog();
}

bool NodeDialog::Validate() {
	TransferDataFromWindow();
	if( name.empty() ) {
		wxMessageBox( "Name is empty", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void NodeDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK && !Validate() ) {
		return;
	}
	wxDialog::EndModal( retCode );
}








NodePanel::NodePanel( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) : wxPanel( parent, id, pos, size, style ) {
	this->file = nullptr;
	ui = new Ui::NodePanel();
	ui->Setup( this );
	Bind( wxEVT_TOOL, &NodePanel::OnAddNode, this, wxID_ADD_ITEM );
	Bind( wxEVT_TOOL, &NodePanel::OnRemoveNode, this, wxID_REM_ITEM );
	Bind( wxEVT_LIST_ITEM_ACTIVATED, &NodePanel::OnEditNode, this, wxID_ITEM_LIST );
}

NodePanel::~NodePanel() {
	file = nullptr;
}

void NodePanel::SetNodes(wxDBCFile *file) {
	this->file = file;
	nodes.clear();
	for(int i = 0; i < file->NodeCount(); i++) {
		wxDBCNode *node = file->GetNode( i );
		nodes.push_back( new wxDBCNode( node ) );
	}
	UpdateList();
}

wxDBCNode *NodePanel::FindName(const wxString &name) const {
	for(int i = 0; i < nodes.size(); i++) {
		wxDBCNode *node = nodes[i];
		if(node->name == name) {
			return node;
		}
	}
	return nullptr;
}

void NodePanel::UpdateList() {
	ui->list->Freeze();
	ui->list->DeleteAllItems();
	for( int i = 0; i < nodes.size(); i++ ) {
		const wxDBCNode *node = nodes.at( i );
		long index = ui->list->InsertItem( i, node->GetName() );
		ui->list->SetItem( index, 1, node->GetComment() );
	}
	ui->list->SetColumnWidth( 0, 150 );
	ui->list->SetColumnWidth( 1, 350 );
	ui->list->Thaw();
}

wxDBCNode *NodePanel::GetSelected() const {
	int index = ui->list->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( index == -1 ) {
		return nullptr;
	}
	wxString name = ui->list->GetItemText( index, 0 );
	return FindName(name);
}

void NodePanel::OnAddNode( wxCommandEvent &event ) {
	NodeDialog dialog( this, file );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	const wxString &name = dialog.GetNodeName();
	wxDBCNode *node = FindName( name );
	if(node) {
		wxMessageBox( "Already node name\n", "Warning", wxOK, this );
		return;
	}
	node = new wxDBCNode(file, name);
	node->attr = dialog.GetAttrs();
	node->comment = dialog.GetComment();
	nodes.push_back(node);
	UpdateList();
}

void NodePanel::OnEditNode( wxListEvent &event ) {
	wxDBCNode *selected = GetSelected();
	if( !selected ) {
		return;
	}
	NodeDialog dialog( this, file );
	dialog.SetName( selected->name );
	dialog.SetAttrs( selected->attr );
	dialog.SetComment( selected->comment );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	selected->name = dialog.GetNodeName();
	selected->attr = dialog.GetAttrs();
	selected->comment = dialog.GetComment();
	UpdateList();
}

void NodePanel::OnRemoveNode( wxCommandEvent &event ) {
	wxDBCNode *selected = GetSelected();
	if( !selected ) {
		return;
	}
	const wxString &name = selected->GetName();
	wxString message = wxString::Format( "Are you sure?\n%s [%s]\n", name, selected->GetComment() );
	if( wxMessageBox( message, "Remove", wxYES_NO | wxICON_EXCLAMATION, this ) != wxYES ) {
		return;
	}
	wxVector<wxDBCNode *>::iterator iter = std::find(nodes.begin(), nodes.end(), selected);
	if(iter != nodes.end()) {
		nodes.erase(iter);
	}
	UpdateList();
}






