#include "Lexer.h"
#ifdef _WIN32
#pragma hdrstop
#endif

std::string to_format( const char *format, ... ) {
	static char buffer[1024];
	va_list argptr;
	int size;

	va_start( argptr, format );
	size = vsnprintf( buffer, sizeof( buffer ), format, argptr );
	va_end( argptr );

	return std::string( buffer, size );
}

wxException::wxException( int line, const char *format, ... ) : line( line ) {
	va_list argptr;
	va_start( argptr, format );
	vsnprintf( buffer, sizeof( buffer ), format, argptr );
	va_end( argptr );
}

void wxException::Print() const {
	wxPrintf( "%04d: %s\n", line, buffer );
	fflush(stdout);
}

wxString wxException::ToString() const {
	return wxString::Format( "%04d: %s", line, buffer );
}

wxToken::wxToken() : wxString() {
	type = TOKEN_NAME;
	ivalue = 0;
	fvalue = 0.0f;
	whitespace_p = nullptr;
	script_p = nullptr;
	line_p = nullptr;
	line = 0;
}

wxString wxToken::GetComment() const {
	int size = script_p - line_p;
	return wxString(whitespace_p, size);
}

bool wxToken::IsLineStart() const {
	return line_p == script_p;
}

wxToken &wxToken::operator=(const char *rhs) {
	wxString::operator=(rhs);
	return *this;
}

wxParser::wxParser( int flags ) : flags( flags ) {
	script_p = end_p = nullptr;
	buffer = nullptr;
	line_p = nullptr;
	line = size = 0;
}

wxParser::~wxParser() {
	FreeSource();
}

bool wxParser::LoadFile( const wxString &filename ) {
	FILE *stream = fopen( filename.c_str(), "rb" );
	if( !stream ) {
		return false;
	}
	fseek( stream, 0, SEEK_END );
	size = ftell( stream );
	rewind( stream );

	buffer = (char *)malloc( sizeof( char ) * ( size + 1 ) );
	fread( buffer, sizeof( char ), size, stream );
	fclose( stream );

	buffer[size] = '\0';
	script_p = &buffer[0];
	end_p = &buffer[size];

	line = 0;
	line_p = script_p;
	return true;
}

void wxParser::FreeSource() {
	if( buffer ) {
		free( buffer );
		buffer = nullptr;
	}
	size = 0;
	script_p = end_p = nullptr;
}

void wxParser::SkipWhitespace() {
	while( *script_p && isspace( *script_p ) ) {
		if( *script_p == 0 ) {
			break;
		}
		if( *script_p == '\n' ) {
			line_p = ++script_p, line++;
			continue;
		}
		script_p++;
	}
}

bool wxParser::ParseName( wxToken &token ) {
	if( !isalpha( *script_p ) ) {
		return false;
	}
	while( isalpha( *script_p ) || isdigit( *script_p ) || *script_p == '_' ) {
		if( *script_p == '\0' ) {
			break;
		}
		token += *script_p++;
	}
	token.type = TOKEN_NAME;
	return true;
}

char wxParser::KeepChar(int ch) {
	if(ch == '\'') { return '\''; }
	if(ch == '\"') { return '\"'; }
	if(ch == 't') { return '\t'; }
	if(ch == 'n') { return '\n'; }
	if(ch == 'r') { return '\r'; }
	return ch;
}

bool wxParser::ParseString( wxToken &token ) {
	if( !( *script_p == '\"' || *script_p == '\'' ) ) {
		return false;
	}
	char end = *script_p++;
	while( *script_p != end ) {
		wxChar ch = *script_p++;
		
		if(ch == '\0') {
			break;
		}
		if(ch == '\\') {
			ch = KeepChar(*script_p++);
		} else if(ch == '\n') {
			line_p = script_p + 1;
			line++;
		}
		token += ch;
	}
	script_p++;
	if(end == '\"') {
		token.type = TOKEN_STRING;
	} else {
		token.type = TOKEN_LITERAL;
	}
	return true;
}

