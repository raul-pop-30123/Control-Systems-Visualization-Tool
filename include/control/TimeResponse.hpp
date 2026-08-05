#ifndef TIME_RESPONSE_HPP
#define TIME_RESPONSE_HPP

#include <vector>
#include "control/StateSpace.hpp"

// One point of a simulated time response.
struct ResponseSample {
    double t;
    double y;
};

// Numerically simulates x' = Ax + Bu, y = Cx + Du using the forward Euler
// method. Euler is the simplest possible integrator -- x_{k+1} = x_k + dt*x'_k --
// and is accurate enough here provided dt is small relative to the
// system's fastest time constant (as a rule of thumb, dt well under
// 1 / (10 * |fastest pole|)).
class TimeResponse{
public:
    // Step response: u(t) = 1 for all t >= 0, x(0) = 0.
    static std::vector<ResponseSample> step(const StateSpace& ss, double tFinal, double dt);

    // Impulse response: rather than simulating an infinite, instantaneous
    // spike (which a fixed-step integrator cannot represent), this uses
    // the standard equivalence for a continuous system with u = delta(t):
    // integrating the impulse over an infinitesimal time jumps the state
    // from 0 to B, so the impulse response equals the free (u=0) response
    // starting from x(0) = B. 
    static std::vector<ResponseSample> impulse(const StateSpace& ss, double tFinal, double dt);

private:
    static std::vector<ResponseSample> simulate(const StateSpace& ss, double tFinal, double dt,
                                                  Matrix x0, bool applyStepInput);
};

#endif