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
        initSharedMemoryHeaps(false);

        dumpSharedMemoryHeap(SHM_LOCATION_ON_HCN);

    #endif

    return 0;
}


#if 0
    T_SHM_AREA             *pShmArea;
    int                     rc;
    DsMappedBuddyHeap      *pCHeap;


    // Get memory for heap
    rc = mapSharedMemDataStructures( NODE_ID_HCN, &pShmArea);
    if( rc != 0 )
    {
        printf("Failed to allocate heap\n");
        return rc;
    }

#ifdef DS_PLATFORM_HCN
    {
        void                   *block1, *block2, *block2b, *block3, *block4, *block5;
        T_DS_HEAP_BLOCK_INFO   *pHeapBlockList;
        UInt32                  numAllocatedBlocks, i, sizeBlock2b, idBlock2b;

        pCHeap = new DsMappedBuddyHeap(pShmArea);

        // Init heap data structure
        pCHeap->init( "testheap", 1 );

        // Perform tests:

        // Allocate a few blocks and show heap status in between
        pCHeap->alloc("block_555", 555, 0xAAAAAA, &block1);
        pCHeap->dump();

        pCHeap->alloc("block_1555", 1555, 0xBBBBBB, &block2);
        pCHeap->dump();

        pCHeap->alloc("block_666", 666, 0xCCCCCC, &block3);
        pCHeap->dump();

        pCHeap->alloc("block_120k", 120000, 0xDDDDDD, &block4);
        pCHeap->dump();

        pCHeap->alloc("block_120kb", 120000, 0xEEEEEE, &block5);
        pCHeap->dump();

        pCHeap->open("block_1555", &sizeBlock2b, &idBlock2b, &block2b );
        pCHeap->dump();

        // Get and display list of allocated blocks
        pCHeap->list( &pHeapBlockList, &numAllocatedBlocks);
        printf("Allocated blocks:\n");
        for( i=0; i<numAllocatedBlocks; i++ )
        {
            T_DS_HEAP_BLOCK_INFO *pHeapBlockInfo;

            pHeapBlockInfo = &pHeapBlockList[i];

            printf("Address = 0x%09llx, Size = %d, Name = %s\n", pHeapBlockInfo->physicalAddress, pHeapBlockInfo->size, pHeapBlockInfo->name);
        }
        free( pHeapBlockList );

        // Release blocks and show heap status in between
        pCHeap->free( block3, 0 );
        pCHeap->dump();

        pCHeap->free( block1, 0 );
        pCHeap->dump();

        pCHeap->free( block2, 0 );
        pCHeap->dump();

        pCHeap->free( block5, 0 );
        pCHeap->dump();

        pCHeap->free( block4, 0 );
        pCHeap->dump();

        pCHeap->free( block2b, 0 );
        pCHeap->dump();

    }
#else

    // Init heap data structure
    pCHeap->init("testheap", 0 );

    pCHeap->dump();

#endif
#endif

