RELEASE_FLAGS = "-O3 -Wall -DNDEBUG"
DEBUG_FLAGS = "-O0 -g -Wall -Werror"
RELEASE_PROFILE_FLAGS = "-O3 -Wall -DNDEBUG -DPROFILE -lprofiler"

all: clean debug release

lib/$(VERSION)/RayBox.o : src/RayBox.cpp
	g++ -std=c++14 -DPERFORMANCE -c $< -pipe $(FLAGS) -o $@

lib/$(VERSION)/Tests.o : src/Tests.cpp
	g++ -std=c++14 -c $< -pipe $(FLAGS) -o $@

release:
	mkdir lib;mkdir lib/release;/bin/true
	VERSION=release FLAGS=$(RELEASE_FLAGS) make main
	# Every little helps .. ( runtime performance, this will make debugging much harder )
	strip RayBox
debug:
	mkdir lib;mkdir lib/debug;/bin/true
	VERSION=debug FLAGS=$(DEBUG_FLAGS) make main-valgrind
	#VERSION=debug FLAGS=$(DEBUG_FLAGS) make tests-valgrind

profile:
	mkdir lib;mkdir lib/profile;/bin/true
	VERSION=profile FLAGS=$(RELEASE_PROFILE_FLAGS) make tests-profile
	CPUPROFILE_FREQUENCY=1000 CPUPROFILE=tests.prof ./tests
	VERSION=profile FLAGS=$(RELEASE_PROFILE_FLAGS) make main
	strip main
	google-pprof --text ./tests ./tests.prof
	echo Measuring how long it takes to run 23445 operations, with output disabled
	time --quiet ./RayBox config.txt rays.txt silent

#style:
	# This is my coding standard. There are many like it, but this is mine
	#astyle --indent=force-tab --pad-oper --pad-paren --delete-empty-lines --suffix=none --indent-namespaces --indent-col1-comments -n --recursive *.cpp *.hpp

#tests: lib/$(VERSION)/Tests.o
#	g++ $^ -lgtest -lgtest_main -o tests

#tests-profile: lib/$(VERSION)/Tests.o -lprofiler
#	g++ $^ -lgtest -lgtest_main -o tests

#tests-valgrind: tests
#	valgrind --error-exitcode=1 ./tests
	
main: lib/$(VERSION)/RayBox.o 
	g++ $^ -o RayBox -pipe
	
main-valgrind: main
	valgrind --error-exitcode=1 ./RayBox config.txt rays.txt
	
clean:
	rm -rf tests RayBox lib/*/*.o RayBox_Vinit_Mhapsekar.tgz tests.prof
	
package: clean debug release
	find . -name "*~" -exec rm {} \;
	rm -rf tests RayBox lib/* RayBox_Vinit_Mhapsekar_1.1.tgz
	tar cvzf RayBox_Vinit_Mhapsekar_1.1.tgz src Makefile config.txt rays.txt ReadMe.txt
	