project(IntervalTests)
cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0054 NEW)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPP17_FLAG "-std=c++17")

include(CTest)
#include(cmake/Catch.cmake)

enable_testing()

if(MSVC)
  #add_definitions(/std:c++latest /W4 /w14545 /w34242 /w34254 /w34287 /w44263 /w44265 /w44296 /w44311 /w44826 /we4289 /w14546 /w14547 /w14549 /w14555 /w14619 /w14905 /w14906 /w14928)

  add_definitions(/std:c++17)


  if (MSVC_VERSION STREQUAL "1800")
    # VS2013 doesn't have magic statics
    add_definitions(/w44640)
  else()
    # enum warnings are too noisy on MSVC2013
    add_definitions(/w34062)
  endif()

  add_definitions(/bigobj /permissive-)
  # Note on MSVC compiler flags.
  # The code base selective disables warnings as necessary when the compiler is complaining too much
  # about something that is perfectly valid, or there is simply no technical way around it
  # This particular warning, C4503 is in regards to the decorated names that MSVC generates internally.
  # The error did not come up until the move to C++11, but the compiler doesn't give enough information
  # to determine where the error is coming from, and the internet provides no real information for
  # how to workaround or fix the error. So I'm disabling it globally.
  add_definitions(/wd4503)
else()
  add_definitions(-Wall -Wextra -Wconversion -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wcast-qual -Wunused -Woverloaded-virtual -Wno-noexcept-type -Wpedantic ${CPP17_FLAG})

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_definitions(-Weverything -Wno-c++98-compat-pedantic -Wno-self-assign-overloaded -Wno-c++98-compat -Wno-documentation -Wno-switch-enum -Wno-weak-vtables -Wno-missing-prototypes -Wno-padded -Wno-missing-noreturn -Wno-exit-time-destructors -Wno-documentation-unknown-command -Wno-unused-template -Wno-undef )
  else()
    add_definitions(-Wnoexcept)
  endif()

  # if(APPLE)
  #   add_definitions(-Wno-sign-compare)
  # endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  #option(USE_LIBCXX "Use clang's libcxx" TRUE)
SET(CMAKE_LINKER lld)
# SET(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

  if(USE_LIBCXX)
    add_definitions(-stdlib=libc++)# -fuse-ld=lld)
    set(LINKER_FLAGS "${LINKER_FLAGS} ${CPP17_FLAG} -fuse-ld=lld -stdlib=libc++")
  else()
    set(LINKER_FLAGS "${LINKER_FLAGS} ${CPP17_FLAG}")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
  set(LINKER_FLAGS "${LINKER_FLAGS} ${CPP17_FLAG}")
endif()
# limitations in MinGW require us to make an optimized build
# for the sake of object sizes or something
if(MINGW OR CYGWIN)
  add_definitions(-O3)
endif()

# set(Catch2 ../catch.hpp)

add_executable(IntervalTests ./tests.cpp)
target_sources(IntervalTests PRIVATE ./StaticInterval.cpp)
target_sources(IntervalTests PRIVATE ./DynamicInterval.cpp)
target_sources(IntervalTests PRIVATE ./Containers.cpp)