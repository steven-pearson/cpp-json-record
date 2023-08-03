[![CMake CI](https://github.com/steven-pearson/cpp-json-record/actions/workflows/cmake.yml/badge.svg)](https://github.com/steven-pearson/cpp-json-record/actions/workflows/cmake.yml)
# cpp-json-record
C++ project allowing a record that's schema and logic is defined in JSON

### Build
```
cd build
cmake ..
make
```

### Run tests
```
cd build
ctest
```

### Generate coverage
```
cd build/src/CMakeFiles/record.dir
gcov record.cpp.gcno
mkdir ../../../../coverage
lcov --capture --directory . --output-file ../../../../coverage/lcov.info
cd ../../../../coverage
genhtml lcov.info --output-directory results
```

### View coverage
Open results/index.html
