# cpp-utils
# LICENSE
* [Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)](https://creativecommons.org/licenses/by-nc/4.0/)
# BUILD
* **CMake**
  * OpenSSL
  ```
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/github.com/unwitting.life/cpp-utils/OpenSSL/3.0.6+/darwin64-x86_64-cc/include)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/github.com/unwitting.life/cpp-utils/OpenSSL/3.0.6+/darwin64-x86_64-cc/lib)
    add_executable(${PROJECT_NAME} main.cpp)
    target_link_libraries(${PROJECT_NAME} crypto ssl)
  ```