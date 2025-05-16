#include "DBCFile.h"
#include "Lexer.h"
#include "Precompiled.h"
#ifdef _WIN32
#pragma hdrstop
#endif

int dbcFileIndex = 0;

wxDBCFile::wxDBCFile() : wxDBCAttr( this, OBJ_TYPE_FILE ) {
	baudrate = -1; // vs 500KBPS
	btr1 = btr2 = 0;
	modified = false;
	name = wxString::Format( "DBC-%d", dbcFileIndex++ );
}

wxDBCFile::~wxDBCFile() {
	CloseFile();
}

template<typename Type> void DeleteAllContent( wxVector<Type> &list ) {
	for( int i = 0; i < list.size(); i++ ) {
		delete list[i];
	}
	list.clear();
}

void wxDBCFile::CloseFile() {
	version.clear();
	DeleteAllContent( nodes );
	DeleteAllContent( tables );
	DeleteAllContent( messages );
	DeleteAllContent( defines );
}

void wxDBCFile::ParseBaudrate( wxParser &src ) {
	wxToken token;
	src.ExpectToken( ":" );
	if( !src.ReadOnLine( token ) ) {
		return;
	}
	baudrate = token.GetInt();
	src.ExpectToken( ":" );
	btr1 = src.ReadInt();
	src.ExpectToken( "," );
	btr2 = src.ReadInt();
}

void wxDBCFile::ParseNodes( wxParser &src ) {
	wxToken token;
	src.ExpectToken( ":" );
	while( src.ReadOnLine( token ) ) {
		AllocNode( token );
	}
}

void wxDBCFile::ParseSignal( wxParser &src, wxDBCMessage *message ) {
	wxString name = src.ReadName();
	wxString multiplexer = wxEmptyString;
	if( !src.CheckToken( ":" ) ) {
		multiplexer = src.ReadName();
		src.ExpectToken( ":" );
	}
	wxDBCSignal *signal = new wxDBCSignal( this, name );
	signal->start_bit = src.ReadInt();
	src.ExpectToken( "|" );
	signal->bit_len = src.ReadInt();
	signal->flags = 0;
	src.ExpectToken( "@" );
	int byte_order = src.ReadInt(); // (0 = little endian (intel), 1 = big endian (motorola))
	// wxPrintf(" --- byte_order: %d\n", byte_order);
	if( byte_order == 0 ) {
		signal->flags |= SF_BIG_ENDIAN;
	}
	if( src.CheckToken( "-" ) ) {
		signal->flags |= SF_SIGNED_TYPE;
	} else {
		src.ExpectToken( "+" );
	}
	if( src.CheckToken( "(" ) ) {
		signal->scale = src.ReadFloat();
		src.ExpectToken( "," );
		signal->offset = src.ReadFloat();
		src.ExpectToken( ")" );
	}
	if( src.CheckToken( "[" ) ) {
		signal->min = src.ReadFloat();
		src.ExpectToken( "|" );
		signal->max = src.ReadFloat();
		src.ExpectToken( "]" );
	}
	signal->unit = src.ReadString();
	do {
		wxString name = src.ReadName();
		signal->receivers.push_back( name );
	} while( src.CheckToken( "," ) );
	signal->multiplexer = multiplexer;
	message->signals.push_back( signal );
}

void wxDBCFile::ParseMessage( wxParser &src ) {
	wxToken token;

	int can = src.ReadInt();
	wxString name = src.ReadName();
	src.ExpectToken( ":" );
	int size = src.ReadInt();
	if( !src.ReadOnLine( token ) ) {
		token = "Vector__XXX"; // default
	}
	wxDBCMessage *message = new wxDBCMessage( this, can, name, size, token );
	while( src.CheckToken( "SG_" ) ) {
		ParseSignal( src, message );
	}
	messages.push_back( message );
}

