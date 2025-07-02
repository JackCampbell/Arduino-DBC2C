#include "GenSignal.h"
#include "MarineApp.h"
#include "DBCFileDialog.h"
#include "Utils.h"
#include "SignalSelector.h"
#include "stubs/GenSignalDialogUi.h"
#include "stubs/GenDeviceDialogUi.h"
#include "stubs/GenChannelDialogUi.h"
#include "stubs/GenPreviewDialogUi.h"
#include "FileSelectDialog.h"
#include <map>
#ifdef WIN32
#pragma hdrstop
#endif

void GetProtocolNames( wxArrayString &protocols ) {
	protocols.push_back( "None" );
	protocols.push_back( "NMEA2000" );
	protocols.push_back( "Holley" );
}

wxString SigPath::GetPath() const {
	return wxString::Format( "CH%d:%s:%03X:%s", channel, file->GetName(), message->ID, signal->name );
}

bool SigPath::SetPath( const wxString &sigpath ) {
	wxArrayString list = wxSplit( sigpath, ':', 0 );
	if( list.size() != 4 ) {
		return false;
	}
	if( !list[0].substr( 2 ).ToInt( &channel, 10 ) ) {
		return false;
	}
	file = FindDBCFile( list[1] );
	if( !file ) {
		return false;
	}
	uint32_t messageID;
	if( !list[2].ToUInt( &messageID, 16 ) ) {
		return false;
	}
	message = file->FindMessage( messageID );
	if( !message ) {
		return false;
	}
	signal = file->FindSignal( messageID, list[3] );
	if( !signal ) {
		return false;
	}
	return true;
}

void CodeGenChannel::Restore( wxDataInputStream &stream, int version ) {
	id = stream.Read32();
	baudrate = stream.Read32();
	name = stream.ReadString();
	target = stream.Read32();
	if( version == 13 ) {
		stream.Read32();
	} else {
		protocol = stream.Read32();
	}
	spt = stream.Read32();
}

void CodeGenChannel::Save( wxDataOutputStream &stream ) const {
	stream.Write32( id );
	stream.Write32( baudrate );
	stream.WriteString( name );
	stream.Write32( target );
	stream.Write32( protocol );
	stream.Write32( spt );
}

wxString CodeGenChannel::GetDescription() const {
	return wxString::Format( "%s [%d]", name, baudrate );
}

void CodeGenLink::Restore( wxDataInputStream &stream, int version ) {
	signals.Clear();
	type = stream.Read32();
	name = stream.ReadString();
	value = stream.Read32();
	hook = stream.ReadString();
	const int count = stream.Read32();
	for( int i = 0; i < count; i++ ) {
		wxString path = stream.ReadString();
		// path.Replace( "GlobalA_HS", "GlobalA - HS", true );
		signals.push_back( path );
	}
}

void CodeGenLink::Save( wxDataOutputStream &stream ) const {
	stream.Write32( type );
	stream.WriteString( name );
	stream.Write32( value );
	stream.WriteString( hook );
	stream.Write32( (int)signals.size() );
	for( int i = 0; i < signals.size(); i++ ) {
		stream.WriteString( signals[i] );
	}
}

wxString CodeGenLink::GetType() const {
	switch( type ) {
	case CGT_RECEIVE:
		return "Receive";
	case CGT_SEND:
		return "Send";
	case CGT_STATIC:
		return "Static";
	case CGT_COUNT:
		return "Count";
	default:
		break;
	}
	return "Undefined";
}

wxString CodeGenLink::GetSignals() const {
	return wxJoin( signals, ',', 0 );
}

wxString CodeGenLink::GetPresent() const {
	if( type == CGT_STATIC || type == CGT_COUNT ) {
		return wxString::Format( "%s (0x%x)", name, value );
	}
	return wxString::Format( "%s", name );
}

wxString CodeGenLink::GetDescription() const {
	switch( type ) {
	case CGT_RECEIVE:
		return wxString::Format( "%s <- %s", name, wxJoin( signals, ',', 0 ) );
	case CGT_SEND:
		return wxString::Format( "%s -> %s", name, wxJoin( signals, ',', 0 ) );
	case CGT_STATIC:
	case CGT_COUNT:
		return wxString::Format( "%s (0x%x) -> %s", name, value, wxJoin( signals, ',', 0 ) );
	default:
		break;
	}
	return "Undefined";
}

const int CodeGen::CODEGEN_VERSION = 15;
CodeGen::CodeGen() {
	channel.id = 0;
	channel.baudrate = 500000;
	channel.target = 0x100;
	channel.protocol = CGP_NONE;
	channel.spt = 87; // 80
}

bool CodeGen::IsValid() const {
	if( channel.name.empty() ) {
		return false;
	}
	return true;
}

void CodeGen::AppendLink( CodeGenLink *const link ) {
	links.push_back( link );
}

void CodeGen::RemoveLink( CodeGenLink *const link ) {
	wxVector<CodeGenLink *>::iterator iter = std::find( links.begin(), links.end(), link );
	if( iter != links.end() ) {
		links.erase( iter );
	}
}

