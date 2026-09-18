
.PHONY: clean build run test

#DON'T CHANGE THIS LENGTH (without first chaning make.sh) pls :3

clean:
	rm -rf protocol/.cmake
	rm -rf usrmgmt/.cmake
	rm -rf test_a/.cmake
	rm -rf test_b/.cmake
	rm -rf build/

	mkdir build
	mkdir build/include
	mkdir build/lib
	mkdir build/bin
build:
	make clean
	# building
	echo "building..."
	mkdir -p build
	cp protocol/include/* build/include/;
	cd protocol; mkdir -p .cmake build/lib build/bin; cmake -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/protocol/; cp -r protocol/build/bin/* build/bin/protocol/; cp -r protocol/build/lib/* build/lib/;
	cp usrmgmt/include/* build/include/;
	cd usrmgmt; mkdir -p .cmake build/lib build/bin; cmake -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/usrmgmt; cp -r usrmgmt/build/bin/* build/bin/usrmgmt/; cp -r usrmgmt/build/lib/* build/lib/;
	cp tests/include/* build/include/;
	cd tests; mkdir -p .cmake build/lib build/bin; cmake -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/tests; cp -r tests/build/bin/* build/bin/tests/; cp -r tests/build/lib/* build/lib/;
	# cd auth/net_rs; cargo build --release;
	# cp -r auth/ build/auth;
debug:
	make clean
	mkdir -p build
	# build
	cp protocol/include/* build/include/;
	cd protocol; mkdir -p .cmake build/lib build/bin; cmake -DCMAKE_BUILD_TYPE=Debug -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/protocol; cp -r protocol/build/bin/* build/bin/protocol/; cp -r protocol/build/lib/* build/lib/;
	cp usrmgmt/include/* build/include/;
	cd usrmgmt; mkdir -p .cmake build/lib build/bin; cmake -DCMAKE_BUILD_TYPE=Debug -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/usrmgmt; cp -r usrmgmt/build/bin/* build/bin/usrmgmt/; cp -r usrmgmt/build/lib/* build/lib/;
	# cd auth/net_rs; cargo build --release;
	# cp -r auth/ build/auth;


run:
	make build
	echo "running..."

rnd:
	make debug
	echo "debugging"
test:
	make rnd
	cp tests/include/* build/include/
	cd tests; mkdir -p .cmake build/lib build/bin; cmake -DCMAKE_BUILD_TYPE=Debug -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/tests; cp -r tests/build/bin/* build/bin/tests/; cp -r tests/build/lib/* build/lib/;
	build/bin/tests/tests
