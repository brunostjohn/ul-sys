#ifndef ULTRALIGHT_CAPI_VIEW_H
#define ULTRALIGHT_CAPI_VIEW_H

#include "CAPI_Defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * ViewConfig
 *****************************************************************************/

///
/// Create view configuration with default values (see <Ultralight/platform/View.h>).
///
ULExport ULViewConfig ulCreateViewConfig();

///
/// Destroy view configuration.
///
ULExport void ulDestroyViewConfig(ULViewConfig config);

///
/// When enabled, the View will be rendered to an offscreen GPU texture using the GPU driver set in
/// ulPlatformSetGPUDriver. You can fetch details for the texture via ulViewGetRenderTarget.
///
/// When disabled (the default), the View will be rendered to an offscreen pixel buffer surface.
/// This pixel buffer can optionally be provided by the user-- for more info @see ulViewGetSurface.
///
ULExport void ulViewConfigSetIsAccelerated(ULViewConfig config, bool is_accelerated);

///
/// Set whether images should be enabled (Default = True).
///
ULExport void ulViewConfigSetIsTransparent(ULViewConfig config, bool is_transparent);

///
/// The initial device scale, ie. the amount to scale page units to screen pixels. This should be
/// set to the scaling factor of the device that the View is displayed on. (Default = 1.0)
///
/// @note 1.0 is equal to 100% zoom (no scaling), 2.0 is equal to 200% zoom (2x scaling)
///
ULExport void ulViewConfigSetInitialDeviceScale(ULViewConfig config, double initial_device_scale);

///
/// Whether or not the View should initially have input focus. (Default = True)
///
ULExport void ulViewConfigSetInitialFocus(ULViewConfig config, bool is_focused);

///
/// Set whether images should be enabled (Default = True).
///
ULExport void ulViewConfigSetEnableImages(ULViewConfig config, bool enabled);

///
/// Set whether JavaScript should be eanbled (Default = True).
///
ULExport void ulViewConfigSetEnableJavaScript(ULViewConfig config, bool enabled);

///
/// Set default font-family to use (Default = Times New Roman).
///
ULExport void ulViewConfigSetFontFamilyStandard(ULViewConfig config, ULString font_name);

///
/// Set default font-family to use for fixed fonts, eg <pre> and <code>
/// (Default = Courier New).
///
ULExport void ulViewConfigSetFontFamilyFixed(ULViewConfig config, ULString font_name);

///
/// Set default font-family to use for serif fonts (Default = Times New Roman).
///
ULExport void ulViewConfigSetFontFamilySerif(ULViewConfig config, ULString font_name);

///
/// Set default font-family to use for sans-serif fonts (Default = Arial).
///
ULExport void ulViewConfigSetFontFamilySansSerif(ULViewConfig config, ULString font_name);

///
/// Set user agent string (See <Ultralight/platform/Config.h> for the default).
///
ULExport void ulViewConfigSetUserAgent(ULViewConfig config, ULString agent_string);

/******************************************************************************
 * View
 *****************************************************************************/

///
/// Create a View with certain size (in pixels).
///
/// @note  You can pass null to 'session' to use the default session.
///
ULExport ULView ulCreateView(ULRenderer renderer, unsigned int width, unsigned int height,
                             ULViewConfig view_config, ULSession session);

///
/// Destroy a View.
///
ULExport void ulDestroyView(ULView view);

///
/// Get current URL.
///
/// @note Don't destroy the returned string, it is owned by the View.
///
ULExport ULString ulViewGetURL(ULView view);

///
/// Get current title.
///
/// @note Don't destroy the returned string, it is owned by the View.
///
ULExport ULString ulViewGetTitle(ULView view);

///
/// Get the width, in pixels.
///
ULExport unsigned int ulViewGetWidth(ULView view);

///
/// Get the height, in pixels.
///
ULExport unsigned int ulViewGetHeight(ULView view);

///
/// Get the device scale, ie. the amount to scale page units to screen pixels.
///
/// For example, a value of 1.0 is equivalent to 100% zoom. A value of 2.0 is 200% zoom.
///
ULExport double ulViewGetDeviceScale(ULView view);

///
/// Set the device scale.
///
ULExport void ulViewSetDeviceScale(ULView view, double scale);

///
/// Whether or not the View is GPU-accelerated. If this is false, the page will be rendered
/// via the CPU renderer.
///
ULExport bool ulViewIsAccelerated(ULView view);

///
/// Whether or not the View supports transparent backgrounds.
///
ULExport bool ulViewIsTransparent(ULView view);

///
/// Check if the main frame of the page is currrently loading.
///
ULExport bool ulViewIsLoading(ULView view);

///
/// Get the RenderTarget for the View.
///
/// @note  Only valid when the GPU renderer is enabled in Config.
///
ULExport ULRenderTarget ulViewGetRenderTarget(ULView view);