wxArrayString CodeGen::GetVariableList() const {
	wxArrayString list;
	for( int i = 0; i < links.size(); i++ ) {
		const CodeGenLink *link = links[i];
		if( !( link->type == CGT_RECEIVE || link->type == CGT_COUNT ) ) {
			continue;
		}
		if( list.Index( link->name ) != wxNOT_FOUND ) {
			list.push_back( link->name );
		}
	}
	return list;
}

bool CodeGen::LoadFile( const wxString &filename ) {
	wxFileInputStream file( filename );
	if( !file.IsOk() ) {
		return false;
	}
	links.clear();
	wxDataInputStream stream( file );
	int version = stream.Read32();
	if( version < 13 ) {
		return false;
	}
	channel.Restore( stream, version );
	int count = stream.Read32();
	for( int i = 0; i < count; i++ ) {
		CodeGenLink *link = new CodeGenLink;
		link->Restore( stream, version );
		links.push_back( link );
	}

	this->filename = filename;
	return true;
}

bool CodeGen::SaveFile( const wxString &filename ) {
	wxFileOutputStream file( filename );
	if( !file.IsOk() ) {
		return false;
	}
	wxDataOutputStream stream( file );
	stream.Write32( CODEGEN_VERSION );
	channel.Save( stream );

	stream.Write32( (int)links.size() );
	for( int i = 0; i < links.size(); i++ ) {
		links[i]->Save( stream );
	}
	this->filename = filename;
	return true;
}

GenChannelDialog::GenChannelDialog( wxWindow *parent, wxWindowID id ) : wxDialog( parent, id, "Generate Signal", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	ui = new Ui::GenChannelDialog();
	ui->Setup( this );
	ui->name->SetValidator( wxGenericValidator( &name ) );
	ui->baudrate->SetValidator( wxIntegerValidator<int>( &baudrate, 25, 1000000 ) );
	ui->target->SetValidator( wxHexValidator( target ) );
	ui->protocol->SetValidator( wxGenericValidator( &protocol ) );
	ui->spt->SetValidator( wxIntegerValidator<int>( &spt, 0, 100 ) );
}

void GenChannelDialog::InitDialog() {
	wxArrayString files, protocols;

	files = DBCFileNames();
	GetProtocolNames( protocols );

	ui->name->Set( files );
	ui->protocol->Set( protocols );
	wxDialog::InitDialog();
}