void wxDBCFile::ParseMessageTransmitter( wxParser &src ) {
	uint32_t message_id = src.ReadInt();
	wxDBCMessage *message = FindMessage( message_id );
	if( !message ) {
		wxPrintf( "message transmitter could not be found: 0x%x\n", message_id );
		src.SkipOnLine();
		return;
	}
	src.ExpectToken( ":" );
	do {
		wxString name = src.ReadName();
		message->transmitter.push_back( name );
	} while( src.CheckToken( "," ) );
	src.ExpectToken( ";" );
}

void wxDBCFile::ParseSignalValues( wxParser &src ) {
	uint32_t message_id = src.ReadInt();
	wxString signal_name = src.ReadName();

	wxDBCSignal *signal = FindSignal( message_id, signal_name );
	if( !signal ) {
		wxPrintf( "signal could not be found: 0x%x %s\n", message_id, signal_name );
		src.SkipOnLine();
		return;
	}
	while( !src.CheckToken( ";" ) ) {
		int value = src.ReadInt();
		wxString desc = src.ReadString();
		signal->values.insert( wxValueMap::value_type( value, desc ) );
	}
}

int wxDBCFile::ParseObjectType( wxParser &src ) const {
	if( src.CheckToken( "EV_" ) ) {
		return OBJ_TYPE_ENV;
	} else if( src.CheckToken( "BU_" ) ) {
		return OBJ_TYPE_NODE;
	} else if( src.CheckToken( "BO_" ) ) {
		return OBJ_TYPE_MESSAGE;
	} else if( src.CheckToken( "SG_" ) ) {
		return OBJ_TYPE_SIGNAL;
	}
	return OBJ_TYPE_FILE;
}

wxString wxDBCFile::GetObjectType( int type ) const {
	switch( type ) {
	case OBJ_TYPE_ENV:
		return "EV_";
	case OBJ_TYPE_NODE:
		return "BU_";
	case OBJ_TYPE_MESSAGE:
		return "BO_";
	case OBJ_TYPE_SIGNAL:
		return "SG_";
	default:
		break;
	}
	return wxEmptyString;
}

void wxDBCFile::ParseComment( wxParser &src ) {
	int object = ParseObjectType( src );
	if( object == OBJ_TYPE_ENV ) {
		src.SkipOnLine();
		return;
	}
	wxDBCComment *comment = this;
	if( object == OBJ_TYPE_NODE ) {
		wxString node_name = src.ReadName();
		comment = FindNode( node_name );
	} else if( object == OBJ_TYPE_MESSAGE ) {
		uint32_t message_id = src.ReadInt(); // message id
		comment = FindMessage( message_id );
	} else if( object == OBJ_TYPE_SIGNAL ) {
		uint32_t message_id = src.ReadInt();   // message id
		wxString signal_name = src.ReadName(); // signal name
		comment = FindSignal( message_id, signal_name );
	}
	if( !comment ) {
		wxPrintf( "Failed Comment: %d\n", src.line );
		src.SkipOnLine();
		return;
	}
	comment->comment = src.ReadString();
	src.ExpectToken( ";" );
}

void wxDBCFile::ParseAttrDefine( wxParser &src ) {
	int object = ParseObjectType( src );
	if( object == OBJ_TYPE_ENV ) {
		src.SkipOnLine();
		return;
	}
	wxString name = src.ReadString();
	wxDBCAttrDefine *define = new wxDBCAttrDefine( this, object, name );
	if( src.CheckToken( "INT" ) ) {
		int min = src.ReadInt();
		int max = src.ReadInt();
		define->SetIntRange( min, max );
		define->type = ATTR_TYPE_INT;
	} else if( src.CheckToken( "HEX" ) ) {
		int min = src.ReadInt();
		int max = src.ReadInt();
		define->SetIntRange( min, max );
		define->type = ATTR_TYPE_HEX;
	} else if( src.CheckToken( "FLOAT" ) ) {
		float min = src.ReadFloat();
		float max = src.ReadFloat();
		define->SetFloatRange( min, max );
		define->type = ATTR_TYPE_FLOAT;
	} else if( src.CheckToken( "ENUM" ) ) {
		define->type = ATTR_TYPE_ENUM;
		do {
			wxString value = src.ReadString();
			define->AppendValue( value );
		} while( src.CheckToken( "," ) );
	} else if( src.CheckToken( "STRING" ) ) {
		define->type = ATTR_TYPE_STRING;
	}
	src.ExpectToken( ";" );
	defines.push_back( define );
}

