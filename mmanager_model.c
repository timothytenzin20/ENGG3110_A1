/* References
• author: Bladactania
• name: Thread: Allocate memory inside allocated memory block?
• year: 2009
• source: https://cboard.cprogramming.com/c-programming/114795-allocate-memory-inside-allocated-memory-block.html

• author: geeksforgeeks
• name: Best-Fit Allocation in Operating System
• year: 2023
• source: https://www.geeksforgeeks.org/best-fit-allocation-in-operating-system/

• author: geeksforgeeks
• name: Program for Worst Fit algorithm in Memory Management
• year: 2023
• source: https://www.geeksforgeeks.org/program-worst-fit-algorithm-memory-management/
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "mmanager.h"

void printSummary(FILE *outputfp, chunk *chunks, int num_chunks, long totalMemorySize) {
	long allocatedBytes = 0;
	long freeBytes = 0;
	long overhead = num_chunks * sizeof(chunk);

	for (int i = 0; i < num_chunks; i++) {
		if (chunks[i].is_allocated) {
			allocatedBytes += chunks[i].size;
		} else {
			freeBytes += chunks[i].size;
		}
	}

	// optional?: sort chunks 
    for (int i = 0; i < num_chunks - 1; i++) {
        for (int j = 0; j < num_chunks - i - 1; j++) {
            if (chunks[j].start > chunks[j + 1].start) {
                chunk temp = chunks[j];
                chunks[j] = chunks[j + 1];
                chunks[j + 1] = temp;
            }
        }
    }

	fprintf(outputfp, "SUMMARY:\n");
	fprintf(outputfp, "%ld bytes allocated\n", allocatedBytes);
	fprintf(outputfp, "%ld bytes free\n", freeBytes);
    fprintf(outputfp, "%ld bytes overhead\n", overhead);
	fprintf(outputfp, "Total memory usage: %ld bytes\n", allocatedBytes + freeBytes + overhead);
	fprintf(outputfp, "%d allocation chunks:\n", num_chunks);

	for (int i = 0; i < num_chunks; i++) {
		fprintf(outputfp, "chunk %2d location %4d:%4d bytes - %s\n",
				i, chunks[i].start, chunks[i].size,
				chunks[i].is_allocated ? "allocated" : "free");
	}
}

// helper function for merging chunks
void mergeFreeChunks(chunk *chunks, int *num_chunks) {
    // sort chunks by start location
    for (int i = 0; i < *num_chunks - 1; i++) {
        for (int j = 0; j < *num_chunks - i - 1; j++) {
            if (chunks[j].start > chunks[j + 1].start) {
                chunk temp = chunks[j];
                chunks[j] = chunks[j + 1];
                chunks[j + 1] = temp;
            }
        }
    }

    int i = 0;
    while (i < *num_chunks - 1) {
        if (!chunks[i].is_allocated && !chunks[i + 1].is_allocated) {
            // ensure chunks are adjacent
            if (chunks[i].start + chunks[i].size + sizeof(chunk) >= chunks[i + 1].start) {
                chunks[i].size = (chunks[i + 1].start + chunks[i + 1].size) - chunks[i].start; // include the overhead space
                
                // shift chunks 
                for (int j = i + 1; j < *num_chunks - 1; j++) {
                    chunks[j] = chunks[j + 1];
                }
                (*num_chunks)--;
                continue; 
            }
        }
        i++;
    }
}

int runModel(FILE *outputfp, FILE *inputfp,
		long totalMemorySize, int fitStrategy,
		int verbosity
	)
{
	char *memoryBlock = NULL;
	int nSuccessfulActions = 0;
	mmgr_action action;
	fprintf(outputfp,
			"Running a %s-fit model in %ld (0x%lx) bytes of memory.\n",
			(fitStrategy == STRAT_FIRST ? "first" :
				fitStrategy == STRAT_BEST ? "best" :
					fitStrategy == STRAT_WORST ? "worst" : "unknown"),
			totalMemorySize, totalMemorySize);

	/**
	 * this is the only allocation you should need -- all requests
	 * from your model should come from this allocated block
	 */
	memoryBlock = (char *) malloc(totalMemorySize);
	if (memoryBlock == NULL) {
		perror("allocation failed!");
		return -1;
	}
    
	// initial overhead for chunk management
    long chunkStructSize = sizeof(chunk);
    long availableMemory = totalMemorySize - chunkStructSize;

    fprintf(outputfp, "overhead per chunk: %ld bytes\n", chunkStructSize);
    fprintf(outputfp, "available memory: %ld bytes\n", availableMemory);

    chunk *chunks = (chunk*)memoryBlock;
    chunks[0].start = chunkStructSize;  
    chunks[0].size = availableMemory;
    chunks[0].is_allocated = FALSE;
    chunks[0].id = -1;
    int num_chunks = 1;

	/**
	 *	+++ Set up anything else you will need for your memory management
	 */

	while (getAction(&action, inputfp, outputfp, verbosity) > 0)
    {
        if (action.type == ACTION_ALLOCATE)
        {
            int found_index = -1;
            int best_size = totalMemorySize + 1;
            int worst_size = -1;

            // search for suitable chunk based on strategy
            for (int i = 0; i < num_chunks; i++) {
                if (!chunks[i].is_allocated && chunks[i].size >= action.size) {
                    switch(fitStrategy) {
                        case STRAT_FIRST:
                            found_index = i;
                            i = num_chunks; // exit loop
                            break;
                        case STRAT_BEST:
                            if (chunks[i].size < best_size) {
                                best_size = chunks[i].size;
                                found_index = i;
                            }
                            break;
                        case STRAT_WORST:
                            if (chunks[i].size > worst_size) {
                                worst_size = chunks[i].size;
                                found_index = i;
                            }
                            break;
                    }
                }
            }

            if (found_index != -1) {
                // split chunk if necessary
                if (chunks[found_index].size > action.size + sizeof(chunk)) {
                    // get start position of remaining chunk
                    chunks[num_chunks].start = chunks[found_index].start + action.size;
                    chunks[num_chunks].size = chunks[found_index].size - action.size - sizeof(chunk);
                    chunks[num_chunks].is_allocated = FALSE;
                    chunks[num_chunks].id = -1;
                    chunks[num_chunks].paint = action.paint ? action.paint : '~';
                    chunks[found_index].size = action.size;
                    num_chunks++;
                }
                
                chunks[found_index].is_allocated = TRUE;
                chunks[found_index].id = action.id;
                chunks[found_index].paint = action.paint ? action.paint : '~'; // am i being redundant
                
                fprintf(outputfp, "alloc %d bytes : SUCCESS - return location %d\n", 
                       action.size, chunks[found_index].start);
                nSuccessfulActions++;
            } else {
                fprintf(outputfp, "alloc %d bytes : FAIL\n", action.size);
            }
        } else
		{
			/* +++ do a release */
			// handle release/free action
            int found_index = -1;
            
            // find chunk with matching ID
            for (int i = 0; i < num_chunks; i++) {
                if (chunks[i].is_allocated && chunks[i].id == action.id) {
                    found_index = i;
                    break;
                }
            }

            if (found_index != -1) {
                // mark chunk as free
                chunks[found_index].is_allocated = FALSE;
                chunks[found_index].id = -1;
                fprintf(outputfp, "free location %d\n", chunks[found_index].start);

                // merge free chunks after each deallocation
                mergeFreeChunks(chunks, &num_chunks); // WHY WONT YOU WORKKKKKKK
                
                /** increment our count of work we did */
				nSuccessfulActions++;
            } else {
                fprintf(outputfp, "free failed - no chunk with id %d\n", action.id);
            }
		}	
	}
	
    printSummary(outputfp, chunks, num_chunks, totalMemorySize);
    
	/** +++ Clean up your memory management */
	free(memoryBlock);
	return nSuccessfulActions;
}

