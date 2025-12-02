@echo off
setlocal enabledelayedexpansion

set CC=cl.exe
set CFLAGS=/W4 /Zi /Od /GS /D_FORTIFY_SOURCE=2
set INCLUDE_DIR=/I ..\include
set OUTPUT=main.exe

set SOURCES=
for %%f in (*.c) do (
	if defined SOURCES (
		set SOURCES=!SOURCES! %%f
	) else (
		set SOURCES=%%f
	)
)

set LIBS=/link ..\lib\yta.lib legacy_stdio_definitions.lib opengl32.lib gdi32.lib user32.lib kernel32.lib /SUBSYSTEM:CONSOLE
set CMD=%CC% %CFLAGS% %INCLUDE_DIR% %SOURCES% %LIBS% /OUT:%OUTPUT%

%CMD%