void wxDBCFile::ParseAttrDefault( wxParser &src ) {
	wxString name = src.ReadString();
	wxDBCAttrDefine *define = FindDefine( name );
	if( !define ) {
		src.SkipOnLine();
		return;
	}
	if( define->type == ATTR_TYPE_ENUM || define->type == ATTR_TYPE_STRING ) {
		define->value = src.ReadString();
	} else if( define->type == ATTR_TYPE_HEX || define->type == ATTR_TYPE_INT ) {
		int value = src.ReadInt();
		define->value = wxString::Format( "%d", value );
	} else if( define->type == ATTR_TYPE_FLOAT ) {
		float value = src.ReadFloat();
		define->value = wxString::Format( "%.8f", value );
	}
	define->has_default = true;
	src.ExpectToken( ";" );
}

void wxDBCFile::ParseAttrValue( wxParser &src ) {
	wxString name = src.ReadString();
	wxDBCAttrDefine *define = FindDefine( name );
	if( !define ) {
		src.SkipOnLine();
		return;
	}
	int object = ParseObjectType( src );
	wxDBCAttr *attr = this;
	if( object == OBJ_TYPE_NODE ) {
		wxString node_name = src.ReadName();
		attr = FindNode( node_name );
	} else if( object == OBJ_TYPE_MESSAGE ) {
		uint32_t message_id = src.ReadInt(); // message id
		attr = FindMessage( message_id );
	} else if( object == OBJ_TYPE_SIGNAL ) {
		uint32_t message_id = src.ReadInt();   // message id
		wxString signal_name = src.ReadName(); // signal name
		attr = FindSignal( message_id, signal_name );
	}
	if( !attr ) {
		src.SkipOnLine();
		return;
	}
	if( define->type == ATTR_TYPE_ENUM ) {
		int index = src.ReadInt();
		wxString value = define->enums[index];
		attr->SetAttr( name, value );
	} else if( define->type == ATTR_TYPE_STRING ) {
		wxString value = src.ReadString();
		attr->SetAttr( name, value );
	} else if( define->type == ATTR_TYPE_HEX || define->type == ATTR_TYPE_INT ) {
		int value = src.ReadInt();
		attr->SetAttr( name, wxString::Format( "%d", value ) );
	} else if( define->type == ATTR_TYPE_FLOAT ) {
		float value = src.ReadFloat();
		attr->SetAttr( name, wxString::Format( "%.8f", value ) );
	}
	src.ExpectToken( ";" );
}

void wxDBCFile::ParseSignalExtType( wxParser &src ) {
	uint32_t message_id = src.ReadInt();   // message id
	wxString signal_name = src.ReadName(); // signal name

	wxDBCSignal *signal = FindSignal( message_id, signal_name );
	if( !signal ) {
		src.SkipOnLine();
		return;
	}
	// src.ExpectToken(":");
	if( src.CheckToken( ":" ) ) {
		// wxPrintf( "Skip token -> wxDBCFile::ParseSignalExtType\n" );
	}
	int type = src.ReadInt();
	src.ExpectToken( ";" );
	signal->flags &= ~( SF_EXT_TYPE_MASK ); // remove all
	if( type == 1 ) {
		signal->flags |= SF_FLOAT_EXT_TYPE;
	} else if( type == 2 ) {
		signal->flags |= SF_DOUBLE_EXT_TYPE;
	} else if( type == 3 ) {
		signal->flags |= SF_STRING_EXT_TYPE;
	}
}

