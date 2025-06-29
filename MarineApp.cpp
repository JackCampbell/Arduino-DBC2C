// wxWidgets "Hello world" Program
#include "MarineApp.h"
#include "Lexer.h"
#include "DBCFile.h"
#include "stubs/Resource.h"
#include "stubs/MainFrameUi.h"
#include "stubs/DBCFileDialogUi.h"
#include "stubs/GenSignalDialogUi.h"
#include "stubs/GenPreviewDialogUi.h"
#include "DBCBitMap.h"
#include "DBCFileDialog.h"
#include "GenSignal.h"
#include "Utils.h"
#include "FileSelectDialog.h"
#ifdef WIN32
#pragma hdrstop
#endif

// clang-format off
IMPLEMENT_APP(MarineApp);
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_DEVELOPER, 					MainFrame::OnDeveloper)
	EVT_MENU(wxID_EXIT,						MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT,						MainFrame::OnAbout)
	EVT_MENU(wxID_ADD_ITEM,					MainFrame::OnAddGenerate)
	EVT_MENU(wxID_REM_ITEM,					MainFrame::OnRemoveGenerate)
	EVT_LIST_ITEM_ACTIVATED(wxID_ITEM_LIST,		MainFrame::OnEditGenerate)
	EVT_MENU(wxID_LOAD_ITEM,					MainFrame::OnLoadGenerator)
	EVT_MENU(wxID_SAVE_ITEM,					MainFrame::OnSaveGenerator)
	EVT_MENU(wxID_SAVE_AS_ITEM,				MainFrame::OnSaveAsGenerator)
	EVT_MENU(wxID_GEN_ITEM,					MainFrame::OnFileGenerator)
	EVT_MENU(wxID_DBC_FILES,					MainFrame::OnDBCFiles)
	EVT_BUTTON(wxID_CHANNEL_BTN,				MainFrame::OnSettingChannel)
	EVT_MENU(wxID_CHANNEL_BTN,					MainFrame::OnSettingChannel)
	EVT_LIST_COL_CLICK(wxID_ITEM_LIST,			MainFrame::OnSortList)
END_EVENT_TABLE()
// clang-format on

void MainFrame::CreateMenuAndStatus() {
	wxMenu *menuFile = new wxMenu;
	menuFile->Append( wxID_LOAD_ITEM, "&Load\tCtrl-O" );
	menuFile->Append( wxID_SAVE_ITEM, "&Save\tCtrl-S" );
	menuFile->Append( wxID_SAVE_AS_ITEM, "Save As" );
	menuFile->AppendSeparator();
	menuFile->Append( wxID_GEN_ITEM, "Code Generator\tCtrl+G" );
	menuFile->AppendSeparator();
	menuFile->Append( wxID_DBC_FILES, "DBC Library\tCtrl+L" );
	menuFile->AppendSeparator();
#ifdef DEBUG
	menuFile->Append( wxID_DEVELOPER, "&Developer...\tCtrl-T", "Help string shown in status bar for this menu item" );
	menuFile->AppendSeparator();
#endif
	menuFile->Append( wxID_EXIT );

	wxMenu *menuEdit = new wxMenu;
	menuEdit->Append( wxID_CHANNEL_BTN, "Edit Channel" );
	menuEdit->AppendSeparator();
	menuEdit->Append( wxID_ADD_ITEM, "Add Signal\tCtrl+A" );
	menuEdit->Append( wxID_REM_ITEM, "Remove Signal\tDelete" );

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append( wxID_ABOUT );

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, "&File" );
	menuBar->Append( menuEdit, "&Edit" );
	menuBar->Append( menuHelp, "&Help" );
	SetMenuBar( menuBar );
	CreateStatusBar( MAX_INDICATOR );
#ifdef WIN32
	wxIcon icon;
	icon.CopyFromBitmap( icon_app() );
	SetIcon( icon );
#endif
}

bool MarineApp::OnInit() {
	if( !wxApp::OnInit() ) {
		return false;
	}
	wxConvCurrent = &wxConvUTF8;
	wxInitAllImageHandlers();
	MainFrame *frame = new MainFrame( "MarineFeatherTools", wxPoint( 64, 64 ), wxSize( 1024, 768 ) );
	frame->CenterOnParent();
	frame->Show( true );
	return true;
}

