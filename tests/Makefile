.PHONY: build_server_client prepare configure build test run callgrind all

build_server_client:
	cd ../src/App-Server && make build
	cd ../src/App-Client && make build
conan_setup:
	pip install conan
	conan profile detect --force # Let Conan try to guess the profile, based on the current operating system and installed tools
	cat /root/.conan2/profiles/default || echo "Wrong profile path"
prepare:
	rm -rf build
	mkdir build
	conan install ../ --output-folder=build --build=missing
configure: prepare
	cd build && cmake ..
build: configure
	cd build && cmake --build .
test: build
	# cd build && make test
run:
	cd build && ./exe
callgrind:
	cd build && valgrind --tool=callgrind ./exe && kcachegrin callgrind.out.*
all: prepare configure build test run