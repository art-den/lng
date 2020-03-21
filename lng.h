/*

Macros and functions for very simple multiple language support for 
microcontroller firmware.

How to use:

Write file lang_strings.incl with content:

---[begin lang_strings.incl]-----------------------------------

LNG_BEGIN_LANG_TABLE
	LNG_ID(lEnglish)
	LNG_ID(lRussian)
LNG_END_LANG_TABLE

LNG_BEGIN_STR_TABLE
	LNG_STR(iGET,  "Get",  "Взять")
	LNG_STR(iPUT,  "Put",  "Положить")
	LNG_STR(iMENU, "Menu", "Меню")
	LNG_STR(iEXIT, "Exit", "Выход")
LNG_END_STR_TABLE

---[end lang_strings.incl]-------------------------------------


Then then use _ or TR macro to translate strings:

---[begin main.c]----------------------------------------------

#include <stdio.h>

#define LNG_IMPL // define LNG_IMPL only in one .c file in project!!!
#define LNG_FILE "lng_strings.incl"
#include "lng.h" 

int main()
{
	// language is first by default, but let define it anyway
	lng_current = lEnglish; 

	// print Exit in english
	printf("Exit = %s\n", _(iEXIT));

	// set current language to russian
	lng_current = lRussian;

	// print Exit in russian
	printf("Exit = %s\n", _(iEXIT));
}

---[end main.c]------------------------------------------------

*/

#ifndef LNG_H_FILE_INCLUDED__
#define LNG_H_FILE_INCLUDED__

#ifndef LNG_FILE
#error LNG_FILE macro is not defined
#endif

// declaration

#define LNG_BEGIN_LANG_TABLE typedef enum LngId_ { _LNG_FIRST_ID = -1
#define LNG_ID(LANG_ID) , LANG_ID
#define LNG_END_LANG_TABLE , _LNG_LAST_ID } LngId; 
#define LNG_BEGIN_STR_TABLE typedef enum { _FIRST_STR_ID = -1
#define LNG_END_STR_TABLE } LngStrId;
#define LNG_STR(STR_ID, ...) , STR_ID

#include LNG_FILE

#undef LNG_BEGIN_LANG_TABLE
#undef LNG_ID
#undef LNG_END_LANG_TABLE
#undef LNG_BEGIN_STR_TABLE
#undef LNG_END_STR_TABLE
#undef LNG_STR

typedef struct LngStrItem_ { const char* items[_LNG_LAST_ID]; } LngStrItem;

extern const LngStrItem lng_str_items[];
extern LngId lng_current;

// implementation

#ifdef LNG_IMPL

#define LNG_BEGIN_LANG_TABLE
#define LNG_ID(LANG_ID)
#define LNG_END_LANG_TABLE
#define LNG_BEGIN_STR_TABLE const LngStrItem lng_str_items[] = {
#define LNG_END_STR_TABLE {0} };
#define LNG_STR(STR_ID, ...) { __VA_ARGS__ },

#include LNG_FILE

LngId lng_current = (LngId)0;

#undef LNG_BEGIN_LANG_TABLE
#undef LNG_ID
#undef LNG_END_LANG_TABLE
#undef LNG_BEGIN_STR_TABLE
#undef LNG_END_STR_TABLE
#undef LNG_STR

#endif // LNG_IMPL

///////////////////////////////////////////////////////////////////////////////

// translation API

#define TR(STR_ID) (lng_str_items[(STR_ID)].items[lng_current])

#define _(STR_ID) (lng_str_items[(STR_ID)].items[lng_current])

#endif
