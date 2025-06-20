cmake_minimum_required(VERSION 3.31)
project(micro_message C)

# Info versione
set(APP_VERSION "0.0.1")
set(APP_BUILD "1")
string(TIMESTAMP APP_RELEASE_DATE "%Y-%m-%d")

# Assicura che la dir esista
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)

# Genera il file version.h
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.h
        @ONLY
)

# Impostazioni C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Disabilita C++
unset(CMAKE_CXX_STANDARD)
unset(CMAKE_CXX_COMPILER)

# Include la directory con i file generati
include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Eseguito finale
add_executable(micro_message src/main.c src/tcp_server.c src/string_man.c)