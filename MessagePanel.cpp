#include "MessagePanel.h"
#include "stubs/MessagePanelUi.h"
#include "stubs/MessageDialogUi.h"
#include "stubs/SignalDialogUi.h"
#include "stubs/SignalPanelUi.h"
#include "Utils.h"
#ifdef WIN32
#pragma hdrstop
#endif

wxDEFINE_EVENT( wxEVT_CHANGE_SIGNALS, wxCommandEvent );
const wxString strTypeList[] = { "Unsigned Type", "Signed Type", "32Bit Float", "64Bit Double", "String" };
SignalDialog::SignalDialog( wxWindow *parent, wxDBCFile *file, wxWindowID id ) : wxDialog( parent, id, "Signal", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	this->file = file;
	scale = 1.0f;
	offset = 0.0f;
	min = max = 0.0f;
	start_bit = 0;
	bit_len = 8;
	endial = false;

	ui = new Ui::SignalDialog();
	ui->Setup( this );
	ui->name->SetValidator( wxTextValidator( wxFILTER_ASCII, &name ) );
	// ui->name->SetValidator( wxGenericValidator( &name ) );
	ui->start_bit->SetValidator( wxIntegerValidator<int>( &start_bit, 0, 63 ) );
	ui->bit_len->SetValidator( wxIntegerValidator<int>( &bit_len, 0, 63 ) );
	ui->endial->SetValidator( wxBoolValidator( &endial ) );
	ui->type->SetValidator( wxGenericValidator( &type ) );
	ui->scale->SetValidator( wxFloatingPointValidator<float>( 6, &scale ) );
	ui->offset->SetValidator( wxFloatingPointValidator<float>( 6, &offset ) );
	ui->min->SetValidator( wxFloatingPointValidator<float>( 6, &min ) );
	ui->max->SetValidator( wxFloatingPointValidator<float>( 6, &max ) );
	ui->unit->SetValidator( wxTextValidator( wxFILTER_NONE, &unit ) );
	ui->multiplexer->SetValidator( wxTextValidator( wxFILTER_NONE, &multiplexer ) );
	ui->comment->SetValidator( wxTextValidator( wxFILTER_NONE, &comment ) );
	ui->attrs->SetValidator( AttrMapValidator( &attrs, OBJ_TYPE_SIGNAL, file ) );
	ui->values->SetValidator( ValueMapValidator( &values, true ) );
	ui->name->Bind( wxEVT_TEXT, &SignalDialog::OnChangeBitmap, this );
	ui->start_bit->Bind( wxEVT_TEXT, &SignalDialog::OnChangeBitmap, this );
	ui->bit_len->Bind( wxEVT_TEXT, &SignalDialog::OnChangeBitmap, this );
	ui->endial->Bind( wxEVT_CHECKBOX, &SignalDialog::OnChangeBitmap, this );
	ui->type->Bind( wxEVT_CHOICE, &SignalDialog::OnChangeBitmap, this );
	ui->type->Set( 5, strTypeList );
	ui->map->Enable( false );
}

SignalDialog::~SignalDialog() {
	file = nullptr;
}

void SignalDialog::SetSignal( const wxDBCSignal *signal ) {
	name = signal->name;
	start_bit = signal->start_bit;
	bit_len = signal->bit_len;
	endial = ( signal->flags & SF_BIG_ENDIAN ) != 0;
	if( signal->flags & SF_SIGNED_TYPE ) {
		type = 1;
	} else if( signal->flags & SF_FLOAT_EXT_TYPE ) {
		type = 2;
	} else if( signal->flags & SF_DOUBLE_EXT_TYPE ) {
		type = 3;
	} else if( signal->flags & SF_STRING_EXT_TYPE ) {
		type = 4;
	} else {
		type = 0;
	}
	scale = signal->scale;
	offset = signal->offset;
	min = signal->min;
	max = signal->max;
	unit = signal->unit;
	comment = signal->comment;
	multiplexer = signal->multiplexer;
	attrs = signal->attr;
	values = signal->values;
}

int SignalDialog::GetFlags() const {
	int bits[] = { 0, SF_SIGNED_TYPE, SF_FLOAT_EXT_TYPE, SF_DOUBLE_EXT_TYPE, SF_STRING_EXT_TYPE };
	int flags = bits[type];
	if( endial ) {
		flags |= SF_BIG_ENDIAN;
	}
	return flags;
}

void SignalDialog::InitDialog() {
	UpdateBitmap();
	wxDialog::InitDialog();
}

