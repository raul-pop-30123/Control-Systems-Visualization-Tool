CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

test_polynomial: test/test_polynomial.cpp
	$(CXX) $(CXXFLAGS) -o $@ $