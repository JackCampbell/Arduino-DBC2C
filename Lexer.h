#pragma once
#include "Precompiled.h"

enum {
	TOKEN_NAME,
	TOKEN_STRING,
	TOKEN_LITERAL,
	TOKEN_FLOAT,
	TOKEN_INT,
	TOKEN_PUNCTUAL,
};

class wxToken : public wxString {
public:
					wxToken();
	wxToken &		operator=(const char *rhs);
	int				GetType() const { return type; }
	int				GetInt() const { return ivalue; }
	float			GetFloat() const { return fvalue; }
	int				GetLine() const { return line; }
	wxString		GetComment() const;
	bool			IsLineStart() const;
public:
	int				type;
	int 			ivalue;
	float			fvalue;
	char *			whitespace_p;
	char *			script_p;
	char *			line_p;
	int				line;
};

class wxException {
public:
					wxException( int line, const char *format, ... );
	void			Print() const;
	wxString		ToString() const;
private:
	char			buffer[1024];
	int				line;
};

class wxParser {
public:
					wxParser( int flags = 0 );
					~wxParser();
	bool			LoadFile( const wxString &filename );
	void			FreeSource();
	bool			ReadToken( wxToken &token );
	bool			PeekToken( const wxString &str );
	bool			CheckToken( const wxString &str );
	bool			ExpectToken( const wxString &str );
	int				ReadInt();
	float			ReadFloat();
	wxString		ReadString();
	wxString		ReadName();
	wxString		ReadPunctual();
	bool			ReadOnLine( wxToken &token );
	wxString		RestOnLine();
	bool			SkipOnLine();
	void			PushSnapshot();
	void			PopSnapshot();
protected:
	void			SkipWhitespace();
	bool			ParseName( wxToken &token );
	bool			ParseString( wxToken &token );
	bool			ParseNumber( wxToken &token );
	char			KeepChar(int ch);
private:
	struct snapshot_t {
		int			line;
		char *		line_p;
		char *		script_p;
	}				snapshot;
	char *			script_p, *end_p;
	char *			buffer;
	long			size;
	char *			line_p;
	int				flags;
public:
	int				line;
};

std::string to_format( const char *format, ... );

class wxWriter {
public:
					wxWriter() : stream( nullptr ) {}
	virtual			~wxWriter();
	bool			OpenFile( const wxString &filename );
	void			CloseFile();
	wxWriter &		Append( char ch);
	wxWriter &		Append( const wxString &name, bool quote = false );
	wxWriter &		Append( float value );
	wxWriter &		Append( int value );
	wxWriter &		Append( unsigned int value );
	void			EndLine( int size = 1 );
private:
	FILE *			stream;
};