bool wxParser::ParseNumber( wxToken &token ) {
	bool is_float = *script_p == '.' && isdigit( *script_p + 1 );
	if( !( isdigit( *script_p ) || is_float ) ) {
		return false;
	}
	int value = 0;
	while( isdigit( *script_p ) ) {
		value = value * 10 + ( *script_p - '0' );
		token += *script_p++;
	}
	if( value == 0 && *script_p == 'x' ) {
		token += *script_p++;
		while( isxdigit( *script_p ) ) {
			if( isdigit( *script_p ) ) {
				value = value * 16 + ( *script_p - '0' );
			} else {
				value = value * 16 + ( 10 + ( tolower( *script_p ) - 'a' ) );
			}
			token += *script_p++;
		}
		token.ivalue = value;
		token.fvalue = value;
		token.type = TOKEN_INT;
		return true;
	} else if( value == 0 && *script_p == 'o' ) {
		token += *script_p++;
		while( isdigit( *script_p ) ) {
			value = value * 8 + ( *script_p - '0' );
			token += *script_p++;
		}
		token.ivalue = value;
		token.fvalue = value;
		token.type = TOKEN_INT;
		return true;
	} else if( value == 0 && *script_p == 'b' ) {
		token += *script_p++;
		while( isdigit( *script_p ) ) {
			value = value * 2 + ( *script_p - '0' );
			token += *script_p++;
		}
		token.ivalue = value;
		token.fvalue = value;
		token.type = TOKEN_INT;
		return true;
	}

	float real = value;
	if( *script_p == '.' ) {
		token += *script_p++;
		int mantis = 1;
		while( isdigit( *script_p ) ) {
			real += powf( 0.1f, mantis++ ) * ( *script_p - '0' );
			token += *script_p++;
		}
		if( tolower( *script_p ) == 'e' ) {
			token += *script_p++;
			int sign = 1;
			if( *script_p == '+' ) {
				token += *script_p++;
				sign = 1;
			} else if( *script_p == '-' ) {
				token += *script_p++;
				sign = -1;
			}
			int base = 0;
			while( isdigit( *script_p ) ) {
				base = ( base * 10 ) + ( *script_p - '0' );
				token += *script_p++;
			}
			real *= powf( 10, sign * base );
		}
		token.fvalue = real;
		token.ivalue = value;
		token.type = TOKEN_FLOAT;
	}
	if( tolower( *script_p ) == 'e' ) {
		token += *script_p++;
		int sign = 1;
		if( *script_p == '+' ) {
			token += *script_p++;
			sign = 1;
		} else if( *script_p == '-' ) {
			token += *script_p++;
			sign = -1;
		}
		int base = 0;
		while( isdigit( *script_p ) ) {
			base = ( base * 10 ) + ( *script_p - '0' );
			token += *script_p++;
		}
		real *= powf( 10, sign * base );
		token.fvalue = real;
		token.ivalue = value;
		token.type = TOKEN_FLOAT;
		return true;
	}
	token.fvalue = real;
	token.ivalue = value;
	token.type = TOKEN_INT;
	return true;
}

bool wxParser::ReadToken( wxToken &token ) {
	token.clear();
	token.fvalue = 0;
	token.ivalue = 0;
	token.whitespace_p = script_p;
	SkipWhitespace();
	if( *script_p == 0 ) {
		return false;
	}
	token.line_p = line_p;
	token.script_p = script_p;
	token.line = line;
	if( isalpha( *script_p ) ) {
		ParseName( token );
		return true;
	}
	if( *script_p == '\"' || *script_p == '\'' ) {
		return ParseString( token );
	}
	if( isdigit( *script_p ) || ( *script_p == '.' && isdigit( *script_p + 1 ) ) ) {
		return ParseNumber( token );
	}
	token += *script_p++;
	token.type = TOKEN_PUNCTUAL;
	return true;
}

bool wxParser::ExpectToken( const wxString &str ) {
	wxToken token;
	PushSnapshot();
	if( !ReadToken( token ) ) {
		throw wxException( line, "file is EOF" );
	}
	if( token != str ) {
		PopSnapshot();
		throw wxException( line, "expection token (%s/%s %64s / %d)", (const char *)str.c_str(), (const char *)token.c_str(), line_p, line );
	}
	return true;
}

bool wxParser::PeekToken( const wxString &str ) {
	wxToken token;
	PushSnapshot();
	if( !ReadToken( token ) ) {
		throw wxException( line, "file is EOF" );
	}
	PopSnapshot();
	if( token != str ) {
		return false;
	}
	return true;
}

bool wxParser::CheckToken( const wxString &str ) {
	wxToken token;

	PushSnapshot();
	if( !ReadToken( token ) ) {
		throw wxException( line, "file is EOF" );
	}
	if( token != str ) {
		PopSnapshot();
		return false;
	}
	return true;
}

int wxParser::ReadInt() {
	int sign = 1;
	wxToken token;

	if( CheckToken( "-" ) ) {
		sign = -1;
	} else if( CheckToken( "+" ) ) {
		// PASS
	}

	PushSnapshot();
	if( !ReadToken( token ) ) {
		throw wxException( line, "file is EOF" );
	}
	if( token.type != TOKEN_INT ) {
		PopSnapshot();
		throw wxException( line, "expection int (%s) (%d)", (const char *)token.c_str(), line );
	}
	return sign * token.ivalue;
}

