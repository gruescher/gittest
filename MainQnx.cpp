/** <!------------------------------------------------------------------------->
*
*  @file MainQnx.c
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
*    $RCSfile: MainQnx.c $ $Revision: 1.0 $ $Date: 10.12.2012 $
*    $ProjectName:  $
*
*   <hr><br>
*<!-------------------------------------------------------------------------->*/


/******************************************************************************
 *  system include files
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/******************************************************************************
 *  dSPACE include files
 ******************************************************************************/
#include "Dstypes.h"
#include "DsSharedMemory.h"
#include "DsMappedBuddyHeap.h"


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
int main(int argc, char *argv[])
{
#ifdef DS_PLATFORM_HCN
    {
        void                   *block1, *block2, *block2b, *block3, *block4, *block5;
        T_SHM_DESCRIPTOR       *pHeapBlockList;
        UInt32                  numAllocatedBlocks, i, size;
        int                     rc;

        initSharedMemoryHeaps(true);

        // Allocate a few blocks and show heap status in between
        rc = openSharedMemory( &block1, &size, "block_555",   0xAAAAAA, 555,    SHM_LOCATION_ON_HCN, 0, SHM_OPEN_CREATE_IF_MISSING );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        rc = openSharedMemory( &block2, &size, "block_1555",  0xBBBBBB, 1555,   SHM_LOCATION_ON_HCN, 0, SHM_OPEN_CREATE_IF_MISSING );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        rc = openSharedMemory( &block3, &size, "block_666",   0xCCCCCC, 666,    SHM_LOCATION_ON_HCN, 0, SHM_OPEN_CREATE_IF_MISSING );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        rc = openSharedMemory( &block4, &size, "block_120k",  0xDDDDDD, 120000, SHM_LOCATION_ON_HCN, 0, SHM_OPEN_CREATE_IF_MISSING );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        rc = openSharedMemory( &block5, &size, "block_120kb", 0xEEEEEE, 120000, SHM_LOCATION_ON_HCN, 0, SHM_OPEN_CREATE_IF_MISSING );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        rc = openSharedMemory( &block2b, &size, "block_555",  0xAAAAAA, 5555,   SHM_LOCATION_ON_HCN, 0, SHM_OPEN_CREATE_IF_MISSING );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        // Get and display list of allocated blocks
        rc = listSharedMemory( &pHeapBlockList, &numAllocatedBlocks, SHM_LOCATION_ON_HCN );
        printf("Allocated blocks:\n");
        for( i=0; i<numAllocatedBlocks; i++ )
        {
            T_SHM_DESCRIPTOR *pHeapBlockInfo;

            pHeapBlockInfo = &pHeapBlockList[i];

            printf("Address = 0x%09llx, Size = %d, Name = %s\n", pHeapBlockInfo->PhysAddress, pHeapBlockInfo->Size, pHeapBlockInfo->Name );
        }
        free( pHeapBlockList );

        // Release blocks and show heap status in between
        closeSharedMemory( block1, SHM_LOCATION_ON_HCN, 0 );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        closeSharedMemory( block2, SHM_LOCATION_ON_HCN, 0 );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        closeSharedMemory( block3, SHM_LOCATION_ON_HCN, 0 );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        closeSharedMemory( block4, SHM_LOCATION_ON_HCN, 0 );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        closeSharedMemory( block5, SHM_LOCATION_ON_HCN, 0 );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

        closeSharedMemory( block2b, SHM_LOCATION_ON_HCN, 0 );
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

    }
#else
    {
        initSharedMemoryHeaps(false);

        // Just dump heap status
        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);
    }
#endif

    return 0;
}
