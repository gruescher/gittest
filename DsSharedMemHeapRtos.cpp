/** <!------------------------------------------------------------------------->
*
*  @file DsSharedMemHeapRtos.cpp
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
*    $RCSfile: DsSharedMemHeapRtos.cpp $ $Revision: 1.0 $ $Date: 03.01.2013 $
*    $ProjectName:  $
*
*   <hr><br>
*<!-------------------------------------------------------------------------->*/


/******************************************************************************
 *  system include files
 ******************************************************************************/


/******************************************************************************
 *  dSPACE include files
 ******************************************************************************/
#include "DsTypes.h"
#include "DsSharedMemHeapRtos.h"
#include "DsRtosAbstraction.h"


/******************************************************************************
 *  internal defines
 ******************************************************************************/


/******************************************************************************
 *  data types
 ******************************************************************************/


/******************************************************************************
 *  global variables
 ******************************************************************************/


/******************************************************************************
 *  local/static function declarations
 ******************************************************************************/


/******************************************************************************
 *  function definitions
 ******************************************************************************/
DsSharedMemHeapRtos::DsSharedMemHeapRtos()
{
}

int
DsSharedMemHeapRtos::openSharedMemory
    (   void           **ppSharedMemory,
        UInt32          *pSize,

        const char      *StrName,
        UInt32           Id,
        UInt32           Size,
        int              AttributeFlags,
        int              CreateFlags
    )
{
    return 0;
}


int
DsSharedMemHeapRtos::checkSharedMemory
    (   T_SHM_DESCRIPTOR    *ppShmDescriptor,
        const char          *StrName,
        UInt32              Id
    )
{
    return 0;
}


int DsSharedMemHeapRtos::closeSharedMemory
    (   void           *pSharedMemory,
        int             ForceRemove
     )
{
    return 0;
}


int
DsSharedMemHeapRtos::closeSharedMemory
    (   const char     *StrName,
        UInt32          Id,
        int             ForceRemove
     )
{
    return 0;
}


int
DsSharedMemHeapRtos::listSharedMemory
    (   T_SHM_DESCRIPTOR   **ppShmList,
        UInt32              *pNumEntries
    )
{
    return 0;
}


int
DsSharedMemHeapRtos::allocateCrossNodeMutex
    (   T_DsCrossNodeMutex     **ppMutex,
        const char              *StrName
    )
{
    return 0;
}


int
DsSharedMemHeapRtos::lockCrossNodeMutex
    (   T_DsCrossNodeMutex     *pMutex,
        UInt32                  TimeoutMicroseconds,
        int                     MayBlock
    )
{
    return 0;
}


int
DsSharedMemHeapRtos::tryLockCrossNodeMutex
    (   T_DsCrossNodeMutex     *pMutex
    )
{
    return 0;
}


int
DsSharedMemHeapRtos::releaseCrossNodeMutex
    (   T_DsCrossNodeMutex     *pMutex
    )
{
    return 0;
}


int
DsSharedMemHeapRtos::freeCrossNodeMutex
    (   T_DsCrossNodeMutex     *pMutex
    )
{
    return 0;
}


int
DsSharedMemHeapRtos::freeCrossNodeMutexByName
    (   const char           *StrName
    )
{
    return 0;
}