///
/// Get the Surface for the View (native pixel buffer container).
///
/// @note  Only valid when the GPU renderer is disabled in Config.
///
///        (Will return a nullptr when the GPU renderer is enabled.)
///
///        The default Surface is BitmapSurface but you can provide your own Surface implementation
///        via ulPlatformSetSurfaceDefinition.
///
///        When using the default Surface, you can retrieve the underlying bitmap by casting
///        ULSurface to ULBitmapSurface and calling ulBitmapSurfaceGetBitmap().
///
ULExport ULSurface ulViewGetSurface(ULView view);

///
/// Load a raw string of HTML.
///
ULExport void ulViewLoadHTML(ULView view, ULString html_string);

///
/// Load a URL into main frame.
///
ULExport void ulViewLoadURL(ULView view, ULString url_string);

///
/// Resize view to a certain width and height (in pixels).
///
ULExport void ulViewResize(ULView view, unsigned int width, unsigned int height);

///
/// Acquire the page's JSContext for use with JavaScriptCore API.
///
/// @note  This call locks the context for the current thread. You should call
///        ulViewUnlockJSContext() after using the context so other worker threads can modify
///        JavaScript state.
///
/// @note  The lock is recusive, it's okay to call this multiple times as long as you call
///        ulViewUnlockJSContext() the same number of times.
///
ULExport JSContextRef ulViewLockJSContext(ULView view);

///
/// Unlock the page's JSContext after a previous call to ulViewLockJSContext().
///
ULExport void ulViewUnlockJSContext(ULView view);

///
/// Evaluate a string of JavaScript and return result.
///
/// @param  js_string  The string of JavaScript to evaluate.
///
/// @param  exception  The address of a ULString to store a description of the last exception. Pass
///                    NULL to ignore this. Don't destroy the exception string returned, it's owned
///                    by the View.
///
/// @note Don't destroy the returned string, it's owned by the View. This value is reset with every
///       call-- if you want to retain it you should copy the result to a new string via
///       ulCreateStringFromCopy().
///
/// @note An example of using this API:
///       <pre>
///         ULString script = ulCreateString("1 + 1");
///         ULString exception;
///         ULString result = ulViewEvaluateScript(view, script, &exception);
///         /* Use the result ("2") and exception description (if any) here. */
///         ulDestroyString(script);
///       </pre>
///
ULExport ULString ulViewEvaluateScript(ULView view, ULString js_string, ULString* exception);

///
/// Check if can navigate backwards in history.
///
ULExport bool ulViewCanGoBack(ULView view);

///
/// Check if can navigate forwards in history.
///
ULExport bool ulViewCanGoForward(ULView view);

///
/// Navigate backwards in history.
///
ULExport void ulViewGoBack(ULView view);

///
/// Navigate forwards in history.
///
ULExport void ulViewGoForward(ULView view);

///
/// Navigate to arbitrary offset in history.
///
ULExport void ulViewGoToHistoryOffset(ULView view, int offset);

///
/// Reload current page.
///
ULExport void ulViewReload(ULView view);

///
/// Stop all page loads.
///
ULExport void ulViewStop(ULView view);

///
/// Give focus to the View.
///
/// You should call this to give visual indication that the View has input focus (changes active
/// text selection colors, for example).
///
ULExport void ulViewFocus(ULView view);

///
/// Remove focus from the View and unfocus any focused input elements.
///
/// You should call this to give visual indication that the View has lost input focus.
///
ULExport void ulViewUnfocus(ULView view);

///
/// Whether or not the View has focus.
///
ULExport bool ulViewHasFocus(ULView view);

///
/// Whether or not the View has an input element with visible keyboard focus (indicated by a
/// blinking caret).
///
/// You can use this to decide whether or not the View should consume keyboard input events (useful
/// in games with mixed UI and key handling).
///
ULExport bool ulViewHasInputFocus(ULView view);

///
/// Fire a keyboard event.
///
ULExport void ulViewFireKeyEvent(ULView view, ULKeyEvent key_event);

///
/// Fire a mouse event.
///
ULExport void ulViewFireMouseEvent(ULView view, ULMouseEvent mouse_event);

///
/// Fire a scroll event.
///
ULExport void ulViewFireScrollEvent(ULView view, ULScrollEvent scroll_event);

typedef void (*ULChangeTitleCallback)(void* user_data, ULView caller, ULString title);

///
/// Set callback for when the page title changes.
///
ULExport void ulViewSetChangeTitleCallback(ULView view, ULChangeTitleCallback callback,
                                           void* user_data);

typedef void (*ULChangeURLCallback)(void* user_data, ULView caller, ULString url);

///
/// Set callback for when the page URL changes.
///
ULExport void ulViewSetChangeURLCallback(ULView view, ULChangeURLCallback callback,
                                         void* user_data);

typedef void (*ULChangeTooltipCallback)(void* user_data, ULView caller, ULString tooltip);

///
/// Set callback for when the tooltip changes (usually result of a mouse hover).
///
ULExport void ulViewSetChangeTooltipCallback(ULView view, ULChangeTooltipCallback callback,
                                             void* user_data);

typedef void (*ULChangeCursorCallback)(void* user_data, ULView caller, ULCursor cursor);

