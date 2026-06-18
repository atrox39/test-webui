#pragma once
#include "webview.h"
#include <string>

void RegisterNativeAPI(webview::webview &w)
{
  w.bind("Native_Hello", [](const std::string &req) -> std::string
         { return "{\"message\": \"Hello World!\"}"; });
  w.bind("Nativo_Alerta", [](const std::string &req) -> std::string
         {
        // Invoca una ventana de alerta pura del sistema operativo
        MessageBoxA(NULL, "Show Alert", "Test WebUI", MB_OK | MB_ICONINFORMATION);
        
        return "{\"status\": \"ok\"}"; });
}
