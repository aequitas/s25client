configure_file(languageFiles.h.cmake languageFiles.h)
add_testcase(NAME LanguageFiles
    LIBS mygettext s25util::common Boost::filesystem
    INCLUDES ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(Test_LanguageFiles translations)
