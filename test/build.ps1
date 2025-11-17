$CC = "cl.exe"
$CFLAGS = "/W4 /Zi /Od /GS /D_FORTIFY_SOURCE=2"
$INCLUDE = "/I ..\include"
$OUTPUT = "main.exe"

$COMMON_SOURCES = Get-ChildItem -Filter *.c | ForEach-Object { $_.FullName }

$LIBS = "/link ..\lib\yta.lib legacy_stdio_definitions.lib opengl32.lib gdi32.lib user32.lib kernel32.lib /SUBSYSTEM:CONSOLE"

$cmd = "$CC $CFLAGS $INCLUDE $($COMMON_SOURCES -join ' ') $LIBS /OUT:$OUTPUT"

Invoke-Expression $cmd