cmake_minimum_required(VERSION 3.16)

project(HanwhaBankApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core Gui Widgets Sql)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.h
    mainwindow.cpp
    bankmodel.h
    bankmodel.cpp
    loginwidget.h
    loginwidget.cpp
    dashboardwidget.h
    dashboardwidget.cpp
    accountwidget.h
    accountwidget.cpp
    transactionwidget.h
    transactionwidget.cpp
    verificationwidget.h
    verificationwidget.cpp
    completionwidget.h
    completionwidget.cpp
    resources.qrc
)

qt_add_executable(HanwhaBankApp
    ${PROJECT_SOURCES}

)

target_link_libraries(HanwhaBankApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)

# 리소스 파일 복사
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/images/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fonts/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/styles/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/styles/)

# 배포 설정
install(TARGETS HanwhaBankApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
