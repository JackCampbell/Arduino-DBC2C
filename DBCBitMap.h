#pragma once
#include "Precompiled.h"

enum DBCBitMapStyles {
	SWS_LOOK_CRAZY = 1,
	DBM_DEFAULT_STYLE = 0
};

struct SignalMap {
	uint8_t mask[8];
	wxString name;
	wxColour colour;
};

class BitMapEvent : public wxEvent {
public:
	BitMapEvent( wxEventType eventType, int winid, const wxString &signalName );
	virtual wxEvent *Clone() const { return new BitMapEvent( *this ); }
	const wxString &GetSignalName() const { return signalName; }
private:
	wxString signalName;
};

wxDECLARE_EVENT( wxEVT_BITMAP_SELECTED, BitMapEvent );

// declare: DBCBitMap.cpp
class BitMapCtrl : public wxControl {
	wxDECLARE_DYNAMIC_CLASS( BitMapCtrl );
	wxDECLARE_EVENT_TABLE();
public:
	BitMapCtrl();
	BitMapCtrl( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxBORDER_NONE );
	bool Create( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxBORDER_NONE );
	void ClearSignal();
	bool AppendSignal( const wxString &name, uint8_t mask[] );
	void SetBigEndial( bool bigEndial );
	SignalMap *GetSelected() const;
protected:
	void InitComponent();
	bool UniqueMask( uint8_t lhs[], uint8_t rhs[] );
	void SendReport( const wxString &signalName );
	virtual wxSize DoGetBestSize() const;
	const SignalMap *FindSignal( int offset, int bit ) const;
	SignalMap *FindSignal( int offset, int bit );
	int CalculateSignal( const SignalMap *signal );
	bool IsBitSelected( int px, int py, int &offset, int &bit ) const;
protected:
	void OnPaint( wxPaintEvent &event );
	void OnSize( wxSizeEvent &event );
	void OnLeftDClick( wxMouseEvent &event );
	void OnLeftClick( wxMouseEvent &event );
private:
	int paddingX, paddingY;
	bool bigEndial;
	int count;
	SignalMap signals[64];
	SignalMap *selected;
	uint8_t value[8];
	int size; // remove
};

inline void BitMapCtrl::ClearSignal() {
	count = 0;
	Refresh();
}