bool wxDBCFile::LoadFile( const wxString &filename ) {
	wxParser src;
	if( !src.LoadFile( filename ) ) {
		return false;
	}
	try {
		wxToken token;
		while( src.ReadToken( token ) ) {
			if( token == "VERSION" ) {
				version = src.ReadString();
				continue;
			}
			if( token == "BS_" && token.IsLineStart() ) {
				ParseBaudrate( src );
				continue;
			}
			if( token == "BU_" && token.IsLineStart() ) {
				ParseNodes( src );
				continue;
			}
			if( token == "VAL_TABLE_" && token.IsLineStart() ) {
				wxDBCValueTable *table = new wxDBCValueTable( this );
				table->ParseFile( src );
				tables.push_back( table );
				continue;
			}
			if( token == "BO_" && token.IsLineStart() ) {
				ParseMessage( src );
				continue;
			}
			if( token == "BO_TX_BU_" && token.IsLineStart() ) {
				ParseMessageTransmitter( src );
				continue;
			}
			if( token == "VAL_" && token.IsLineStart() ) {
				ParseSignalValues( src );
				continue;
			}
			if( token == "CM_" && token.IsLineStart() ) {
				ParseComment( src );
				continue;
			}
			if( token == "BA_DEF_" && token.IsLineStart() ) {
				ParseAttrDefine( src );
				continue;
			}
			if( token == "BA_DEF_DEF_" && token.IsLineStart() ) {
				ParseAttrDefault( src );
				continue;
			}
			if( token == "BA_" && token.IsLineStart() ) {
				ParseAttrValue( src );
				continue;
			}
			if( token == "SIG_VALTYPE_" && token.IsLineStart() ) {
				ParseSignalExtType( src );
				continue;
			}
			src.SkipOnLine();
		}
		std::sort( nodes.begin(), nodes.end(), &NodeCompare );
		std::sort( messages.begin(), messages.end(), &MessageCompare );
		this->filename = filename;
		this->name = wxFileName( filename ).GetName();
		// Print();
	} catch( wxException &e ) {
		e.Print();
		error = e.ToString();
		return false;
	}
	return true;
}

void wxDBCFile::Print() const {
	wxPrintf( "Version: %s\n", version );
	if( baudrate != -1 ) {
		wxPrintf( "BS %d:%d,%d\n", baudrate, btr1, btr2 );
	}
	for( int i = 0; i < nodes.size(); i++ ) {
		nodes[i]->Print();
	}
	for( int i = 0; i < tables.size(); i++ ) {
		tables[i]->Print();
	}
	for( int i = 0; i < messages.size(); i++ ) {
		messages[i]->Print();
	}
	for( int i = 0; i < defines.size(); i++ ) {
		defines[i]->Print();
	}
	fflush( stdout );
}

