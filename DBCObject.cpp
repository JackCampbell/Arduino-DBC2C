#include "DBCFile.h"
#include "Lexer.h"
#include "Precompiled.h"
#ifdef _WIN32
#pragma hdrstop
#endif

// ================================================================================================================================
wxDBCNode::wxDBCNode( wxDBCFile *file, const wxString &name ) : wxDBCComment(), wxDBCAttr( file, OBJ_TYPE_NODE ) {
	this->name = name;
}

wxDBCNode::wxDBCNode( const wxDBCNode *copy ) : wxDBCComment(), wxDBCAttr( copy->file, OBJ_TYPE_NODE ) {
	name = copy->name;
	comment = copy->comment;
	file = copy->file;
}

void wxDBCNode::Print() const {
	wxPrintf( "Node: %s Comment: %s\n", name, comment );
}

// ================================================================================================================================
wxDBCValueTable::wxDBCValueTable( wxDBCFile *file ) {
	this->file = file;
}

wxDBCValueTable::wxDBCValueTable( const wxDBCValueTable *copy ) {
	file = copy->file;
	name = copy->name;
	map = copy->map;
}

void wxDBCValueTable::Clear() {
	map.clear();
}

void wxDBCValueTable::Append( int value, const wxString &desc ) {
	map.insert( wxValueMap::value_type( value, desc ) );
}

wxString wxDBCValueTable::GetDesc( int value ) const {
	wxValueMap::const_iterator iter = map.find( value );
	if( iter == map.end() ) {
		return wxEmptyString;
	}
	return iter->second;
}

void wxDBCValueTable::Print() const {
	wxPrintf( "Value Table: %s", name );
	for( wxValueMap::const_iterator iter = map.begin(); iter != map.end(); iter++ ) {
		wxPrintf( " [%d = %s]", iter->first, iter->second );
	}
	wxPrintf( "\n" );
}

wxString wxDBCValueTable::GetDesc() const {
	wxArrayString list;
	for( wxValueMap::const_iterator iter = map.begin(); iter != map.end(); iter++ ) {
		list.push_back( wxString::Format( "0x%02x=%s", iter->first, iter->second ) );
	}
	return wxJoin( list, ',', 0 );
}

void wxDBCValueTable::SaveFile( wxWriter &writer ) const {
	writer.Append( "VAL_TABLE_ " ).Append( name );
	for( wxValueMap::const_iterator iter = map.begin(); iter != map.end(); iter++ ) {
		writer.Append( " " ).Append( iter->first ).Append( " " ).Append( iter->second, true );
	}
	writer.Append( ";" ).EndLine();
}

bool wxDBCValueTable::ParseFile( wxParser &src ) {
	name = src.ReadName();
	while( !src.CheckToken( ";" ) ) {
		int value = src.ReadInt();
		wxString desc = src.ReadString();
		map.insert( wxValueMap::value_type( value, desc ) );
	}
	return true;
}

// ================================================================================================================================
wxDBCMessage::wxDBCMessage( wxDBCFile *file, int ID, const wxString &name, int size, const wxString &sender ) : wxDBCComment(), wxDBCAttr( file, OBJ_TYPE_MESSAGE ) {
	this->ID = ID;
	this->name = name;
	this->size = size;
	this->sender = sender;
}

wxDBCMessage::wxDBCMessage( const wxDBCMessage *copy ) : wxDBCComment(), wxDBCAttr( copy->file, OBJ_TYPE_MESSAGE ) {
	ID = copy->ID;
	name = copy->name;
	size = copy->size;
	sender = copy->sender;
	signals = copy->signals;
	transmitter = copy->transmitter;
}

void wxDBCMessage::Print() const {
	wxPrintf( "Message (%X) %s size:%d sender:%s", ID, name, size, sender );
	if( !transmitter.empty() ) {
		wxPrintf( " Transmitter: %s", wxJoin( transmitter, ',', 0 ) );
	}
	wxPrintf( " Comment:%s\n", comment );
	for( int i = 0; i < signals.size(); i++ ) {
		signals[i]->Print();
	}
}

void wxDBCMessage::SaveFile( wxWriter &writer, bool includeSignals ) const {
	writer.Append( "BO_ " ).Append( ID ).Append( " " ).Append( name ).Append( ": " ).Append( size ).Append( " " ).Append( sender ).EndLine();
	for( int j = 0; j < signals.size() && includeSignals; j++ ) {
		signals[j]->SaveFile( writer );
	}
	writer.EndLine();
}

