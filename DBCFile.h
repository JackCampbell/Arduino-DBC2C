#pragma once
#include "Precompiled.h"
#include "Lexer.h"

// declare: DBCFile.cpp
class wxDBCSignal;
class wxDBCMessage;
class wxDBCNode;
class wxDBCAttr;
class wxDBCValueTable;
class wxDBCFile;


WX_DECLARE_HASH_MAP( int, wxString, wxIntegerHash, wxIntegerEqual, wxValueMap );
WX_DECLARE_HASH_MAP( wxString, wxString, wxStringHash, wxStringEqual, wxStrMap );


enum {
	OBJ_TYPE_FILE, // global
	OBJ_TYPE_NODE,
	OBJ_TYPE_MESSAGE,
	OBJ_TYPE_SIGNAL,
	OBJ_TYPE_ENV // dont use
};

enum {
	ATTR_TYPE_STRING,
	ATTR_TYPE_INT,
	ATTR_TYPE_HEX,
	ATTR_TYPE_FLOAT,
	ATTR_TYPE_ENUM
};


class wxDBCComment {
public:
	const wxString &			GetComment() const { return comment; }
	void						SetComment(const wxString &comment) { this->comment = comment; }
	bool						HasComment() const { return !comment.empty(); }
public:
	wxString					comment;
};

class wxDBCAttr {
public:
								wxDBCAttr(wxDBCFile *file, int object);
								~wxDBCAttr();
	wxString					FindAttr(const wxString &name, const wxString &defs = wxEmptyString) const;
	void						SetAttr(const wxString &name, const wxString &value);
	bool						RemoveAttr(const wxString &name);
	bool						HasAttr(const wxString &name) const;
	const wxDBCFile *			GetFile() const { return file; }
	wxDBCFile *					GetFile() { return file; }
	const wxStrMap &			GetAttrMap() const { return attr; }
	wxStrMap &					GetAttrMap() { return attr; }
	int							GetAttrObject() const { return object; }
public:
	wxStrMap					attr;
	int							object;
	wxDBCFile *					file;
};

class wxDBCNode : public wxDBCComment, public wxDBCAttr {
public:
								wxDBCNode(wxDBCFile *file, const wxString &name);
								wxDBCNode(const wxDBCNode *copy);
	const wxString &			GetName() const { return name; }
	void						Print() const;
public:
	wxString					name;
};


class wxDBCValueTable {
public:
								wxDBCValueTable(wxDBCFile *file);
								wxDBCValueTable(const wxDBCValueTable *copy);
	void						Append(int value, const wxString &desc);
	void						Clear();
	const wxString &			GetName() const { return name; }
	wxValueMap &				GetValues() { return map; }
	const wxValueMap &			GetValues() const { return map; }
	wxString					GetDesc(int value) const;
	void						Print() const;
	wxString					GetDesc() const;
	
	void						SaveFile(wxWriter &writer) const;
	bool						ParseFile( wxParser &src );
public:
	wxString					name;
	wxValueMap					map;
	wxDBCFile *					file;
};

enum {
	SF_BIG_ENDIAN				= BIT(1), // byte_order
	SF_SIGNED_TYPE				= BIT(2), // value_type
	SF_FLOAT_EXT_TYPE			= BIT(3),
	SF_DOUBLE_EXT_TYPE			= BIT(4),
	SF_STRING_EXT_TYPE			= BIT(5),
	SF_EXT_TYPE_MASK				= SF_FLOAT_EXT_TYPE | SF_DOUBLE_EXT_TYPE | SF_STRING_EXT_TYPE
};

