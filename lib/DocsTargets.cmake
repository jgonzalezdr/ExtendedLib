#
# Source code documentation
#

option( CHM_DOCS "Generate CHM documentation" OFF )
option( HTML_DOCS "Generate HTML documentation" OFF )
option( PDF_DOCS "Generate PDF documentation" OFF )

if( CHM_DOCS OR HTML_DOCS OR PDF_DOCS )

    find_package( Doxygen )
    if( NOT DOXYGEN_FOUND )
        message(FATAL_ERROR "Doxygen is not installed")
    endif( NOT DOXYGEN_FOUND )

    add_custom_target( ${TARGET_NAMESPACE}docs )

endif()

if( CHM_DOCS )

    find_package( HTMLHelp )
    if( NOT EXISTS ${HTML_HELP_COMPILER} )
        message(FATAL_ERROR "HTML Help Compiler is not installed")
    endif()
    
    set( DOXYGEN_CHM_INPUT doxygen/chm.doxyfile )
    set( DOXYGEN_CHM_FILE ${PROJECT_NAME}.Reference.chm )
    set( DOXYGEN_CHM_OUT_PATH doxygen/gendocs/chm )

    configure_file( ${DOXYGEN_CHM_INPUT}.in ${DOXYGEN_CHM_INPUT} )

    add_custom_target( ${TARGET_NAMESPACE}chm_docs
        COMMAND ${CMAKE_COMMAND} -E echo "Building CHM Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CHM_INPUT}
        COMMAND ${CMAKE_COMMAND} -E copy ${DOXYGEN_CHM_OUT_PATH}/output.chm ${PROJECT_SOURCE_DIR}/gendocs/${DOXYGEN_CHM_FILE}
    )

    add_dependencies( ${TARGET_NAMESPACE}docs ${TARGET_NAMESPACE}chm_docs )

endif( CHM_DOCS )

if( PDF_DOCS )

    find_package( Latex )
    if( NOT EXISTS ${PDFLATEX_COMPILER} )
        message(FATAL_ERROR "PDFLatex Compiler is not installed")
    endif()

    set( DOXYGEN_PDF_INPUT doxygen/pdf.doxyfile )
    set( DOXYGEN_PDF_FILE ${PROJECT_NAME}.Reference.pdf )
    set( DOXYGEN_PDF_OUT_PATH doxygen/gendocs/latex )

    configure_file( ${DOXYGEN_PDF_INPUT}.in ${DOXYGEN_PDF_INPUT} )

    add_custom_target( ${TARGET_NAMESPACE}pdf_docs
        COMMAND ${CMAKE_COMMAND} -E echo "Building PDF Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_PDF_INPUT}
        COMMAND cd ${PROJECT_BINARY_DIR}/${DOXYGEN_PDF_OUT_PATH} && make.bat
        COMMAND ${CMAKE_COMMAND} -E copy ${DOXYGEN_PDF_OUT_PATH}/refman.pdf ${PROJECT_SOURCE_DIR}/gendocs/${DOXYGEN_PDF_FILE}
    )

    add_dependencies( ${TARGET_NAMESPACE}docs ${TARGET_NAMESPACE}pdf_docs )

endif( PDF_DOCS )

if( HTML_DOCS )

    set( DOXYGEN_HTML_INPUT doxygen/html.doxyfile )
    set( DOXYGEN_HTML_OUT_PATH doxygen/gendocs/html )

    configure_file( ${DOXYGEN_HTML_INPUT}.in ${DOXYGEN_HTML_INPUT} )

    add_custom_target( ${TARGET_NAMESPACE}html_docs
        COMMAND ${CMAKE_COMMAND} -E echo "Building HTML Documentation..."
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/gendocs/html
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_HTML_INPUT}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${DOXYGEN_HTML_OUT_PATH} ${PROJECT_SOURCE_DIR}/gendocs/html
    )

    add_dependencies( ${TARGET_NAMESPACE}docs ${TARGET_NAMESPACE}html_docs )

endif( HTML_DOCS )
