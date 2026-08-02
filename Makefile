CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC_MATH = src/math/ComplexNumber.cpp src/math/Matrix.cpp src/math/Polynomial.cpp
SRC_CONTROL = src/control/TransferFunction.cpp src/control/Stability.cpp \
              src/control/RootLocus.cpp src/control/StateSpace.cpp \
              src/control/Conversions.cpp src/control/TimeResponse.cpp

# Interactive console menu: choose the input representation, then the
# analysis to run. No GUI dependency, so it always builds.
main: src/main.cpp $(SRC_MATH) $(SRC_CONTROL)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Math/control validation target: checks every header against
# hand-checked and book-sourced results. Run after touching any file.
test_math: test/test_math.cpp $(SRC_MATH) $(SRC_CONTROL)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f main test_math

.PHONY: clean
