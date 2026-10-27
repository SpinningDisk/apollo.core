# git testing (personal stuff)

.PHONY: clean build-c run-c test-c build-rust run-rust test-rust

clean-c:
	rm -rf protocol/.cmake
	rm -rf usrmgmt/.cmake
	rm -rf test_a/.cmake
	rm -rf test_b/.cmake
	rm -rf build/

	mkdir build
	mkdir build/include
	mkdir build/lib
	mkdir build/bin
build-c:
	make clean
	# building
	echo "building..."
	mkdir -p build
	cp protocol/include/* build/include/;
	cd protocol; mkdir -p .cmake build/lib build/bin; cmake -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/protocol/; cp -r protocol/build/bin/* build/bin/protocol/; cp -r protocol/build/lib/* build/lib/;
	cp tests/include/* build/include/;
	cd tests; mkdir -p .cmake build/lib build/bin; cmake -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/tests; cp -r tests/build/bin/* build/bin/tests/; cp -r tests/build/lib/* build/lib/;
	# cd auth/net_rs; cargo build --release;
	# cp -r auth/ build/auth;
debug-c:
	make clean
	mkdir -p build
	# build
	cp protocol/include/* build/include/;
	cd protocol; mkdir -p .cmake build/lib build/bin; cmake -DCMAKE_BUILD_TYPE=Debug -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/protocol; cp -r protocol/build/bin/* build/bin/protocol/; cp -r protocol/build/lib/* build/lib/;
	cp tests/include/* build/include/
	cd tests; mkdir -p .cmake build/lib build/bin; cmake -DCMAKE_BUILD_TYPE=Debug -S ./ -B ./.cmake; cd .cmake; make;
	mkdir build/bin/tests; cp -r tests/build/bin/* build/bin/tests/; cp -r tests/build/lib/* build/lib/;
	# cd auth/net_rs; cargo build --release;
	# cp -r auth/ build/auth;
test-c:
	make debug-c
	clear
	build/bin/tests/tests


clean-rust:
	rm -rf protocol/src/impl/rust/target/
build-rust:
	clean-c
	clean-rust
	make build-c
	cd protocol/src/impl/rust; cargo build --release; cp protocol/target/release/rust build/bin/protocol/proto.rs
debug-rust:
	@echo NJI
test-rust:
	@echo NJI
