/** <!------------------------------------------------------------------------->
*
*  @file DsSharedMemHeap.h
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
*    Copyright 2012, dSPACE GmbH. All rights reserved.
*
*  @version
*    $RCSfile: DsSharedMemHeap.h $ $Revision: 1.0 $ $Date: 03.01.2013 $
*    $ProjectName:  $
*
*   <hr><br>
*<!-------------------------------------------------------------------------->*/


#ifndef DSSHAREDMEMHEAP_H_
#define DSSHAREDMEMHEAP_H_

#include "DsTypes.h"
#include "DsSharedMemory.h"

class DsSharedMemHeap
{
public:
     virtual int openSharedMemory
         (   void           **ppSharedMemory,
             UInt32          *pSize,

             const char      *StrName,
             UInt32           Id,
             UInt32           Size,
             int              AttributeFlags,
             int              CreateFlags
         ) = 0;

     virtual int checkSharedMemory
         (   T_SHM_DESCRIPTOR    *ppShmDescriptor,
             const char          *StrName,
             UInt32              Id
         ) = 0;

     virtual int closeSharedMemory
         (   void           *pSharedMemory,
             int             ForceRemove
          ) = 0;

     virtual int closeSharedMemory
         (   const char     *StrName,
             UInt32          Id,
             int             ForceRemove
          ) = 0;

     virtual int listSharedMemory
         (   T_SHM_DESCRIPTOR   **ppShmList,
             UInt32              *pNumEntries
         ) = 0;

     virtual int allocateCrossNodeMutex
         (   T_DsCrossNodeMutex     **ppMutex,
             const char              *StrName
         ) = 0;

     virtual int lockCrossNodeMutex
         (   T_DsCrossNodeMutex     *pMutex,
             UInt32                  TimeoutMicroseconds,
             int                     MayBlock
         ) = 0;

     virtual int tryLockCrossNodeMutex
         (   T_DsCrossNodeMutex     *pMutex
         ) = 0;

     virtual int releaseCrossNodeMutex
         (   T_DsCrossNodeMutex     *pMutex
         ) = 0;

     virtual int freeCrossNodeMutex
         (   T_DsCrossNodeMutex     *pMutex
         ) = 0;

     virtual int freeCrossNodeMutexByName
         (   const char           *StrName
         ) = 0;

     virtual void dump() { }

private:

};

#endif /* DSSHAREDMEMHEAP_H_ */
