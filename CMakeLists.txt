cmake_minimum_required(VERSION 3.14)
project(FileDistance C)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_FLAGS "-O3 -flto -fwhole-program -march=native -Wall -Wextra")
#set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)


add_executable(FileDistance
        src/main.c
        include/distance.h
        src/distance.c
        include/search.h
        src/search.c
        include/apply.h
        src/apply.c
        include/script.h
        src/script.c
        include/util.h
        src/util.c
        include/list.h
        src/list.c

        include/safe_str/strlcpy.h
            src/safe_str/strlcpy.c)