bool GenChannelDialog::Validate() {
	TransferDataFromWindow();
	if( name.empty() ) {
		wxMessageBox( "Name is empty", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void GenChannelDialog::SetChannel( const CodeGenChannel *channel ) {
	name = channel->name;
	baudrate = channel->baudrate;
	target = channel->target;
	protocol = channel->protocol;
	spt = channel->spt;
	SetTitle( wxString::Format( "CH%d - Setting", channel->id ) );
}

void GenChannelDialog::GetChannel( CodeGenChannel *channel ) const {
	channel->name = name;
	channel->baudrate = baudrate;
	channel->target = target;
	channel->protocol = protocol;
	channel->spt = spt;
}

void GenChannelDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK && !Validate() ) {
		return;
	}
	wxDialog::EndModal( retCode );
}

GenSignalDialog::GenSignalDialog( wxWindow *parent, wxWindowID id ) : wxDialog( parent, id, "Generate Signal", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	value = 0;

	ui = new Ui::GenSignalDialog();
	ui->Setup( this );
	ui->type->SetValidator( wxGenericValidator( &type ) );
	ui->name->SetValidator( wxGenericValidator( &name ) );
	ui->hook->SetValidator( wxGenericValidator( &hook ) );
	ui->variable->SetValidator( wxGenericValidator( &variable ) );
	ui->value->SetValidator( wxHexValidator( value ) );
	ui->type->Bind( wxEVT_CHOICE, &GenSignalDialog::OnChangeType, this );
	ui->select->Bind( wxEVT_BUTTON, &GenSignalDialog::OnSelectSignal, this );
	// nosignals
}

void GenSignalDialog::InitDialog() {
	UpdateType();
	UpdateSignals();
	wxDialog::InitDialog();
}

bool GenSignalDialog::Validate() {
	TransferDataFromWindow();
	if( type == CGT_SEND && variable.empty() ) {
		wxMessageBox( "Variable is empty", "Warning", wxOK, this );
		return false;
	} else if( type != CGT_SEND && name.empty() ) {
		wxMessageBox( "Name is empty", "Warning", wxOK, this );
		return false;
	}
	/* if( ( type == CGT_COUNT || type == CGT_STATIC ) && value.empty() ) {
	    wxMessageBox( "Value is empty", "Warning", wxOK, this );
	    return false;
	} */
	if( signals.size() == 0 ) {
		wxMessageBox( "Signal List is empty", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void GenSignalDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK ) {
		// -
	}
	wxDialog::EndModal( retCode );
}

void GenSignalDialog::OnChangeType( wxCommandEvent &event ) {
	TransferDataFromWindow();
	UpdateType();
}

void GenSignalDialog::OnSelectSignal( wxCommandEvent &event ) {
	SignalSelectorDialog dialog( this, files );
	dialog.SetSelected( signals );
	if( dialog.ShowModal() == wxID_OK ) {
		signals = dialog.GetSelected();
		UpdateSignals();
	}
}

void GenSignalDialog::UpdateSignals() {
	ui->signals->SetLabel( wxJoin( signals, '\n', 0 ) );
}

void GenSignalDialog::UpdateType() {
	ui->name->Enable( type != CGT_SEND );
	ui->variable->Enable( type == CGT_SEND );
	ui->value->Enable( type == CGT_STATIC || type == CGT_COUNT );
}

void GenSignalDialog::SetVariables( const wxArrayString &variables ) {
	ui->variable->Set( variables );
}

void GenSignalDialog::SetFiles( const wxVector<wxDBCFile *> &files ) {
	this->files = files;
}

void GenSignalDialog::SetLink( const CodeGenLink *link ) {
	type = link->type;
	variable = name = link->name;
	value = link->value;
	signals = link->signals;
	hook = link->hook;
}

void GenSignalDialog::GetLink( CodeGenLink *link ) const {
	link->type = type;
	if( type == CGT_SEND ) {
		link->name = variable;
	} else {
		link->name = name;
	}
	link->value = value;
	link->signals = signals;
	link->hook = hook;
}

GenPreviewDialog::GenPreviewDialog( wxWindow *parent, const CodeGen *codes, wxWindowID id ) : wxDialog( parent, id, "Code Preview", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	ui = new Ui::GenPreviewDialog();
	ui->Setup( this );
	SetupCodeStyled( ui->source );
	this->codes = codes;
}

void GenPreviewDialog::InitDialog() {
	CodeGenerate();
	wxDialog::InitDialog();
}

void GenPreviewDialog::SetupCodeStyled( wxStyledTextCtrl *text ) {
#if _WIN32
	wxFont font = wxSystemSettings::GetFont( wxSYS_ANSI_FIXED_FONT );
#else
	wxFont font( wxFontInfo( 12 ).FaceName( "Menlo" ).Family( wxFONTFAMILY_DEFAULT ) );
#endif
	text->SetLexer( wxSTC_LEX_CPP );
	text->SetFont( font );
	text->SetFontQuality( wxSTC_EFF_QUALITY_ANTIALIASED );
	text->StyleSetFont( wxSTC_STYLE_DEFAULT, font );
	text->StyleClearAll();

	text->SetMarginWidth( MARGIN_LINE_NUMBERS, 50 );
	text->StyleSetForeground( wxSTC_STYLE_LINENUMBER, wxColour( 75, 75, 75 ) );
	text->StyleSetBackground( wxSTC_STYLE_LINENUMBER, wxColour( 220, 220, 220 ) );
	text->SetMarginType( MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER );

	// ---- Enable code folding
	text->SetMarginType( MARGIN_FOLD, wxSTC_MARGIN_SYMBOL );
	text->SetMarginWidth( MARGIN_FOLD, 15 );
	text->SetMarginMask( MARGIN_FOLD, wxSTC_MASK_FOLDERS );
	text->StyleSetBackground( MARGIN_FOLD, wxColor( 200, 200, 200 ) );
	text->SetMarginSensitive( MARGIN_FOLD, true );

	// Properties found from http://www.scintilla.org/SciTEDoc.html
	text->SetProperty( wxT( "fold" ), wxT( "1" ) );
	text->SetProperty( wxT( "fold.comment" ), wxT( "1" ) );
	text->SetProperty( wxT( "fold.compact" ), wxT( "1" ) );

	wxColor grey( 100, 100, 100 );
	text->MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_ARROW );
	text->MarkerSetForeground( wxSTC_MARKNUM_FOLDER, grey );
	text->MarkerSetBackground( wxSTC_MARKNUM_FOLDER, grey );

	text->MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN );
	text->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, grey );
	text->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, grey );

	text->MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY );
	text->MarkerSetForeground( wxSTC_MARKNUM_FOLDERSUB, grey );
	text->MarkerSetBackground( wxSTC_MARKNUM_FOLDERSUB, grey );

	text->MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_ARROW );
	text->MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, grey );
	text->MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, _T("WHITE") );

	text->MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN );
	text->MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, grey );
	text->MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, _T("WHITE") );

	text->MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY );
	text->MarkerSetForeground( wxSTC_MARKNUM_FOLDERMIDTAIL, grey );
	text->MarkerSetBackground( wxSTC_MARKNUM_FOLDERMIDTAIL, grey );

	text->MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY );
	text->MarkerSetForeground( wxSTC_MARKNUM_FOLDERTAIL, grey );
	text->MarkerSetBackground( wxSTC_MARKNUM_FOLDERTAIL, grey );
	// ---- End of code folding part

	text->SetWrapMode( wxSTC_WRAP_NONE ); // other choice is wxSCI_WRAP_WORD

	text->StyleSetForeground( wxSTC_C_STRING, wxColour( 150, 0, 0 ) );
	text->StyleSetForeground( wxSTC_C_PREPROCESSOR, wxColour( 165, 105, 0 ) );
	text->StyleSetForeground( wxSTC_C_IDENTIFIER, wxColour( 40, 0, 60 ) );
	text->StyleSetForeground( wxSTC_C_NUMBER, wxColour( 0, 150, 0 ) );
	text->StyleSetForeground( wxSTC_C_CHARACTER, wxColour( 150, 0, 0 ) );
	text->StyleSetForeground( wxSTC_C_WORD, wxColour( 0, 0, 150 ) );
	text->StyleSetForeground( wxSTC_C_WORD2, wxColour( 0, 150, 0 ) );
	text->StyleSetForeground( wxSTC_C_COMMENT, wxColour( 150, 150, 150 ) );
	text->StyleSetForeground( wxSTC_C_COMMENTLINE, wxColour( 150, 150, 150 ) );
	text->StyleSetForeground( wxSTC_C_COMMENTDOC, wxColour( 150, 150, 150 ) );
	text->StyleSetForeground( wxSTC_C_COMMENTDOCKEYWORD, wxColour( 0, 0, 200 ) );
	text->StyleSetForeground( wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour( 0, 0, 200 ) );
	text->StyleSetBold( wxSTC_C_WORD, true );
	text->StyleSetBold( wxSTC_C_WORD2, true );
	text->StyleSetBold( wxSTC_C_COMMENTDOCKEYWORD, true );

	text->SetKeyWords( 0, "return for while break continue case switch default if else do continue break volatile" );
	text->SetKeyWords( 1, "const int float void char double unsigned" );
}

