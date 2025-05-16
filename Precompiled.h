#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>

#ifdef WIN32
#pragma warning(disable:4244)
#pragma warning(disable:4996)
#pragma warning(disable:4566)
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/aboutdlg.h>
#include <wx/valtext.h>
#include <wx/valgen.h>
#include <wx/valnum.h>
#include <wx/listctrl.h>
#include <wx/object.h>
#include <wx/datstrm.h>
#include <wx/wfstream.h>

#include <wx/stc/stc.h>

#ifndef BIT
#define BIT(x) (1 << (x))
#endif

#ifndef MIN
#define MIN(x, y) (x < y ? x : y)
#endif

#ifndef MAX
#define MAX(x, y) (x > y ? x : y)
#endif



