/*
==========================================================================================
$Name: $
$Description: $
$Creator: Andreas Gaida$
$Copyright: $
$Example: $
==========================================================================================
*/
#ifndef _GAI_INPUT_H_

#include "gai_types.h"
#include "gai_utils.h"

#ifdef __cplusplus
extern "C" {
#endif
    
typedef struct
{
    i32 count;
    b32 ended_down;
} gaiKeyState;

enum { GAI_ENUM_MOUSE_LEFT = 0, GAI_ENUM_MOUSE_RIGHT = 1, GAI_ENUM_MOUSE_MIDDLE = 2 };
typedef struct 
{
    gaiKeyState keys[256], buttons[3];
    i32 x, y, dtx, dty, dtwheel;
    i32 scrx, scry;
} gaiInput;

GAI_DEF u32  gaiInputKeyDownAsync(int key);
GAI_DEF u32  gaiInputKeyDown(int key);
GAI_DEF void gaiInputUpdate(gaiInput *input);
GAI_DEF u32  gaiInputKeyPressed(gaiInput *input, int key);

#include "gai_input_win32.cc"
    
#ifdef __cplusplus
}
#endif

#define _GAI_INPUT_H_
#endif
