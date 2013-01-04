################################################################################
# File:
#   Makefile
#
# Author:
#   Gerald Ruescher
#
# Description:
#   Configuration for component DsSharedMemory
#
# (c) Copyright 2012, dSPACE GmbH. All rights reserved.
#
#  $RCSfile: Makefile $ $Revision: 1.3 $ $Date: 2011/03/10 14:16:01MEZ $
################################################################################

AUTHOR              := Gerald Ruescher
DESCRIPTION         := Shared Memory layer

APPL_NAME             := DsSharedMemory

EXPORT_INCLUDES     := 

TARGET_PLATFORMS    := DS1007CN DS1007HCN

FEATURE_COMPILE_ALL_SUBDIRS := yes

EXPORT_INCLUDES 	:=  DsSharedMemHeap.h DsSharedMemory.h 

###### Optional Application Settings

## Optimize code?
SO_LIB_FILES += DsRtosAbstraction.so DsSharedMemMutex.so DsSharedMemHeapSrtk.so
LIB_FILES    += SrtkMemoryMap.a

## Optimize code?
FEATURE_OPTIMZE := no
## Add debug info?
FEATURE_DEBUG := yes
## Source code inlining
FEATURE_INLINE := no

##### do not modify beyond this line ###########################
PROJECT_REVISION 	:=  $ProjectRevision: 1.10 $
include $(DSPACE_MK_DIR)\_cfg_root.mk

## Additional compiler flags
## Values:	yes,no
CFLAGS += -Wno-format 