void wxDBCFile::SaveFile( const wxString &filename ) {
	wxWriter writer;
	if( !writer.OpenFile( filename ) ) {
		return;
	}
	writer.Append( "VERSION " ).Append( version, true ).EndLine( 2 );
#if 1
	const wxString strNamespace[] = { "CM_", "BA_DEF_", "BA_", "BA_DEF_DEF_", "VAL_", "VAL_TABLE_", "BO_TX_BU_", "SIG_VALTYPE_" };
	writer.Append( "NS_:" ).EndLine();
	for( int i = 0; i < sizeof( strNamespace ) / sizeof( strNamespace[0] ); i++ ) {
		writer.Append( "  " ).Append( strNamespace[i] ).EndLine();
	}
	writer.EndLine( 2 );
#endif

	if( baudrate == -1 ) {
		writer.Append( "BS_:" ).EndLine( 2 );
	} else {
		writer.Append( "BS_: " ).Append( baudrate ).Append( ":" ).Append( btr1 ).Append( "," ).Append( btr2 ).EndLine( 2 );
	}
	writer.Append( "BU_:" );
	for( int i = 0; i < nodes.size(); i++ ) {
		const wxDBCNode *node = nodes[i];
		const wxString &name = node->GetName();
		writer.Append( " " ).Append( name );
	}
	writer.EndLine( 2 );

	for( int i = 0; i < messages.size(); i++ ) {
		messages[i]->SaveFile( writer, true );
	}
	writer.EndLine( 2 );

	for( int i = 0; i < tables.size(); i++ ) {
		tables[i]->SaveFile( writer );
	}

	for( int i = 0; i < messages.size(); i++ ) {
		const wxDBCMessage *message = messages[i];
		if( message->transmitter.size() == 0 ) {
			continue;
		}
		wxString list = wxJoin( message->transmitter, ',', 0 );
		writer.Append( "BO_TX_BU_ " ).Append( message->ID ).Append( ":" ).Append( list ).Append( ";" ).EndLine();
	}
	WriteSignalValue( writer );
	WriteComments( writer );
	WriteAttrDefines( writer, true );
	WriteAttrValues( writer );
	WriteSignalExtType( writer );
	writer.CloseFile();

	this->filename = filename;
	this->name = wxFileName( filename ).GetName();
}

void wxDBCFile::WriteComments( wxWriter &writer ) {
	if( this->HasComment() ) {
		writer.Append( "CM_ " ).Append( comment, true ).Append( ";" ).EndLine();
	}
	for( int i = 0; i < nodes.size(); i++ ) {
		const wxDBCNode *node = nodes[i];
		if( !node->HasComment() ) {
			continue;
		}
		writer.Append( "CM_ BU_ " ).Append( node->name ).Append( " " ).Append( node->comment, true ).Append( ";" ).EndLine();
	}
	for( int i = 0; i < messages.size(); i++ ) {
		const wxDBCMessage *message = messages[i];
		if( message->HasComment() ) {
			writer.Append( "CM_ BO_ " ).Append( message->ID ).Append( " " ).Append( message->comment, true ).Append( ";" ).EndLine();
		}
		for( int j = 0; j < message->signals.size(); j++ ) {
			const wxDBCSignal *signal = message->signals[j];
			if( signal->HasComment() ) {
				writer.Append( "CM_ SG_ " ).Append( message->ID ).Append( " " ).Append( signal->name ).Append( " " ).Append( signal->comment, true ).Append( ";" ).EndLine();
			}
		}
	}
}

void wxDBCFile::WriteSignalValue( wxWriter &writer ) {
	for( int i = 0; i < messages.size(); i++ ) {
		const wxDBCMessage *message = messages[i];
		for( int j = 0; j < message->signals.size(); j++ ) {
			const wxDBCSignal *signal = message->signals[j];
			if( signal->values.empty() ) {
				continue;
			}
			writer.Append( "VAL_ " ).Append( message->ID ).Append( " " ).Append( signal->name );
			for( wxValueMap::const_iterator iter = signal->values.begin(); iter != signal->values.end(); iter++ ) {
				writer.Append( " " ).Append( iter->first ).Append( " " ).Append( iter->second, true );
			}
			writer.Append( ";" ).EndLine();
		}
	}
}

