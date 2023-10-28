#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o files
# 'make clean_all' removes all .o and executable files

# define debug/release mode
ver = release

# define Platform Architecture(32/64)
ARCH := 64

# define the Cpp compiler to use
CXX := g++

# define any compile-time flags
ifeq ($(ver), release)
CXXFLAGS	:= -std=c++17 -O3
else
CXXFLAGS	:= -std=c++17 -Werror -g
endif

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define program name
PROGRAM	:= main

# define output directory
OUTPUT	:= output

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib/lib$(ARCH)

ifeq ($(OS),Windows_NT)
	MAIN	:= $(PROGRAM).exe
	SOURCEDIRS	:= $(SRC) $(shell powershell "Get-ChildItem -Path $(SRC) -Recurse -Directory | Select-Object FullName")
	INCLUDEDIRS	:= $(INCLUDE)
	LIBDIRS		:= $(LIB)/win
	FIXPATH = $(subst /,\,$1)
	# 使用 shell 命令检测当前的 shell
	DETECTED_SHELL := $(shell echo $$0)

	# 如果检测到的 shell 不是 Unix-like 的，尝试使用 Windows 的方法
	ifeq ($(DETECTED_SHELL),$$0)
		ifeq ($(OS),Windows_NT)
			DETECTED_SHELL := $(shell echo %COMSPEC%)
			ifeq ($(DETECTED_SHELL),C:\Windows\System32\cmd.exe)
				CMD_EXISTS := $(shell where del 2>NUL)
				ifndef CMD_EXISTS
				RM			:=  rm -f
				else
				RM			:=  del /q /f
				endif
			else
				# 尝试使用 PowerShell 的方法
				DETECTED_SHELL := $(shell echo $$PSVersionTable.PSVersion)
				ifneq ($(DETECTED_SHELL),)
					CMD_EXISTS := $(shell where del 2>NUL)
					ifndef CMD_EXISTS
					RM			:=  rm -f
					else
					RM			:=  del /q /f
					endif
				endif
			endif
		endif
	else
		RM	:=  rm -f
	endif
	MD	:= mkdir
else
	MAIN	:= $(PROGRAM)
	SOURCEDIRS	:= $(shell find $(SRC) -type d)
	INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
	LIBDIRS		:= $(shell find $(LIB)/linux -type d)
	FIXPATH = $1
	RM = rm -f
	MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%)) $(patsubst $(LIBDIRS)/lib%.a,-l%, $(wildcard $(LIBDIRS)/*.a)) 

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))

# define the C object files 
OBJECTS		:= $(SOURCES:.cpp=.o)

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,./$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

.PHONY: clean clean_all
clean:
	$(RM) $(call FIXPATH,$(OBJECTS))
	@echo Cleanup .o files complete!

clean_all:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	@echo Cleanup all complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!
