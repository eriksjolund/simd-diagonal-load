project(array-helper)

add_library(${PROJECT_NAME} INTERFACE)
add_library(simd-diagonal-load::${PROJECT_NAME}
            ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>)

add_subdirectory(include/simd-diagonal-load/array-helper)

install(TARGETS array-helper
        EXPORT simd-diagonal-load_Targets
        INCLUDES DESTINATION include
)