void GenPreviewDialog::OnChangePreview( wxCommandEvent &event ) {
	CodeGenerate();
}

const wxString preDefineCode = R"(// Arduino-DBC2C 2025 J.C. - K.R.
#include <Arduino.h>
#include <CANSAME5x.h>

// #define CAN_DEBUG
// #define SERIAL_LOG

class EvTimer {
public:
	EvTimer() : last( -1 ) {}
	bool IsElapsed( const unsigned long delta ) {
		unsigned long current = millis();
		if( last == -1 ) {
			last = current;
		} else if( ( current - last ) > delta ) {
			last = current;
			return true;
		}
		return false;
	}
	bool IsElapsedMicro( const unsigned long delta ) {
		unsigned long current = micros();
		if( last == -1 ) {
			last = current;
		} else if( ( current - last ) > delta ) {
			last = current;
			return true;
		}
		return false;
	}
public:
	unsigned long last;
};

CANSAME5x CAN;

struct CAN_message_t {
	uint32_t id;
	uint8_t len;
	uint32_t buf[8];
};

int CAN_Read( CAN_message_t &msg );
void CAN_Print( const char *TAG, CAN_message_t &msg );
void CAN_Write( CAN_message_t &msg );

int32_t clamp32(int32_t value, int32_t min, int32_t max) {
	if(value < min) {
		return min;
	} else if(value > max) {
		return max;
	}
	return value;
}

#define CLAMP_VALUE(value, min, max) (value < min ? min : (value > max ? max : value))
#define CNTR_VALUE(name, size) ((name + 1) % size)

volatile uint32_t timer_counter = 0; // CanTx_Fx_Timer: 2.5ms
EvTimer canTxTimer;    // 2500 micros => 2.5 millis => 0.0025 sec

)";

const wxString extraFunctions = R"(
#ifdef SERIAL_LOG
void CAN_Print( const char *TAG, CAN_message_t &msg ) {
	Serial.print( TAG );
	Serial.print( ": " );
	Serial.print( msg.id, HEX );
	Serial.print( " = Size: " );
	Serial.print( msg.len );
	for( int i = 0; i < msg.len; i++ ) {
		Serial.print( " [" );
		Serial.print( msg.buf[i], HEX );
		Serial.print( "]" );
	}
	Serial.println();
}
#endif

void CAN_Write( CAN_message_t &msg ) {
	CAN.beginPacket( msg.id, msg.len ); // send packet: id is 11 bits, packet can contain up to 8 bytes of data
	// CAN.CAN.beginExtendedPacket => send extended packet: id is 29 bits, packet can contain up to 8 bytes of data
	for( int i = 0; i < msg.len; i++ ) {
		CAN.write( msg.buf[i] );
	}
	CAN.endPacket();
#ifdef SERIAL_LOG
	CAN_Print("Send", msg);
#endif
}

