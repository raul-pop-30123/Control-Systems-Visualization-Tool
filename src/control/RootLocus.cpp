#include "control/RootLocus.hpp"
#include "control/Stability.hpp"
#include <cmath>

#define M_PI 3.14159265358979323846

using namespace std;

RootLocus::RootLocus(const TransferFunction& openLoopPlant) : plant(openLoopPlant) {}

vector<RootLocusPoint> RootLocus::compute(double kMax, int samples) const {
    vector<RootLocusPoint> locus;
    locus.reserve(samples);
    for(int i = 0; i < samples; i++){
        double k = (samples == 1) ? 0.0 : kMax * static_cast<double>(i) / (samples - 1);
        RootLocusPoint point;
        point.k = k;
        point.poles = plant.closedLoopPoles(k);
        locus.push_back(point);
    }
    return locus;
}

double RootLocus::asymptoteCentroid() const {
    vector<ComplexNumber> poles = plant.poles();
    vector<ComplexNumber> zeros = plant.zeros();
    int n = static_cast<int>(poles.size());
    int m = static_cast<int>(zeros.size());
    if(n <= m) return 0.0;

    double sumPoles = 0.0, sumZeros = 0.0;
    for(auto& p : poles) sumPoles += p.getReal();
    for(auto& z : zeros) sumZeros += z.getReal();
    return (sumPoles - sumZeros) / static_cast<double>(n - m);
}

vector<double> RootLocus::asymptoteAngles() const {
    vector<ComplexNumber> poles = plant.poles();
    vector<ComplexNumber> zeros = plant.zeros();
    int n = static_cast<int>(poles.size());
    int m = static_cast<int>(zeros.size());
    int diff = n - m;
    vector<double> angles;
    if(diff <= 0) return angles;
    for(int i = 1; i <= diff; i++){
        angles.push_back((2.0 * i - 1.0) * M_PI / diff);
    }
    return angles;
}

bool RootLocus::findCriticalK(double kMax, double& outCriticalK) const {
    const int steps = 2000;
    // Seed from a tiny positive k rather than exactly 0: if the open-loop
    // plant has a pole exactly at the origin (a free integrator, common in
    // these examples), k=0 puts a closed-loop pole exactly on the
    // imaginary axis too, which Routh-Hurwitz correctly reports as
    // marginal -- but that's an artifact of k=0 itself, not the crossing
    // we're looking for.
    bool prevStable = Stability::routhHurwitz(plant.closedLoopCharacteristicPolynomial(kMax * 1e-6)).stable;

    for(int i = 1; i <= steps; i++){
        double k = kMax * static_cast<double>(i) / steps;
        bool nowStable = Stability::routhHurwitz(plant.closedLoopCharacteristicPolynomial(k)).stable;

        if(nowStable != prevStable){
            // Bisect between the previous sample and this one to refine.
            double lo = kMax * static_cast<double>(i - 1) / steps;
            double hi = k;
            for(int b = 0; b < 60; b++){
                double mid = (lo + hi) / 2.0;
                bool midStable = Stability::routhHurwitz(plant.closedLoopCharacteristicPolynomial(mid)).stable;
                if(midStable == prevStable) lo = mid; else hi = mid;
            }
            outCriticalK = (lo + hi) / 2.0;
            return true;
        }
        prevStable = nowStable;
    }
    return false;
}