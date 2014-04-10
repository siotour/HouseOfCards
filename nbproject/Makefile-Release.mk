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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/708781350/battlescene.o \
	${OBJECTDIR}/src/button.o \
	${OBJECTDIR}/src/card.o \
	${OBJECTDIR}/src/castle.o \
	${OBJECTDIR}/src/events.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/mainmenu.o \
	${OBJECTDIR}/src/object.o \
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
LDLIBSOPTIONS=-L/usr/local/lib/avl/bin/Release -lSDL2 -lavl -lSDL2_image

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/houseofcards

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/houseofcards: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/houseofcards ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/708781350/battlescene.o: /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/battlescene.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/708781350
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708781350/battlescene.o /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/battlescene.cpp

${OBJECTDIR}/src/button.o: src/button.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/button.o src/button.cpp

${OBJECTDIR}/src/card.o: src/card.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/card.o src/card.cpp

${OBJECTDIR}/src/castle.o: src/castle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/castle.o src/castle.cpp

${OBJECTDIR}/src/events.o: src/events.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/events.o src/events.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/mainmenu.o: src/mainmenu.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mainmenu.o src/mainmenu.cpp

${OBJECTDIR}/src/object.o: src/object.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/object.o src/object.cpp

${OBJECTDIR}/src/scene.o: src/scene.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/scene.o src/scene.cpp

${OBJECTDIR}/src/sdlutility.o: src/sdlutility.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sdlutility.o src/sdlutility.cpp

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