float wxParser::ReadFloat() {
	float sign = 1.0f;
	wxToken token;

	if( CheckToken( "-" ) ) {
		sign = -1.0f;
	} else if( CheckToken( "+" ) ) {
		// PASS
	}

	PushSnapshot();
	if( !ReadToken( token ) ) {
		throw wxException( line, "file is EOF" );
	}
	if( !( token.type == TOKEN_FLOAT || token.type == TOKEN_INT ) ) {
		PopSnapshot();
		throw wxException( line, "expection float (%s) (%d)", (const char *)token.c_str(), line );
	}
	return sign * token.fvalue;
}

wxString wxParser::ReadString() {
	wxToken token;

	PushSnapshot();
	if( !ReadToken( token ) ) {
		throw wxException( line, "file is EOF" );
	}
	if( token.type != TOKEN_STRING ) {
		PopSnapshot();
		throw wxException( line, "expection string (%s) (%d)", (const char *)token.c_str(), line );
	}
	return token;
}

wxString wxParser::ReadName() {
	wxToken token;

	PushSnapshot();
	if( !ReadToken( token ) ) {
		throw wxException( line, "file is EOF" );
	}
	if( token.type != TOKEN_NAME ) {
		PopSnapshot();
		throw wxException( line, "expection name (%s) (%d)", (const char *)token.c_str(), line );
	}
	return token;
}

wxString wxParser::ReadPunctual() {
	wxToken token;
	PushSnapshot();
	if( !ReadToken( token ) ) {
		throw wxException( line, "file is EOF" );
	}
	if( token.type != TOKEN_PUNCTUAL ) {
		PopSnapshot();
		throw wxException( line, "expection punctual (%s) (%d)", (const char *)token.c_str(), line );
	}
	return token;
}

bool wxParser::ReadOnLine( wxToken &token ) {
	int current = line;

	PushSnapshot();
	if( !ReadToken( token ) ) {
		throw wxException( line, "file is EOF" );
	}
	if( token.line == current ) {
		return true;
	}
	PopSnapshot();
	return false;
}

wxString wxParser::RestOnLine() {
	wxString out;
	wxToken token;
	while( ReadOnLine( token ) ) {
		if( !out.empty() ) {
			out += " ";
		}
		out += token;
	}
	return out;
}

bool wxParser::SkipOnLine() {
	while( *script_p && *script_p != '\n' ) {
		script_p++;
	}
	if( *script_p == '\0' ) {
		return false;
	}
	line++;
	line_p = ++script_p;
	return true;
}

void wxParser::PushSnapshot() {
	snapshot.line = line;
	snapshot.line_p = line_p;
	snapshot.script_p = script_p;
}

void wxParser::PopSnapshot() {
	line = snapshot.line;
	line_p = snapshot.line_p;
	script_p = snapshot.script_p;
}




wxWriter::~wxWriter() {
	CloseFile();
}

bool wxWriter::OpenFile( const wxString &filename ) {
	stream = fopen( filename.c_str(), "w+" );
	if( !stream ) {
		return false;
	}
	wxPrintf( "Write file: %s\n", filename );
	return true;
}

void wxWriter::CloseFile() {
	if( !stream ) {
		return;
	}
	fclose( stream );
	stream = nullptr;
}

void wxWriter::EndLine( int size ) {
	wxASSERT_MSG( stream, "stream is empty" );
	for(int i = 0; i < size; i++) {
		putc( '\n', stream );
	}
	fflush(stream);
}

wxWriter &wxWriter::Append( char ch ) {
	wxASSERT_MSG( stream, "stream is empty" );
	putc( ch, stream );
	return *this;
}

wxWriter &wxWriter::Append( const wxString &name, bool quote ) {
	wxASSERT_MSG( stream, "stream is empty" );
	if( quote ) {
		putc( '\"', stream );
	}
	fprintf( stream, "%s", (const char *)name.c_str() );
	if( quote ) {
		putc( '\"', stream );
	}
	return *this;
}

wxWriter &wxWriter::Append( float value ) {
	wxASSERT_MSG( stream, "stream is empty" );
	fprintf( stream, "%.8g", value );
	return *this;
}

wxWriter &wxWriter::Append( int value ) {
	wxASSERT_MSG( stream, "stream is empty" );
	fprintf( stream, "%d", value );
	return *this;
}

wxWriter &wxWriter::Append( unsigned int value ) {
	wxASSERT_MSG( stream, "stream is empty" );
	fprintf( stream, "%u", value );
	return *this;
}