int CAN_Read( CAN_message_t &msg ) {
	int size = CAN.parsePacket();
#ifdef SERIAL_LOG
	Serial.print( "Read packet length: " );
	Serial.println( size );
#endif
	if( size ) {
		if( CAN.packetExtended() ) {
#ifdef SERIAL_LOG
			Serial.print( "extended !" );
#endif
		}
		if( CAN.packetRtr() ) {
			// Remote transmission request, packet contains no data
#ifdef SERIAL_LOG
			Serial.print( "RTR " );
#endif
		}
		msg.id = CAN.packetId();
		if( CAN.packetRtr() ) {
#ifdef SERIAL_LOG
			Serial.print( " and requested length " );
			Serial.println( CAN.packetDlc() );
#endif
			return 0; // PASS RTR packaged
		} else {
			for( int i = 0; i < size; i++ ) {
				if( CAN.available() ) {
					msg.buf[i] = CAN.read();
				}
			}
			msg.len = size;
#ifdef SERIAL_LOG
			CAN_Print("Recv", msg);
#endif
		}
		return 1;
	}
	return 0;
}
)";

const wxString preSetupFunction = R"(
void setup() {
	Serial.begin( 115200 );

	pinMode( 10, OUTPUT );
	pinMode( 9, OUTPUT );

	pinMode( PIN_CAN_STANDBY, OUTPUT );
	digitalWrite( PIN_CAN_STANDBY, false ); // turn off STANDBY
	pinMode( PIN_CAN_BOOSTEN, OUTPUT );
	digitalWrite( PIN_CAN_BOOSTEN, true ); // turn on booster

	if( !CAN.begin( CAN_baudrate ) ) {
		Serial.println( "Starting CAN failed!" );
		while( 1 ) {
			delay( 10 );
		}
	}
}
)";

const wxString preLoopFunction = R"(
void loop() {
	CAN_message_t msg;
	while( CAN_Read( msg ) ) {
		CAN_receive_processing( &msg );
	}
	// For 0x0C9 control, the message queue must be read first.
	// then the timer will send data with control.
	if( canTxTimer.IsElapsedMicro( 2500 ) ) {
		// calling every 2.5ms
		CanTx_Fx_Timer();
	}
}
)";

struct TimerItem {
	long time;
	wxVector<wxDBCMessage *> messages;
};

void AppendUniqueMessage( wxVector<wxDBCMessage *> &messages, wxDBCMessage *message ) {
	for( int i = 0; i < messages.size(); i++ ) {
		if( messages[i]->ID == message->ID ) {
			return;
		}
	}
	messages.push_back( message );
}

int MakeTimeList( wxVector<TimerItem> &timers, wxDBCMessage *message ) {
	long timeout = wxAtol( message->FindAttr( "GenMsgCycleTime", "20" ) );

	for( int i = 0; i < timers.size(); i++ ) {
		TimerItem &group = timers.at( i );
		if( group.time == timeout ) {
			AppendUniqueMessage( group.messages, message );
			return i;
		}
	}
	TimerItem group;
	group.time = timeout;
	AppendUniqueMessage( group.messages, message );
	timers.push_back( group );
	return timers.size() - 1;
}

void GroupTimeList( const CodeGen *codes, wxVector<TimerItem> &timers ) {
	for( int i = 0; i < codes->links.size(); i++ ) {
		CodeGenLink *link = codes->links.at( i );
		if( link->type == CGT_RECEIVE ) {
			continue;
		}
		for( int j = 0; j < link->signals.size(); j++ ) {
			SigPath sigpath;
			if( !sigpath.SetPath( link->signals[j] ) ) {
				continue;
			}
			MakeTimeList( timers, sigpath.message );
		}
	}
}

struct GroupItem {
	wxDBCMessage *message;
	wxArrayString names;
	wxVector<wxDBCSignal *> signals;
	wxVector<int> types;
	wxVector<int> values;
};

int MakeGroupList( wxVector<GroupItem> &groups, wxDBCMessage *message, wxDBCSignal *signal, const wxString &name, int type, int value ) {
	for( int i = 0; i < groups.size(); i++ ) {
		GroupItem &group = groups.at( i );
		if( group.message->ID == message->ID ) {
			group.signals.push_back( signal );
			group.names.push_back( name );
			group.types.push_back( type );
			group.values.push_back( value );
			return i;
		}
	}
	GroupItem group;
	group.message = message;
	group.signals.push_back( signal );
	group.names.push_back( name );
	group.types.push_back( type );
	group.values.push_back( value );
	groups.push_back( group );
	return groups.size() - 1;
}

int GroupMessageList( const CodeGen *codes, wxVector<GroupItem> &send_groups, wxVector<GroupItem> &recv_groups ) {
	for( int i = 0; i < codes->links.size(); i++ ) {
		CodeGenLink *link = codes->links.at( i );

		for( int j = 0; j < link->signals.size(); j++ ) {
			SigPath sigpath;
			if( !sigpath.SetPath( link->signals[j] ) ) {
				continue;
			}
			if( link->type == CGT_RECEIVE ) {
				MakeGroupList( recv_groups, sigpath.message, sigpath.signal, link->name, link->type, link->value );
			} else {
				MakeGroupList( send_groups, sigpath.message, sigpath.signal, link->name, link->type, link->value );
			}
		}
	}
	return 0;
}

