prepare:
	rm -rf build
	mkdir build
conan: prepare
	#pip install conan
	conan profile detect --force # Let Conan try to guess the profile, based on the current operating system and installed tools
	#cat /root/.conan2/profiles/default || echo "Wrong profile path"
	conan install ./ --output-folder=build --build=missing
configure: conan
	cd build && cmake ..
build: configure
	cd build && cmake --build . -j
run_server:
	./build/src/app_server/exe_server
run_client:
	./build/src/app_client/exe_client
run_tests:
	./build/tests/exe_tests