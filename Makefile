

.PHONY: clean build run

build:
# cleaning
	rm -rf protocol/.cmake
	rm -rf usrmgmt/.cmake
# building
	echo "building..."
	cd protocol; mkdir -p .cmake build/lib build/bin; cmake -S ./ -B ./.cmake; cd .cmake; make;
	cd usrmgmt; mkdir -p .cmake build/lib build/bin; cmake -S ./ -B ./.cmake; cd .cmake; make;
run:
	make build
	echo "running..."
	protocol/build/bin/test