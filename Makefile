.PHONY: conan_setup prepare configure build run all

conan_setup:
	pip install conan
	conan profile detect --force # Let Conan try to guess the profile, based on the current operating system and installed tools
	cat /root/.conan2/profiles/default || echo "Wrong profile path"
prepare:
	rm -rf build
	mkdir build
	conan install ./ --output-folder=build --build=missing
configure: prepare
	cd build && cmake ..
build: configure
	cd build && cmake --build .
run_server:
	./build/src/App-Server/exe_server
run_client:
	./build/src/App-Server/exe_client
run_tests:
	./build/src/App-Server/exe_tests
all: prepare configure build