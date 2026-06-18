#pragma once
#include "webview.h"
#include <windows.h>
#include <string>

static inline HWND NativeHwnd(webview::webview &w)
{
       auto win = w.window();
       if (!win.ok())
       {
              return nullptr;
       }
       return static_cast<HWND>(win.value());
}

void RegisterNativeAPI(webview::webview &w)
{
       w.bind("Native_Hello", [](const std::string &req) -> std::string
              { return "{\"message\": \"Hello World!\"}"; });

       w.bind("Nativo_Alerta", [](const std::string &req) -> std::string
              {
    MessageBoxA(NULL, "Show Alert", "Test WebUI", MB_OK | MB_ICONINFORMATION);
    return "{\"status\": \"ok\"}"; });

       w.bind("Native_BeginDrag", [&w](const std::string &req) -> std::string
              {
    auto hwnd = NativeHwnd(w);
    if (!hwnd) {
      return "{\"status\": \"error\"}";
    }
    ReleaseCapture();
    SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
    return "{\"status\": \"ok\"}"; });

       w.bind("Native_ToggleMaximize", [&w](const std::string &req) -> std::string
              {
    auto hwnd = NativeHwnd(w);
    if (!hwnd) {
      return "{\"status\": \"error\"}";
    }
    if (IsZoomed(hwnd)) {
      ShowWindow(hwnd, SW_RESTORE);
      return "{\"status\": \"restored\"}";
    }
    ShowWindow(hwnd, SW_MAXIMIZE);
    return "{\"status\": \"maximized\"}"; });

       w.bind("Native_Minimize", [&w](const std::string &req) -> std::string
              {
    auto hwnd = NativeHwnd(w);
    if (!hwnd) {
      return "{\"status\": \"error\"}";
    }
    ShowWindow(hwnd, SW_MINIMIZE);
    return "{\"status\": \"minimized\"}"; });

       w.bind("Native_Close", [&w](const std::string &req) -> std::string
              {
    auto hwnd = NativeHwnd(w);
    if (!hwnd) {
      return "{\"status\": \"error\"}";
    }
    PostMessageW(hwnd, WM_CLOSE, 0, 0);
    return "{\"status\": \"closing\"}"; });
}
