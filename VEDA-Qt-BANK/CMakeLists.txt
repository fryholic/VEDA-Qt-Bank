# cmake_minimum_required(VERSION 3.16)

# project(VEDA-Qt-BANK VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# set(PROJECT_SOURCES
#         main.cpp
#         mainwindow.cpp
#         mainwindow.h
#         mainwindow.ui
# )

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(VEDA-Qt-BANK
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#         resources.qrc
#         main.qml
#         bankmodel.cpp
#         bankmodel.cpp bankmodel.h
#     )
# # Define target properties for Android with Qt 6 as:
# #    set_property(TARGET VEDA-Qt-BANK APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
# else()
#     if(ANDROID)
#         add_library(VEDA-Qt-BANK SHARED
#             ${PROJECT_SOURCES}
#         )
# # Define properties for Android with Qt 5 after find_package() calls as:
# #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#     else()
#         add_executable(VEDA-Qt-BANK
#             ${PROJECT_SOURCES}
#         )
#     endif()
# endif()

# target_link_libraries(VEDA-Qt-BANK PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# target_link_libraries(your_target Qt5::Qml Qt5::Quick Qt5::Core)  # CMake

# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# # If you are developing for iOS or macOS you should consider setting an
# # explicit, fixed bundle identifier manually though.
# if(${QT_VERSION} VERSION_LESS 6.1.0)
#   set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VEDA-Qt-BANK)
# endif()
# set_target_properties(VEDA-Qt-BANK PROPERTIES
#     ${BUNDLE_ID_OPTION}
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# include(GNUInstallDirs)
# install(TARGETS VEDA-Qt-BANK
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(VEDA-Qt-BANK)
# endif()



## -------------------------

cmake_minimum_required(VERSION 3.16)
project(VEDA-Qt-BANK VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 필수 Qt 모듈 검색 (Widgets + QML/Quick)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Qml Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Qml Quick)

set(PROJECT_SOURCES
    main.cpp



    bankmodel.cpp
    bankmodel.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(VEDA-Qt-BANK
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
    )

    # QML 모듈 통합 (Qt6)
    qt_add_qml_module(VEDA-Qt-BANK
        URI "com.veda.bank"
        VERSION 1.0
        QML_FILES
            main.qml
        SOURCES
            bankmodel.cpp
            QML_FILES main.qml
    )
else()
    # Qt5 처리
    if(ANDROID)
        add_library(VEDA-Qt-BANK SHARED ${PROJECT_SOURCES})
    else()
        add_executable(VEDA-Qt-BANK ${PROJECT_SOURCES})
    endif()

    # Qt5 QML 모듈 수동 설정
    qt5_add_resources(VEDA-Qt-BANK "qml_resources" main.qml)
endif()

# 공통 링크 설정
target_link_libraries(VEDA-Qt-BANK PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Quick
)

# macOS/iOS 설정
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.VEDA-Qt-BANK)
endif()
set_target_properties(VEDA-Qt-BANK PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS VEDA-Qt-BANK
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(VEDA-Qt-BANK)
endif()
