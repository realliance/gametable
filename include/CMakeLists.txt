set(DIR ${CMAKE_CURRENT_LIST_DIR})

target_sources(${PROJECT_NAME}
  PRIVATE
  "${DIR}/gametable.h"
  "${DIR}/enginecontroller.h"
  "${DIR}/aiproxy.h"
  "${DIR}/tablemanager.h"
  "${DIR}/requestutilities.h"
)
