default:
	mkdir -p bin/
	clang++ -o bin/smoothclimb src/*.cpp
run: default
	./bin/smoothclimb
clean:
	rm -rf bin/
