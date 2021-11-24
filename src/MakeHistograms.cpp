//////////////////////////////////////////////////////////////////////////////////
// Creates Channel vs Charge histograms
//
// Author:        Connor Natzke (cnatzke@triumf.ca)
// Creation Date: 15-11-2021
// Last Update:  15-11-2021
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "TChain.h"
#include "TGriffin.h"
#include "MakeHistograms.h"
#include "LoadingMessenger.h"
#include "progress_bar.h"

//////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////
MakeHistograms::MakeHistograms(InputManager* inputs) : inputs(inputs)
{
} // end constructor

//////////////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////////////
MakeHistograms::~MakeHistograms()
{
} // end destructor

//////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////
TH2F* MakeHistograms::MakeCalibrationHistogram(const char* cal_file)
{
    TChain *analysis = new TChain("AnalysisTree");
    std::vector<TString> run_list = inputs->file_list;

    // add all subruns to TChain
    for(auto i = 0; i < (int) run_list.size(); i++) {
        analysis->AddFile(Form("%s/%s", inputs->data_dir, run_list.at(i).Data()));
    }

    std::cout << "Processing run:  " << inputs->GetRunNumber() << std::endl;
    std::cout << "Found " << analysis->GetNtrees() << " analysis trees" << std::endl;

    TGriffin * griffin = 0;
    if (analysis->FindBranch("TGriffin")) {
        analysis->SetBranchAddress("TGriffin", &griffin);
    } else {
        std::cout << "No TGriffin branch found, exiting..." << std::endl;
        std::exit(1);
    }

    // Create histograms
    char hname[20];
    sprintf(hname,"run_%i", inputs->GetRunNumber());
    TH2F * hist = new TH2F(hname, hname, 66, 0, 66, 4000, 0, 4000);

    //filling histograms with data from analysis root file
    Int_t num_entries = analysis->GetEntries();
    TChannel::ReadCalFile(cal_file);
    LoadingMessenger load_man;
    load_man.DisplayLoadingMessage();
    ProgressBar progress_bar(num_entries, 70, '=', ' ');
    for (int i = 0; i < num_entries - 1; i++) {
        analysis->GetEntry(i);
        for (int j = 0; j < griffin->GetMultiplicity(); j++) {
            TGriffinHit *grif_hit = griffin->GetGriffinHit(j);
            hist->Fill(grif_hit->GetArrayNumber() - 1, grif_hit->GetCharge());
        }

        if (i % 10000 == 0) progress_bar.display();
        ++progress_bar;
    }
    progress_bar.done();

    return hist;

} // end MakeCalibrationHistograms
