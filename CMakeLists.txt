cmake_minimum_required(VERSION 3.0)
project(bpr)

macro(use_cxx11)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
  endif ()
endmacro(use_cxx11)

set(SOURCE_FILES main.c
                 BPR/bpr.c
                 Evaluation/recomm.c
                 GeneralStruct/gvStruct.c
                 MF/mf.c)

set_source_files_properties(${SOURCE_FILES} PROPERTIES LANGUAGE CXX)
add_executable(bpr ${SOURCE_FILES})

include_directories(BPR Evaluation MF GeneralStruct)
