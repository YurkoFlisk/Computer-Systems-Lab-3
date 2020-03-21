# Computer-Systems-Lab-3
Contains solution for INVXOR problem from March Challenge 2020 contest from CodeChef (main.cpp)
which is compiled and tested with different compilation options on University's cluster (G++/ICC compiler)

Also contains custom testcase generator (testGenerator.cpp), which is pretty simple, but produces
relatively adequate (not just "-1"-answer) tests

run.sh is a script used for time-testing different compilation parameters and generating reports.
+1st argument is the "index" of filenames used. E.g. log$1.txt is the output log file, where $1 is "index";
+2nd argument is the source file;
+3rd argument is the input file to be redirected to STDIN of the compiled source;
+4th argument is compiler for the first subtask (different -O options). For testing extensions icc is used anyway.

qsubRun.sh is launched on cluster from the repository directory to start 5 qsub jobs
of run.sh with different first ("index") argument (if they were the same, we could have concurrent writes).
+Arguments 1-3 are passed to arguments 2-4 of run.sh