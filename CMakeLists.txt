cmake_minimum_required(VERSION 3.11.4)
project(simd-diagonal-load
    VERSION
        0.9.0
    LANGUAGES
        CXX
)
#add_subdirectory(src)


add_subdirectory(src/algorithm)
add_subdirectory(src/array-helper)

set(cmake_configuration_install_dir lib/cmake/simd-diagonal-load)

install(EXPORT simd-diagonal-load_Targets
        DESTINATION ${cmake_configuration_install_dir}
        FILE simd-diagonal-load_Targets.cmake
        NAMESPACE simd-diagonal-load::)

export(EXPORT simd-diagonal-load_Targets
       FILE simd-diagonal-load_Targets.cmake
       NAMESPACE simd-diagonal-load::)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("simd-diagonal-loadConfigVersion.cmake"
                                 VERSION ${PACKAGE_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_file(cmake/modules/simd-diagonal-loadConfig.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/simd-diagonal-loadConfig.cmake
               COPYONLY)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/Export/lib/cmake/simd-diagonal-load/simd-diagonal-load_Targets.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/simd-diagonal-loadConfigVersion.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/simd-diagonal-loadConfig.cmake
  DESTINATION
    lib/cmake/simd-diagonal-load/)
