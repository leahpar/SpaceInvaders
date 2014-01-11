# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH
  /usr/local
)

SET(CMAKE_C_COMPILER gcc-4.8)
SET(CMAKE_CXX_COMPILER g++-4.8)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


