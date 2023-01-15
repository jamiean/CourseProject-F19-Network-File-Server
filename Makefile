CC=g++ -g -Wall

# List of source files for your file server
FS_SOURCES=fs.cpp parser.cpp

# Generate the names of the file server's object files
FS_OBJS=${FS_SOURCES:.cpp=.o}

EXES=fs testspec parsertest teststress1 teststress2 testinvalidparam testcreate testwrite testread testread2 testdelete testinit testoverflowdir testbasic1 testdir testmulti testowner testoverflowfile testowner2 testshrink

all: ${EXES}

# Compile the file server and tag this compilation
fs: ${FS_OBJS} libfs_server.o
	./autotag.sh
	${CC} -o $@ $^ -lboost_thread -lboost_system -pthread -ldl

# Compile a client program
testspec: testspec.cpp libfs_client.o
	${CC} -o $@ $^

testinvalidparam: test_invalid_param.cpp libfs_client.o
	${CC} -o $@ $^
	
testcreate: test_invalid_create.cpp libfs_client.o
	${CC} -o $@ $^

testwrite: test_invalid_write.cpp libfs_client.o
	${CC} -o $@ $^

testread: test_invalid_read.cpp libfs_client.o
	${CC} -o $@ $^

testread2: test_invalid_read_2.cpp libfs_client.o
	${CC} -o $@ $^

testdir: test_many_dir.cpp libfs_client.o
	${CC} -o $@ $^

testdelete: test_invalid_delete.cpp libfs_client.o
	${CC} -o $@ $^

testowner: test_owner.cpp libfs_client.o
	${CC} -o $@ $^

testowner2: test_owner_2.cpp libfs_client.o
	${CC} -o $@ $^

testinit: testinit.cpp libfs_client.o
	${CC} -o $@ $^

parsertest: parser_test.cpp parser.o
	${CC} -o $@ $^  -lboost_thread -lboost_system -pthread -ldl

testoverflowdir: test_overflow_dir.cpp libfs_client.o
	${CC} -o $@ $^

testoverflowfile: test_overflow_file.cpp libfs_client.o
	${CC} -o $@ $^

testbasic1: testbasic1.cpp libfs_client.o
	${CC} -o $@ $^

testmulti: testmulti.cpp libfs_client.o
	${CC} -o $@ $^

testshrink: test_shrink.cpp libfs_client.o
	${CC} -o $@ $^

teststress1: test_stress_1.cpp libfs_client.o
	${CC} -o $@ $^

teststress2: test_stress_2.cpp libfs_client.o
	${CC} -o $@ $^


# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${FS_OBJS} ${EXES}