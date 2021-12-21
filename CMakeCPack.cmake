# Replace spaces in project name with underscores.
string(REPLACE " - " "-" PackageName "${PROJECT_NAME}")
string(REPLACE " " "_" PackageName "${PackageName}")

set(CPACK_PACKAGE_NAME "${PackageName}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

# Files to be ignored
list(APPEND IgnoreFiles
    # Build directory
    "[Bb]uild"
    "[Oo]ut"

    # IDE-generated
    ".vs"
    "CMakeLists.txt.user.*"
    ".*.kdev4"
    ".kdev4/"
    ".vscode"
    ".idea"

    # Files specific to version control.
    ".git/"

    # ImGui
    "imgui.ini"
)

set(CPACK_SOURCE_IGNORE_FILES ${IgnoreFiles})
set(CPACK_SOURCE_GENERATOR "ZIP")

include(CPack)
