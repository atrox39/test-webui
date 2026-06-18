cmake_minimum_required(VERSION 3.10)

project(MotorWebView2 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(app src/main.cpp)

target_compile_definitions(app PRIVATE
  $<$<CONFIG:Debug>:DEBUG>
)

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

find_program(PNPM_EXECUTABLE NAMES pnpm pnpm.cmd REQUIRED)

if(EXISTS "${CMAKE_SOURCE_DIR}/ui" AND NOT EXISTS "${CMAKE_SOURCE_DIR}/ui/node_modules")
    message(STATUS "Dependencias faltantes detectadas: Ejecutando 'pnpm install' en /ui...")
    execute_process(
        COMMAND ${PNPM_EXECUTABLE} install
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ui
        RESULT_VARIABLE PNPM_RESULT
    )
    if(NOT PNPM_RESULT EQUAL 0)
        message(FATAL_ERROR "Fallo la instalacion de paquetes con pnpm. Revisa tu entorno o package.json.")
    endif()
endif()

add_custom_target(build_ui
    COMMAND ${PNPM_EXECUTABLE} run build
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/ui
    COMMENT "Empaquetando frontend moderno con Vite..."
)

add_dependencies(app build_ui)

add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/ui/dist
    $<TARGET_FILE_DIR:app>/ui
    
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/lib/WebView2Loader.dll
    $<TARGET_FILE_DIR:app>/WebView2Loader.dll
    
    COMMENT "Ensamblando artefactos finales en el directorio de salida..."
)
