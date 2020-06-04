usage: ./tester.sh <executable>
       ./tester_no_mem.sh <executable>

tester.sh checks both output logic and memory with valgrind
tester_no_mem.sh cehck logic only
input folder "in" contains all the input files, they must begin with in to register
output folder "out" contains good output files to compare with,
they must begin with out and have same name as input file (similar to the existing files).
test folder contains your output.
err folder contains diff between your output and good output if exists.

chmod +x <script name> to give execution permissions if needed
