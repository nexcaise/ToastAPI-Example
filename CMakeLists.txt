cmake_minimum_required(VERSION 3.22)

project(ToastAPI-Example VERSION 1.0.0 LANGUAGES C CXX)

if(NOT ANDROID)
    message(FATAL_ERROR "ToastAPI-Example must be built with the Android NDK toolchain.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PRELOADER_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/preloader")
set(PRELOADER_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libpreloader.so")
set(TOASTAPI_LIB "${CMAKE_CURRENT_SOURCE_DIR}/libs/arm64-v8a/libToastAPI.so")

if(NOT EXISTS "${PRELOADER_LIB}")
    message(FATAL_ERROR "libpreloader.so not found: ${PRELOADER_LIB}")
endif()

if(NOT EXISTS "${TOASTAPI_LIB}")
    message(FATAL_ERROR "libToastAPI.so not found: ${TOASTAPI_LIB}")
endif()

file(GLOB_RECURSE MOD_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_library(ToastAPI-Example SHARED
    ${MOD_SOURCES}
)

target_include_directories(ToastAPI-Example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PRELOADER_ROOT}/include
)

target_compile_definitions(ToastAPI-Example PRIVATE
    ToastAPI-Example_VERSION="1.0.0"
)

set_target_properties(ToastAPI-Example PROPERTIES
    OUTPUT_NAME "ToastAPI-Example"
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    POSITION_INDEPENDENT_CODE ON
)

# Size-first release profile. Avoid RTTI/unwind metadata and keep only reachable code.
target_compile_options(ToastAPI-Example PRIVATE
    -Oz
    -flto=thin
    -fvisibility=hidden
    -fvisibility-inlines-hidden
    -ffunction-sections
    -fdata-sections
    -fno-semantic-interposition
    -fno-ident
    -fno-rtti
    -fomit-frame-pointer
    -fmerge-all-constants
    -Wno-unused-parameter
)

target_link_options(ToastAPI-Example PRIVATE
    -Wl,-z,max-page-size=16384
    -Wl,--gc-sections
    -Wl,--icf=safe
    -Wl,--as-needed
    -Wl,--lto-O2
    -Wl,--strip-all
    -Wl,-z,relro
    -Wl,-z,now
    -Wl,--pack-dyn-relocs=android
)

# ToastAPI-Example talks to Android's asset manager through dlsym + Preloader detours,
# so libandroid.so is intentionally not a direct DT_NEEDED dependency.
target_link_libraries(ToastAPI-Example PRIVATE
    "${PRELOADER_LIB}"
    "${TOASTAPI_LIB}"
    dl
)
