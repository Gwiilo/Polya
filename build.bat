@echo off

REM Set up the Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

REM Set Vulkan SDK environment variable if not already set
if not defined VULKAN_SDK (
    set VULKAN_SDK=C:\Path\To\Your\VulkanSDK
)

SET includes=/Isrc /I%VULKAN_SDK%/Include /Iclasses /Ifuncs
SET links=/link /LIBPATH:%VULKAN_SDK%/Lib vulkan-1.lib user32.lib
SET defines=/D DEBUG

echo "Building main..."

cl /EHsc /Z7 /Fe"main" %includes% %defines% src/platform/win32platform.cpp %links%