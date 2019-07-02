cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(BlitzMaster C CXX)

set(wxWidgets_ROOT_DIR src/wxWidgets)
find_package(wxWidgets COMPONENTS base core adv net stc propgrid regex jpeg REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(src/wxWidgets/include)
include_directories(src/wxWidgets/contrib/include)

#include(${CMAKE_CURRENT_SOURCE_DIR}/src/tinyxml2/CMakeLists.txt)

set(TINYXML2_SRC
	src/tinyxml2/tinyxml2.cpp
	src/tinyxml2/tinyxml2.h
)
add_library(tinyxml2 STATIC ${TINYXML2_SRC})

include_directories(src/tinyxml2)

# set(WXWIDGETS_SRC
	
	
	# #PropGrid
	# src/wxWidgets/contrib/include/wx/propgrid/advprops.h
	# src/wxWidgets/contrib/src/propgrid/advprops.cpp
	# src/wxWidgets/contrib/include/wx/propgrid/extras.h
	# src/wxWidgets/contrib/src/propgrid/extras.cpp
	# src/wxWidgets/contrib/include/wx/propgrid/manager.h
	# src/wxWidgets/contrib/src/propgrid/manager.cpp
	# src/wxWidgets/contrib/include/wx/propgrid/odcombo.h
	# src/wxWidgets/contrib/src/propgrid/odcombo.cpp
	# src/wxWidgets/contrib/include/wx/propgrid/propgrid.h
	# src/wxWidgets/contrib/src/propgrid/propgrid.cpp
	# src/wxWidgets/contrib/include/wx/propgrid/propdev.h
	# src/wxWidgets/contrib/src/propgrid/props.cpp
# )
# add_library(wxWidgets STATIC ${WXWIDGETS_SRC})

set(TORSION_SRC
	# Sources
	src/Torsion/AboutDlg.cpp
	src/Torsion/AppPrefs.cpp
	src/Torsion/AutoCompClass.cpp
	src/Torsion/AutoCompData.cpp
	src/Torsion/AutoCompExports.cpp
	src/Torsion/AutoCompFunction.cpp
	src/Torsion/AutoCompManager.cpp
	src/Torsion/AutoCompPage.cpp
	src/Torsion/AutoCompText.cpp
	src/Torsion/AutoCompThread.cpp
	src/Torsion/AutoCompVar.cpp
	src/Torsion/Bookmark.cpp
	src/Torsion/Breakpoint.cpp
	src/Torsion/BreakpointPropertyDlg.cpp
	src/Torsion/BreakpointsPanel.cpp
	src/Torsion/BuildExportsDlg.cpp
	src/Torsion/BuildExportsThread.cpp
	src/Torsion/CallTipInfo.cpp
	src/Torsion/ClientWindow.cpp
	src/Torsion/CodeBrowserCtrl.cpp
	src/Torsion/CodeBrowserPanel.cpp
	src/Torsion/ColourHLS.cpp
	src/Torsion/ConfigDlg.cpp
	src/Torsion/ConnectDlg.cpp
	src/Torsion/CRC.cpp
	src/Torsion/Debugger.cpp
	src/Torsion/DirWatcher.cpp
	src/Torsion/DirWatcherWin9x.cpp
	src/Torsion/DirWatcherWinNT.cpp
	src/Torsion/DocManager.cpp
	src/Torsion/ExternalToolsDlg.cpp
	src/Torsion/FileTypeImageList.cpp
	src/Torsion/FileWatcher.cpp
	src/Torsion/FindInFilesDlg.cpp
	src/Torsion/FindReplaceDlg.cpp
	src/Torsion/FindResultsCtrl.cpp
	src/Torsion/FindSymbolDlg.cpp
	src/Torsion/FindThread.cpp
	src/Torsion/FunctionCall.cpp
	src/Torsion/GotoDefDialog.cpp
	src/Torsion/Icons.cpp
	src/Torsion/ImageUtil.cpp
	src/Torsion/MainFrame.cpp
	src/Torsion/OutputCtrl.cpp
	src/Torsion/OutputPanel.cpp
	src/Torsion/PlatformWin.cpp
	src/Torsion/PreCompiler.cpp
	src/Torsion/ProjectConfig.cpp
	src/Torsion/ProjectCtrl.cpp
	src/Torsion/ProjectDlg.cpp
	src/Torsion/ProjectDoc.cpp
	src/Torsion/ProjectPanel.cpp
	src/Torsion/ProjectView.cpp
	src/Torsion/ReloadFile.cpp
	src/Torsion/ScriptCtrl.cpp
	src/Torsion/ScriptDoc.cpp
	src/Torsion/ScriptFrame.cpp
	src/Torsion/ScriptPrintout.cpp
	src/Torsion/ScriptScanner.cpp
	src/Torsion/ScriptView.cpp
	src/Torsion/ScrollMapCtrl.cpp
	src/Torsion/SettingsDlg.cpp
	src/Torsion/ShellMenu.cpp
	src/Torsion/shNotebook.cpp
	src/Torsion/StackDump.cpp
	src/Torsion/ToolCommand.cpp
	src/Torsion/TorsionApp.cpp
	src/Torsion/tsComboBox.cpp
	src/Torsion/tsMenu.cpp
	src/Torsion/tsMenuBar.cpp
	src/Torsion/tsSashLayoutWindow.cpp
	src/Torsion/tsShellExecute.cpp
	src/Torsion/tsStatusBar.cpp
	src/Torsion/tsToolBar.cpp
	src/Torsion/UpdateChecker.cpp
	src/Torsion/VSMenuItem.cpp
	src/Torsion/WatchCtrl.cpp
	src/Torsion/WatchVar.cpp
	src/Torsion/XmlFile.cpp
	
	# Headers
	src/Torsion/AboutDlg.h
	src/Torsion/AppPrefs.h
	src/Torsion/AutoComp.h
	src/Torsion/AutoCompClass.h
	src/Torsion/AutoCompData.h
	src/Torsion/AutoCompExports.h
	src/Torsion/AutoCompFunction.h
	src/Torsion/AutoCompManager.h
	src/Torsion/AutoCompPage.h
	src/Torsion/AutoCompText.h
	src/Torsion/AutoCompThread.h
	src/Torsion/AutoCompUtil.h
	src/Torsion/AutoCompVar.h
	src/Torsion/Bookmark.h
	src/Torsion/Breakpoint.h
	src/Torsion/BreakpointPropertyDlg.h
	src/Torsion/BreakpointsPanel.h
	src/Torsion/BuildExportsDlg.h
	src/Torsion/BuildExportsThread.h
	src/Torsion/BuildVersion.h
	src/Torsion/CallTipInfo.h
	src/Torsion/ClientWindow.h
	src/Torsion/CodeBrowserCtrl.h
	src/Torsion/CodeBrowserPanel.h
	src/Torsion/ColourHLS.h
	src/Torsion/ConfigDlg.h
	src/Torsion/ConnectDlg.h
	src/Torsion/CRC.h
	src/Torsion/Debugger.h
	src/Torsion/DirWatcher.h
	src/Torsion/DirWatcherWin9x.h
	src/Torsion/DirWatcherWinNT.h
	src/Torsion/DocManager.h
	src/Torsion/ExternalToolsDlg.h
	src/Torsion/FileTypeImageList.h
	src/Torsion/FileWatcher.h
	src/Torsion/FindInFilesDlg.h
	src/Torsion/FindReplaceDlg.h
	src/Torsion/FindResultsCtrl.h
	src/Torsion/FindSymbolDlg.h
	src/Torsion/FindThread.h
	src/Torsion/FunctionCall.h
	src/Torsion/GotoDefDialog.h
	src/Torsion/Icons.h
	src/Torsion/Identifiers.h
	src/Torsion/ImageUtil.h
	src/Torsion/MainFrame.h
	src/Torsion/OutputCtrl.h
	src/Torsion/OutputPanel.h
	src/Torsion/Platform.h
	src/Torsion/PlatformWin.h
	src/Torsion/PreCompiled.h
	src/Torsion/PreCompiler.h
	src/Torsion/ProjectConfig.h
	src/Torsion/ProjectCtrl.h
	src/Torsion/ProjectDlg.h
	src/Torsion/ProjectDoc.h
	src/Torsion/ProjectPanel.h
	src/Torsion/ProjectView.h
	src/Torsion/ReloadFile.h
	src/Torsion/resource.h
	src/Torsion/ScriptCtrl.h
	src/Torsion/ScriptDoc.h
	src/Torsion/ScriptFrame.h
	src/Torsion/ScriptPrintout.h
	src/Torsion/ScriptScanner.h
	src/Torsion/ScriptView.h
	src/Torsion/ScrollMapCtrl.h
	src/Torsion/SettingsDlg.h
	src/Torsion/ShellMenu.h
	src/Torsion/shNotebook.h
	src/Torsion/StackDump.h
	src/Torsion/ToolCommand.h
	src/Torsion/TorsionApp.h
	src/Torsion/tsComboBox.h
	src/Torsion/tsMenu.h
	src/Torsion/tsMenuBar.h
	src/Torsion/tsSashLayoutWindow.h
	src/Torsion/tsShellExecute.h
	src/Torsion/tsStatusBar.h
	src/Torsion/tsToolBar.h
	src/Torsion/UpdateChecker.h
	src/Torsion/VSMenuItem.h
	src/Torsion/WatchCtrl.h
	src/Torsion/WatchVar.h
	src/Torsion/XmlFile.h
	
	# Resources
	src/Torsion/Torsion.rc
	src/Torsion/Torsion.rc2
	
	# TinyXML2
	src/tinyxml2/tinyxml2.h
)

add_executable(BlitzMaster ${TORSION_SRC})
target_link_libraries(BlitzMaster ${wxWidgets_LIBRARIES} tinyxml2)
# target_link_libraries(BlitzMaster wxWidgets tinyxml2)

if (MSVC)
	set_target_properties(BlitzMaster PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif()

# Install rules 
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set (CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE PATH "default install path" FORCE)
endif()

install(TARGETS BlitzMaster DESTINATION bin)