bool SignalDialog::Validate() {
	TransferDataFromWindow();
	if( name.empty() ) {
		wxMessageBox( "Name is empty!", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void SignalDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK && !Validate() ) {
		return;
	}
	return wxDialog::EndModal( retCode );
}

void SignalDialog::OnChangeBitmap( wxCommandEvent &event ) {
	if(!IsShown()) {
		return;
	}
	TransferDataFromWindow();
	UpdateBitmap();
}

void SignalDialog::UpdateBitmap() {
	wxDBCSignal signal( file, name );
	signal.flags = GetFlags();
	signal.start_bit = start_bit;
	signal.bit_len = bit_len;

	uint8_t mask[8] = { 0x0 };
	signal.SetValue( -1, mask, 8 );
	// wxPrintf("-- %d\n", endial);

	ui->map->ClearSignal();
	ui->map->SetBigEndial( endial );
	ui->map->AppendSignal( name, mask );
}

SignalPanel::SignalPanel( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) : wxPanel( parent, id, pos, size, style ) {
	file = nullptr;
	ui = new Ui::SignalPanel();
	ui->Setup( this );
	Bind( wxEVT_TOOL, &SignalPanel::OnAddSignal, this, wxID_ADD_ITEM );
	Bind( wxEVT_TOOL, &SignalPanel::OnRemoveSignal, this, wxID_REM_ITEM );
	Bind( wxEVT_LIST_ITEM_ACTIVATED, &SignalPanel::OnEditSignal, this, wxID_ITEM_LIST );
}

void SignalPanel::SendReport() {
	wxCommandEvent event( wxEVT_CHANGE_SIGNALS, GetId() );
	event.SetEventObject( this );
	ProcessWindowEvent( event );
}

void SignalPanel::SetSignalList( const wxVector<wxDBCSignal *> &copy, wxDBCFile *file ) {
	this->file = file;
	signals.clear();
	for( int i = 0; i < copy.size(); i++ ) {
		wxDBCSignal *signal = copy[i];
		signals.push_back( new wxDBCSignal( signal ) );
	}
	UpdateList();
}

wxDBCSignal *SignalPanel::FindSignal( const wxString &name ) const {
	for( int i = 0; i < signals.size(); i++ ) {
		wxDBCSignal *signal = signals[i];
		if( signal->name == name ) {
			return signal;
		}
	}
	return nullptr;
}

void SignalPanel::UpdateList() {
	ui->list->Freeze();
	ui->list->DeleteAllItems();
	for( wxVector<wxDBCSignal *>::const_iterator iter = signals.begin(); iter != signals.end(); iter++ ) {
		const wxDBCSignal *signal = ( *iter );
		int count = ui->list->GetItemCount();
		long index = ui->list->InsertItem( count, signal->name );
		ui->list->SetItem( index, 1, wxString::Format( "%d:%d", signal->start_bit, signal->bit_len ) );
		ui->list->SetItem( index, 2, wxString::Format( "%.2f * x %c %.2f", signal->scale, signal->offset < 0 ? '-' : '+', fabs( signal->offset ) ) );
		ui->list->SetItem( index, 3, wxString::Format( "%.2f < x < %.2f", signal->min, signal->max ) );
		ui->list->SetItem( index, 4, signal->unit );
		ui->list->SetItem( index, 5, signal->comment );
	}
	ListCtrlAutoColumn( ui->list );
	ui->list->Thaw();
}

wxDBCSignal *SignalPanel::GetSelected() const {
	int index = ui->list->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( index == wxNOT_FOUND ) {
		return nullptr;
	}
	const wxString &name = ui->list->GetItemText( index );
	return FindSignal( name );
}

void SignalPanel::OnAddSignal( wxCommandEvent &event ) {
	SignalDialog dialog( this, file );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	wxString name = dialog.GetSignalName();
	wxDBCSignal *signal = new wxDBCSignal( file, name );
	signal->start_bit = dialog.GetStartBit();
	signal->bit_len = dialog.GetBitLen();
	signal->flags = dialog.GetFlags();
	signal->scale = dialog.GetScale();
	signal->offset = dialog.GetOffset();
	signal->min = dialog.GetMin();
	signal->max = dialog.GetMax();
	signal->multiplexer = dialog.GetMultiplexer();
	signal->unit = dialog.GetUnit();
	signal->comment = dialog.GetComment();
	signal->attr = dialog.GetAttrs();
	signal->values = dialog.GetValues();
	signals.push_back( signal );
	SendReport();
	UpdateList();
}

void SignalPanel::OnEditSignal( wxListEvent &event ) {
	wxDBCSignal *signal = GetSelected();
	if( !signal ) {
		return;
	}
	SignalDialog dialog( this, file );
	dialog.SetSignal( signal );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	signal->name = dialog.GetSignalName();
	signal->start_bit = dialog.GetStartBit();
	signal->bit_len = dialog.GetBitLen();
	signal->flags = dialog.GetFlags();
	signal->scale = dialog.GetScale();
	signal->offset = dialog.GetOffset();
	signal->min = dialog.GetMin();
	signal->max = dialog.GetMax();
	signal->multiplexer = dialog.GetMultiplexer();
	signal->unit = dialog.GetUnit();
	signal->comment = dialog.GetComment();
	signal->attr = dialog.GetAttrs();
	signal->values = dialog.GetValues();
	SendReport();
	UpdateList();
}

void SignalPanel::OnRemoveSignal( wxCommandEvent &event ) {
	wxDBCSignal *signal = GetSelected();
	if( !signal ) {
		return;
	}
	wxString message = wxString::Format( "Are you sure?\nName:%s %s", signal->name, signal->comment );
	if( wxMessageBox( message, "Remove", wxYES_NO | wxICON_EXCLAMATION ) != wxYES ) {
		return;
	}
	wxVector<wxDBCSignal *>::iterator iter = std::find( signals.begin(), signals.end(), signal );
	if( iter != signals.end() ) {
		signals.erase( iter );
	}
	SendReport();
	UpdateList();
}

MessageDialog::MessageDialog( wxWindow *parent, wxDBCFile *file, wxWindowID id ) : wxDialog( parent, id, "Message", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	this->file = file;
	this->canID = "0";
	this->size = 8;
	ui = new Ui::MessageDialog();
	ui->Setup( this );
	// ui->id->SetValidator( wxGenericValidator( &canID ) );
	ui->id->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &canID ) );
	ui->name->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &name ) );
	ui->size->SetValidator( wxGenericValidator( &size ) );
	ui->sender->SetValidator( wxGenericValidator( &sender ) );
	ui->comment->SetValidator( wxTextValidator( wxFILTER_NONE, &comment ) );
	ui->attrs->SetValidator( AttrMapValidator( &attrs, OBJ_TYPE_MESSAGE, file ) );
	ui->transmitters->SetValidator( NodeListValidator( &transmitters, file ) );
	ui->signals->Bind( wxEVT_CHANGE_SIGNALS, &MessageDialog::OnUpdateSignals, this );
	ui->id->SetFocus();
}

