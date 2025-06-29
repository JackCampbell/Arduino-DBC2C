// Generate UiResource 1.0
#pragma once
	
#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/bitmap.h>

enum {
	wxID_DEVELOPER = wxID_HIGHEST + 1,
	wxID_ADD_DBC_FILE,
	wxID_NEW_DBC_FILE,
	wxID_REMOVE_DBC_FILE,
	wxID_DBC_FILES,
	wxID_ATTR_LIST,
	wxID_REM_ATTR,
	wxID_ADD_ATTR,
	wxID_ADD_DEFINE,
	wxID_REM_DEFINE,
	wxID_DEFINE_LIST,
	wxID_ADD_ITEM,
	wxID_REM_ITEM,
	wxID_UP_ITEM,
	wxID_DOWN_ITEM,
	wxID_ITEM_LIST,
	wxID_LOAD_ITEM,
	wxID_SAVE_ITEM,
	wxID_GEN_ITEM,
	wxID_FILE_LIST,
	wxID_MSG_LIST,
	wxID_SIG_LIST,
	wxID_GEN_EDIT,
	wxID_MOVE_UP,
	wxID_MOVE_DOWN,
	wxID_CHANNEL_BTN,
	wxID_SAVE_AS_ITEM,
	wxID_MAX_GENERATE
};
extern const wxBitmap &file_icon();
extern const wxBitmap &folder_icon();
extern const wxBitmap &edit_icon();
extern const wxBitmap &trash_icon();
extern const wxBitmap &add_icon();
extern const wxBitmap &move_up_icon();
extern const wxBitmap &move_down_icon();
extern const wxBitmap &save_icon();
extern const wxBitmap &setting_icon();
extern const wxBitmap &books_icon();
extern const wxBitmap &swap_icon();
extern const wxBitmap &icon_app();

