find_package(Qt6 REQUIRED COMPONENTS Test Sql)
qt_standard_project_setup()

enable_testing(true)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

set(testFiles
    testmysqldb.cpp
)

foreach(testFile ${testFiles})
    get_filename_component(testFilename ${testFile} NAME_WE)

    add_executable(${testFilename} ${testFile})
    target_link_libraries(${testFilename} PRIVATE Qt6::Test Qt6::Sql)
    add_test(NAME ${testFilename} COMMAND ${testFilename})

    add_dependencies(check ${testFilename})
endforeach()
