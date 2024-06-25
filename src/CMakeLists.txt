include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)
qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    # app sources
    app/mainwindow.cpp
    app/main.cpp

    # database sources
    database/mysqldb.cpp
    database/mysqlquery.cpp

    # gui sources
    gui/librarianportal.cpp
    gui/userportal.cpp
    gui/loginform.cpp

    # models sources
    models/bookmanager.cpp
    models/datatable.cpp
    # models/transactionmanager.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Sql)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