void wxDBCFile::WriteAttrDefines( wxWriter &writer, bool includeDefaults ) {
	for( int i = 0; i < defines.size(); i++ ) {
		wxDBCAttrDefine *define = defines[i];
		if( define->object == OBJ_TYPE_ENV ) {
			continue;
		}
		const wxString strObject[] = { wxEmptyString, "BU_", "BO_", "SG_", "EV_" };
		const wxString strType[] = { "STRING", "INT", "HEX", "FLOAT", "ENUM" };
		writer.Append( "BA_DEF_ " ).Append( strObject[define->object] ).Append( " " ).Append( define->name, true ).Append( " " ).Append( strType[define->type] );
		if( define->type == ATTR_TYPE_INT || define->type == ATTR_TYPE_HEX || define->type == ATTR_TYPE_FLOAT ) {
			writer.Append( " " ).Append( define->min ).Append( " " ).Append( define->max );
		} else if( define->type == ATTR_TYPE_ENUM ) {
			writer.Append( " " );
			for( int i = 0; i < define->enums.size(); i++ ) {
				writer.Append( define->enums[i], true );
				if( i + 1 != define->enums.size() ) {
					writer.Append( "," );
				}
			}
		}
		writer.Append( ";" ).EndLine();
	}

	for( int i = 0; i < defines.size() && includeDefaults; i++ ) {
		wxDBCAttrDefine *define = defines[i];
		if( define->object == OBJ_TYPE_ENV ) {
			continue;
		}
		if( !define->has_default ) {
			continue;
		}
		int quote = false;
		if( define->type == ATTR_TYPE_ENUM || define->type == ATTR_TYPE_STRING ) {
			quote = true;
		}
		writer.Append( "BA_DEF_DEF_ " ).Append( define->name, true ).Append( " " ).Append( define->value, quote ).Append( ";" ).EndLine();
	}
}

void wxDBCFile::WriteAttrValues( wxWriter &writer ) {
	for( wxStrMap::const_iterator iter = attr.begin(); iter != attr.end(); iter++ ) {
		wxDBCAttrDefine *define = FindDefine( iter->first );
		if( !define ) {
			continue;
		}
		bool quote = false;
		wxString value = iter->second;
		if( define->type == ATTR_TYPE_ENUM ) {
			int index = define->enums.Index( value );
			value = wxString::Format( "%d", index );
		} else if( define->type == ATTR_TYPE_STRING ) {
			quote = true;
		}
		writer.Append( "BA_ " ).Append( iter->first, true ).Append( " " ).Append( value, quote ).Append( ";" ).EndLine();
	}

	for( int i = 0; i < nodes.size(); i++ ) {
		const wxDBCNode *node = nodes[i];
		for( wxStrMap::const_iterator iter = node->attr.begin(); iter != node->attr.end(); iter++ ) {
			wxDBCAttrDefine *define = FindDefine( iter->first );
			if( !define ) {
				continue;
			}
			bool quote = false;
			wxString value = iter->second;
			if( define->type == ATTR_TYPE_ENUM ) {
				int index = define->enums.Index( value );
				value = wxString::Format( "%d", index );
			} else if( define->type == ATTR_TYPE_STRING ) {
				quote = true;
			}
			writer.Append( "BA_ " ).Append( iter->first, true ).Append( " BU_ " ).Append( node->name ).Append( " " ).Append( value, quote ).Append( ";" ).EndLine();
		}
	}
	for( int i = 0; i < messages.size(); i++ ) {
		const wxDBCMessage *message = messages[i];

		for( wxStrMap::const_iterator iter = message->attr.begin(); iter != message->attr.end(); iter++ ) {
			wxDBCAttrDefine *define = FindDefine( iter->first );
			if( !define ) {
				continue;
			}
			bool quote = false;
			wxString value = iter->second;
			if( define->type == ATTR_TYPE_ENUM ) {
				int index = define->enums.Index( value );
				value = wxString::Format( "%d", index );
			} else if( define->type == ATTR_TYPE_STRING ) {
				quote = true;
			}
			writer.Append( "BA_ " ).Append( iter->first, true ).Append( " BO_ " ).Append( message->ID ).Append( " " ).Append( value, quote ).Append( ";" ).EndLine();
		}
		for( int j = 0; j < message->signals.size(); j++ ) {
			const wxDBCSignal *signal = message->signals[j];
			for( wxStrMap::const_iterator iter = signal->attr.begin(); iter != signal->attr.end(); iter++ ) {
				wxDBCAttrDefine *define = FindDefine( iter->first );
				if( !define ) {
					continue;
				}
				bool quote = false;
				wxString value = iter->second;
				if( define->type == ATTR_TYPE_ENUM ) {
					int index = define->enums.Index( value );
					value = wxString::Format( "%d", index );
				} else if( define->type == ATTR_TYPE_STRING ) {
					quote = true;
				}
				writer.Append( "BA_ " ).Append( iter->first, true ).Append( " SG_ " ).Append( message->ID ).Append( " " ).Append( signal->name ).Append( " " ).Append( value, quote ).Append( ";" ).EndLine();
			}
		}
	}
}