MessageDialog::~MessageDialog() {
	file = nullptr;
}

int MessageDialog::GetCanID() const {
	uint32_t id;
	if( canID.ToUInt( &id, 16 ) ) {
		return id;
	}
	return 0;
}

void MessageDialog::SetMessage( int canID, const wxString &name, int size, const wxString &sender ) {
	this->canID = wxString::Format( "%X", canID );
	this->name = name;
	this->size = size;
	this->sender = sender;
}

void MessageDialog::SetMessage( wxDBCMessage *message ) {
	canID = wxString::Format( "%X", message->CanID() );
	name = message->name;
	size = message->size;
	sender = message->sender;
	comment = message->comment;
	attrs = message->attr;
	transmitters = message->transmitter;
	signals = message->signals;
	UpdateSignalMap();
}

void MessageDialog::OnUpdateSignals( wxCommandEvent &event ) {
	TransferDataFromWindow();
	UpdateSignalMap();
}

void MessageDialog::UpdateSignalMap() {
	ui->maps->ClearSignal();
	for( int i = 0; i < signals.size(); i++ ) {
		wxDBCSignal *signal = signals.at( i );

		uint8_t mask[8] = { 0x0 };
		signal->SetValue( -1, mask, size );
		ui->maps->SetBigEndial( signal->flags & SF_BIG_ENDIAN );
		ui->maps->AppendSignal( signal->name, mask );
	}
}

void MessageDialog::FillSenderNames() {
	wxArrayString nodes;
	for( int i = 0; i < file->NodeCount(); i++ ) {
		const wxString &name = file->GetNode( i )->GetName();
		nodes.push_back( name );
	}
	ui->sender->Set( nodes );
}

void MessageDialog::InitDialog() {
	FillSenderNames();
	TransferDataToWindow();
	wxDialog::InitDialog();
}

bool MessageDialog::TransferDataFromWindow() {
	signals = ui->signals->GetSignalList();
	return wxDialog::TransferDataFromWindow();
}

