cmake_minimum_required(VERSION 3.14)
project(filedistance C)

set(CMAKE_C_STANDARD 11)

#set(CMAKE_C_FLAGS "-O3 -flto -fwhole-program -march=native -Wall -Wextra")
#set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

include_directories(include)

add_executable(filedistance
        src/main.c
        src/distance.c
        src/search.c
        src/apply.c
        src/script.c
        src/util.c
        src/list.c

        src/safe_str/strlcpy.c)