project( html2mediawiki )
find_package( KDE4 REQUIRED )
include_directories( ${KDE4_INCLUDES} )
 
set( html2mediawikiSources main.cpp )
 
kde4_add_executable( html2mediawiki ${html2mediawikiSources} )
 
target_link_libraries( html2mediawiki ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} )

install( TARGETS html2mediawiki  ${INSTALL_TARGETS_DEFAULT_ARGS} )
