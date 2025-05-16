#pragma once
#include "Precompiled.h"

// declare: Utils.cpp

class wxBoolValidator : public wxValidator {
public:
	wxBoolValidator( bool *value ) : wxValidator() { this->value = value; }
	~wxBoolValidator() { value = nullptr; }
	virtual wxObject *Clone() const { return new wxBoolValidator( *this ); }
	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();
	virtual bool Validate( wxWindow *parent ) { return true; }
private:
	bool *value;
};

void ListCtrlAutoColumn( wxListCtrl *view );

class wxHexValidator : public wxValidator {
public:
	wxHexValidator( uint32_t &value ) : wxValidator(), value( value ) {}
	~wxHexValidator() {}
	virtual wxObject *Clone() const { return new wxHexValidator( *this ); }
	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();
	virtual bool Validate( wxWindow *parent );
private:
	uint32_t &value;
};

class wxCanValidator : public wxValidator {
public:
	wxCanValidator( uint32_t &value ) : wxValidator(), value( value ) {}
	~wxCanValidator() {}
	virtual wxObject *Clone() const { return new wxCanValidator( *this ); }
	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();
	virtual bool Validate( wxWindow *parent );
private:
	bool GetCanID( wxString data, uint32_t &test ) const;
private:
	uint32_t &value;
};