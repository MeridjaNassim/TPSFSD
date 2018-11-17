cmake_minimum_required(VERSION 3.10)
project(TPSFSD C)

set(CMAKE_C_STANDARD 11)

add_executable(TPSFSD main.c machineAbstraite.c machineAbstraite.h declarations.h utility.c utility.h)