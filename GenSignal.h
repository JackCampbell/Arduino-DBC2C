#pragma once
#include "Precompiled.h"

namespace Ui {
	class DBCFileDialog;
	class GenSignalDialog;
	class GenPreviewDialog;
	class GenDeviceDialog;
	class GenChannelDialog;
};
class wxDBCFile;
class wxDBCMessage;
class wxDBCSignal;

class SigPath {
public:
	int channel;
	wxDBCFile *file;
	wxDBCMessage *message;
	wxDBCSignal *signal;
public:
	SigPath();
	wxString GetPath() const;
	bool SetPath( const wxString &path );
};

inline SigPath::SigPath() {
	channel = 0;
	file = nullptr;
	message = nullptr;
	signal = nullptr;
}

enum {
	CGT_RECEIVE, // CGT_RECEIVE	-> name + signal,
	CGT_SEND,    // CGT_SEND		-> vars + signal,
	CGT_STATIC,  // CGT_STATIC	-> name + value + signal,
	CGT_COUNT    // CGT_COUNT	-> name + value + signal
};

enum {
	CGP_NONE,
	CGP_NMEA2000,
	CGP_HOLLEY,
	// ADD NEW PROTOCOL
};

class CodeGenChannel {
public:
	void Restore( wxDataInputStream &stream, int version );
	void Save( wxDataOutputStream &stream ) const;
	wxString GetDescription() const;
public:
	int id;
	int baudrate;
	wxString name; // basename
	uint32_t target;
	int protocol;
	int spt;
};

class CodeGenLink {
public:
	void Restore( wxDataInputStream &stream, int version );
	void Save( wxDataOutputStream &stream ) const;
	wxString GetType() const;
	wxString GetDescription() const;
	wxString GetSignals() const;
	wxString GetPresent() const;
public:
	int type;
	wxString name;
	uint32_t value;
	wxString hook;
	wxArrayString signals;
};

class CodeGen {
public:
	CodeGen();
	bool LoadFile( const wxString &filename );
	bool SaveFile( const wxString &filename );
	int NumLink() const { return links.size(); }
	CodeGenLink *GetLink( int index ) { return links[index]; }
	const CodeGenLink *GetLink( int index ) const { return links[index]; }
	const wxString &GetFilename() const { return filename; }
	bool IsValid() const;
	wxArrayString GetVariableList() const;
	void AppendLink( CodeGenLink *const link );
	void RemoveLink( CodeGenLink *const link );

	wxString GetCodePath( const wxString &ext ) const;
public:
	CodeGenChannel channel;
	wxVector<CodeGenLink *> links;
	wxArrayString files; // dbc files
	wxString filename;
private:
	static const int CODEGEN_VERSION;
};

class GenChannelDialog : public wxDialog {
public:
	GenChannelDialog( wxWindow *parent, wxWindowID id = wxID_ANY );
	virtual void InitDialog();
	virtual bool Validate();
	virtual void EndModal( int retCode );
	void SetChannel( const CodeGenChannel *channel );
	void GetChannel( CodeGenChannel *channel ) const;
private:
	wxString name;
	int protocol;
	int baudrate, spt;
	uint32_t target;
	wxSharedPtr<Ui::GenChannelDialog> ui;
};

// declare: GenSignal.cpp
class GenSignalDialog : public wxDialog {
public:
	GenSignalDialog( wxWindow *parent, wxWindowID id = wxID_ANY );
	virtual void InitDialog();
	virtual bool Validate();
	virtual void EndModal( int retCode );
	void SetVariables( const wxArrayString &variables );
	void SetFiles( const wxVector<wxDBCFile *> &files );
	void SetLink( const CodeGenLink *link );
	void GetLink( CodeGenLink *link ) const;
	void UpdateType();
	void UpdateSignals();
protected:
	void OnChangeType( wxCommandEvent &event );
	void OnSelectSignal( wxCommandEvent &event );
private:
	int type;
	wxString name, variable, hook;
	uint32_t value;
	wxArrayString signals;
	wxVector<wxDBCFile *> files;
	wxSharedPtr<Ui::GenSignalDialog> ui;
};

struct GenRecvSignal {
	const wxDBCMessage *message;
	wxVector<const CodeGenLink *> links;
	wxVector<const wxDBCSignal *> signals;
};

struct GenRegRecv {
	int channel;
	wxVector<GenRecvSignal> groups;
};

struct GenSendSignal {
	const wxDBCMessage *message;
	wxVector<const CodeGenLink *> links;
	wxVector<const wxDBCSignal *> signals;
	wxVector<const wxDBCSignal *> valid;
};

struct GenRegSend {
	int channel;
	wxVector<GenSendSignal> groups;
};

enum {
	MARGIN_LINE_NUMBERS = wxSTC_STYLE_DEFAULT,
	MARGIN_FOLD,
};

class GenPreviewDialog : public wxDialog {
public:
	GenPreviewDialog( wxWindow *parent, const CodeGen *codes, wxWindowID id = wxID_ANY );
	void SetupCodeStyled( wxStyledTextCtrl *text );
	virtual void InitDialog();
	virtual bool Validate();
	virtual void EndModal( int retCode );
private:
	void CodeGenerate();
	void RegisterReceiveList( int channel, GenRegRecv &regs ) const;
	void RegisterSendList( int channel, GenRegSend &regs ) const;

	wxString MakeWriteLinearAndClamp( const wxDBCSignal *signal, const wxString &variable ) const;
	void MakeRawWriteCode( const wxDBCSignal *signal, int msg_size, const wxString &name, wxString output[8], const wxString &suffix ) const;
	wxString MakeReadLinearAndClamp( const wxDBCSignal *signal, const wxString &variable ) const;
	wxString MakeRawReadCode( const wxDBCSignal *signal, int msg_size ) const;
protected:
	void OnChangePreview( wxCommandEvent &event );
private:
	const CodeGen *codes;
	wxSharedPtr<Ui::GenPreviewDialog> ui;
};

void GetProtocolNames( wxArrayString &protocols );
bool LinkSortName( CodeGenLink *const lhs, CodeGenLink *const rhs );
bool LinkSortNameInvert( CodeGenLink *const lhs, CodeGenLink *const rhs );
bool LinkSortType( CodeGenLink *const lhs, CodeGenLink *const rhs );
bool LinkSortTypeInvert( CodeGenLink *const lhs, CodeGenLink *const rhs );
bool LinkSortPath( CodeGenLink *const lhs, CodeGenLink *const rhs );
bool LinkSortPathInvert( CodeGenLink *const lhs, CodeGenLink *const rhs );

typedef bool ( *LinkSort_f )( CodeGenLink *const lhs, CodeGenLink *const rhs );
