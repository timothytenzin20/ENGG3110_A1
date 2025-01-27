First Fit:
Best for quick allocation when memory is relatively free of fragmentation.
Performs well when allocations are simple and space is not heavily fragmented.
Drawback: Leads to fragmentation over time, which can slow down future allocation.

Best Fit:
Best for scenarios where minimizing fragmentation is a priority and allocations tend to be close to the size of available blocks.
Works well when you have predictable and frequent allocations and deallocations.
Drawback: Can be slower due to the need to find the most fitting space.

Worst Fit: 
Best when the goal is to keep large blocks available for future allocations.
Effective when you need to allocate large blocks repeatedly and can afford the inefficiency of small allocations.
Drawback: Leads to fragmentation by leaving large free spaces that can't be efficiently utilized by smaller blocks.


###########################################################################################

best-fit.txt: ./allocator -m 6000 -s *type* best-fit.txt
Best-Fit performs the best for this test, as it minimizes fragmentation and reduces overhead.
First-Fit is efficient but leaves moderate fragmentation.
Worst-Fit creates more fragmented free space and performs less optimally in this scenario.
For systems prioritizing memory utilization and minimizing fragmentation, Best-Fit is the preferred method. However, in high-performance systems where speed is critical, First-Fit might be better due to its simpler search logic.

first-fit.txt: ./allocator -m 2000 -s *type* first-fit.txt
First-Fit and Best-Fit behave similarly when small freed chunks match allocation requests well.
Worst-Fit introduces more fragmentation by allocating from the largest block, leaving smaller free chunks unused.

worst-fit.txt: ./allocator -m 2000 -s *type* worst-fit.txt
First-Fit:
Allocates memory in the first available space that is large enough.
The final free chunk is at location 1150, which is large (850 bytes) but not optimal.
Best-Fit:
Allocates memory in the smallest available chunk that fits.
The final free chunk is at location 1150, similar to First-Fit, and still large (850 bytes).
Worst-Fit:
Allocates memory in the largest available chunk, aiming to leave the most significant remaining chunk.
The final free chunk is at location 1270, which is 730 bytes (slightly smaller than the First-Fit and Best-Fit final free chunk).