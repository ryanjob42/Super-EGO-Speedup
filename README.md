# Super-EGO Speedup
Super-EGO is an efficient in-memory implementation of the $\epsilon$-join.
This join compares pairs of records from two datasets,
returning any pairs with an absolute difference less than an $\epsilon$ parameter.
The original source code can be found [here][1].
Also see the license file which was retained from the source at [LICENSE.txt][2].

This repository was created for a term project at Colorado State University for [CS 533 Database Management Systems][3].
The goal of this project is to speed up the implementation of the implementation.
Since the code is from 2013 and intended to be compiled with GCC 4.7,
we will evaluate the use of recent compilers (as of 2023) and optimization techniques for improving the performance.

## Important Functions
This section lists some of the important functions in the Super-EGO code to aid the user in understanding.
This includes functions which were not modified (see the [Code Changes](#code-changes) section for this information).

- The `multiThreadJoin()` function in [MultiThreadJoin.cpp][8] is where setup is done, including thread creation. This is also where the timing is performed.
- The `thread_function()` function in [MultiThreadJoin.cpp][9] retrieves work from a queue and calls into the main algorithm.
- The `egoJoin()` function in [Util.cpp][10] is the main recursive step of the algorithm which decides whether to execute the work directly or to break up the job into multiple.
- The `simpleJoin2()` function in [Util.cpp][11] is the base case of the recursion which executes a join on sufficiently small sets of records.
  - This is where the optimizations are implemented for this project.
  - To see the unmodified code, [click this link][12].

## Code Changes
The loop where the algorithm spends most of its runtime is found in [Util.cpp][4] around line 700.
Optimized variations of this loop were created with compiler directives used to determine which one is compiled.

Some of the optimized versions require an extra file to be included to enable vector intrinsics.
To avoid this affecting any of the other versions, this is only included for the versions which need it.
See the top of [Util.cpp][5].

The original code performed an inequality comparison between a pointer and a constant 0 near the end of [Util.cpp][6].
Almost all of the compilers generated an error for this, as it is considered undefined behavior.
A compiler directive was used to conditionally compile a fixed version (by checking that the pointer is not equal to 0)
only for the compilers which generate this error.

## Compiling and Running the Experiments
The [scripts][7] folder is where all of the scripts are for compiling all versions of the program and running the experiments used in the term project.
Each one is a Bash script and should be run from inside the [scripts][7] folder.

Note: the ICX build is broken as of 28-Oct-2023.

[1]: https://ics.uci.edu/~dvk/code/SuperEGO.html
[2]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/LICENSE.txt
[3]: https://www.cs.colostate.edu/~cs533/
[4]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/Util.cpp#L697
[5]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/Util.cpp#L11
[6]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/Util.cpp#L1140
[7]: https://github.com/ryanjob42/Super-EGO-Speedup/tree/main/scripts
[8]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/MultiThreadJoin.cpp#L35
[9]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/MultiThreadJoin.cpp#L97
[10]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/Util.cpp#L167
[11]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/Util.cpp#L664
[12]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/7b4ece3fdfe35294a57ed278ca5c230bd90fcc00/Util.cpp#L600
