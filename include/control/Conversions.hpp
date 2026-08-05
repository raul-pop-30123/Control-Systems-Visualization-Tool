#ifndef CONVERSIONS_HPP
#define CONVERSIONS_HPP

#include "control/StateSpace.hpp"
#include "control/TransferFunction.hpp"

// Bridges between the two system representations. Lives above both
// StateSpace and TransferFunction in the include graph on purpose: neither
// of those two headers knows about the other, so there's no cycle, and any
// analysis that only needs one representation (e.g. StateSpace-based
// stability, or TransferFunction-based root locus) never has to include
// this file at all.
class Conversions{
public:
    // H(s) = C*adj(sI-A)*B / det(sI-A) + D
    // Built from StateSpace::characteristicPolynomial's
    // Faddeev-LeVerrier output, so no separate matrix-inversion code is
    // needed for this direction.
    static TransferFunction toTransferFunction(const StateSpace& ss);

    // Builds a state-space realization directly from a transfer function's
    // coefficients using Controllable Canonical Form.
    static StateSpace toStateSpace(const TransferFunction& tf);
};

#endif