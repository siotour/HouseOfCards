#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/base.o \
	${OBJECTDIR}/src/battlescene.o \
	${OBJECTDIR}/src/button.o \
	${OBJECTDIR}/src/card.o \
	${OBJECTDIR}/src/cardfactory.o \
	${OBJECTDIR}/src/events.o \
	${OBJECTDIR}/src/fort.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/mainmenu.o \
	${OBJECTDIR}/src/object.o \
	${OBJECTDIR}/src/resourcemanager.o \
	${OBJECTDIR}/src/room.o \
	${OBJECTDIR}/src/scene.o \
	${OBJECTDIR}/src/sdlutility.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/usr/local/lib/avl/bin/Debug -lSDL2 -lavl -lSDL2_image

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/houseofcards

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/houseofcards: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/houseofcards ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/base.o: src/base.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/base.o src/base.cpp

${OBJECTDIR}/src/battlescene.o: src/battlescene.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/battlescene.o src/battlescene.cpp

${OBJECTDIR}/src/button.o: src/button.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/button.o src/button.cpp

${OBJECTDIR}/src/card.o: src/card.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/card.o src/card.cpp

${OBJECTDIR}/src/cardfactory.o: src/cardfactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cardfactory.o src/cardfactory.cpp

${OBJECTDIR}/src/events.o: src/events.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/events.o src/events.cpp

${OBJECTDIR}/src/fort.o: src/fort.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/fort.o src/fort.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/mainmenu.o: src/mainmenu.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mainmenu.o src/mainmenu.cpp

${OBJECTDIR}/src/object.o: src/object.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/object.o src/object.cpp

${OBJECTDIR}/src/resourcemanager.o: src/resourcemanager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/resourcemanager.o src/resourcemanager.cpp

${OBJECTDIR}/src/room.o: src/room.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/room.o src/room.cpp

${OBJECTDIR}/src/scene.o: src/scene.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/scene.o src/scene.cpp

${OBJECTDIR}/src/sdlutility.o: src/sdlutility.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sdlutility.o src/sdlutility.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/houseofcards

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