wxString AppDesc = R"(MarineFeatherTools CAN Communication Code Generator
This application reads DBC files and generates code for CAN communication on the Arduino Feather M4 board.
It was developed specifically for MarineMods

+1 888-281-2228)";

void MainFrame::OnAbout( wxCommandEvent &event ) {
	wxIcon icon;
	icon.CopyFromBitmap( icon_app() );

	wxAboutDialogInfo dialog;
	dialog.SetName( "MarineFeatherTools" );
	dialog.SetIcon( icon );
	dialog.SetVersion( "1.0" );
	dialog.SetCopyright( "Copyright (c) MarineMods Company. All rights reserved." );
	dialog.SetDescription(AppDesc);
	dialog.SetWebSite( "https://marinemods.us" );
	dialog.AddDeveloper( "B.Firat OZDEMIR (jack_campbell_512@hotmail.com)" );
	dialog.AddDeveloper( "Ken Riggs (info@marinemods.us)" );
	wxAboutBox( dialog );
}

int MarineApp::OnExit() {
	SaveDBCFilenames();
	return wxApp::OnExit();
}

MainFrame::MainFrame( const wxString &title, const wxPoint &pos, const wxSize &size ) : wxFrame( NULL, wxID_ANY, title, pos, size ) {
	CreateMenuAndStatus();
	ui = new Ui::MainFrame();
	ui->Setup( this );
	SetStatusText( wxString::Format( "Welcome to %s!", title ), APP_STATUS );
	ReadDBCFilenames();
}

MainFrame::~MainFrame() {
}

void MainFrame::SendStatus( const wxString &message ) {
	if( wxThread::IsMain() ) {
		SetStatusText( message, APP_STATUS );
	} else {
		CallAfter( &MainFrame::SetStatusText, message, APP_STATUS );
	}
}

void MainFrame::OnExit( wxCommandEvent &event ) {
	Close( true );
}

LinkSort_f sortCallback = LinkSortName;

void MainFrame::OnSortList( wxListEvent &event ) {
	const int col = event.GetColumn();
	if( col == -1 ) {
		return;
	}
	LinkSort_f callbacks[] = { LinkSortType, LinkSortTypeInvert, LinkSortName, LinkSortNameInvert, LinkSortPath, LinkSortPathInvert };
	if( sortCallback == callbacks[col * 2] ) {
		sortCallback = callbacks[col * 2 + 1];
	} else {
		sortCallback = callbacks[col * 2];
	}
	UpdateGenList();

	fflush( stdout );
}

void MainFrame::UpdateGenList() {
	std::sort( codes.links.begin(), codes.links.end(), sortCallback );

	ui->gens->Freeze();
	ui->gens->DeleteAllItems();
	for( int i = 0; i < codes.NumLink(); i++ ) {
		const CodeGenLink *item = codes.GetLink( i );
		long index = ui->gens->InsertItem( i, item->GetType() );
		ui->gens->SetItem( index, 1, item->GetPresent() );
		ui->gens->SetItem( index, 2, item->GetSignals() );
		ui->gens->SetItemData( index, i );
	}
	ListCtrlAutoColumn( ui->gens );
	ui->gens->Thaw();
	ui->genfile->SetLabel( codes.GetFilename() );
}

wxArrayString MainFrame::GetCodeGenVariables() {
	wxArrayString list;
	for( int i = 0; i < codes.NumLink(); i++ ) {
		const CodeGenLink *item = codes.GetLink( i );
		if( item->type == CGT_SEND || item->type == CGT_STATIC ) {
			continue;
		}
		if( list.Index( item->name ) == wxNOT_FOUND ) {
			list.push_back( item->name );
		}
	}
	return list;
}

CodeGenLink *MainFrame::GetCurrentGen() {
	long selected = ui->gens->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( selected == -1 ) {
		return nullptr;
	}
	long index = ui->gens->GetItemData( selected );
	return codes.GetLink( index );
}

wxVector<wxDBCFile *> MainFrame::GetFileFromChannel() const {
	wxVector<wxDBCFile *> files;
	files.push_back( FindDBCFile( codes.channel.name ) );
	return files;
}