void GenPreviewDialog::CodeGenerate() {
	wxString builder;

	wxArrayString defines;
	builder += preDefineCode;
	builder += wxString::Format( "#define CAN_baudrate %ld\n", codes->channel.baudrate );

	defines.clear();
	for( int i = 0; i < codes->links.size(); i++ ) {
		CodeGenLink *link = codes->links.at( i );
		if( defines.Index( link->name, false ) != wxNOT_FOUND ) {
			continue;
		}
		if( link->type == CGT_RECEIVE ) {
			builder += wxString::Format( "volatile int32_t %s = 0; // %s\n", link->name, link->GetDescription() );
			defines.Add( link->name );
		}
		if( link->type == CGT_COUNT ) {
			builder += wxString::Format( "volatile int32_t %s = 0; // %s\n", link->name, link->GetDescription() );
			defines.Add( link->name );
		}
	}
	fflush( stdout );

	builder += "\n\n";

	wxVector<GroupItem> recv_groups, send_groups;
	GroupMessageList( codes, send_groups, recv_groups );

	wxVector<TimerItem> time_groups;
	GroupTimeList( codes, time_groups );

	builder += "void CAN_receive_processing( CAN_message_t *msg ) {\n";
	builder += "    switch( msg->id ) {\n";
	for( int i = 0; i < recv_groups.size(); i++ ) {
		const GroupItem &group = recv_groups.at( i );
		const wxDBCMessage *message = group.message;

		builder += wxString::Format( "    case 0x%03x: {\n", message->ID );
		for( int j = 0; j < group.names.size(); j++ ) {
			const wxDBCSignal *signal = group.signals[j];
			const wxString name = group.names[j];

			builder += wxString::Format( "        int32_t %s_raw = %s;\n", name, MakeRawReadCode( signal, message->size ) );
			builder += wxString::Format( "        %s = %s;\n", name, MakeReadLinearAndClamp( signal, name ) );
		}
		builder += "        break;\n";
		builder += "    }\n";
	}
	builder += "    default:\n";
	builder += "        break;\n";
	builder += "    }\n";
	builder += "}\n\n";

	for( int i = 0; i < send_groups.size(); i++ ) {
		const GroupItem &group = send_groups.at( i );
		const wxDBCMessage *message = group.message;
		wxString output[8];
		wxArrayString suffix_code;

		builder += wxString::Format( "void CAN_%s() {\n", message->name );
		wxArrayString defined;
		for( int j = 0; j < group.names.size(); j++ ) {
			const wxDBCSignal *signal = group.signals[j];
			const wxString name = group.names[j];
			if( defined.Index( name ) == wxNOT_FOUND ) {
				int type = group.types[j];
				if( type == CGT_STATIC ) {
					builder += wxString::Format( "    uint32_t %s_raw = %d;\n", name, group.values[j] );
				} else if( type == CGT_COUNT ) {
					builder += wxString::Format( "    uint32_t %s_raw = %s;\n", name, MakeWriteLinearAndClamp( signal, name ) );
					//
					suffix_code.push_back( wxString::Format( "    %s = (%s + 1) & 0x%02x;\n", name, name, ( 1 << signal->bit_len ) - 1 ) );
				} else {
					builder += wxString::Format( "    uint32_t %s_raw = %s;\n", name, MakeWriteLinearAndClamp( signal, name ) );
				}
				defined.Add( name );
			}
			MakeRawWriteCode( signal, message->size, name, output, "raw" );
		}

		builder += "\n";
		builder += "    CAN_message_t msg;\n";
		builder += wxString::Format( "    msg.id = 0x%03x;\n", message->ID );
		builder += wxString::Format( "    msg.len = %d;\n", message->size );
		for( int j = 0; j < message->size; j++ ) {
			wxString raw = "0x0";
			if( !output[j].empty() ) {
				raw = output[j];
			}
			builder += wxString::Format( "    msg.buf[%d] = %s;\n", j, raw );
		}
		builder += "    CAN_Write(msg);\n";

		builder += "\n";
		for( int j = 0; j < suffix_code.size(); j++ ) {
			builder += suffix_code[j];
		}

		builder += "}\n\n";
	}

	builder += "void CanTx_Fx_Timer() {\n";
	for( int i = 0; i < time_groups.size(); i++ ) {
		const TimerItem &group = time_groups.at( i );

		int step = int( group.time / 2.5f );
		builder += wxString::Format( "    if( timer_counter %% %d == %d ) {\n", step, step - 1 );
		for( int j = 0; j < group.messages.size(); j++ ) {
			builder += wxString::Format( "        CAN_%s();\n", group.messages[j]->name );
		}
		builder += "    }\n";
	}
	builder += "\n";
	builder += "    timer_counter++;\n";
	builder += "    if( timer_counter >= 400 ) { // 1000 msec\n";
	builder += "        timer_counter = 0;\n";
	builder += "    }\n";
	builder += "}\n\n";

	builder += preSetupFunction;
	builder += preLoopFunction;
	builder += extraFunctions;

	builder += "\n";

	ui->source->SetValue( builder );
}

