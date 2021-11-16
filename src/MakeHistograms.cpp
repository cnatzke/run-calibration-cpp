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
void MakeHistograms::MakeCalibrationHistograms(const char* cal_file)
{
    TChain *analysis = new TChain("AnalysisTree");
    std::vector<TString> run_list = inputs->file_list;

    // add all subruns to TChain
    for(auto i = 0; i < (int) run_list.size(); i++) {
        analysis->AddFile(Form("%s/%s", inputs->data_dir, run_list.at(i).Data()));
    }

    std::cout << "Found " << analysis->GetNtrees() << " analysis trees" << std::endl;
    TTree *tree = (TTree *) analysis->GetTree();

    TGriffin * griffin = 0;
    if (analysis->FindBranch("TGriffin")) {
        analysis->SetBranchAddress("TGriffin", &griffin);
    } else {
        std::cout << "No TGriffin branch found, exiting..." << std::endl;
        std::exit(1);
    }

    // Create histograms
    char hname[20];
    for(int i = 0; i < num_crystals; i++) {
        sprintf(hname,"run%i_%i", inputs->GetRunNumber(), i);
        histograms[i] = new TH1F(hname, hname, 4000, 0, 4000);
    }

    //std::cout << "Histograms created." << std::endl;

    //filling histograms with data from analysis root file
    Int_t num_entries = analysis->GetEntries();
    TChannel::ReadCalFile(cal_file);
    LoadingMessenger load_man;
    load_man.DisplayLoadingMessage();
    ProgressBar progress_bar(num_entries, 70, '=', ' ');
    for (int i = 0; i < num_entries - 1; i++) {
        analysis->GetEntry(i);
        for (int j = 0; j < griffin->GetMultiplicity(); j++) {
            TGriffinHit *griffin_hit = griffin->GetGriffinHit(j);
            histograms[griffin_hit->GetArrayNumber()-1]->Fill(griffin_hit->GetCharge());
        }

        if (i % 10000 == 0) progress_bar.display();
        ++progress_bar;
    }
    progress_bar.done();

} // end MakeCalibrationHistograms