// ================================================================================================================================
const wxString strObjectType[] = { "File", "Node", "Message", "Signal" };
const wxString strValueType[] = { "String", "Int", "Hex", "Float", "Enum" };

wxDBCAttrDefine::wxDBCAttrDefine( wxDBCFile *file, int object, const wxString &name ) {
	this->object = object;
	this->name = name;
	this->has_default = false;
	this->file = file;
	this->type = ATTR_TYPE_STRING;
}

wxDBCAttrDefine::wxDBCAttrDefine( const wxDBCAttrDefine *copy ) {
	this->object = copy->object;
	this->name = copy->name;
	this->has_default = copy->has_default;
	this->file = copy->file;
	this->type = copy->type;
}

wxDBCAttrDefine::~wxDBCAttrDefine() {
	file = nullptr;
}

void wxDBCAttrDefine::Print() const {
	wxString args = wxEmptyString;
	if( type == ATTR_TYPE_ENUM ) {
		args = wxJoin( enums, ',', 0 );
	} else if( type == ATTR_TYPE_INT || type == ATTR_TYPE_HEX || type == ATTR_TYPE_FLOAT ) {
		args = wxString::Format( "[min:%s, max:%s]", min, max );
	}
	if( has_default ) {
		args += wxString::Format( " [value: '%s']", value );
	}
	wxPrintf( "Attr Define: %s <%s> <%s> %s\n", name, strObjectType[object], strValueType[type], args );
}

void wxDBCAttrDefine::SetIntRange( int min, int max ) {
	this->min = wxString::Format( "%d", min );
	this->max = wxString::Format( "%d", max );
}

void wxDBCAttrDefine::SetFloatRange( float min, float max ) {
	this->min = wxString::Format( "%.8f", min );
	this->max = wxString::Format( "%.8f", max );
}

void wxDBCAttrDefine::AppendValue( const wxString &value ) {
	enums.push_back( value );
}

// ================================================================================================================================
wxDBCAttr::wxDBCAttr( wxDBCFile *file, int object ) {
	this->file = file;
	this->object = object;
}

wxDBCAttr::~wxDBCAttr() {
	file = nullptr;
}

wxString wxDBCAttr::FindAttr( const wxString &name, const wxString &defs ) const {
	wxStrMap::const_iterator iter = attr.find( name );
	if( iter == attr.end() ) {
		return defs;
	}
	return iter->second;
}

void wxDBCAttr::SetAttr( const wxString &name, const wxString &value ) {
	wxStrMap::iterator iter = attr.find( name );
	if( iter == attr.end() ) {
		attr.insert( wxStrMap::value_type( name, value ) );
	} else {
		iter->second = value;
	}
}

bool wxDBCAttr::RemoveAttr( const wxString &name ) {
	wxStrMap::iterator iter = attr.find( name );
	if( iter == attr.end() ) {
		return false;
	}
	attr.erase( iter );
	return true;
}

bool wxDBCAttr::HasAttr( const wxString &name ) const {
	wxStrMap::const_iterator iter = attr.find( name );
	if( iter != attr.end() ) {
		return true;
	}
	return false;
}

// ================================================================================================================================
wxDBCSignal::wxDBCSignal( wxDBCFile *file, const wxString &name ) : wxDBCComment(), wxDBCAttr( file, OBJ_TYPE_SIGNAL ) {
	this->name = name;
	this->multiplexer = wxEmptyString;
	this->start_bit = 0;
	this->bit_len = 0;
	this->flags = 0;
	this->scale = 1.0f;
	this->offset = 0.0f;
	this->min = 0.0f;
	this->max = 0.0f;
}

wxDBCSignal::wxDBCSignal( const wxDBCSignal *copy ) : wxDBCComment(), wxDBCAttr( copy->file, OBJ_TYPE_SIGNAL ) {
	this->name = copy->name;
	this->multiplexer = copy->multiplexer;
	this->start_bit = copy->start_bit;
	this->bit_len = copy->bit_len;
	this->flags = copy->flags;
	this->scale = copy->scale;
	this->offset = copy->offset;
	this->min = copy->min;
	this->max = copy->max;
	this->unit = copy->unit;
	this->receivers = copy->receivers;
	this->values = copy->values;
	this->comment = copy->comment;
}

wxDBCSignal::~wxDBCSignal() {
}