class wxDBCSignal : public wxDBCComment, public wxDBCAttr {
public:
								wxDBCSignal(wxDBCFile *file, const wxString &name);
								wxDBCSignal(const wxDBCSignal *copy);
								~wxDBCSignal();
	const wxString &			GetName() const { return name; }
	const wxString &			GetUnit() const { return unit; }
	void						Print() const;
	void						SaveFile(wxWriter &writer) const;
	bool						HasExtType() const { return (flags & SF_EXT_TYPE_MASK) != 0; }
	void						SetValue(uint64_t value, uint8_t *buffer, const int msg_size) const;
	wxString					GetCodeType() const;
private:
	bool						UpdateSignalBounds(int &lsb, int &msb) const;
public:
	wxString					name;
	wxString					multiplexer; // indicator !!
	int							flags;
	int							start_bit, bit_len;
	float						scale, offset;
	float						min, max;
	wxString					unit;
	wxArrayString				receivers;
	wxValueMap					values;
};

class wxDBCMessage : public wxDBCComment, public wxDBCAttr {
public:
								wxDBCMessage(wxDBCFile *file, int ID, const wxString &name, int size, const wxString &sender);
								wxDBCMessage(const wxDBCMessage *copy);
	const wxString &			GetName() const { return name; }
	uint32_t					CanID() const { return ID; }
	int							GetSize() const { return size; }
	void						Print() const;
	void						SaveFile(wxWriter &writer, bool includeSignals) const;
public:
	uint32_t					ID;
	wxString					name;
	int							size; // dlc
	wxString					sender;
	wxVector<wxDBCSignal *>		signals;
	wxArrayString				transmitter; // multiple transmitter
};


class wxDBCAttrDefine {
public:
								wxDBCAttrDefine(wxDBCFile *file, int object, const wxString &name);
								wxDBCAttrDefine(const wxDBCAttrDefine *copy);
								~wxDBCAttrDefine();
	int							GetObject() const { return object; }
	int							GetType() const { return type; }
	const wxString &			GetName() const { return name; }
	int							GetMinIntValue() const { return wxAtoi(min); }
	int							GetMaxIntValue() const { return wxAtoi(max); }
	float						GetMinFloatValue() const { return wxAtof(min); }
	float						GetMaxFloatValue() const { return wxAtof(max); }
	const wxArrayString &		GetEnums() const { return enums; }
	void						SetIntRange(int min, int max);
	void						SetFloatRange(float min, float max);
	void						AppendValue(const wxString &value);
	int							GetIntValue() const { return wxAtoi(value); }
	float						GetFloatValue() const { return wxAtof(value); }
	wxString					GetStringValue() const { return value; }
	void						Print() const;
	bool						HasDefault() const { return has_default; }
	wxDBCFile *					GetFile() { return file; }
	const wxDBCFile *			GetFile() const { return file; }
	const wxString &			GetMin() const { return min; }
	const wxString &			GetMax() const { return max; }
public:
	int							object;
	int							type;
	wxString					name;
	wxString					min, max;
	wxArrayString				enums;
	wxString					value;
	bool						has_default;
	wxDBCFile *					file;
};


class wxDBCFile : public wxDBCComment, public wxDBCAttr {
	friend class DBCFileDialog;
public:
								wxDBCFile();
	virtual						~wxDBCFile();
	bool						LoadFile(const wxString &filename);
	void						SaveFile(const wxString &filename);
	void						CloseFile();
	void						Print() const;
	wxString					GetFilename() const { return filename; }
	int							GetBaudrate() const { return baudrate; }
	int							GetBaudrate(int defs) const;

	bool						IsModified() const { return modified; }
	void						SetModified(bool modified);
	const wxString &			GetName() const { return name; }
	const wxString &			GetLastError() const { return error; }
	
	wxDBCMessage *				FindMessage(uint32_t message_id) const;
	wxDBCSignal *				FindSignal(uint32_t message_id, const wxString &signal_name) const;
	int							NumMessage() const { return messages.size(); }
	wxDBCMessage *				GetMessage( int index ) { return messages[index]; }
	const wxDBCMessage *			GetMessage( int index ) const { return messages[index]; }
	wxArrayString				GetMessageList() const;
	wxArrayString				GetSignalList( uint32_t message_id, bool skipValid = false ) const;
	
