#pragma once
#include "Precompiled.h"
#include "DBCFile.h"
#include "GenSignal.h"

namespace Ui {
	class MainFrame;
};
class wxListEvent;

class MarineApp : public wxApp {
public:
	virtual bool OnInit();
	virtual int OnExit();
};

DECLARE_APP( MarineApp );

class MainFrame : public wxFrame {
	DECLARE_EVENT_TABLE();
	friend class FileSelectDialog;
public:
	enum {
		APP_STATUS,
		MAX_INDICATOR
	};
	MainFrame( const wxString &title, const wxPoint &pos, const wxSize &size );
	virtual ~MainFrame();
	void CreateMenuAndStatus();
	void SendStatus( const wxString &message );
	void UpdateGenList();
	CodeGenLink *GetCurrentGen();
	wxVector<wxDBCFile *> GetFileFromChannel() const;
private:
	void OnDeveloper( wxCommandEvent &event );
	void OnExit( wxCommandEvent &event );
	void OnAbout( wxCommandEvent &event );

	void OnAddGenerate( wxCommandEvent &event );
	void OnEditGenerate( wxListEvent &event );
	void OnRemoveGenerate( wxCommandEvent &event );
	void OnLoadGenerator( wxCommandEvent &event );
	void OnSaveGenerator( wxCommandEvent &event );
	void OnFileGenerator( wxCommandEvent &event );
	void OnSettingChannel( wxCommandEvent &event );
	void OnDBCFiles( wxCommandEvent &event );
	wxArrayString GetCodeGenVariables();
	
	void UpdateConnectInfo();
private:
	wxSharedPtr<Ui::MainFrame> ui;
	wxVector<wxDBCFile *> files;
	CodeGen codes;
};
