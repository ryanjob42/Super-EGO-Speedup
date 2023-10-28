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

## Compiling and Running the Experiments
The `./scripts/` folder is where all of the scripts are for compiling all versions of the program and running the experiments used in the term project.
Each one is a Bash script and should be run from inside the `scripts` folder.

[1]: https://ics.uci.edu/~dvk/code/SuperEGO.html
[2]: https://github.com/ryanjob42/Super-EGO-Speedup/blob/main/LICENSE.txt
[3]: https://www.cs.colostate.edu/~cs533/
