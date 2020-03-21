# Computer-Systems-Lab-3
Contains solution for INVXOR problem from March Challenge 2020 contest from CodeChef (main.cpp)
which is compiled and tested with different compilation options on University's cluster (ICC compiler)

Also contains custom testcase generator (testGenerator.cpp), which is pretty simple, but produces
relatively adequate (not just "-1"-answer) tests

run.sh is a script used for time-testing differenct compilation parameters and generating reports.
log$1.txt is the output log file, where $1 is "index", expected to be the first argument of the script

qsubRun.sh is launched on cluster from the repository directory to start 5 qsub jobs
of run.sh with different "index" option (if they were the same, we could have concurrent writes).