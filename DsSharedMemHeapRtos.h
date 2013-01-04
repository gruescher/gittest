/** <!------------------------------------------------------------------------->
*
*  @file DsSharedMemHeapRtos.h
*
*  @brief <Brief description here>
*
*  @author
*    GeraldR
*
*  @description
*    <Description here>
*
*  @copyright
*    Copyright 2013, dSPACE GmbH. All rights reserved.
*
*  @version
*    $RCSfile: DsSharedMemHeapRtos.h $ $Revision: 1.0 $ $Date: 03.01.2013 $
*    $ProjectName:  $
*
*   <hr><br>
*<!-------------------------------------------------------------------------->*/


#ifndef DSSHAREDMEMHEAPRTOS_H_
#define DSSHAREDMEMHEAPRTOS_H_
#include "DsTypes.h"
#include "DsSharedMemory.h"
#include "DsSharedMemHeap.h"


class DsSharedMemHeapRtos : public DsSharedMemHeap
{
public:
    DsSharedMemHeapRtos();

    int initHeap(void) { return 0; }

     int openSharedMemory
         (   void           **ppSharedMemory,
             UInt32          *pSize,

             const char      *StrName,
             UInt32           Id,
             UInt32           Size,
             int              AttributeFlags,
             int              CreateFlags
         );

     int checkSharedMemory
         (   T_SHM_DESCRIPTOR    *ppShmDescriptor,
             const char          *StrName,
             UInt32              Id
         );

     int closeSharedMemory
         (   void           *pSharedMemory,
             int             ForceRemove
          );

     int closeSharedMemory
         (   const char     *StrName,
             UInt32          Id,
             int             ForceRemove
          );

     int listSharedMemory
         (   T_SHM_DESCRIPTOR   **ppShmList,
             UInt32              *pNumEntries
         );

     int allocateCrossNodeMutex
         (   T_DsCrossNodeMutex     **ppMutex,
             const char              *StrName
         );

     int lockCrossNodeMutex
         (   T_DsCrossNodeMutex     *pMutex,
             UInt32                  TimeoutMicroseconds,
             int                     MayBlock
         );

     int tryLockCrossNodeMutex
         (   T_DsCrossNodeMutex     *pMutex
         );

     int releaseCrossNodeMutex
         (   T_DsCrossNodeMutex     *pMutex
         );

     int freeCrossNodeMutex
         (   T_DsCrossNodeMutex     *pMutex
         );

     int freeCrossNodeMutexByName
         (   const char           *StrName
         );
};

#endif /* DSSHAREDMEMHEAPRTOS_H_ */
