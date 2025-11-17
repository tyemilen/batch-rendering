import os
import sys
import time
import subprocess
from pathlib import Path
import re

PLATFORM = None
DEBUG = False
CLEAN = False

start_time = time.time()

for arg in sys.argv[1:]:
	if arg.startswith("PLATFORM="):
		PLATFORM = arg.split("=", 1)[1]
	elif arg.startswith("DEBUG="):
		DEBUG = arg.split("=", 1)[1] == "1"
	elif arg == "clean":
		CLEAN = True

if CLEAN:
	for ext in ("*.o", "*.obj"):
		for f in Path("./src").rglob(ext):
			f.unlink()
	for f in Path("./lib").glob("*"):
		f.unlink()
	print("DONE")
	sys.exit(0)

if PLATFORM == "win32":
	CC = "cl"
	AR = "lib"
	OBJ_EXT = ".obj"
	OUTPUT = "./lib/yta.lib"
	CFLAGS = [
		"/nologo", "/EHsc", "/W4",
		"/O2", "/GS", "/D_FORTIFY_SOURCE=2"
	]
	LIBS = ["opengl32.lib", "gdi32.lib"]

else:
	CC = "gcc"
	AR = "ar"
	OBJ_EXT = ".o"
	OUTPUT = "./lib/libyta.a"
	CFLAGS = [
		"-Wall", "-Wextra", "-Wpedantic", "-Wshadow", "-Wpointer-arith",
		"-Wcast-align", "-Wstrict-aliasing", "-Wformat=2", "-Wundef",
		"-g", "-O1", "-fstack-protector-strong", "-D_FORTIFY_SOURCE=2"
	]
	LIBS = []

if DEBUG:
	if PLATFORM == "win32":
		CFLAGS.append("/DDEBUG")
	else:
		CFLAGS.append("-DDEBUG")

INCLUDE = ["./include"]
for inc in INCLUDE:
	if PLATFORM == "win32":
		CFLAGS.append(f"/I{inc}")
	else:
		CFLAGS.append(f"-I{inc}")

PLATFORM_SOURCES = []

if PLATFORM == "win32":
	CFLAGS.append("/DPLATFORM_WIN32")
	PLATFORM_SOURCES.append("src/platform/win32.c")
elif PLATFORM == "sdl3":
	sdl_cflags = subprocess.check_output(
		["pkg-config", "--cflags", "sdl3"], text=True
	).strip().split()
	CFLAGS.extend(sdl_cflags)
	CFLAGS.append("-DPLATFORM_SDL3")
	PLATFORM_SOURCES.append("src/platform/sdl3.c")

COMMON_SOURCES = []
for path in Path("./src").rglob("*.c"):
	path_posix = path.as_posix()
	if path_posix.startswith("src/platform/"):
		continue
	COMMON_SOURCES.append(str(path))

SOURCES = COMMON_SOURCES + PLATFORM_SOURCES
OBJECTS = [s.replace(".c", OBJ_EXT) for s in SOURCES]

def escape_shader(content: str) -> str:
	lines = content.splitlines()
	escaped_lines = []
	for line in lines:
		line_escaped = line.replace('\\', '\\\\').replace('"', '\\"')
		escaped_lines.append(f'"{line_escaped}\\n"')
	return '\n'.join(escaped_lines)

def preprocess_shader_includes(src_file: str) -> str:
	with open(src_file, "r") as f:
		code = f.read()

	pattern = r'__include_shader\("([^"]+)"\)'

	if not re.search(pattern, code):
		return src_file

	def replacer(match):
		shader_path = f"shaders/{match.group(1)}"
		if not os.path.exists(shader_path):
			raise FileNotFoundError(f"Shader file not found: {shader_path}")
		with open(shader_path, "r") as sf:
			content = sf.read()
		escaped = escape_shader(content)
		return escaped

	code_processed = re.sub(pattern, replacer, code)

	src_path = Path(src_file)
	tmp_file_path = src_path.with_name(src_path.stem + f".tmp{time.time()}.c")

	with open(tmp_file_path, "w") as tmp_file:
		tmp_file.write(code_processed)

	return str(tmp_file_path)

for src, obj in zip(SOURCES, OBJECTS):
	tmp_src = preprocess_shader_includes(src)

	if PLATFORM == "win32":
		cmd = [CC] + CFLAGS + ["/c", tmp_src, "/Fo" + obj]
	else:
		cmd = [CC] + CFLAGS + ["-c", tmp_src, "-o", obj]

	print(' '.join(cmd))
	subprocess.check_call(cmd)

	if tmp_src != src:
		os.remove(tmp_src)

os.makedirs("./lib", exist_ok=True)

if PLATFORM == "win32":
	cmd = [AR, "/nologo", "/OUT:" + OUTPUT] + OBJECTS
else:
	cmd = [AR, "rcs", OUTPUT] + OBJECTS

print(' '.join(cmd))
subprocess.check_call(cmd)

print(f"DONE: {OUTPUT} in {(time.time() - start_time):.2f} seconds")
