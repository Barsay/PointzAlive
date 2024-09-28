
#Arg: subdirectory of project. The directory Must contain cmakelists and gameInfo
function(PA_add_game subdir)
    #Set cmake policy CMP0079: target_link_libraries() allows use with targets in other directories.
    cmake_policy(PUSH)
    cmake_policy(SET "CMP0079" NEW)

    set(caller_dir ${CMAKE_CURRENT_LIST_DIR})
    set(full_path "${caller_dir}/${subdir}")

    #Check cmakeLists
    if(NOT EXISTS ${full_path}/CMakeLists.txt)
        message(FATAL_ERROR "${full_path} doesn't contain CMakeLists.txt")
    endif()

    #Check gameInfo
    if(NOT EXISTS ${full_path}/gameInfo.cmake)
        message(FATAL_ERROR "${full_path} doesn't contain gameInfo.cmake")
    endif()


    #Add project as subdirectory
    add_subdirectory(${full_path})

    #Read game Information
    include(${full_path}/gameInfo.cmake)

    #Print game info
    PA_printGameInfo(${GAME_NAME} ${GAME_LIB_NAME} ${GAME_VERSION} ${GAME_NEEDS_CUDA} ${GAME_CLASS_NAME} ${GAME_HEADER})

    #Link game to
    target_include_directories(${GAME_LIB_NAME} PRIVATE ../../lib/common_include)
    target_link_libraries(${GAME_LIB_NAME} PUBLIC common_include)

    # Append game to selection menu enum header file
    if(NOT DEFINED PA_SUBPROJECTS_SELECTION)
        set(PA_SUBPROJECTS_SELECTION "" PARENT_SCOPE)
    endif()
    set(PA_SUBPROJECTS_SELECTION ${PA_SUBPROJECTS_SELECTION}${GAME_NAME}, PARENT_SCOPE)

    # Append game to selection function header file
    if(NOT DEFINED PA_SUBPROJECT_MENU)
        set(PA_SUBPROJECT_MENU "" PARENT_SCOPE)
    endif()

    #Append to GameSelection.h
    PA_AppendToGameSelection(${GAME_NAME} ${GAME_CLASS_NAME})

    #Append to Include
    PA_AppendToInclude(${GAME_HEADER})

    #Link and include game to PointzALive
    target_include_directories(${PROJECT_NAME} PRIVATE ${full_path})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${GAME_LIB_NAME})

    cmake_policy(POP)
endfunction()

function(PA_printGameInfo GAME_NAME GAME_LIB_NAME GAME_VERSION GAME_NEEDS_CUDA GAME_CLASS_NAME GAME_HEADER)
    message(STATUS "-------------------------------")
    message(STATUS "${BLUE}GAME ${GAME_NAME}${RESET}")
    message(STATUS "${BLUE}VERSION ${GAME_VERSION}${RESET}")
    message(STATUS "${BLUE}GAME LIB NAME ${GAME_LIB_NAME}${RESET}")
    message(STATUS "${BLUE}NEEDS CUDA ${GAME_NEEDS_CUDA}${RESET}")
    message(STATUS "${BLUE}GAME CLASS NAME ${GAME_CLASS_NAME}${RESET}")
    message(STATUS "${BLUE}HEADER ${GAME_HEADER}${RESET}")
    message(STATUS "-------------------------------")

endfunction()

function(PA_init)

    set(SUBPROJECTS "" CACHE INTERNAL "List of subprojects")

    #Set global variable to include VCPKG in Common include and games.
    #This looks for imgui.h only
    find_path(VCPKG_INCLUDES imgui.h)
    set(VCPKG_INCLUDE_DIRS ${VCPKG_INCLUDE_DIRS} PARENT_SCOPE)

    #Remove old PA_GameSelection
    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_Selection.h)
        file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_Selection.h)
        message(STATUS "File ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_Selection.h cleaned.")
    else()
        message(STATUS "File ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_Selection.h")
    endif()

    #Init PA_GameSelection
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_Selection.h
            "#ifndef PA_SHOWSELECTIONMENU_H\n"
            "#define PA_SHOWSELECTIONMENU_H\n"
            "#define PA_SHOWSELECTIONMENU ")

    #Remove old PA_GameIncludes.h
    if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/PA_GameIncludes.h)
        file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/PA_GameIncludes.h)
        message(STATUS "File ${CMAKE_CURRENT_BINARY_DIR}/PA_GameIncludes.h cleaned.")
    else()
        message(STATUS "File ${CMAKE_CURRENT_BINARY_DIR}/PA_GameIncludes.h")
    endif()

    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/PA_GameIncludes.h "#ifndef PA_GAMEINCLUDES\n#define PA_GAMEINCLUDES\n")




endfunction()

function(PA_generateHeaders pa_subproject_selection)

    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/utils/files/PA_Headers.h.in
            ${CMAKE_CURRENT_BINARY_DIR}/PA_Headers.h
            @ONLY
    )

    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_Selection.h "\n#endif /// PA_SELECTION_ENUM")

    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/PA_GameIncludes.h "\n#endif /// PA_GAMEINCLUDES")
    #string(REPLACE ";" " " pa_game_selection "${pa_game_selection}")
    #configure_file(
    #        ${CMAKE_CURRENT_SOURCE_DIR}/utils/files/PA_Game_selection.h.in
    #        ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_selection.h
    #        @ONLY
    #)

endfunction()

function(PA_AppendToGameSelection game_name game_class)
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_Selection.h "\\\nif (ImGui::MenuItem(\"PA_SELECTION::${GAME_NAME}\")) { \\\n\t selection = ${GAME_NAME}; \\\n\t onParameterChanged(selection, tempSelection, myMap, hasSelectionChanged, nogame::init); \\\n }")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_Selection.h "")
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/PA_Game_Selection.h "")

endfunction()

function(PA_AppendToInclude game_header)
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/PA_GameIncludes.h "#include \"${game_header}\"\n")

endfunction()