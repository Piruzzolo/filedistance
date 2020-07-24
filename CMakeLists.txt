cmake_minimum_required(VERSION 3.14)

project(filedistance C)

set(CMAKE_C_STANDARD 11)

#set(CMAKE_C_FLAGS "-O3 -flto -fwhole-program -march=native -Wall -Wextra")
#set(CMAKE_CONFIGURATION_TYPES "Release" CACHE STRING "" FORCE)

add_executable(filedistance
        include/name_distance.h
        include/list_namedistance.h
        include/distance.h
        include/search.h
        include/apply.h
        include/script.h
        include/util.h
        include/list.h
        include/safe_str/strlcpy.h

        src/main.c
        src/distance.c
        src/search.c
        src/apply.c
        src/script.c
        src/util.c
        src/list.c
        src/list_namedistance.c
        src/name_distance.c
        src/safe_str/strlcpy.c)









