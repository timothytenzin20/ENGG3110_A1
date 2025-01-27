# ENGG3110 Assignment 1 Submission

**Student:** Timothy Khan (1239165)  
**Instructor:** Andrew Hamilton-Wright  
**Date:** January 27, 2025

## Assignment Completion
- [x] Basic coding of a program that does allocations and reports them as described (35%)
- [x] Merging adjacent free allocations upon release (15%)
- [x] Managing indexing within the allocation block (20%)
- [x] Creating experiments to explore different allocation patterns (15%)
- [x] Final report, explaining your code and your findings (15%)
 
## Experiments Performed
Throughout the development process, several tests were used. These tests ensured basic allocation and release functionality without using malloc() or other prohibited functions, preserving the principle of not adding more RAM to a computer. Furthermore, test cases featured adjacent free memory chunks to be respectively merged upon release if adjacent. Furthermore, this means managing the indices of each chunk as surrounding chunks are released. Testing ensured functionality across program iterations while introducing new errors to be fixed. Please note that all tests are not included in the final submission, only those that demonstrate the program's functionality and provide use cases for each allocation method. The included test results are as follows.

### Test Cases
#### 1. smalltest-prof.txt
**Commands:** `./allocator -m 4096 -s *type* smalltest-prof.txt`
**Explanation:** This test case was generated to match the assignment outlines provided output. The default file given (smalltest.txt) failed to allocate an additional 512 bits as a fourth initial allocation and therefore differed in results. Adjusting this file allowed for cross-checking between assignment expectations and actual results.
**Results:** Outputs matched (before implementing overhead adjustments)

#### 2. best-fit.txt
**Commands:** `./allocator -m 6000 -s *type* best-fit.txt`
**Explanation:** This test case was used to display the benefits and drawbacks of each allocation method concerning the tasks requested. Specifically, this test was meant to advocate for the ‘best’ strategy.
**Results:** 
First-Fit:
The final free chunk is at location 720, which is large (5160 bytes) but not optimal.
Best-Fit:
The final free chunk is at location 570, which is slightly larger (5330 bytes) than First-Fit, leaving small fragmented chunks behind.
Worst-Fit:
The final free chunk is at location 790, which is slightly smaller (5050 bytes) than the First-Fit and Best-Fit final free chunk but leaves significant merged space.


| Strategy   | Allocated | Free Memory | Overhead | Memory Fragmentation |
|------------|-----------|-------------|----------|---------------------|
| First-Fit  | 440 bytes | 5400 bytes  | 140 bytes | Moderate fragmentation |
| Best-Fit   | 450 bytes | 5410 bytes  | 140 bytes | Small fragmented chunks |
| Worst-Fit  | 440 bytes | 5380 bytes  | 140 bytes | Large merged blocks |

Therefore, for systems using memory efficiently and minimizing fragmentation, best-fit is the preferred method. It utilizes smaller free chunks more effectively, leaving behind a larger merged free block (5330 bytes). 

**SUMMARY:**
```markdown
tkhan12@linux-01:~/CIS3110/CIS3110-W25-A1-code$ ./allocator -m 6000 -s first best-fit.txt
Running a first-fit model in 6000 (0x1770) bytes of memory.
overhead per chunk: 20 bytes
available memory: 5980 bytes
’’’
’’’
SUMMARY:
440 bytes allocated
5400 bytes free
140 bytes overhead
Total memory usage: 5980 bytes
7 allocation chunks:
chunk  0 location   20: 100 bytes - allocated
chunk  1 location  120:  40 bytes - allocated
chunk  2 location  160:  30 bytes - allocated
chunk  3 location  190: 240 bytes - free
chunk  4 location  450: 120 bytes - allocated
chunk  5 location  570: 150 bytes - allocated
chunk  6 location  720:5160 bytes - free

tkhan12@linux-01:~/CIS3110/CIS3110-W25-A1-code$ ./allocator -m 6000 -s best best-fit.txt
Running a best-fit model in 6000 (0x1770) bytes of memory.
overhead per chunk: 20 bytes
available memory: 5980 bytes
’’’
’’’
SUMMARY:
450 bytes allocated
5410 bytes free
140 bytes overhead
Total memory usage: 6000 bytes
7 allocation chunks:
chunk  0 location   20: 100 bytes - allocated
chunk  1 location  120: 150 bytes - allocated
chunk  2 location  270:  30 bytes - allocated
chunk  3 location  300:  80 bytes - free
chunk  4 location  400:  50 bytes - allocated
chunk  5 location  450: 120 bytes - allocated
chunk  6 location  570:5330 bytes - free

tkhan12@linux-01:~/CIS3110/CIS3110-W25-A1-code$ ./allocator -m 6000 -s worst best-fit.txt
Running a worst-fit model in 6000 (0x1770) bytes of memory.
overhead per chunk: 20 bytes
available memory: 5980 bytes
’’’
’’’
SUMMARY:
440 bytes allocated
5380 bytes free
140 bytes overhead
Total memory usage: 5960 bytes
7 allocation chunks:
chunk  0 location   20: 100 bytes - allocated
chunk  1 location  120: 330 bytes - free
chunk  2 location  450: 120 bytes - allocated
chunk  3 location  570:  40 bytes - allocated
chunk  4 location  610: 150 bytes - allocated
chunk  5 location  760:  30 bytes - allocated
chunk  6 location  790:5050 bytes - free
```

