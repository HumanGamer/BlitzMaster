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

set(BlitzMaster_SRC
	# Sources
	src/BlitzMaster/AboutDlg.cpp
	src/BlitzMaster/AppPrefs.cpp
	src/BlitzMaster/AutoCompClass.cpp
	src/BlitzMaster/AutoCompData.cpp
	src/BlitzMaster/AutoCompExports.cpp
	src/BlitzMaster/AutoCompFunction.cpp
	src/BlitzMaster/AutoCompManager.cpp
	src/BlitzMaster/AutoCompPage.cpp
	src/BlitzMaster/AutoCompText.cpp
	src/BlitzMaster/AutoCompThread.cpp
	src/BlitzMaster/AutoCompVar.cpp
	src/BlitzMaster/Bookmark.cpp
	src/BlitzMaster/Breakpoint.cpp
	src/BlitzMaster/BreakpointPropertyDlg.cpp
	src/BlitzMaster/BreakpointsPanel.cpp
	src/BlitzMaster/BuildExportsDlg.cpp
	src/BlitzMaster/BuildExportsThread.cpp
	src/BlitzMaster/CallTipInfo.cpp
	src/BlitzMaster/ClientWindow.cpp
	src/BlitzMaster/CodeBrowserCtrl.cpp
	src/BlitzMaster/CodeBrowserPanel.cpp
	src/BlitzMaster/ColourHLS.cpp
	src/BlitzMaster/ConfigDlg.cpp
	src/BlitzMaster/ConnectDlg.cpp
	src/BlitzMaster/CRC.cpp
	src/BlitzMaster/Debugger.cpp
	src/BlitzMaster/DirWatcher.cpp
	src/BlitzMaster/DirWatcherWin9x.cpp
	src/BlitzMaster/DirWatcherWinNT.cpp
	src/BlitzMaster/DocManager.cpp
	src/BlitzMaster/ExternalToolsDlg.cpp
	src/BlitzMaster/FileTypeImageList.cpp
	src/BlitzMaster/FileWatcher.cpp
	src/BlitzMaster/FindInFilesDlg.cpp
	src/BlitzMaster/FindReplaceDlg.cpp
	src/BlitzMaster/FindResultsCtrl.cpp
	src/BlitzMaster/FindSymbolDlg.cpp
	src/BlitzMaster/FindThread.cpp
	src/BlitzMaster/FunctionCall.cpp
	src/BlitzMaster/GotoDefDialog.cpp
	src/BlitzMaster/Icons.cpp
	src/BlitzMaster/ImageUtil.cpp
	src/BlitzMaster/MainFrame.cpp
	src/BlitzMaster/OutputCtrl.cpp
	src/BlitzMaster/OutputPanel.cpp
	src/BlitzMaster/PlatformWin.cpp
	src/BlitzMaster/PreCompiler.cpp
	src/BlitzMaster/ProjectConfig.cpp
	src/BlitzMaster/ProjectCtrl.cpp
	src/BlitzMaster/ProjectDlg.cpp
	src/BlitzMaster/ProjectDoc.cpp
	src/BlitzMaster/ProjectPanel.cpp
	src/BlitzMaster/ProjectView.cpp
	src/BlitzMaster/ReloadFile.cpp
	src/BlitzMaster/ScriptCtrl.cpp
	src/BlitzMaster/ScriptDoc.cpp
	src/BlitzMaster/ScriptFrame.cpp
	src/BlitzMaster/ScriptPrintout.cpp
	src/BlitzMaster/ScriptScanner.cpp
	src/BlitzMaster/ScriptView.cpp
	src/BlitzMaster/ScrollMapCtrl.cpp
	src/BlitzMaster/SettingsDlg.cpp
	src/BlitzMaster/ShellMenu.cpp
	src/BlitzMaster/shNotebook.cpp
	src/BlitzMaster/StackDump.cpp
	src/BlitzMaster/ToolCommand.cpp
	src/BlitzMaster/BlitzMasterApp.cpp
	src/BlitzMaster/tsComboBox.cpp
	src/BlitzMaster/tsMenu.cpp
	src/BlitzMaster/tsMenuBar.cpp
	src/BlitzMaster/tsSashLayoutWindow.cpp
	src/BlitzMaster/tsShellExecute.cpp
	src/BlitzMaster/tsStatusBar.cpp
	src/BlitzMaster/tsToolBar.cpp
	src/BlitzMaster/UpdateChecker.cpp
	src/BlitzMaster/VSMenuItem.cpp
	src/BlitzMaster/WatchCtrl.cpp
	src/BlitzMaster/WatchVar.cpp
	src/BlitzMaster/XmlFile.cpp
	
	# Headers
	src/BlitzMaster/AboutDlg.h
	src/BlitzMaster/AppPrefs.h
	src/BlitzMaster/AutoComp.h
	src/BlitzMaster/AutoCompClass.h
	src/BlitzMaster/AutoCompData.h
	src/BlitzMaster/AutoCompExports.h
	src/BlitzMaster/AutoCompFunction.h
	src/BlitzMaster/AutoCompManager.h
	src/BlitzMaster/AutoCompPage.h
	src/BlitzMaster/AutoCompText.h
	src/BlitzMaster/AutoCompThread.h
	src/BlitzMaster/AutoCompUtil.h
	src/BlitzMaster/AutoCompVar.h
	src/BlitzMaster/Bookmark.h
	src/BlitzMaster/Breakpoint.h
	src/BlitzMaster/BreakpointPropertyDlg.h
	src/BlitzMaster/BreakpointsPanel.h
	src/BlitzMaster/BuildExportsDlg.h
	src/BlitzMaster/BuildExportsThread.h
	src/BlitzMaster/BuildVersion.h
	src/BlitzMaster/CallTipInfo.h
	src/BlitzMaster/ClientWindow.h
	src/BlitzMaster/CodeBrowserCtrl.h
	src/BlitzMaster/CodeBrowserPanel.h
	src/BlitzMaster/ColourHLS.h
	src/BlitzMaster/ConfigDlg.h
	src/BlitzMaster/ConnectDlg.h
	src/BlitzMaster/CRC.h
	src/BlitzMaster/Debugger.h
	src/BlitzMaster/DirWatcher.h
	src/BlitzMaster/DirWatcherWin9x.h
	src/BlitzMaster/DirWatcherWinNT.h
	src/BlitzMaster/DocManager.h
	src/BlitzMaster/ExternalToolsDlg.h
	src/BlitzMaster/FileTypeImageList.h
	src/BlitzMaster/FileWatcher.h
	src/BlitzMaster/FindInFilesDlg.h
	src/BlitzMaster/FindReplaceDlg.h
	src/BlitzMaster/FindResultsCtrl.h
	src/BlitzMaster/FindSymbolDlg.h
	src/BlitzMaster/FindThread.h
	src/BlitzMaster/FunctionCall.h
	src/BlitzMaster/GotoDefDialog.h
	src/BlitzMaster/Icons.h
	src/BlitzMaster/Identifiers.h
	src/BlitzMaster/ImageUtil.h
	src/BlitzMaster/MainFrame.h
	src/BlitzMaster/OutputCtrl.h
	src/BlitzMaster/OutputPanel.h
	src/BlitzMaster/Platform.h
	src/BlitzMaster/PlatformWin.h
	src/BlitzMaster/PreCompiled.h
	src/BlitzMaster/PreCompiler.h
	src/BlitzMaster/ProjectConfig.h
	src/BlitzMaster/ProjectCtrl.h
	src/BlitzMaster/ProjectDlg.h
	src/BlitzMaster/ProjectDoc.h
	src/BlitzMaster/ProjectPanel.h
	src/BlitzMaster/ProjectView.h
	src/BlitzMaster/ReloadFile.h
	src/BlitzMaster/resource.h
	src/BlitzMaster/ScriptCtrl.h
	src/BlitzMaster/ScriptDoc.h
	src/BlitzMaster/ScriptFrame.h
	src/BlitzMaster/ScriptPrintout.h
	src/BlitzMaster/ScriptScanner.h
	src/BlitzMaster/ScriptView.h
	src/BlitzMaster/ScrollMapCtrl.h
	src/BlitzMaster/SettingsDlg.h
	src/BlitzMaster/ShellMenu.h
	src/BlitzMaster/shNotebook.h
	src/BlitzMaster/StackDump.h
	src/BlitzMaster/ToolCommand.h
	src/BlitzMaster/BlitzMasterApp.h
	src/BlitzMaster/tsComboBox.h
	src/BlitzMaster/tsMenu.h
	src/BlitzMaster/tsMenuBar.h
	src/BlitzMaster/tsSashLayoutWindow.h
	src/BlitzMaster/tsShellExecute.h
	src/BlitzMaster/tsStatusBar.h
	src/BlitzMaster/tsToolBar.h
	src/BlitzMaster/UpdateChecker.h
	src/BlitzMaster/VSMenuItem.h
	src/BlitzMaster/WatchCtrl.h
	src/BlitzMaster/WatchVar.h
	src/BlitzMaster/XmlFile.h
	
	# Resources
	src/BlitzMaster/BlitzMaster.rc
	src/BlitzMaster/BlitzMaster.rc2
	
	# TinyXML2
	src/tinyxml2/tinyxml2.h
)

add_executable(BlitzMaster ${BlitzMaster_SRC})
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