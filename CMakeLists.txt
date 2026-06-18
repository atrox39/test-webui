cmake_minimum_required(VERSION 3.10)

project(MotorWebView2 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(app src/main.cpp)

# Oculta la consola negra en Windows
set_target_properties(app PROPERTIES WIN32_EXECUTABLE TRUE)

target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(app PRIVATE
  user32
  shell32
  shlwapi
  ole32
  oleaut32
  uuid
  advapi32
  version
  comdlg32
)

target_link_libraries(app PRIVATE ${CMAKE_SOURCE_DIR}/lib/libWebView2Loader.a)

# ==========================================
# --- INTEGRACIÓN VITE & NODE.JS ---
# ==========================================

# 1. Buscar npm de forma cruzada (npm.cmd es vital para Windows)
find_program(NPM_EXECUTABLE NAMES npm npm.cmd REQUIRED)

# 2. Instalación automática: Si existe la carpeta ui pero no los node_modules, hace install
if(EXISTS "${CMAKE_SOURCE_DIR}/ui" AND NOT EXISTS "${CMAKE_SOURCE_DIR}/ui/node_modules")
    message(STATUS "Dependencias faltantes detectadas: Ejecutando 'npm install' en /ui...")
    execute_process(
        COMMAND ${NPM_EXECUTABLE} install
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ui
        RESULT_VARIABLE NPM_RESULT
    )
    if(NOT NPM_RESULT EQUAL 0)
        message(FATAL_ERROR "Fallo la instalación de paquetes npm. Revisa tu conexión o package.json.")
    endif()
endif()

# 3. Target del Frontend: Dispara el empaquetado de Vite
add_custom_target(build_ui
    COMMAND ${NPM_EXECUTABLE} run build
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ui
    COMMENT "Empaquetando frontend moderno con Vite..."
)

# 4. Sincronización: Obliga a CMake a esperar a que termine 'build_ui' antes de dar por listo 'app'
add_dependencies(app build_ui)

# ==========================================
# --- ENSAMBLADO Y DISTRIBUCIÓN (POST-BUILD) ---
# ==========================================

# 5. Mover todo a su lugar al terminar la compilación
add_custom_command(TARGET app POST_BUILD
    # Copia el resultado minificado de Vite (ui/dist) y lo pega como 'ui' junto al .exe
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/ui/dist
    $<TARGET_FILE_DIR:app>/ui
    
    # Copia el cargador dinámico de Microsoft para evitar el crash silencioso al abrir
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/lib/WebView2Loader.dll
    $<TARGET_FILE_DIR:app>/WebView2Loader.dll
    
    COMMENT "Ensamblando artefactos finales en el directorio de salida..."
)
