# COMPILER
CC:=g++
CFLAGS:=-Wall -ggdb -std=c++11

# APP
APPNAME:=connect-four

# APP DIRECTORIES
BUILDDIR:=bin
INCLUDEDIR:=include
LIBDIR:=lib
SRCDIR:=src
TMPDIR:=.obj

# SOURCES
MAINFILE=$(SRCDIR)/main.cpp
SOURCES:=$(shell find $(SRCDIR) -name '*.cpp')
_SOURCESFOLDERS:=$(shell find $(SRCDIR) -type d)
SOURCESFOLDERS:=$(subst ./,,$(_SOURCESFOLDERS))

# OBJECTS
_OBJECTS:=$(addprefix $(TMPDIR)/,$(SOURCES:%.cpp=%.o))
_OBJECTS:=$(subst ./,,$(_OBJECTS))
OBJECTS:=$(subst $(TMPDIR)/$(SRCDIR)/main.o,,$(_OBJECTS))

# EXTERNAL DEPENDENCIES
#SHAREDIR:=../shared
#SHAREDLIB:=$(SHAREDIR)/lib
#SHAREDHEADERS:=$(SHAREDIR)/include
_EXTLIBS:=sfml-graphics sfml-window sfml-system
EXTLIBS:=$(addprefix -l,$(_EXTLIBS))

# ================================== COMPILE ===================================
all: check $(OBJECTS) $(MAINFILE)
	@echo "========================="
	@echo " Building $(APPNAME) ..."
	@echo "========================="
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) $(MAINFILE) $(OBJECTS) $(EXTLIBS) -o $(BUILDDIR)/$(APPNAME)

$(TMPDIR)/%.o: %.cpp
	@echo "------------------------"
	@echo "> COMPILING $< ..."
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) $< -c -o $@

check:
	@test -d $(BUILDDIR) || mkdir $(BUILDDIR)
	@$(shell find $(SRCDIR) -depth -type d -print | cpio -pd $(TMPDIR))

# =================================== OTHER ====================================
clean:
	-rm -Rf $(TMPDIR)
	-rm -Rf $(TMPTESTDIR)

.PHONY: clean all
