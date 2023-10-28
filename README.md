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

[1]: https://ics.uci.edu/~dvk/code/SuperEGO.html
[2]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/LICENSE.txt
[3]: https://www.cs.colostate.edu/~cs533/
[4]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/Util.cpp#L697
[5]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/Util.cpp#L11
[6]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/Util.cpp#L1140
[7]: https://github.com/ryanjob42/Super-EGO-Speedup/tree/main/scripts