#### 3. worst-fit.txt
**Commands:** `./allocator -m 2000 -s *type* worst-fit.txt`
**Explanation:** This test case was used to display the benefits and drawbacks of each allocation method concerning the tasks requested. Specifically, this test was meant to advocate for the ‘worst’ strategy.
**Results:**
First-Fit:
The final free chunk is at location 1070, which is large (850 bytes) but not optimal.
Best-Fit:
The final free chunk is at location 1070, similar to First-Fit, and still large (850 bytes).
Worst-Fit:
The final free chunk is at location 1170, which is 730 bytes (slightly smaller than the First-Fit and Best-Fit final free chunk).

| Strategy   | Allocated | Free Memory | Overhead | Memory Fragmentation |
|------------|-----------|-------------|----------|---------------------|
| First-Fit  | 950 bytes | 4930 bytes  | 120 bytes | Moderate fragmentation |
| Best-Fit   | 950 bytes | 4930 bytes  | 120 bytes | Small fragmented chunks |
| Worst-Fit  | 950 bytes | 4930 bytes  | 120 bytes | Large merged blocks |

Therefore, all three strategies achieve identical memory allocation and free memory results; yet, for systems making large allocations the worst-fit strategy leaves a larger block to use, albeit with wasted smaller chunks existing. 

**SUMMARY:**
```markdown
tkhan12@linux-01:~/CIS3110/CIS3110-W25-A1-code$ ./allocator -m 2000 -s first worst-fit.txt
Running a first-fit model in 2000 (0x7d0) bytes of memory.
overhead per chunk: 20 bytes
available memory: 1980 bytes
’’’
’’’
SUMMARY:
950 bytes allocated
930 bytes free
120 bytes overhead
Total memory usage: 2000 bytes
6 allocation chunks:
chunk  0 location   20: 100 bytes - allocated
chunk  1 location  120:  80 bytes - free
chunk  2 location  220: 400 bytes - allocated
chunk  3 location  620: 300 bytes - allocated
chunk  4 location  920: 150 bytes - allocated
chunk  5 location 1070: 850 bytes - free

tkhan12@linux-01:~/CIS3110/CIS3110-W25-A1-code$ ./allocator -m 2000 -s best worst-fit.txt
Running a best-fit model in 2000 (0x7d0) bytes of memory.
overhead per chunk: 20 bytes
available memory: 1980 bytes
’’’
’’’
SUMMARY:
950 bytes allocated
930 bytes free
120 bytes overhead
Total memory usage: 2000 bytes
6 allocation chunks:
chunk  0 location   20: 100 bytes - allocated
chunk  1 location  120:  80 bytes - free
chunk  2 location  220: 400 bytes - allocated
chunk  3 location  620: 300 bytes - allocated
chunk  4 location  920: 150 bytes - allocated
chunk  5 location 1070: 850 bytes - free

tkhan12@linux-01:~/CIS3110/CIS3110-W25-A1-code$ ./allocator -m 2000 -s worst worst-fit.txt
Running a worst-fit model in 2000 (0x7d0) bytes of memory.
overhead per chunk: 20 bytes
available memory: 1980 bytes
’’’
’’’
SUMMARY:
950 bytes allocated
930 bytes free
120 bytes overhead
Total memory usage: 2000 bytes
6 allocation chunks:
chunk  0 location   20: 200 bytes - free
chunk  1 location  220: 400 bytes - allocated
chunk  2 location  620: 300 bytes - allocated
chunk  3 location  920: 100 bytes - allocated
chunk  4 location 1020: 150 bytes - allocated
chunk  5 location 1170: 730 bytes - free
```

