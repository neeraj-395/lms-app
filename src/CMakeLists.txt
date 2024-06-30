include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(${PROJECT_NAME}
    # app sources
    app/mainwindow.h
    app/mainwindow.cpp
    app/mainwindow.ui
    app/main.cpp

    # database sources
    database/mysqldb.h
    database/mysqldb.cpp
    database/mysqlquery.h
    database/mysqlquery.cpp

    # gui sources
    gui/librarianportal.h
    gui/librarianportal.cpp
    gui/librarianportal.ui
    gui/userportal.h
    gui/userportal.cpp
    gui/userportal.ui
    gui/loginform.h
    gui/loginform.cpp
    gui/loginform.ui

    # maagers sources
    managers/bookmanager.h
    managers/bookmanager.cpp
    managers/bookmanager.ui
    managers/transacmanager.h
    managers/transacmanager.cpp
    managers/checkoutbook.ui
    managers/returnbook.ui
    managers/searchbook.ui

    # utils sources
    utils/qutils.h
    utils/qutils.cpp
    utils/userqueries.h
    utils/bookqueries.h
    utils/transacqueries.h

    # resources
    ../res/db.res.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
