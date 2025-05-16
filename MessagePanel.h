#pragma once
#include "Precompiled.h"
#include "DBCFile.h"

namespace Ui {
	class MessagePanel;
	class MessageDialog;
	class SignalDialog;
	class SignalPanel;
};



class SignalDialog : public wxDialog {
public:
							SignalDialog( wxWindow *parent, wxDBCFile *file, wxWindowID id = wxID_ANY );
							~SignalDialog();
	virtual void			InitDialog();
	virtual bool			Validate();
	virtual void			EndModal( int retCode );
	void					SetSignal( const wxDBCSignal *signal );
	const wxString &		GetSignalName() const { return name; }
	int						GetStartBit() const { return start_bit; }
	int						GetBitLen() const { return bit_len; }
	int						GetFlags() const;
	float					GetScale() const { return scale; }
	float					GetOffset() const { return offset; }
	float					GetMin() const { return min; }
	float					GetMax() const { return max; }
	const wxString &		GetUnit() const { return unit; }
	const wxString &		GetMultiplexer() const { return multiplexer; }
	const wxString &		GetComment() const { return comment; }
	const wxStrMap &		GetAttrs() const { return attrs; }
	const wxValueMap &		GetValues() const { return values; }
protected:
	void					OnChangeBitmap( wxCommandEvent &event );
	void					UpdateBitmap();
private:
	wxDBCFile *						file;
	wxString						name;
	int								start_bit, bit_len;
	bool							endial;
	int								type;
	float							scale, offset;
	float							min, max;
	wxString						unit;
	wxString						multiplexer;
	wxString						comment;
	wxStrMap						attrs;
	wxValueMap						values;
	wxSharedPtr<Ui::SignalDialog>	ui;
};




wxDECLARE_EVENT( wxEVT_CHANGE_SIGNALS, wxCommandEvent );
class SignalPanel : public wxPanel {
public:
									SignalPanel( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
	void							SetSignalList( const wxVector<wxDBCSignal *> &signals, wxDBCFile *file );
	wxVector<wxDBCSignal *> &		GetSignalList() { return signals; }
	void							SendReport();
protected:
	void							OnAddSignal( wxCommandEvent &event );
	void							OnRemoveSignal( wxCommandEvent &event );
	void							OnEditSignal( wxListEvent &event );
private:
	void							UpdateList();
	wxDBCSignal *					GetSelected() const;
	wxDBCSignal *					FindSignal( const wxString &name ) const;
private:
	wxDBCFile *						file;
	wxVector<wxDBCSignal *>			signals;
	wxSharedPtr<Ui::SignalPanel>		ui;
};

class MessageDialog : public wxDialog {
public:
									MessageDialog( wxWindow *parent, wxDBCFile *file, wxWindowID id = wxID_ANY );
									~MessageDialog();
	virtual void					InitDialog();
	virtual bool					Validate();
	virtual bool					TransferDataFromWindow();
	virtual bool					TransferDataToWindow();
	virtual void					EndModal( int retCode );
	void							SetMessage( int canID, const wxString &name, int size, const wxString &sender );
	void							SetMessage( wxDBCMessage *message );
	int 							GetCanID() const;
	const wxString &				GetMessageName() const { return name; }
	int								GetMessageSize() const { return size; }
	const wxString &				GetSender() const { return sender; }
	const wxString &				GetComment() const { return comment; }
	const wxStrMap &				GetAttrs() const { return attrs; }
	const wxArrayString &			GetTransmitters() const { return transmitters; }
	const wxVector<wxDBCSignal *> &	GetSignals() const { return signals; }
protected:
	void							FillSenderNames();
	void							OnUpdateSignals( wxCommandEvent &event );
	void							UpdateSignalMap();
private:
	wxDBCFile *						file;
	// int							canID;
	wxString						canID;
	wxString						name;
	int								size;
	wxString						sender;
	wxString						comment;
	wxStrMap						attrs;
	wxArrayString					transmitters;
	wxVector<wxDBCSignal *>			signals;
	wxSharedPtr<Ui::MessageDialog>	ui;
};


// declare: MessagePanel.cpp
class MessagePanel : public wxPanel {
public:
								MessagePanel( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
	void						SetMessages( wxDBCFile *file );
	wxVector<wxDBCMessage *> &	GetMessages() { return messages; }
protected:
	void						OnAddMessage( wxCommandEvent &event );
	void						OnEditMessage( wxListEvent &event );
	void						OnRemoveMessage( wxCommandEvent &event );
private:
	void						UpdateList();
	wxDBCMessage *				FindId(uint32_t id) const;
	wxDBCMessage *				GetSelected();
private:
	wxDBCFile *						file;
	wxVector<wxDBCMessage *>			messages;
	wxSharedPtr<Ui::MessagePanel>	ui;
};
