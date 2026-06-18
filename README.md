# Test WebUI

Aplicacion de escritorio para Windows que integra una interfaz web hecha con React + Vite dentro de un contenedor nativo en C++ usando WebView2.

El proyecto compila el frontend, copia los archivos generados junto al ejecutable y carga la interfaz desde disco mediante `file:///.../ui/index.html`.

## Caracteristicas

- App nativa en C++17 con punto de entrada `WinMain`.
- Interfaz React 19 + TypeScript + Vite 8.
- Empaquetado del frontend en un unico bundle con `vite-plugin-singlefile`.
- Integracion JavaScript <-> nativo mediante `webview::bind`.
- Soporte de compilacion `Debug` y `Release` con comportamiento distinto en tiempo de ejecucion.
- Copia automatica de `ui/dist` y `WebView2Loader.dll` al directorio final del binario.

## Tecnologias

- C++17
- CMake 3.10+
- WebView2
- React 19
- TypeScript
- Vite
- ESLint

## Estructura Del Proyecto

```text
.
|-- CMakeLists.txt        # Build nativo y orquestacion del frontend
|-- include/              # Headers de webview y WebView2
|-- lib/                  # Loader y librerias de WebView2
|-- src/
|   |-- main.cpp          # Inicializa la ventana y carga la UI empaquetada
|   `-- api.h             # Funciones nativas expuestas a JavaScript
`-- ui/
    |-- src/
    |   |-- App.tsx       # UI de ejemplo con llamada a la API nativa
    |   |-- main.tsx      # Entry point de React
    |   `-- index.css     # Estilos base
    |-- package.json      # Scripts del frontend
    `-- vite.config.ts    # Configuracion para bundle relativo y single-file
```

## Requisitos

- Windows
- Node.js
- `pnpm`
- CMake 3.10 o superior
- Un compilador C++ compatible con CMake en Windows

## Desarrollo Del Frontend

Para trabajar solo en la UI web:

```powershell
cd ui
pnpm install
pnpm run dev
```

Scripts disponibles en `ui/package.json`:

- `pnpm run dev`: levanta Vite en modo desarrollo.
- `pnpm run build`: ejecuta `tsc -b` y luego `vite build`.
- `pnpm run lint`: analiza el frontend con ESLint.
- `pnpm run preview`: sirve localmente el bundle generado.

Nota: el modo `dev` sirve para iterar sobre la interfaz en el navegador, pero la aplicacion nativa carga el frontend compilado desde archivos estaticos.

## Compilacion De La Aplicacion

Desde la raiz del proyecto:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

`CMakeLists.txt` define la macro `DEBUG` solo para la configuracion `Debug`. En `Release`, la macro no se define. La integracion del frontend usa `pnpm`, en linea con el archivo `ui/pnpm-lock.yaml`.

### Modo Debug

En `Debug` la aplicacion:

- Crea `webview::webview` con herramientas de desarrollo habilitadas.
- Navega a `http://localhost:5173`.
- Espera que el frontend de Vite este corriendo localmente.

Flujo recomendado:

```powershell
cd ui
pnpm install
pnpm run dev
```

En otra terminal:

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug
```

Nota:

- En generadores multi-config como Visual Studio, `--config Debug` es lo que determina la configuracion de compilacion.
- En generadores single-config, `-DCMAKE_BUILD_TYPE=Debug` define la configuracion al generar el proyecto.

### Modo Release

En `Release` la aplicacion:

- Crea `webview::webview` sin el flujo de depuracion de desarrollo.
- Carga la UI empaquetada desde `file:///.../ui/index.html`.
- No depende de un servidor local de Vite en ejecucion.

Compilacion de ejemplo:

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Que hace la build:

1. Detecta `pnpm` o `pnpm.cmd`.
2. Ejecuta `pnpm install` en `ui/` si `node_modules` no existe.
3. Ejecuta `pnpm run build` para generar `ui/dist`.
4. Compila el ejecutable nativo `app`.
5. Copia el contenido de `ui/dist` como `ui/` junto al ejecutable.
6. Copia `lib/WebView2Loader.dll` junto al binario final.

Segun el generador de CMake, el ejecutable suele quedar en una ruta similar a:

- `build/Release/app.exe`
- `build/app.exe`

## Como Funciona

El flujo principal es este:

1. `src/main.cpp` obtiene el directorio del ejecutable.
2. Construye la ruta `file:///.../ui/index.html`.
3. Inicializa la ventana con WebView2.
4. Registra funciones nativas con `RegisterNativeAPI`.
5. Si `DEBUG` esta definido, navega a `http://localhost:5173`.
6. Si `DEBUG` no esta definido, carga la interfaz React empaquetada desde disco.

La API nativa actual expone:

- `Native_Hello()`: devuelve un JSON con un mensaje de prueba.
- `Nativo_Alerta()`: muestra una ventana nativa de Windows y devuelve `{"status":"ok"}`.

En la UI actual, el boton `Press Me` llama a `window.Nativo_Alerta()`.

## Notas Importantes

- El proyecto esta orientado a Windows; usa `WinMain`, `MessageBoxA` y `WebView2Loader.dll`.
- La UI se sirve desde archivos locales, por eso `vite.config.ts` usa `base: './'`.
- El modo `Debug` depende de tener Vite corriendo en `http://localhost:5173`.
- El frontend esta pensado para salir en un solo artefacto HTML/JS/CSS embebido para simplificar la distribucion.
- No hay tests automatizados configurados actualmente.
- `ui/README.md` todavia contiene el README generico de la plantilla de Vite.

## Proximos Pasos Sugeridos

- Reemplazar la UI de ejemplo por una interfaz real.
- Tipar la API nativa en el frontend para evitar `any` en `window`.
- Agregar una estrategia de pruebas para la logica de UI y la integracion nativa.
- Documentar distribucion, instalacion y versionado del ejecutable si el proyecto evoluciona a producto.
