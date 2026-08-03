#ifndef ROOT_LOCUS_HPP
#define ROOT_LOCUS_HPP

#include <vector>
#include "control/TransferFunction.hpp"

struct RootLocusPoint {
    double k;
    std::vector<ComplexNumber> poles;
};

class RootLocus {
private:
    TransferFunction plant;
public:
    explicit RootLocus(const TransferFunction& openLoopPlant);

     // Samples the locus for k in [0, kMax].
    std::vector<RootLocusPoint> compute(double kMax, int samples) const;

    // sigma_a = ( sum Poles - sum Zeroes ) / (n - m)
    double asymptoteCentroid() const;

    // Phi_ai = (2i-1)*pi / (n-m), i = 1..n-m
    std::vector<double> asymptoteAngles() const;

    // Scans k upward in small steps and bisects across the first
    // stability-verdict change found via Stability::routhHurwitz applied to
    // the closed-loop characteristic polynomial
    // Returns false if no such crossing is found within [0, kMax].
    bool findCriticalK(double kMax, double& outCriticalK) const;
};

#endif
