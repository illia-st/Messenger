

set(SERVER_NAME Server)
set(ROOT  ../)

include_directories(../Common)
set(COMMON_FILES_LIST)

foreach(X IN LISTS COMMON_FILES)
    string(CONCAT X ${ROOT} ${X})
    LIST(APPEND COMMON_FILES_LIST ${X})
endforeach()

add_library(${SERVER_NAME} STATIC
            "Header/IServer.h"
            "Source/IServer.cpp"
            ${COMMON_FILES_LIST}
        )

