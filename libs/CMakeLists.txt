FetchContent_Declare(
  libmahjong
  GIT_REPOSITORY https://github.com/realliance/libmahjong.git
  GIT_TAG        fdb7dc06c71656781ba1f17aa7eb174fd6f4fb76
)

FetchContent_GetProperties(libmahjong)
if(NOT libmahjong_POPULATED)
  FetchContent_Populate(libmahjong)
  add_subdirectory(${libmahjong_SOURCE_DIR} ${libmahjong_BINARY_DIR})
endif()
