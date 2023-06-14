add_library(UI OBJECT MainMenu.cpp)


list(APPEND daed_libs "UI")
set(daed_libs ${daed_libs} PARENT_SCOPE)
