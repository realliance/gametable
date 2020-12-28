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

FetchContent_Declare(
  libmahjonghttp
  GIT_REPOSITORY https://github.com/realliance/libmahjonghttp.git
  GIT_TAG        eeb651a2f51c37d7b490e74a4ea71de2c4134ed9
)

FetchContent_GetProperties(libmahjonghttp)
if(NOT libmahjonghttp_POPULATED)
  FetchContent_Populate(libmahjonghttp)
  add_subdirectory(${libmahjonghttp_SOURCE_DIR} ${libmahjonghttp_BINARY_DIR})
endif()
