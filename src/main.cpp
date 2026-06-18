#define WEBVIEW_IMPLEMENTATION
#include <windows.h>
#include <string>
#include <algorithm>
#include "api.h"
#include "webview.h"

// Función auxiliar para obtener el directorio exacto donde reside el ejecutable
std::string GetExeDirectory()
{
  char buffer[MAX_PATH];
  // Obtiene la ruta completa del archivo .exe actual
  GetModuleFileNameA(NULL, buffer, MAX_PATH);
  std::string path(buffer);

  // Busca la última barra para recortar el nombre del binario (app.exe)
  std::string::size_type pos = path.find_last_of("\\/");
  return path.substr(0, pos);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // 1. Obtener la ruta base del ejecutable
  std::string exeDir = GetExeDirectory();

  // 2. Construir la URL con el protocolo file:///
  std::string uiPath = "file:///" + exeDir + "/ui/index.html";

  // 3. Reemplazar las barras invertidas de Windows (\) por barras normales (/)
  // Esto es vital para que el esquema de URL local sea válido en el motor Chromium
  std::replace(uiPath.begin(), uiPath.end(), '\\', '/');

  // 4. Inicializar el contenedor de WebView2
  // El primer parámetro en 'true' habilita las DevTools (F12 / Inspeccionar)
  webview::webview w(false, nullptr);

  w.set_title("Test WebUI");
  w.set_size(1024, 768, WEBVIEW_HINT_NONE);

  RegisterNativeAPI(w);

  // 5. Navegar a la interfaz empaquetada por Vite
  w.navigate(uiPath);

  w.run();
  return 0;
}
