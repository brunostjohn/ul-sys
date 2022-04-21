///
/// @file CAPI.h
///
/// @brief The C-language API for Ultralight
///
/// @author
///
/// This file is a part of Ultralight, a fast, lightweight, HTML UI engine
///
/// Website: <http://ultralig.ht>
///
/// Copyright (C) 2022 Ultralight, Inc. All rights reserved.
///
#ifndef ULTRALIGHT_CAPI_H
#define ULTRALIGHT_CAPI_H

/******************************************************************************
 * API Note:
 *
 * You should only destroy objects that you explicitly create. Do not destroy
 * any objects returned from the API or callbacks unless otherwise noted.
 *****************************************************************************/

#include "Ultralight/CAPI_Defines.h"
#include "Ultralight/CAPI_Bitmap.h"
#include "Ultralight/CAPI_Buffer.h"
#include "Ultralight/CAPI_Clipboard.h"
#include "Ultralight/CAPI_Config.h"
#include "Ultralight/CAPI_FileSystem.h"
#include "Ultralight/CAPI_FontFile.h"
#include "Ultralight/CAPI_FontLoader.h"
#include "Ultralight/CAPI_Geometry.h"
#include "Ultralight/CAPI_GPUDriver.h"
#include "Ultralight/CAPI_KeyEvent.h"
#include "Ultralight/CAPI_Logger.h"
#include "Ultralight/CAPI_MouseEvent.h"
#include "Ultralight/CAPI_Platform.h"
#include "Ultralight/CAPI_Renderer.h"
#include "Ultralight/CAPI_ScrollEvent.h"
#include "Ultralight/CAPI_Session.h"
#include "Ultralight/CAPI_String.h"
#include "Ultralight/CAPI_Surface.h"
#include "Ultralight/CAPI_View.h"

#endif // ULTRALIGHT_CAPI_H
