cmake_minimum_required(VERSION 3.10)
project(DemoApp C)

# Настройка стандарта C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(demo main.c)

# Установка приложения
install(TARGETS demo
    RUNTIME DESTINATION bin
)
