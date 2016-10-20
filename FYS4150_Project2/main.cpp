#include <iostream>
#include <armadillo>
#include <time.h>
#include "system.h"
#include "WaveFunctions/wavefunction.h"
#include "WaveFunctions/doublewell.h"

using namespace std;
using namespace arma;

int main() {

    int N                       = 1000;
    double posMin               = -10;
    double posMax               = 10;
    double omega_r              = 0.5;                                         // =m*w/hbar Just a constant to keep the results correct, while we figure out the omega conundrum.
    int nMax 					= 25;
    int nPrimeMax               = 3;

    int numberOfEigstates       = 10;
    int numberOfDimensions      = 1;

    vec L(3);
    L.fill(0.);
    L(0) = 5.;

    //Set up the vector x and the matrix A:
    double h                    = (posMax-posMin)/N;
    mat r = zeros(N+1,numberOfDimensions);
    vec rAbs = zeros(N+1);

    for (int d = 0; d < numberOfDimensions; d++) {
        r.col(d) = posMin + linspace(0, N, N+1)*h;
        rAbs += r.col(d)%r.col(d);
    }

    rAbs = sqrt(rAbs);


    cube diagMat(N-1, N-1, numberOfDimensions);
    cube eigvecs(N-1, N-1, numberOfDimensions);
    mat  eigvals(N-1, numberOfDimensions);

    mat saveEigenvector         = ones(N-1, numberOfEigstates);
    mat SavePositionvector      = zeros(N-1, numberOfDimensions+1);

    for (int d = 0; d < numberOfDimensions; d++) {
        SavePositionvector.col(d)   = r.col(d).subvec(1, N-1);    //Saves the y vector for output.
    }

    SavePositionvector.col(numberOfDimensions) = rAbs.subvec(1, N-1);    //Saves the r vector for output.

    vec SaveConstants           = {omega_r, double(numberOfDimensions), L(0), L(1), L(2), double(N), double(numberOfEigstates), h};

    //Init system
    System* system = new System(omega_r, numberOfDimensions, h, N);

    system->setWaveFunction(new DoubleWell(system, omega_r));

    system->diagonalizeMatrix(r, L, N, diagMat);
    system->findEigenstate(eigvals, eigvecs, diagMat, numberOfEigstates, saveEigenvector);
    mat supPos = system->findSuperPos(r, nMax, nPrimeMax);

    SaveConstants.save("../FYS4150_Project2/PlotAndData/omega5constants.dat", raw_ascii);
    SavePositionvector.save("../FYS4150_Project2/PlotAndData/omega5position.dat", raw_ascii);
    saveEigenvector.save("../FYS4150_Project2/PlotAndData/omega5norepulsion.dat", raw_ascii);
    supPos.save("../FYS4150_Project2/PlotAndData/supPos.dat", raw_ascii);
    //saveEigenvector.print();

    cout << "eigvals, Armadillo:" << endl;
    int displayVals = 15;
    for (int i = 0; i < displayVals; ++i) {
        for (int d = 0; d < numberOfDimensions; d++) {
            cout << i+1 << ": E"<< d <<": " << eigvals.col(d)(i);
        }
        cout << endl;
    }
    cout << endl;
    cout << "Computation time (sec):" << endl;
    cout << "Aramadillo: " << system->getComputationTime() << endl;

    return 0;
}
