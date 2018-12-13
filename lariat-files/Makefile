PRG=gnu.exe
GCC=g++
GCCFLAGS=-Wall -Werror -Wextra -std=c++11 -pedantic -Wconversion -Wno-unused-result -g -O2

OBJECTS0=
DRIVER0=driver.cpp

VALGRIND_OPTIONS=-q --leak-check=full --error-exitcode=1
DIFF_OPTIONS=-C100 -a
DIFF_OPTIONS=-y # -W 210 --suppress-common-lines -a

TESTS:=0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 24
OTHERTESTS:=
MEMTESTS:=$(addprefix mem,$(TESTS) 23 25 26)
SUBMISSION:=cs280_assignment2_lariat_dennis.l.zip
SUBMISSION_FILES:=lariat.cpp lariat.h

$(SUBMISSION) : $(TESTS) 24 $(MEMTESTS) mem23 mem24 mem25 mem26
	zip $(SUBMISSION) $(SUBMISSION_FILES)

latex/Makefile doxysuccess : $(SUBMISSION_FILES) Doxyfile
	! (cat Doxyfile; echo EXTRACT_ALL=YES) | doxygen - 2>&1 | grep --color=always -F -e '' -e 'warning'
	! (cat Doxyfile; echo EXTRACT_ALL=NO) | doxygen - 2>&1 | grep --color=always -F -e '' -e 'warning'
	touch doxysuccess

$(PRG): *.cpp *.h Makefile
	$(GCC) -o $(PRG) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS)
$(TESTS) : $(PRG)
	(./$(PRG) $@ | colordiff $(DIFF_OPTIONS) out$@ -)
	@touch $@
$(MEMTESTS) : $(PRG)
	valgrind $(VALGRIND_OPTIONS) ./$(PRG) $(subst mem,,$@) 1>/dev/null
	@touch $@
clean :
	rm -f *.exe *.o *.obj *.exe.manifest $(TESTS) $(MEMTESTS)