void wxDBCSignal::Print() const {
	wxString extended = "Integer";
	if( flags & SF_FLOAT_EXT_TYPE ) {
		extended = "32 IEEE-Float";
	}
	if( flags & SF_DOUBLE_EXT_TYPE ) {
		extended = "64 IEEE-Float";
	}
	wxPrintf( "    Signal %s (unit: %s) order:%s type:%s ext:%s Map:[%d:%d] Linear:%.2fx + %.2f Range: %.2f < x < %.2f Recv:%s Values:", name, unit, flags & SF_BIG_ENDIAN ? "Big Endian (motorola)" : "Little Endian (intel)", flags & SF_SIGNED_TYPE ? "Signed Type" : "Unsigned Type", extended, start_bit, bit_len, scale, offset, min, max, wxJoin( receivers, ',', 0 ) );
	for( wxValueMap::const_iterator iter = values.begin(); iter != values.end(); iter++ ) {
		wxPrintf( "[%d = %s]", iter->first, iter->second );
	}
	wxPrintf( " Comment '%s'\n", comment );
}

void wxDBCSignal::SaveFile( wxWriter &writer ) const {
	writer.Append( "  SG_ " ).Append( name );
	if( !multiplexer.empty() ) {
		writer.Append( " " ).Append( multiplexer );
	}
	writer.Append( ": " ).Append( start_bit ).Append( "|" ).Append( bit_len );
	writer.Append( "@" ).Append( flags & SF_BIG_ENDIAN ? "1" : "0" ).Append( flags & SF_SIGNED_TYPE ? "-" : "+" );
	writer.Append( " (" ).Append( scale ).Append( "," ).Append( offset ).Append( ")" );
	writer.Append( " [" ).Append( min ).Append( "|" ).Append( max ).Append( "]" );
	writer.Append( " " ).Append( unit, true );
	for( int i = 0; i < receivers.size(); i++ ) {
		writer.Append( " " ).Append( receivers[i] );
	}
	writer.EndLine();
}

static wxVector<int> be_bits = { 0 };
static bool be_init_bits = false;
void InitBigEndialMap() {
	if( be_init_bits ) {
		return;
	}
	be_bits.clear();
	for( int i = 0; i < 64; i++ ) {
		for( int j = 7; j >= 0; j-- ) {
			be_bits.push_back( j + i * 8 );
		}
	}
	be_init_bits = true;
}
bool wxDBCSignal::UpdateSignalBounds(int &lsb, int &msb) const {
	if( ( flags & SF_BIG_ENDIAN ) != 0 ) {
		wxVector<int>::const_iterator iter = std::find( be_bits.begin(), be_bits.end(), start_bit );
		lsb = be_bits[( iter - be_bits.begin() ) + bit_len - 1];
		msb = start_bit;
	} else {
		lsb = start_bit;
		msb = start_bit + bit_len - 1;
	}
	if( !( lsb < ( 64 * 8 ) && msb < ( 64 * 8 ) ) ) {
		wxPrintf( "Signal out of bounds: [%d:%d] -> [%d %d]\n", start_bit, bit_len, lsb, msb );
		return false;
	}
	return true;
}

void wxDBCSignal::SetValue(uint64_t value, uint8_t *buffer, const int msg_size) const {
	int msb, lsb;
	InitBigEndialMap();
	if( !UpdateSignalBounds( lsb, msb ) ) {
		return;
	}
	int i = lsb / 8;
	int bits = bit_len;
	if( bit_len < 64 ) {
		value &= ( ( 1ULL << bit_len ) - 1 );
	}
	while( i >= 0 && i < msg_size && bits > 0 ) {
		int shift = 0;
		if( (int)( lsb / 8 ) == i ) {
			shift = lsb % 8;
		}
		int size = MIN( bits, 8 - shift );
		buffer[i] &= ~( ( ( 1ULL << size ) - 1 ) << shift );
		buffer[i] |= ( value & ( ( 1ULL << size ) - 1 ) ) << shift;
		bits -= size;
		value >>= size;
		if( ( flags & SF_BIG_ENDIAN ) != 0 ) {
			i--;
		} else {
			i++;
		}
	}
}

wxString wxDBCSignal::GetCodeType() const {
	if(flags & SF_STRING_EXT_TYPE) {
		wxASSERT_MSG(false, "String signal value!\nFixME!!!!");
		return "char";
	}
	if(flags & SF_DOUBLE_EXT_TYPE) {
		return "double";
	}
	if(flags & SF_FLOAT_EXT_TYPE) {
		return "float";
	}
	if(flags & SF_SIGNED_TYPE) {
		return "int";
	}
	return "unsigned int";
}
// ================================================================================================================================
