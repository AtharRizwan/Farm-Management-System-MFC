# CMake build for use with MSVC (e.g. msvc-wine on Linux).
# The Visual Studio solution remains the primary build on Windows.
cmake_minimum_required(VERSION 3.23)
project(FarmManagementSystem CXX RC)

# Static MFC and static CRT, matching UseOfMfc=Static in the .vcxproj
set(CMAKE_MFC_FLAG 1)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_executable(FarmManagementSystem WIN32
	"src/Crop.cpp"
	"src/CropsDlg.cpp"
	"src/employee.cpp"
	"src/EmployeeDlg.cpp"
	"src/Farm Management System MFC.cpp"
	"src/Farm Management System MFCDlg.cpp"
	"src/HomePageDlg.cpp"
	"src/pch.cpp"
	"src/FarmManagementSystemMFC.rc"
)

target_compile_definitions(FarmManagementSystem PRIVATE UNICODE _UNICODE _WINDOWS)
target_include_directories(FarmManagementSystem PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_options(FarmManagementSystem PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/EHsc>)
# Unicode MFC apps start at wWinMain (Visual Studio sets this automatically)
target_link_options(FarmManagementSystem PRIVATE /ENTRY:wWinMainCRTStartup)