#### 4. first-fit.txt
**Commands:** `./allocator -m 2000 -s *type* first-fit.txt`
**Explanation:** This test case was used to display the benefits and drawbacks of each allocation method concerning the tasks requested. Specifically, this test was meant to advocate for the ‘first’ strategy.
**Results:**
First-Fit:
The final free chunk is at location 1020, which is large (900 bytes) but not optimal.
Best-Fit:
The final free chunk is at location 1020, similar to First-Fit, and still large (900 bytes).
Worst-Fit:
The final free chunk is at location 1120, which is 780 bytes (slightly smaller than the First-Fit and Best-Fit final free chunk).

| Strategy   | Allocated | Free Memory | Overhead | Memory Fragmentation |
|------------|-----------|-------------|----------|---------------------|
| First-Fit  | 1000 bytes | 900 bytes  | 100 bytes | Minimal fragmentation |
| Best-Fit   | 1000 bytes | 900 bytes  | 100 bytes | Minimal fragmentation |
| Worst-Fit  | 1000 bytes | 880 bytes  | 120 bytes | Larger merged blocks |

Therefore, when a system needs to make fast allocations with relatively minimal fragmentation, first-fit is the best strategy. Although it can have fragmentations over time as we saw in previous tests, for the specified test file it operated at an optimal rate.

**SUMMARY:**
```markdown
tkhan12@linux-01:~/CIS3110/CIS3110-W25-A1-code$ ./allocator -m 2000 -s first first-fit.txt
Running a first-fit model in 2000 (0x7d0) bytes of memory.
overhead per chunk: 20 bytes
available memory: 1980 bytes
’’’
’’’
SUMMARY:
1000 bytes allocated
900 bytes free
100 bytes overhead
Total memory usage: 2000 bytes
5 allocation chunks:
chunk  0 location   20: 100 bytes - allocated
chunk  1 location  120: 200 bytes - allocated
chunk  2 location  320: 300 bytes - allocated
chunk  3 location  620: 400 bytes - allocated
chunk  4 location 1020: 900 bytes - free

tkhan12@linux-01:~/CIS3110/CIS3110-W25-A1-code$ ./allocator -m 2000 -s best first-fit.txt
Running a best-fit model in 2000 (0x7d0) bytes of memory.
overhead per chunk: 20 bytes
available memory: 1980 bytes
’’’
’’’
SUMMARY:
1000 bytes allocated
900 bytes free
100 bytes overhead
Total memory usage: 2000 bytes
5 allocation chunks:
chunk  0 location   20: 100 bytes - allocated
chunk  1 location  120: 200 bytes - allocated
chunk  2 location  320: 300 bytes - allocated
chunk  3 location  620: 400 bytes - allocated
chunk  4 location 1020: 900 bytes - free

tkhan12@linux-01:~/CIS3110/CIS3110-W25-A1-code$ ./allocator -m 2000 -s worst first-fit.txt
Running a worst-fit model in 2000 (0x7d0) bytes of memory.
overhead per chunk: 20 bytes
available memory: 1980 bytes
’’’
’’’
SUMMARY:
1000 bytes allocated
880 bytes free
120 bytes overhead
Total memory usage: 2000 bytes
6 allocation chunks:
chunk  0 location   20: 100 bytes - free
chunk  1 location  120: 200 bytes - allocated
chunk  2 location  320: 300 bytes - allocated
chunk  3 location  620: 400 bytes - allocated
chunk  4 location 1020: 100 bytes - allocated
chunk  5 location 1120: 780 bytes - free
```

## Overall Conclusion
Across all the tests conducted some results were more similar than others. This reflects the three respective strategies as they utilize different techniques to allocate memory. First-fit allocation means a quicker allocation and performs well under simple circumstances, but can lead to fragmentations which waste space and slow the system down. Best-fit can perform well under fragmentation-prone cases as it can prioritize minimizing these fragments and search for the smallest available block. Still, this does mean a slower completion time as it must actively search for the ‘best’ not just the ‘first’. Meanwhile, worst-fit is good when trying to keep large blocks available for future allocations. Although this means being able to manage larger block allocations, it does mean inefficiency in smaller requirements and also has fragmentations. Conclusively it is reasonable to say all three have their optimal environments, but best-fit would likely be the most practical as the additional time is comparably smaller than the additional space lost by other methods.
