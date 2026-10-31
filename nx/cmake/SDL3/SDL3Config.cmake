# Shim exposing the NroForge SDK's static SDL3 (Switch, Vulkan via Mesa NVK).
set(SDL3_VERSION 3.2.27)
set(_sdk "$ENV{NROFORGE_SDK}/sdk")
file(GLOB _rust "${_sdk}/mesa/lib/rust/*.rlib")
if(NOT TARGET SDL3::SDL3)
  add_library(SDL3::SDL3 INTERFACE IMPORTED)
  set_target_properties(SDL3::SDL3 PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_sdk}/sdl3/include"
    INTERFACE_LINK_LIBRARIES
      "${_sdk}/sdl3/lib/libSDL3.a;${_sdk}/mesa/lib/libvulkan.a;${_sdk}/mesa/lib/libEGL.a;${_sdk}/mesa/lib/libglapi.a;${_rust};drm_nouveau;expat;z;zstd;stdc++;nx;m;pthread")
  add_library(SDL3::SDL3-shared ALIAS SDL3::SDL3)
endif()
set(SDL3_FOUND TRUE)
