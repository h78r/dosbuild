export PATH="/opt/djgpp/bin:$PATH"
COMPILER="i386-pc-msdosdjgpp-gcc"

echo "Checking for DJGPP cross-compiler..."
if ! which $COMPILER >/dev/null 2>&1; then
  echo "Error: $COMPILER not found in PATH"
  exit 1
fi
echo "Compiler found: $COMPILER"

echo "Compiling src/test.c to src/test.exe..."
if ! $COMPILER -o src/test.exe src/test.c; then
  echo "Error: Compilation failed"
  exit 1
fi
echo "Compilation successful."

echo "Inspecting executable format:"
file_output=$(file src/test.exe)
echo "$file_output"

echo "Copying CWSDPMI.EXE into src/..."
if ! cp inc/bin/CWSDPMI.EXE src/cwsdpmi.exe; then
  echo "Error: Failed to copy CWSDPMI.EXE"
  exit 1
fi
echo "CWSDPMI.EXE copied."

echo "Launching DOSBox (silent)..."
if ! dosbox src/test.exe > /dev/null 2>&1; then
  echo "Error: DOSBox failed to run"
  exit 1
fi
echo "DOSBox execution finished."

echo "Cleaning up generated executables..."
if ! rm src/*.exe; then
  echo "Error: Cleanup failed"
  exit 1
fi
echo "Cleanup successful."
