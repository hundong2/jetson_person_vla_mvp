cmake_minimum_required(VERSION 3.16)
project(jetson_person_vla_mvp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs videoio highgui dnn objdetect)

add_library(person_vla_core
    src/ActionPolicy.cpp
    src/AppConfig.cpp
    src/IdentityStore.cpp
    src/PersonDetector.cpp
    src/SceneNarrator.cpp
    src/Tracker.cpp
)

target_include_directories(person_vla_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(person_vla_core PUBLIC ${OpenCV_LIBS})

add_executable(person_vla src/main.cpp)
target_link_libraries(person_vla PRIVATE person_vla_core)

enable_testing()

add_executable(identity_store_test tests/identity_store_test.cpp)
target_link_libraries(identity_store_test PRIVATE person_vla_core)
add_test(NAME identity_store_test COMMAND identity_store_test)

