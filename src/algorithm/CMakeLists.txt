project(algorithm)
add_library(${PROJECT_NAME} INTERFACE)
add_library(simd-diagonal-load::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
find_package(libsimdpp REQUIRED)

target_include_directories(${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>  
)

target_link_libraries(${PROJECT_NAME} INTERFACE libsimdpp::libsimdpp)

add_subdirectory(include/simd-diagonal-load/algorithm)

install(TARGETS algorithm EXPORT simd-diagonal-load_Targets
        INCLUDES DESTINATION include)