void wxDBCFile::WriteSignalExtType( wxWriter &writer ) {
	for( int i = 0; i < messages.size(); i++ ) {
		const wxDBCMessage *message = messages[i];
		for( int j = 0; j < message->signals.size(); j++ ) {
			const wxDBCSignal *signal = message->signals[j];
			if( !signal->HasExtType() ) {
				continue;
			}
			int type = 0;
			if( signal->flags & SF_FLOAT_EXT_TYPE ) {
				type = 1;
			} else if( signal->flags & SF_DOUBLE_EXT_TYPE ) {
				type = 2;
			} else if( signal->flags & SF_STRING_EXT_TYPE ) {
				type = 3;
			}
			writer.Append( "SIG_VALTYPE_ " ).Append( message->ID ).Append( " " ).Append( signal->name ).Append( ":" ).Append( type ).Append( ";" ).EndLine();
		}
	}
}

wxDBCMessage *wxDBCFile::FindMessage( uint32_t message_id ) const {
	for( int i = 0; i < messages.size(); i++ ) {
		wxDBCMessage *message = messages[i];
		if( message->ID == message_id ) {
			return message;
		}
	}
	return nullptr;
}

wxDBCSignal *wxDBCFile::FindSignal( uint32_t message_id, const wxString &signal_name ) const {
	wxDBCMessage *message = FindMessage( message_id );
	if( message ) {
		for( int i = 0; i < message->signals.size(); i++ ) {
			wxDBCSignal *signal = message->signals[i];
			if( signal->name == signal_name ) {
				return signal;
			}
		}
	}
	return nullptr;
}

wxDBCAttrDefine *wxDBCFile::FindDefine( const wxString &name ) const {
	for( int i = 0; i < defines.size(); i++ ) {
		wxDBCAttrDefine *define = defines[i];
		if( define->name == name ) {
			return define;
		}
	}
	return nullptr;
}

wxDBCValueTable *wxDBCFile::FindTable( const wxString &name ) const {
	for( int i = 0; i < tables.size(); i++ ) {
		wxDBCValueTable *table = tables[i];
		if( table->name == name ) {
			return table;
		}
	}
	return nullptr;
}

wxDBCNode *wxDBCFile::FindNode( const wxString &name ) const {
	for( int i = 0; i < nodes.size(); i++ ) {
		wxDBCNode *node = nodes[i];
		if( node->name == name ) {
			return node;
		}
	}
	return nullptr;
}

wxDBCNode *wxDBCFile::AllocNode( const wxString &name ) {
	wxDBCNode *node = FindNode( name );
	if( !node ) {
		node = new wxDBCNode( this, name );
		nodes.push_back( node );
	}
	return node;
}

wxArrayString wxDBCFile::GetMessageList() const {
	wxArrayString list;
	for( int i = 0; i < messages.size(); i++ ) {
		wxDBCMessage *message = messages[i];
		list.push_back( wxString::Format( "%X - %s", message->ID, message->name ) );
	}
	return list;
}

wxArrayString wxDBCFile::GetSignalList( uint32_t message_id, bool skipValid ) const {
	wxArrayString list;
	wxDBCMessage *message = FindMessage( message_id );
	if( message ) {
		for( int i = 0; i < message->signals.size(); i++ ) {
			wxDBCSignal *signal = message->signals[i];
			if( skipValid && signal->name.EndsWith( "V" ) ) {
				continue;
			}
			list.push_back( signal->name );
		}
	}
	return list;
}