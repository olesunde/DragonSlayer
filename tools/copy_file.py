#!/usr/bin/env python3
"""Copy one file. Used by meson.build to stage the SDL2 DLLs next to program.exe."""
import shutil, sys
shutil.copyfile(sys.argv[1], sys.argv[2])