void MainFrame::OnAddGenerate( wxCommandEvent &event ) {
	GenSignalDialog dialog( this );

	wxArrayString variables = GetCodeGenVariables();
	if( codes.channel.protocol == CGP_NMEA2000 ) {
		// extern uint8_t engineSide;
		variables.push_back( "engineSide" );
	}
	dialog.SetVariables( variables );
	dialog.SetFiles( GetFileFromChannel() );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	CodeGenLink *link = new CodeGenLink();
	dialog.GetLink( link );
	codes.links.push_back( link );
	UpdateGenList();
}

void MainFrame::OnEditGenerate( wxListEvent &event ) {
	CodeGenLink *selected = GetCurrentGen();
	if( !selected ) {
		return;
	}
	GenSignalDialog dialog( this );
	dialog.SetVariables( GetCodeGenVariables() );
	dialog.SetFiles( GetFileFromChannel() );
	dialog.SetLink( selected );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	dialog.GetLink( selected );
	UpdateGenList();
}

void MainFrame::OnRemoveGenerate( wxCommandEvent &event ) {
	CodeGenLink *selected = GetCurrentGen();
	if( !selected ) {
		return;
	}
	wxString message = wxString::Format( "Are you sure?\n:%s -> %s", selected->GetType(), selected->GetDescription() );
	if( wxMessageBox( message, "Remove", wxYES_NO | wxICON_EXCLAMATION ) != wxYES ) {
		return;
	}
	codes.RemoveLink( selected );
	UpdateGenList();
}

void MainFrame::OnLoadGenerator( wxCommandEvent &event ) {
	wxFileDialog dialog( this, "Open Gen file", "", "", "Marine Feather Generate Files (*.gen2f)|*.gen2f", wxFD_OPEN | wxFD_FILE_MUST_EXIST );
	if( dialog.ShowModal() == wxID_CANCEL ) {
		return;
	}
	const wxString filename = dialog.GetPath();
	if( !codes.LoadFile( filename ) ) {
		return;
	}
	UpdateGenList();
	UpdateConnectInfo();
}

void MainFrame::OnSaveGenerator( wxCommandEvent &event ) {
	wxString filename = codes.GetFilename();
	if( filename.empty() ) {
		OnSaveAsGenerator( event );
		return;
	}
	codes.SaveFile( filename );
	// UpdateConnectInfo();
	// UpdateGenList();
}

void MainFrame::OnSaveAsGenerator(wxCommandEvent &event) {
	wxFileDialog dialog( this, "Save Gen file", "", "", "Marine Feather Generate Files (*.gen2f)|*.gen2f", wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
	if( dialog.ShowModal() == wxID_CANCEL ) {
		return;
	}
	const wxString filename = dialog.GetPath();
	codes.SaveFile( filename );

	UpdateConnectInfo();
	UpdateGenList();
}

void MainFrame::OnFileGenerator( wxCommandEvent &event ) {
	GenPreviewDialog dialog( this, &codes );
	dialog.ShowModal();
}

void MainFrame::UpdateLinkSignals() {
	bool is_update = false;
	for( int i = 0; i < codes.links.size(); i++ ) {
		CodeGenLink *link = codes.links.at( i );
		for( int j = 0; j < link->signals.size(); j++ ) {
			wxArrayString path = wxSplit( link->signals[j], ':', 0 );
			if( path[0] != "CH0" ) {
				continue;
			}
			path[1] = codes.channel.name;
			link->signals[j] = wxJoin( path, ':', 0 );
			is_update = true;
		}
	}
	if( is_update ) {
		UpdateGenList();
	}
}

void MainFrame::OnSettingChannel( wxCommandEvent &event ) {
	GenChannelDialog dialog( this );
	dialog.SetChannel( &codes.channel );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	dialog.GetChannel( &codes.channel );
	UpdateLinkSignals();
	UpdateConnectInfo();
}

void MainFrame::UpdateConnectInfo() {
	wxArrayString protocols;
	GetProtocolNames( protocols );

	if( codes.channel.name.empty() ) {
		ui->channel_info->SetLabel( "CH0" );
	} else {
		const wxString protocol = protocols[codes.channel.protocol];
		const wxString name = codes.channel.name;
		const float baudrate = codes.channel.baudrate / 1000.0f;
		ui->channel_info->SetLabel( wxString::Format( "Channel\n%s\n%s\n%.3f KBPS", name, protocol, baudrate ) );
	}
	ui->channel_info->SendSizeEventToParent();
}

void MainFrame::OnDBCFiles(wxCommandEvent &event) {
	FileSelectDialog dialog( this );
	dialog.ShowModal();
}
