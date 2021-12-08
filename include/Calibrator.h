#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "TFile.h"
#include "TF1.h"

class Calibrator
{
public:
    Calibrator(const char * filename, const char * coeff_file = "lin_energy_coeff.txt");
    ~Calibrator(void);
    void Calibrate(std::string type = "linear");

private:
    void GetCentroids(int channel, TH2F* h, std::vector<float> peak_energy, TFile * out_file);
    void FindCalibrationParameters(int channel, TF1* cal_fit, std::vector<float> peak_energy, std::vector<float> peak_energy_error, TFile * out_file, std::ofstream &param_file);

    TFile * hist_file = NULL;
    double linear_gains[64] = {0};
    double linear_offsets[64] = {0};
    int num_channels;
    TF1 *cal_fit;
    std::vector<float> centroids;
    std::vector<float> centroids_error;
    std::vector<float> cal_parameters;
    std::vector<float> cal_residuals;
    std::vector<float> linear_params_vec;
    std::vector<float> offset_params_vec;


};

#endif
