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

#include "EnergyCalibration.h"
#include "InputManager.h"
#include "MakeHistograms.h"

int main(int argc, char **argv)
{
    if (argc == 1) { // no inputs given
        PrintUsage(argv);
        return 0;
    }
    else if (argc == 2) {
        //const char *data_dir = "/vagrant/data/histograms/pre-src-calibration/runs";
        std::cout << "Here" << std::endl;

    }
    else if (argc == 3) {
        InitGRSISort();
        // makes output look nicer
        std::cout << std::endl;

        const char *data_dir = "/vagrant/data/analysis-trees";
        InputManager *inputs = new InputManager(data_dir);
        for (int i = std::atoi(argv[1]); i <= atoi(argv[2]); i++) {
            // get list of subruns
            std::vector<TString> file_list = inputs->GetFileList(i);
            // check to make sure we found some analysis files
            if (file_list.size() != 0){
                MakeHistograms *hist_maker = new MakeHistograms(inputs);
                hist_maker->MakeCalibrationHistograms();
                delete hist_maker;
            }
            inputs->ClearFileList();
        }
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
} // end InitGRSISort

/******************************************************************************
 * Prints usage message and version
 *****************************************************************************/
void PrintUsage(char* argv[]){
    std::cerr << argv[0] << " Version: " << EnergyCalibration_VERSION_MAJOR << "." << EnergyCalibration_VERSION_MINOR << "\n"
              << "\n----- Background Subtractions ------\n"
              << "usage: " << argv[0] << " source_file background_file \n"
              << " source_file: Source histograms\n"
              << " background_file: Background histograms\n"
              << "\n----- Matrix Creation ------\n"
              << "usage: " << argv[0] << " histogram_file\n"
              << " histogram_file: ROOT file containing background subtracted histograms\n"
              << std::endl;
} // end PrintUsage
