if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zlib/CMakeLists.txt")

    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)

    add_subdirectory(zlib)
    if(NOT TARGET ZLIB::ZLIB)
        if(TARGET zlibstatic)
            add_library(ZLIB::ZLIB ALIAS zlibstatic)
        elseif(TARGET zlib)
            add_library(ZLIB::ZLIB ALIAS zlib)
        endif()
    endif()
endif()