///
/// Set callback for when the mouse cursor changes.
///
ULExport void ulViewSetChangeCursorCallback(ULView view, ULChangeCursorCallback callback,
                                            void* user_data);

typedef void (*ULAddConsoleMessageCallback)(void* user_data, ULView caller, ULMessageSource source,
                                            ULMessageLevel level, ULString message,
                                            unsigned int line_number, unsigned int column_number,
                                            ULString source_id);

///
/// Set callback for when a message is added to the console (useful for JavaScript / network errors
/// and debugging).
///
ULExport void ulViewSetAddConsoleMessageCallback(ULView view, ULAddConsoleMessageCallback callback,
                                                 void* user_data);

typedef ULView (*ULCreateChildViewCallback)(void* user_data, ULView caller, ULString opener_url,
                                            ULString target_url, bool is_popup,
                                            ULIntRect popup_rect);

///
/// Set callback for when the page wants to create a new View.
///
/// This is usually the result of a user clicking a link with target="_blank" or by JavaScript
/// calling window.open(url).
///
/// To allow creation of these new Views, you should create a new View in this callback, resize it
/// to your container, and return it. You are responsible for displaying the returned View.
///
/// You should return NULL if you want to block the action.
///
ULExport void ulViewSetCreateChildViewCallback(ULView view, ULCreateChildViewCallback callback,
                                               void* user_data);

typedef void (*ULBeginLoadingCallback)(void* user_data, ULView caller, unsigned long long frame_id,
                                       bool is_main_frame, ULString url);

///
/// Set callback for when the page begins loading a new URL into a frame.
///
ULExport void ulViewSetBeginLoadingCallback(ULView view, ULBeginLoadingCallback callback,
                                            void* user_data);

typedef void (*ULFinishLoadingCallback)(void* user_data, ULView caller, unsigned long long frame_id,
                                        bool is_main_frame, ULString url);

///
/// Set callback for when the page finishes loading a URL into a frame.
///
ULExport void ulViewSetFinishLoadingCallback(ULView view, ULFinishLoadingCallback callback,
                                             void* user_data);

typedef void (*ULFailLoadingCallback)(void* user_data, ULView caller, unsigned long long frame_id,
                                      bool is_main_frame, ULString url, ULString description,
                                      ULString error_domain, int error_code);

///
/// Set callback for when an error occurs while loading a URL into a frame.
///
ULExport void ulViewSetFailLoadingCallback(ULView view, ULFailLoadingCallback callback,
                                           void* user_data);

typedef void (*ULWindowObjectReadyCallback)(void* user_data, ULView caller,
                                            unsigned long long frame_id, bool is_main_frame,
                                            ULString url);

///
/// Set callback for when the JavaScript window object is reset for a new page load.
///
/// This is called before any scripts are executed on the page and is the earliest time to setup any
/// initial JavaScript state or bindings.
///
/// The document is not guaranteed to be loaded/parsed at this point. If you need to make any
/// JavaScript calls that are dependent on DOM elements or scripts on the page, use DOMReady
/// instead.
///
/// The window object is lazily initialized (this will not be called on pages with no scripts).
///
ULExport void ulViewSetWindowObjectReadyCallback(ULView view, ULWindowObjectReadyCallback callback,
                                                 void* user_data);

typedef void (*ULDOMReadyCallback)(void* user_data, ULView caller, unsigned long long frame_id,
                                   bool is_main_frame, ULString url);

///
/// Set callback for when all JavaScript has been parsed and the document is ready.
///
/// This is the best time to make any JavaScript calls that are dependent on DOM elements or scripts
/// on the page.
///
ULExport void ulViewSetDOMReadyCallback(ULView view, ULDOMReadyCallback callback, void* user_data);

typedef void (*ULUpdateHistoryCallback)(void* user_data, ULView caller);

///
/// Set callback for when the history (back/forward state) is modified.
///
ULExport void ulViewSetUpdateHistoryCallback(ULView view, ULUpdateHistoryCallback callback,
                                             void* user_data);

///
/// Set whether or not a view should be repainted during the next call to ulRender.
///
/// @note  This flag is automatically set whenever the page content changes but you can set it
///        directly in case you need to force a repaint.
///
ULExport void ulViewSetNeedsPaint(ULView view, bool needs_paint);

///
/// Whether or not a view should be painted during the next call to ulRender.
///
ULExport bool ulViewGetNeedsPaint(ULView view);

///
/// Create an inspector for this View, this is useful for debugging and inspecting pages locally.
/// This will only succeed if you have the inspector assets in your filesystem-- the inspector will
/// look for file:///inspector/Main.html when it loads.
///
/// @note  The initial dimensions of the returned View are 10x10, you should call ulViewResize on
///        the returned View to resize it to your desired dimensions.
///
/// @note  You will need to call ulDestroyView on the returned instance when you're done using it.
///
ULExport ULView ulViewCreateInspectorView(ULView view);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ULTRALIGHT_CAPI_VIEW_H