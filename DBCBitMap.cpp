#include "DBCBitMap.h"
#ifdef _WIN32
#pragma hdrstop
#endif

// clang-format off
wxDEFINE_EVENT(wxEVT_BITMAP_SELECTED, BitMapEvent);
wxIMPLEMENT_DYNAMIC_CLASS(BitMapCtrl, wxControl);
wxBEGIN_EVENT_TABLE(BitMapCtrl, wxControl)
	EVT_PAINT(BitMapCtrl::OnPaint)
	EVT_SIZE(BitMapCtrl::OnSize)
	EVT_LEFT_DCLICK(BitMapCtrl::OnLeftDClick)
	EVT_LEFT_DOWN(BitMapCtrl::OnLeftClick)
wxEND_EVENT_TABLE()
    // clang-format on

    BitMapEvent::BitMapEvent( wxEventType eventType, int winid, const wxString &signalName )
    : wxEvent( winid, eventType ) {
	this->signalName = signalName;
}

BitMapCtrl::BitMapCtrl() {
	InitComponent();
}

BitMapCtrl::BitMapCtrl( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) {
	InitComponent();
	Create( parent, id, pos, size, style );
}

bool BitMapCtrl::Create( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) {
	if( !wxControl::Create( parent, id, pos, size, style ) ) {
		return false;
	}
	DoGetBestSize();
	return true;
}

wxSize BitMapCtrl::DoGetBestSize() const {
	return wxSize( 400, 400 );
}

void BitMapCtrl::InitComponent() {
	paddingX = 64;
	paddingY = 34;
	count = 0;
	size = 8;
	selected = nullptr;
	bigEndial = true;
	memset( value, 0x0, sizeof value );
}

bool BitMapCtrl::UniqueMask( uint8_t lhs[], uint8_t rhs[] ) {
	for( int i = 0; i < 8; i++ ) {
		if( ( lhs[i] & rhs[i] ) != 0 ) {
			return false;
		}
	}
	return true;
}

static const wxColor bitmapColors[] = { wxColor( 250, 128, 114 ), wxColor( 255, 159, 0 ), wxColor( 255, 255, 0 ), wxColor( 157, 194, 9 ), wxColor( 176, 224, 230 ), wxColor( 135, 206, 250 ), wxColor( 216, 191, 216 ), wxColor( 241, 167, 254 ), wxColor( 251, 204, 231 ), wxColor( 255, 99, 71 ), wxColor( 255, 130, 67 ), wxColor( 255, 247, 0 ), wxColor( 232, 244, 140 ), wxColor( 164, 244, 249 ), wxColor( 64, 224, 208 ), wxColor( 179, 158, 181 ), wxColor( 223, 0, 255 ), wxColor( 114, 47, 55 ), wxColor( 155, 221, 255 ) };

bool BitMapCtrl::AppendSignal( const wxString &name, uint8_t mask[] ) {
	for( int i = 0; i < count; i++ ) {
		if( !UniqueMask( mask, signals[i].mask ) ) {
			return false;
		}
	}
	const int numCount = sizeof( bitmapColors ) / sizeof( wxColor );
	signals[count].name = name;
	signals[count].colour = bitmapColors[count % numCount];
	memcpy( signals[count].mask, mask, sizeof( SignalMap::mask ) );
	count++;
	Refresh( true, nullptr );
	return true;
}

void BitMapCtrl::SetBigEndial( bool bigEndial ) {
	this->bigEndial = bigEndial;
	Refresh( true, nullptr );
}

