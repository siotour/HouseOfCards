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
	${OBJECTDIR}/_ext/708781350/buttonobject.o \
	${OBJECTDIR}/_ext/708781350/events.o \
	${OBJECTDIR}/_ext/708781350/mainmenu.o \
	${OBJECTDIR}/_ext/708781350/object.o \
	${OBJECTDIR}/_ext/708781350/scene.o \
	${OBJECTDIR}/src/main.o \
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

${OBJECTDIR}/_ext/708781350/buttonobject.o: /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/buttonobject.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/708781350
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708781350/buttonobject.o /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/buttonobject.cpp

${OBJECTDIR}/_ext/708781350/events.o: /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/events.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/708781350
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708781350/events.o /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/events.cpp

${OBJECTDIR}/_ext/708781350/mainmenu.o: /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/mainmenu.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/708781350
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708781350/mainmenu.o /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/mainmenu.cpp

${OBJECTDIR}/_ext/708781350/object.o: /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/object.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/708781350
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708781350/object.o /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/object.cpp

${OBJECTDIR}/_ext/708781350/scene.o: /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/scene.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/708781350
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/708781350/scene.o /mnt/LinuxHome/sbachstein/Workspace/netbeans/HouseOfCards/src/scene.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Wall -I/usr/local/lib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

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
