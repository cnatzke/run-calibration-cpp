//////////////////////////////////////////////////////////////////////////////////
// Creates calibration parameters
//
// Author:          Connor Natzke (cnatzke@triumf.ca)
// Creation Date:   08-11-2021
// Last Update:     08-11-2021
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "TKey.h"
#include "TH2.h"
#include "TF1.h"

#include "Calibrator.h"

//////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////
Calibrator::Calibrator(const char * filename, const char * coeff_file)
{
    hist_file = new TFile(filename);

    // read in linear calibration for rough peak positions
    std::ifstream lin_coeff_file;
    lin_coeff_file.open(coeff_file);

    if (lin_coeff_file) {
        std::cout << "Found linear coefficient file: " << coeff_file << std::endl;
        for (int i = 0; i < num_channels; i++) {
            lin_coeff_file >> linear_gains[i];
            lin_coeff_file >> linear_offsets[i];
        }//for
        lin_coeff_file.close();
    }
    else {
        std::cout << "Cannot find linear coefficient file: " << coeff_file << " exiting. " << std::endl;
        std::exit(1);
    }


} // end constructor

//////////////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////////////
Calibrator::~Calibrator(void)
{
} // end destructor

//////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////
void Calibrator::Calibrate(std::string type)
{

    std::vector<float> peak_energy = {511.0, 1460.82, 2103.511, 2614.511};
    std::vector<float> peak_energy_error = {0.0, 0.005, 0.01, 0.01};

    TList* list = hist_file->GetListOfKeys();
    if (!list) { printf("<E> No keys found in file\n"); exit(1); }
    TIter next(list);
    TKey* key;
    TObject* obj;

    // loop through all histograms in file
    while ( (key = (TKey*)next()) ) {
        obj = key->ReadObj();
        // make sure we get histograms
        if ((strcmp(obj->IsA()->GetName(),"TProfile")!=0) && (!obj->InheritsFrom("TH2"))) {
            printf("<W> Object %s is not 1D or 2D histogram : "
                   "will not be converted\n", obj->GetName());
        }
        else {
            for (int i = 0; i < num_channels; i++) {
                std::vector<float> centroids = GetCentroids(i, dynamic_cast<TH2F*>(obj), peak_energy);
                for (auto const& iterator : centroids) {
                    std::cout << iterator << ", ";
                }
                std::cout << std::endl;
            }
        }
    }

    if (type.compare("linear") == 0) {
    }
    else if (type.compare("quadratic") == 0) {
    } else {
        std::exit(1);
    }

} // end Calibrate

//////////////////////////////////////////////////////////////////////////////////
// Fits centroids for each channel
//////////////////////////////////////////////////////////////////////////////////
std::vector<float> Calibrator::GetCentroids(int channel, TH2F* h, std::vector<float> peak_energy)
{
    std::vector<float> centroids(peak_energy.size(), 0);
    if ((linear_offsets[channel] == -1) && (linear_gains[channel] == -1)){
        //std::cout << "Channel " << channel << " missing valid linear calibration, skipping..." << std::endl;
        return centroids;
    }
    TH1F * p = (TH1F*) h->ProjectionY("p", channel + 1, channel + 1);
    for (int j = 0; j < (int)peak_energy.size(); j++) {
        int min_bin = p->GetXaxis()->GetFirst();
        int max_bin = p->GetXaxis()->GetLast();
        int bin_guess = (peak_energy[j] - linear_offsets[channel]) / linear_gains[channel];
        p->SetAxisRange(bin_guess - 10, bin_guess + 10, "X");
        //to help with the peak fitting, move initial centroid guess to bin with most counts
        Int_t bin = p->GetMaximumBin();
        Double_t x_guess = p->GetXaxis()->GetBinCenter(bin);
        Double_t y_guess = p->GetBinContent(bin);
        p->SetAxisRange(min_bin, max_bin, "X");

        // Gaussian + step bg + scaling
        TF1 *fit = new TF1(Form("fit %i-%i", channel, j),"[0]*(exp(-((x-[1])^2/(2*[2]^2)))) + [3] + [4]*(0.5*(1-(ROOT::Math::erf(((x-[1])/([2]*2^(0.5)))))))", x_guess - 8, x_guess + 8);
        fit->SetParName(0, "amplitude");
        fit->SetParName(1, "centroid");
        fit->SetParName(2, "sigma");
        // set initial guesses and parameter limits
        fit->SetParameters(y_guess, x_guess, 2, 10, 1, 1, 1, 1);
        fit->SetParLimits(0, y_guess*0.8, y_guess*1.2);             //area
        fit->SetParLimits(1, x_guess - 5, x_guess + 5);             //centroid
        fit->SetParLimits(2, 0.2, 5);             //sigma of gaussian distribution

        //fitting equation and saving centroids
        p->GetXaxis()->SetRangeUser(bin_guess - 8, bin_guess + 8);
        p->Fit(fit, "QR+");

        centroids.at(j) = fit->GetParameter(1);
        //std::cout << "Graph|" << channel << "|Peak|" << peak_energy.at(j) << "|Guess|" << x_guess << "|Fitted|" << fit->GetParameter(1) << "|RCS|" << fit->GetChisquare()/fit->GetNDF() << std::endl;

    }             // end peak loop
    delete p;
    return centroids;
} // end GetCentroids