bool GenPreviewDialog::Validate() {
	if( codes->GetFilename().empty() ) {
		wxMessageBox( "filename is empty", "Info", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void GenPreviewDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK ) {
		const wxString source = codes->GetCodePath( "cpp" );
		wxString message = wxString::Format( "would you like to save the generated file?\n\n%s", source );
		if( wxMessageBox( message, "Alert", wxOK | wxCANCEL ) != wxOK ) {
			return;
		}
		if( !ui->source->SaveFile( source ) ) {
			wxMessageBox( "source file failed to save.", "Warning", wxOK, this );
			return;
		}
		wxMessageBox( "Saved", "Info", wxOK, this );
	}
	wxDialog::EndModal( retCode );
}

wxString CodeGen::GetCodePath( const wxString &ext ) const {
	wxFileName resolve( filename );
	resolve.SetExt( ext );
	return resolve.GetFullPath();
}

wxString GenPreviewDialog::MakeRawReadCode( const wxDBCSignal *signal, int msg_size ) const {
	uint8_t mask[8] = { 0 };
	signal->SetValue( -1, mask, msg_size );

	wxArrayString blocks;
	int shift = 0;
	for( int i = 0; i < 8; i++ ) {
		int index = i;
		if( signal->flags & SF_BIG_ENDIAN ) {
			index = 8 - i - 1;
		}
		if( mask[index] == 0 ) {
			continue;
		}
		wxString part = wxString::Format( "msg->buf[%d]", index );
		if( mask[index] != 0xff ) {
			part = wxString::Format( "(%s & 0x%02x)", part, mask[index] );
		}
		if( shift != 0 ) {
			part = wxString::Format( "(%s << %d)", part, shift * 8 );
		}
		blocks.push_back( part );
		shift++;
	}
	if( blocks.IsEmpty() ) {
		return "0x00";
	}
	return wxJoin( blocks, '|' );
}

wxString GenPreviewDialog::MakeReadLinearAndClamp( const wxDBCSignal *signal, const wxString &variable ) const {
	wxString code = wxString::Format( "%s_raw", variable );
	if( signal->flags & SF_FLOAT_EXT_TYPE ) {
		code = wxString::Format( "(*(float *)&%s)", code );
	} else if( signal->flags & SF_FLOAT_EXT_TYPE ) {
		code = wxString::Format( "(*(double *)&%s)", code );
	}
	if( signal->scale != 1.0f ) {
		float invscale = 1.0f / signal->scale;
		if( IEEE_MANTIS( invscale ) == 0 ) {
			code = wxString::Format( "%s / %.d", code, (int)invscale );
		} else {
			code = wxString::Format( "%.6g * %s", signal->scale, code );
		}
	}
	if( signal->offset != 0.0f ) {
		if( signal->offset < 0.0f ) {
			code = wxString::Format( "%s - %.6g", code.c_str(), fabs( signal->offset ) );
		} else {
			code = wxString::Format( "%s + %.6g", code.c_str(), signal->offset );
		}
	}
	if( ( signal->min != 0 || signal->max != 0 ) && ( signal->min < signal->max ) ) {
		code = wxString::Format( "clamp32(%s, %d, %d)", code, (int)signal->min, (int)signal->max );
	}
	return code;
}

void swap_byte( uint8_t &lhs, uint8_t &rhs ) {
	uint8_t tmp = lhs;
	lhs = rhs;
	rhs = tmp;
}

/*
CCAN_MSG_OBJ_T msg;
msg.id = 0x589; // ETEI_Engine_Torque_Capability
msg.dlc = 8;
msg.data[0] = (ETC_UnsdRsrvd_cntr++ % 0x07);
msg.data[1] = eng_speed;
msg.data[2] = (torque_max >> 4) & 0xFF; // ETC_MaxTorq
msg.data[3] = ((torque_max & 0x0F) << 4) | ((torque_mrun >> 8) & 0x0F); // ETC_MaxTorq, ETC_MinRunTorq
msg.data[4] = (torque_mrun & 0xFF); // ETC_MinRunTorq
msg.data[5] = (torque_min >> 4) & 0xFF; // ETC_MinTorq
msg.data[6] = (torque_min & 0x0F) << 4; // ETC_MinTorq
msg.data[7] = 0xc5; // 197 ?!! PrplsnTrqRelCap
*/

void GenPreviewDialog::MakeRawWriteCode( const wxDBCSignal *signal, int msg_size, const wxString &name, wxString output[8], const wxString &suffix ) const {
	uint8_t mask[8] = { 0 };
	int shift = 0;
	signal->SetValue( -1, mask, msg_size );

	if( name == "torq_max" ) {
		wxPrintf( "Debug\n" );
	}

	for( int i = 0; i < 8; i++ ) {
		int index = i;
		if( ( signal->flags & SF_BIG_ENDIAN ) != 0 ) {
			index = 8 - i - 1;
		}
		if( mask[index] == 0 ) {
			continue;
		}

		wxString part = name;
		if( !suffix.empty() ) {
			part = wxString::Format( "%s_%s", name, suffix );
		}
		
		int rshift = 0;
		uint8_t rmask = mask[index];
		while( ( rmask & 0x01 ) == 0 ) {
			rshift++, rmask >>= 1;
		}
		if( shift != 0 ) {
			part = wxString::Format( "%s >> %d", part, shift );
		}
		part = wxString::Format( "(%s) & 0x%02x", part, rmask );
		if(rshift != 0) {
			part = wxString::Format( "(%s) << %d", part, rshift );
		}
		shift += ( 8 - rshift );
		
		if( !output[index].empty() ) {
			output[index] += " | ";
		}
		output[index] += part;
	}
}

wxString GenPreviewDialog::MakeWriteLinearAndClamp( const wxDBCSignal *signal, const wxString &variable ) const {
	wxString code = variable;
	if( signal->min != 0 || signal->max != 0 ) {
		// code = wxString::Format("CLAMP_VALUE(%s, %.6g, %.6g)", code, signal->min, signal->max);
	}
	if( signal->offset != 0 ) {
		if( signal->offset < 0.0f ) {
			code = wxString::Format( "(%s + %.6g)", code, fabs( signal->offset ) );
		} else {
			code = wxString::Format( "(%s - %.6g)", code, signal->offset );
		}
	}
	if( signal->scale != 1.0f ) {
		code = wxString::Format( "(%s * %.6g)", code, 1.0f / signal->scale );
	}
	return code;
}

void GenPreviewDialog::RegisterReceiveList( int channel, GenRegRecv &regs ) const {
	for( int i = 0; i < codes->NumLink(); i++ ) {
		const CodeGenLink *link = codes->GetLink( i );
		if( link->type != CGT_RECEIVE ) {
			continue;
		}
		for( int j = 0; j < link->signals.size(); j++ ) {
			SigPath sigpath;
			if( !sigpath.SetPath( link->signals[j] ) ) {
				continue;
			}
			if( sigpath.channel != channel ) {
				continue;
			}
			bool found = false;
			wxVector<GenRecvSignal> &list = regs.groups;
			for( wxVector<GenRecvSignal>::iterator iter = list.begin(); iter != list.end(); iter++ ) {
				GenRecvSignal &group = ( *iter );
				if( group.message == sigpath.message ) {
					group.links.push_back( link );
					group.signals.push_back( sigpath.signal );
					found = true;
				}
			}
			if( !found ) {
				GenRecvSignal group;
				group.message = sigpath.message;
				group.signals.push_back( sigpath.signal );
				group.links.push_back( link );
				list.push_back( group );
			}
		}
	}
	regs.channel = channel;
}

void GenPreviewDialog::RegisterSendList( int channel, GenRegSend &regs ) const {
	wxVector<GenSendSignal> &list = regs.groups;
	for( int i = 0; i < codes->NumLink(); i++ ) {
		const CodeGenLink *link = codes->GetLink( i );
		if( link->type == CGT_RECEIVE ) {
			continue;
		}
		for( int j = 0; j < link->signals.size(); j++ ) {
			SigPath sigpath;
			if( !sigpath.SetPath( link->signals[j] ) ) {
				continue;
			}
			if( sigpath.channel != channel ) {
				continue;
			}
			wxDBCSignal *valid = sigpath.file->FindSignal( sigpath.message->ID, sigpath.signal->name + "V" );

			bool found = false;
			for( wxVector<GenSendSignal>::iterator iter = list.begin(); iter != list.end(); iter++ ) {
				GenSendSignal &group = ( *iter );
				if( group.message == sigpath.message ) {
					group.links.push_back( link );
					group.signals.push_back( sigpath.signal );
					group.valid.push_back( valid );
					found = true;
				}
			}
			if( !found ) {
				GenSendSignal group;
				group.message = sigpath.message;
				group.links.push_back( link );
				group.signals.push_back( sigpath.signal );
				group.valid.push_back( valid );
				list.push_back( group );
			}
		}
	}
	regs.channel = channel;
}



bool LinkSortName( CodeGenLink *const lhs, CodeGenLink *const rhs ) {
	return lhs->name.CmpNoCase( rhs->name ) < 0;
}


bool LinkSortNameInvert( CodeGenLink *const lhs, CodeGenLink *const rhs ) {
	return lhs->name.CmpNoCase( rhs->name ) > 0;
}


bool LinkSortType( CodeGenLink *const lhs, CodeGenLink *const rhs ) {
	return (lhs->type - rhs->type) < 0;
}

bool LinkSortTypeInvert( CodeGenLink *const lhs, CodeGenLink *const rhs ) {
	return (lhs->type - rhs->type) > 0;
}

bool LinkSortPath( CodeGenLink *const lhs, CodeGenLink *const rhs ) {
	return lhs->GetSignals().CmpNoCase( rhs->GetSignals() ) < 0;
}

bool LinkSortPathInvert( CodeGenLink *const lhs, CodeGenLink *const rhs ) {
	return lhs->GetSignals().CmpNoCase( rhs->GetSignals() ) > 0;
}

