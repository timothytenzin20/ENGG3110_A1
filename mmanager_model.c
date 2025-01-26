/* References
https://cboard.cprogramming.com/c-programming/114795-allocate-memory-inside-allocated-memory-block.html
https://www.geeksforgeeks.org/best-fit-allocation-in-operating-system/

*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "mmanager.h"

void printSummary(FILE *outputfp, chunk *chunks, int num_chunks, long totalMemorySize) {
	long allocatedBytes = 0;
	long freeBytes = 0;

	for (int i = 0; i < num_chunks; i++) {
		if (chunks[i].is_allocated) {
			allocatedBytes += chunks[i].size;
		} else {
			freeBytes += chunks[i].size;
		}
	}

	fprintf(outputfp, "SUMMARY:\n");
	fprintf(outputfp, "%ld bytes allocated\n", allocatedBytes);
	fprintf(outputfp, "%ld bytes free\n", freeBytes);
	fprintf(outputfp, "%d allocation chunks:\n", num_chunks);

	for (int i = 0; i < num_chunks; i++) {
		fprintf(outputfp, "chunk %2d location %4d:%4d bytes - %s\n",
				i, chunks[i].start, chunks[i].size,
				chunks[i].is_allocated ? "allocated" : "free");
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
	chunk *chunks = (chunk*)memoryBlock;
    chunks[0].start = 0;
    chunks[0].size = totalMemorySize;
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

            // Search for suitable chunk based on strategy
            for (int i = 0; i < num_chunks; i++) {
                if (!chunks[i].is_allocated && chunks[i].size >= action.size) {
                    switch(fitStrategy) {
                        case STRAT_FIRST:
                            found_index = i;
                            i = num_chunks; // Exit loop
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
                // Split chunk if necessary
                if (chunks[found_index].size > action.size) {
                    chunks[num_chunks].start = chunks[found_index].start + action.size;
                    chunks[num_chunks].size = chunks[found_index].size - action.size;
                    chunks[num_chunks].is_allocated = FALSE;
                    chunks[num_chunks].id = -1;
                    chunks[num_chunks].paint = action.paint ? action.paint : '~'; // am i being redundant
                    chunks[found_index].size = action.size;
                    num_chunks++;
                }
                
                chunks[found_index].is_allocated = TRUE;
                chunks[found_index].id = action.id;
                chunks[found_index].paint = action.paint ? action.paint : '~';
                
                fprintf(outputfp, "alloc %d bytes : SUCCESS - return location %d\n", 
                       action.size, chunks[found_index].start);
                nSuccessfulActions++;
            } else {
                fprintf(outputfp, "alloc %d bytes : FAIL\n", action.size);
            }
        } else
		{
			/* +++ do a release */
			// Handle release/free action
            int found_index = -1;
            
            // Find chunk with matching ID
            for (int i = 0; i < num_chunks; i++) {
                if (chunks[i].is_allocated && chunks[i].id == action.id) {
                    found_index = i;
                    break;
                }
            }

            if (found_index != -1) {
                // Mark chunk as free
                chunks[found_index].is_allocated = FALSE;
                chunks[found_index].id = -1;
                fprintf(outputfp, "free location %d\n", chunks[found_index].start);

                // Optional: Merge adjacent free chunks
                for (int i = 0; i < num_chunks - 1; i++) {
                    if (!chunks[i].is_allocated && !chunks[i+1].is_allocated &&
                        (chunks[i].start + chunks[i].size == chunks[i+1].start)) {
                        // Merge chunks[i] and chunks[i+1]
                        chunks[i].size += chunks[i+1].size;
                        // Shift remaining chunks left
                        for (int j = i + 1; j < num_chunks - 1; j++) {
                            chunks[j] = chunks[j+1];
                        }
                        num_chunks--;
                        i--; // Check this position again
                    }
                }
                
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

