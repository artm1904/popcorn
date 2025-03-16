cmake_minimum_required(VERSION 3.5.0)
project(popcorn VERSION 0.1.0 LANGUAGES C CXX)

# Определение переменных с исходными файлами
set(SOURCES
    ../Main/Main.cpp
    ../Engine/Engine.cpp
    ../Border/Border.cpp
    ../Level/Level.cpp
    ../Config/Config.cpp
    ../Ball/Ball.cpp
    ../Platform/Platform.cpp
    ../Level/Active_Brick.cpp
    ../Level/Falling_Letter.cpp
    # Main.cpp
    # Engine.cpp
    # Border.cpp
    # Level.cpp
    # Config.cpp
    # Ball.cpp
    # Platform.cpp
    # Active_Brick.cpp
    
)

# Определение переменных с include файлами
set(HEADERS
    ../Engine/Engine.h
    ../Border/Border.h
    ../Level/Level.h
    ../Config/Config.h
    ../Ball/Ball.h
    ../Platform/Platform.h
    ../Level/Active_Brick.h
    ../Level/Falling_Letter.h
    # Engine.h
    # Border.h
    # Level.h
    # Config.h
    # Ball.h
    # Platform.h
    # Active_Brick.h

)

# Создание исполняемого файла
add_executable(popcorn ${SOURCES} ${HEADERS})

# Указываем где искать header файлы
target_include_directories(popcorn PUBLIC
    include
)

set(MYCOMPILE_FLAGS "-lgdi32 -mwindows -municode -static ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MYCOMPILE_FLAGS}")








