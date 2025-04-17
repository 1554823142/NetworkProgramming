# 环境

使用boost库中的网络编程库

- 到官网上下载boost, 然后编译

- vs

  属性中添加库的路径, 其中`include dir`为下载的boost路径, `lib dir`为boost路径下的`stage/lib`, 这里是编译后的库二进制文件

- vscode

  使用的是cmake进行编译

  - 首先在当前项目的`cc_pp_properties.json`中添加`includepath`字段为你自己的boost路径(这样可以在包含头文件的时候可以定位到boost库)

  - cmake的配置:

    这里是我的配置:

    ```cmake
    cmake_minimum_required(VERSION 3.0)
    
    # 项目的名称
    project (homework1)
    
    # 参数1: 生成的可执行文件 参数2: 需要的依赖
    
    set(BOOST_ROOT "E:/boost_1_88_0_b1")
    set(BOOST_INCLUDEDIR "${BOOST_ROOT}")
    set(BOOT_LIBRARYDIR "E:/boost_1_88_0_b1/stage/lib")
    
    # 可执行文件输出到项目根目录的bin文件夹
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)   
    
    add_executable(test ./src/test.cpp ./src/Connection.cpp ./src/ConnectionMgr.cpp ./src/WebsocketServer.cpp)
    target_include_directories(test PRIVATE ${BOOST_ROOT})
    
    target_link_libraries(test PRIVATE ws2_32 mswsock)
    ```

  - 最后可以使用以下的指令进行编译:

    ```bash
    cmake -G "MinGW Makefiles" CMakeLists.txt
    CMake Deprecation Warning at CMakeLists.txt:1 (cmake_minimum_required):
      Compatibility with CMake < 3.10 will be removed from a future version of
      CMake.
    
      Update the VERSION argument <min> value.  Or, use the <min>...<max> syntax
      to tell CMake that the project requires at least <min> but has been updated
      to work with policies introduced by <max> or earlier.
    
    
    -- Configuring done (0.1s)
    -- Generating done (0.0s)
    -- Build files have been written to: C:/Users/Administrator/Desktop/boostTest
    
    
    
    
    mingw32-make
    [100%] Built target test
    ```

    

若有其他的问题, 待补充