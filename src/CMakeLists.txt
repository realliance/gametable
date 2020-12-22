set(DIR ${CMAKE_CURRENT_LIST_DIR})

target_sources(${PROJECT_NAME}
  PRIVATE
  "${DIR}/gametable.cpp"
  "${DIR}/enginecontroller.cpp"
  "${DIR}/aiproxy.cpp"
  "${DIR}/tablemanager.cpp"
)
