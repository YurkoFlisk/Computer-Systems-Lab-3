srcfiles = main.cpp $(wildcard 'engine/*.cpp')

perft: $(srcfiles)
	icc $(FLAGS) $@ -o $(EXENAME)