bool MessageDialog::TransferDataToWindow() {
	ui->signals->SetSignalList( signals, file );
	return wxDialog::TransferDataToWindow();
}

bool MessageDialog::Validate() {
	TransferDataFromWindow();
	if( name.empty() ) {
		wxMessageBox( "Name is empty!", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void MessageDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK && !Validate() ) {
		return;
	}
	return wxDialog::EndModal( retCode );
}

MessagePanel::MessagePanel( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) : wxPanel( parent, id, pos, size, style ) {
	file = nullptr;
	ui = new Ui::MessagePanel();
	ui->Setup( this );
	Bind( wxEVT_TOOL, &MessagePanel::OnAddMessage, this, wxID_ADD_ITEM );
	Bind( wxEVT_TOOL, &MessagePanel::OnRemoveMessage, this, wxID_REM_ITEM );
	Bind( wxEVT_LIST_ITEM_ACTIVATED, &MessagePanel::OnEditMessage, this, wxID_ITEM_LIST );
}

void MessagePanel::SetMessages( wxDBCFile *file ) {
	this->file = file;
	messages.clear();
	for( int i = 0; i < file->NumMessage(); i++ ) {
		wxDBCMessage *message = file->GetMessage( i );
		messages.push_back( new wxDBCMessage( message ) );
	}
	UpdateList();
}

wxDBCMessage *MessagePanel::FindId( uint32_t id ) const {
	for( int i = 0; i < messages.size(); i++ ) {
		wxDBCMessage *message = messages[i];
		if( message->ID == id ) {
			return message;
		}
	}
	return nullptr;
}

void MessagePanel::UpdateList() {
	ui->list->Freeze();
	ui->list->DeleteAllItems();
	for( int i = 0; i < messages.size(); i++ ) {
		const wxDBCMessage *message = messages.at( i );
		long index = ui->list->InsertItem( i, wxString::Format( "%X", message->ID ) );
		ui->list->SetItem( index, 1, message->name );
		ui->list->SetItem( index, 2, wxString::Format( "%d", message->size ) );
		ui->list->SetItem( index, 3, message->sender );
		ui->list->SetItem( index, 4, message->comment );
	}
	ListCtrlAutoColumn( ui->list );
	ui->list->Thaw();
}

wxDBCMessage *MessagePanel::GetSelected() {
	int index = ui->list->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( index == -1 ) {
		return nullptr;
	}
	uint32_t canID;
	wxString str = ui->list->GetItemText( index, 0 );
	if( !str.ToUInt( &canID, 16 ) ) {
		return nullptr;
	}
	return FindId( canID );
}

void MessagePanel::OnAddMessage( wxCommandEvent &event ) {
	MessageDialog dialog( this, file );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	int canID = dialog.GetCanID();
	wxDBCMessage *message = FindId( canID );
	if( message ) {
		wxMessageBox( "Already CanID", "Warning", wxOK, this );
		return;
	}
	int size = dialog.GetMessageSize();
	const wxString &name = dialog.GetMessageName();
	const wxString &sender = dialog.GetSender();
	message = new wxDBCMessage( file, canID, name, size, sender );
	message->comment = dialog.GetComment();
	message->attr = dialog.GetAttrs();
	message->transmitter = dialog.GetTransmitters();
	message->signals = dialog.GetSignals();
	UpdateList();
}

void MessagePanel::OnEditMessage( wxListEvent &event ) {
	wxDBCMessage *message = GetSelected();
	if( !message ) {
		return;
	}
	MessageDialog dialog( this, file );
	dialog.SetMessage( message );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	message->ID = dialog.GetCanID();
	message->name = dialog.GetMessageName();
	message->size = dialog.GetMessageSize();
	message->sender = dialog.GetSender();
	message->comment = dialog.GetComment();
	message->attr = dialog.GetAttrs();
	message->transmitter = dialog.GetTransmitters();
	message->signals = dialog.GetSignals();
	UpdateList();
}

void MessagePanel::OnRemoveMessage( wxCommandEvent &event ) {
	wxDBCMessage *message = GetSelected();
	if( !message ) {
		return;
	}
	wxString crete = wxString::Format( "Are you sure?\nName:%s (%x) %s", message->name, message->ID, message->comment );
	if( wxMessageBox( crete, "Remove", wxYES_NO | wxICON_EXCLAMATION ) != wxYES ) {
		return;
	}
	wxVector<wxDBCMessage *>::iterator iter = std::find( messages.begin(), messages.end(), message );
	if( iter != messages.end() ) {
		messages.erase( iter );
	}
	UpdateList();
}
