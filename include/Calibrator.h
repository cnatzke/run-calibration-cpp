#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "TFile.h"

class Calibrator
{
public:
    Calibrator(const char * filename, const char * coeff_file = "lin_energy_coeff.txt");
    ~Calibrator(void);
    void Calibrate(std::string type = "quadratic");

private:
    std::vector<float> GetCentroids(int channel, TH2F* h, std::vector<float> peak_energy);

    TFile * hist_file = NULL;
    double linear_gains[64] = {0};
    double linear_offsets[64] = {0};
    int num_channels = 64;


};

#endif
