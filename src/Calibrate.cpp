//////////////////////////////////////////////////////////////////////////////////
// EnergyCalibrations energy based on bg peaks; run by run
//
// Author:        Connor Natzke (cnatzke@triumf.ca)
// Creation Date: 08-11-2021
// Last Update:  08-11-2021
// Usage:
//
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TGRSIUtilities.h"
#include "TParserLibrary.h"
#include "TEnv.h"
#include "TH2.h"

#include "EnergyCalibration.h"
#include "InputManager.h"
#include "MakeHistograms.h"
#include "Calibrator.h"

int main(int argc, char **argv)
{
    if (argc == 1) { // no inputs given
        PrintUsage(argv);
        return 0;
    }
    else if (argc == 2) {
        std::cout << "Calibrating channels" << std::endl;
        Calibrator *cal = new Calibrator(argv[1]);
        cal->Calibrate();

    }
    else if (argc == 3) {
        InitGRSISort();
        // makes output look nicer
        std::cout << std::endl;

        const char *data_dir = "/vagrant/data/analysis-trees";
        const char *cal_file = "/vagrant/analysis/two-photon-calibration/July2020_calibration.cal";
        TFile *out = new TFile("charge_histograms.root", "RECREATE");
        InputManager *inputs = new InputManager(data_dir);
        for (int i = std::atoi(argv[1]); i <= atoi(argv[2]); i++) {
            // get list of subruns
            inputs->GetFileList(i);
            // check to make sure we found some analysis files
            if (inputs->GetNumberOfSubruns() != 0) {
                MakeHistograms *hist_maker = new MakeHistograms(inputs);
                TH2F* hist = hist_maker->MakeCalibrationHistogram(cal_file);
                out->cd();
                hist->Write();

                // cleaning up
                delete hist_maker;
                delete hist;
            }
            inputs->ClearFileList();
        }

        std::cout << "Histograms written to: " << out->GetName() << std::endl;
        out->Close();
        // cleaning up
        delete inputs;
    }

    return 0;
} // main()

void InitGRSISort(){
    // makes time retrival happy and loads GRSIEnv
    grsi_path = getenv("GRSISYS");
    if(grsi_path.length() > 0) {
        grsi_path += "/";
    }
    grsi_path += ".grsirc";
    gEnv->ReadFile(grsi_path.c_str(), kEnvChange);

    TParserLibrary::Get()->Load();
    TGRSIOptions::AnalysisOptions()->SetCorrectCrossTalk(false);
} // end InitGRSISort

/******************************************************************************
 * Prints usage message and version
 *****************************************************************************/
void PrintUsage(char* argv[]){
    std::cerr << argv[0] << " Version: " << EnergyCalibration_VERSION_MAJOR << "." << EnergyCalibration_VERSION_MINOR << "\n"
              << "\n----- Basics ------\n"
              << "This program requires a file called 'lin_energy_coeff.txt' that details a basic linear calibration. This should be an output from the initial energy calibration procedure. The fitting and calibration step requires two directories 'run-fits' and 'cal-parameters' to be initialized and available. I could not figure out an easy way to get the program to do this automatically so some user input is required\n"
              << "\n----- Matrix Creation ------\n"
              << "usage: " << argv[0] << " first_run last_run \n"
              << " first_run: Run number of first run in range\n"
              << " last_run: Run number of last run in range\n"
              << "\n----- Fitting and Calibration------\n"
              << "usage: " << argv[0] << " histogram_file\n"
              << " histogram_file: ROOT file containing charge histograms\n"
              << std::endl;
} // end PrintUsage
