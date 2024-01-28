cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_DEBUG_POSTFIX d)

set(TARGET_NAME "sapv")
set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(EXTERNAL_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(JSON_EXTERNAL_DIR ${EXTERNAL_LIBS_DIR}/json)
set(LEXBOR_EXTERNAL_DIR ${EXTERNAL_LIBS_DIR}/lexbor)

project(${TARGET_NAME} VERSION 0.1.0)

# Include directory
set(INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include)

# External libraries not in external/
find_package(CURL REQUIRED)
list(APPEND INCLUDE_DIRECTORIES ${CURL_INCLUDE_DIR})

# External libraries in external/
add_subdirectory(${JSON_EXTERNAL_DIR} ${BUILD_DIR}/json)
add_subdirectory(${LEXBOR_EXTERNAL_DIR} ${BUILD_DIR}/lexbor)
list(APPEND INCLUDE_DIRECTORIES ${JSON_EXTERNAL_DIR}/include)
list(APPEND INCLUDE_DIRECTORIES ${LEXBOR_EXTERNAL_DIR}/include)

file(GLOB_RECURSE SOURCE_FILES "src/*.cc")
add_executable(${TARGET_NAME} ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDE_DIRECTORIES})
target_link_libraries(${TARGET_NAME} ${CURL_LIBRARIES} nlohmann_json lexbor)
set_target_properties(${TARGET_NAME} PROPERTIES DEBUD_POSTFIX ${CMAKE_DEBUG_POSTFIX})