	wxDBCValueTable *			FindTable(const wxString &name) const;
	int							NumTable() const { return tables.size(); }
	wxDBCValueTable *			GetTable( int index ) { return tables[index]; }
	const wxDBCValueTable *		GetTable( int index ) const { return tables[index]; }
	
	wxDBCAttrDefine *			FindDefine( const wxString &name ) const;
	int							NumDefine() const { return defines.size(); }
	wxDBCAttrDefine *			GetDefine( int index ) { return defines[index]; }
	const wxDBCAttrDefine *		GetDefine( int index ) const { return defines[index]; }
	
	wxDBCNode *					AllocNode( const wxString &name );
	wxDBCNode *					FindNode( const wxString &name ) const;
	int							NodeCount() const { return nodes.size(); }
	wxDBCNode *					GetNode( int index ) { return nodes[index]; }
	const wxDBCNode *			GetNode( int index ) const { return nodes[index]; }
private:	
	void						ParseBaudrate(wxParser &src);
	void						ParseNodes(wxParser &src);
	void						ParseMessage(wxParser &src);
	void						ParseSignal(wxParser &src, wxDBCMessage *message);
	void						ParseMessageTransmitter(wxParser &src);
	void						ParseSignalValues(wxParser &src);
	void						ParseComment(wxParser &src);
	void						ParseAttrDefine(wxParser &src);
	int							ParseObjectType(wxParser &src) const;
	wxString					GetObjectType(int object) const;
	void						ParseAttrDefault(wxParser &src);
	void						ParseAttrValue(wxParser &src);
	void						ParseSignalExtType(wxParser &src);
	void						WriteComments(wxWriter &writer);
	void						WriteSignalValue(wxWriter &writer);
	void						WriteAttrDefines(wxWriter &writer, bool includeDefaults);
	void						WriteAttrValues(wxWriter &writer);
	void						WriteSignalExtType(wxWriter &writer);
private:	
	wxString						version;
	int								baudrate, btr1, btr2;
	wxVector<wxDBCNode *>			nodes;
	wxVector<wxDBCValueTable *>		tables;
	wxVector<wxDBCMessage *>			messages;
	wxVector<wxDBCAttrDefine *>		defines;
	
	bool							modified;
	wxString						filename;
	wxString						name;
	wxString 						error;
};

extern const wxString strObjectType[];
extern const wxString strValueType[];

inline void wxDBCFile::SetModified(bool modified) {
	this->modified = modified;
}

inline bool NodeCompare(wxDBCNode *const &lhs, wxDBCNode *const &rhs) {
	return lhs->name.compare(rhs->name) < 0;
}

inline bool TableCompare(wxDBCValueTable *const &lhs, wxDBCValueTable *const &rhs) {
	return lhs->name.compare(rhs->name) < 0;
}

inline bool MessageCompare(wxDBCMessage *const &lhs, wxDBCMessage *const rhs) {
	return lhs->ID < rhs->ID;
}

inline bool SignalCompare(wxDBCSignal *const &lhs, wxDBCSignal *const &rhs) {
	return lhs->name.compare(rhs->name) < 0;
}

inline bool DefineCompare(wxDBCAttrDefine *const &lhs, wxDBCAttrDefine *const &rhs) {
	return lhs->name.compare(rhs->name) < 0;
}

inline int wxDBCFile::GetBaudrate(int defs) const {
	if(baudrate <= 0) {
		return defs;
	}
	return baudrate;
}

// 0 - 23 bit empty
#define IEEE_MANTIS(value)   (*reinterpret_cast<int *>(&value) & 0x007fffff)
#define IEEE_SIGN(value)     (*reinterpret_cast<int *>(&value) & 0x7f800000)
#define IEEE_EXPONENT(value) (*reinterpret_cast<int *>(&value) & 0x80000000)

#define CMP_FLOAT(lhs, rhs) 	(absf(lhs - rhs) < 1.0e-6f)

#define MASK_29BIT 			(0x7FF << 14)
