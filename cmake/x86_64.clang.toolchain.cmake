set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# C compiler
set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE STRING "clang compiler" FORCE)

# C compiler options
set(CMAKE_C_FLAGS_INIT "-Weverything -Werror -Wno-padded -Wno-covered-switch-default -Wno-switch-enum" CACHE STRING "c flags init" FORCE)
set(CMAKE_C_FLAGS_DEBUG "-g3 -Og" CACHE STRING "c flags debug" FORCE)
set(CMAKE_C_FLAGS_RELEASE "-O3" CACHE STRING "c flags release" FORCE)
set(CMAKE_C_FLAGS_MINSIZEREL "-Oz" CACHE STRING "c flags minsizerel" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g" CACHE STRING "c flags relwithdebinfo" FORCE)

# C++ compiler
set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE STRING "clang++ compiler" FORCE)

# C++ compiler options
set(CMAKE_CXX_FLAGS_INIT "-Weverything -Werror -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-covered-switch-default -Wno-switch-enum" CACHE STRING "c++ flags init" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Og" CACHE STRING "c++ flags debug" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3" CACHE STRING "c++ flags release" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Oz" CACHE STRING "c++ flags minsizerel" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g" CACHE STRING "c++ flags relwithdebinfo" FORCE)