void BitMapCtrl::OnPaint( wxPaintEvent &event ) {
	const wxSize size = GetSize();
	wxPaintDC dc( this );
	// wxFont font( 9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT( "Courier 10 Pitch" ));
	// dc.SetFont( font );
	const int sx = ( size.x - paddingX - 4 ) / 8;
	const int sy = ( size.y - paddingY - 4 ) / 8;
	int px, py;

	dc.SetPen( *wxBLACK_PEN );
	dc.SetBrush( *wxTRANSPARENT_BRUSH );
	for( int i = 0; i < 8; i++ ) {
		px = ( i + 0.4f ) * sx + paddingX;
		py = paddingY * 0.5f;
		dc.DrawText( wxString::Format( "%02x", 7 - i ), px, py );

		px = paddingX * 0.1f;
		py = ( i + 0.4f ) * sy + paddingY;
		dc.DrawText( wxString::Format( "%d=%02x", i, value[i] ), px, py );
	}
	dc.DrawText( bigEndial ? "BE" : "LE", 8, 8 );

	for( int offset = 0; offset < 8; offset++ ) {
		for( int bit = 0; bit < 8; bit++ ) {
			px = bit * sx + paddingX;
			py = offset * sy + paddingY;
			dc.DrawRectangle( px, py, sx, sy );
		}
	}
	SignalMap *last = nullptr;
	for( int offset = 0; offset < 8; offset++ ) {
		for( int bit = 0; bit < 8; bit++ ) {
			SignalMap *signal = FindSignal( offset, 7 - bit );
			if( !signal ) {
				dc.SetBrush( *wxTRANSPARENT_BRUSH );
			} else if( signal == selected ) {
				dc.SetBrush( *wxRED_BRUSH );
			} else {
				dc.SetBrush( wxBrush( signal->colour ) );
			}
			dc.SetPen( *wxBLACK_PEN );
			px = bit * sx + paddingX;
			py = offset * sy + paddingY;
			dc.DrawRectangle( px + 1, py + 1, sx - 2, sy - 2 );

			px = ( bit + 0.1f ) * sx + paddingX;
			py = ( offset + 0.1f ) * sy + paddingY;
			dc.SetPen( *wxBLACK_PEN );
			dc.SetBrush( *wxTRANSPARENT_BRUSH );

			int mask = ( 1 << ( 7 - bit ) );
			bool state = ( value[offset] & mask ) != 0;
			dc.DrawText( wxString::Format( "%d", state ), px, py );
			if( signal && last != signal ) {
				int value = CalculateSignal( signal );

				// dc.SetPen( *wxWHITE );
				// dc.SetBrush( *wxWHITE_BRUSH );
				py = ( offset + 0.4f ) * sy + paddingY;
				// dc.SetTextForeground( *wxWHITE );
				dc.DrawText( wxString::Format( "%s=%d", signal->name, value ), px, py );
			}
			last = signal;
		}
	}
}

int BitMapCtrl::CalculateSignal( const SignalMap *signal ) {
	int result = 0;
	for( int offset = 0; offset < 8; offset++ ) {
		for( int bit = 0; bit < 8; bit++ ) {
			if( ( signal->mask[offset] & ( 1 << bit ) ) == 0 ) {
				continue;
			}
			// TODO big endial
			result <<= 1;
			int mask = ( 1 << bit );
			if( ( value[offset] & mask ) != 0 ) {
				result |= 0x01;
			}
		}
	}
	return result;
}

void BitMapCtrl::OnSize( wxSizeEvent &event ) {
	Refresh( true, nullptr );
}

bool BitMapCtrl::IsBitSelected( int px, int py, int &offset, int &bit ) const {
	const wxSize size = GetSize();
	const int sx = ( size.x - paddingX - 4 ) / 8;
	const int sy = ( size.y - paddingY - 4 ) / 8;
	int mx = px - ( paddingX + 4 );
	int my = py - ( paddingY + 4 );
	if( mx < 0 || my < 0 ) {
		return false;
	}
	offset = my / sy, bit = mx / sx;
	return true;
}

void BitMapCtrl::OnLeftDClick( wxMouseEvent &event ) {
	int offset, bit;
	if( !IsBitSelected( event.m_x, event.m_y, offset, bit ) ) {
		bigEndial ^= 0x01;
		Refresh( true, nullptr );
		return;
	}
	int mask = ( 1 << ( 7 - bit ) );
	if( ( value[offset] & mask ) != 0 ) {
		value[offset] &= ~mask;
	} else {
		value[offset] |= mask;
	}
	Refresh( true, nullptr );
}

void BitMapCtrl::OnLeftClick( wxMouseEvent &event ) {
	int offset, bit;
	if( !IsBitSelected( event.m_x, event.m_y, offset, bit ) ) {
		return;
	}
	SignalMap *signal = FindSignal( offset, bit );
	if( !signal ) {
		return;
	}
	SendReport( signal->name );
	Refresh( true, nullptr );
}

void BitMapCtrl::SendReport( const wxString &signalName ) {
	wxPrintf( "Selected Signal: %s\n", signalName );
	fflush( stdout );

	BitMapEvent event( wxEVT_BITMAP_SELECTED, GetId(), wxEmptyString );
	event.SetEventObject( this );
	ProcessWindowEvent( event );
}

const SignalMap *BitMapCtrl::FindSignal( int offset, int bit ) const {
	for( int i = 0; i < count; i++ ) {
		if( signals[i].mask[offset] & ( 1 << bit ) ) {
			return &signals[i];
		}
	}
	return nullptr;
}

SignalMap *BitMapCtrl::FindSignal( int offset, int bit ) {
	for( int i = 0; i < count; i++ ) {
		if( signals[i].mask[offset] & ( 1 << bit ) ) {
			return &signals[i];
		}
	}
	return nullptr;
}

SignalMap *BitMapCtrl::GetSelected() const {
	return selected;
}
