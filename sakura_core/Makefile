# Makefile for MinGW32/MinGW-W64

# Example usages:
#   Out-of-source build:
#     $ mkdir -p build/MinGW/Release
#     $ cd build/MinGW/Release
#     $ mingw32-make -f ../../../sakura_core/Makefile MYCFLAGS=-O2 OUTDIR=. -j4
#
#   Debug build with coverage:
#     $ cd sakura_core
#     $ mingw32-make MYCFLAGS="-g --coverage" MYLIBS=--coverage -j4

# Path of "sakura_core" directory. Compute it from the path of Makefile.
SRCDIR = $(patsubst %/,%,$(subst \,/,$(dir $(firstword $(MAKEFILE_LIST)))))

# If SRCDIR is different from the current directory, set it to VPATH.
# (If SRCDIR ends with a backslash, remove it before set to VPATH.)
ifneq ($(SRCDIR),.)
VPATH = $(patsubst %\,%,$(SRCDIR))
endif

# The directory where the .exe files will be output.
# If empty, they will be output to the default directories.
OUTDIR =

ifeq ($(SHELL),sh.exe)
# If cmd.exe is used as a shell.
MKDIR = md
RM = del
DIRSEP = $(strip \ )
DEVNULL = NUL
else
# If unix-like shell is used.
MKDIR = mkdir -p
RM = rm -f
DIRSEP = /
DEVNULL = /dev/null
endif

ifndef PREFIX
PREFIX=
RCPREFIX=
else ifeq ($(PREFIX),x86_64-w64-mingw32-)
RCPREFIX=$(PREFIX)
else ifeq ($(PREFIX),i686-w64-mingw32-)
ifeq ($(OS),Windows_NT)
RCPREFIX=
else
RCPREFIX=$(PREFIX)
endif
endif

CC= $(PREFIX)gcc
CXX= $(PREFIX)g++
RC= $(RCPREFIX)windres

DEFINES= \
 -DWIN32 \
 -D_WIN32_WINNT=_WIN32_WINNT_WIN7 \
 -D_UNICODE \
 -DUNICODE \
 $(MYDEFINES)

ifeq (,$(findstring -D_DEBUG,$(DEFINES)))
ifeq (,$(findstring -DNDEBUG,$(DEFINES)))
DEFINES += -DNDEBUG
endif
endif

CFLAGS= \
 -finput-charset=utf-8 \
 -fexec-charset=cp932 \
 -MMD \
 -I. \
 -I$(SRCDIR) \
 $(DEFINES) $(MYCFLAGS)
CXXFLAGS= $(CFLAGS) \
 -std=c++17 \
 $(MYCXXFLAGS)
LIBS= \
 -static \
 -lwinspool \
 -lole32 \
 -loleaut32 \
 -luuid \
 -lcomctl32 \
 -limm32 \
 -lmpr \
 -limagehlp \
 -lshlwapi \
 -lwinmm \
 -lwindowscodecs \
 -lmsimg32 \
 -mwindows \
 -municode \
 $(MYLIBS)

exe= $(or $(OUTDIR),.)/sakura.exe

SRCS = $(wildcard $(SRCDIR)/*.cpp) \
       $(wildcard $(SRCDIR)/*/*.cpp) \
       $(wildcard $(SRCDIR)/*/*/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=%.o)

_DIRS = $(filter %/, $(wildcard $(SRCDIR)/*/)) \
        $(filter %/, $(wildcard $(SRCDIR)/*/*/))
DIRS = $(_DIRS:$(SRCDIR)/%/=%)

DEPS= $(OBJS:%.o=%.d) StdAfx.h.d

GENERATED_FILES= \
 Funccode_define.h \
 Funccode_enum.h \
 githash.h \

HEADERMAKETOOLDIR= $(SRCDIR)/../HeaderMake
HEADERMAKE= $(or $(OUTDIR),$(HEADERMAKETOOLDIR))/HeaderMake.exe

all: $(exe)

$(exe): $(OBJS) sakura_rc.o
	$(CXX) -o $@ $(OBJS) sakura_rc.o $(LIBS)

Funccode_define.h: Funccode_x.hsrc $(HEADERMAKE)
	$(HEADERMAKE) -in=$< -out=$@ -mode=define

Funccode_enum.h: Funccode_x.hsrc $(HEADERMAKE)
	$(HEADERMAKE) -in=$< -out=$@ -mode=enum -enum=EFunctionCode

githash.h:
	cmd /c $(subst /,\,$(SRCDIR))\..\sakura\githash.bat .

StdAfx.h.gch: StdAfx.h githash.h Funccode_enum.h
ifneq ($(SRCDIR),.)
	-$(MKDIR) $(subst /,$(DIRSEP),$(DIRS)) > $(DEVNULL) 2>&1
endif
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJS): StdAfx.h.gch

$(HEADERMAKE): $(HEADERMAKETOOLDIR)/HeaderMake.cpp
	$(CXX) $(CXXFLAGS:-MMD=) $< -o $@ -static-libgcc

sakura_rc.o: sakura_rc.rc githash.h Funccode_define.h
	$(RC) -c utf-8 --language=0411 $(DEFINES) -I. -I$(SRCDIR) $< -o $@

clean:
	-$(RM) $(subst /,$(DIRSEP),$(exe) $(OBJS) $(HEADERMAKE)) StdAfx.h.gch $(GENERATED_FILES)
	-$(RM) $(subst /,$(DIRSEP),$(DEPS))

.SUFFIXES: .cpp .o .rc
.PHONY: all clean

-include $(DEPS)
