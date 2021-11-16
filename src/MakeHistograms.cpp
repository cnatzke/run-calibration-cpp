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
void MakeHistograms::MakeCalibrationHistograms()
{
    TChain *analysis = new TChain("AnalysisTree");
    std::vector<TString> run_list = inputs->file_list;

    // add all subruns to TChain
    for(auto i = 0; i < (int) run_list.size(); i++) {
        analysis->AddFile(Form("%s/%s", inputs->data_dir, run_list.at(i).Data()));
    }

    std::cout << "Found %i analysis trees " << analysis->GetNtrees()) << std::endl;
    TTree *tree = (TTree *) analysis->GetTree();

    Int_t num_entries = analysis->GetEntries();
    TGriffin * griffin = 0;
    if (analysis->FindBranch("TGriffin")) {
        analysis->SetBranchAddress("TGriffin", &griffin);
        std::cout << "Creating GRIFFIN Matrices..." << std::endl;
    } else {
        std::cout << "No TTigress or TGriffin Branch Found Things will go wrong" << std::endl;
    }

    std::cout << "Entries: " << num_entries << std::endl;

    /*
       char hname[20];
       for(int i = 0; i < num_cores; i++) {
        sprintf(hname,"hist%i_%i",source_count, i);
        hist[i] = new TH1F(hname, hname, num_bins, min_bin, max_bin);
       }

       cout << "Histograms created." << endl;

       //filling histograms with data from analysis root file
       for (int i = 0; i < num_entries - 1; i++) {
        analysis->GetEntry(i);
        if(tig) {
            for (int j = 0; j < tigress->GetMultiplicity(); j++) {
                TTigressHit *tigress_hit = tigress->GetTigressHit(j);
                hist[tigress_hit->GetArrayNumber()]->Fill(tigress_hit->GetCharge());
            }    //for
        } else {
            for (int j = 0; j < griffin->GetMultiplicity(); j++) {
                TGriffinHit *griffin_hit = griffin->GetGriffinHit(j);
                hist[griffin_hit->GetArrayNumber()-1]->Fill(griffin_hit->GetCharge());
            }    //for
        }
        if (i % 10000 == 0) {
            cout << setiosflags(ios::fixed) << "Entry " << i << " of " << num_entries << ", " << 100 * (i) / num_entries << "% complete" << "\r" << flush;
        }    //if
       }    //for
       cout << setiosflags(ios::fixed) << "Entry " << num_entries << " of " << num_entries << ", 100% complete" << "\r" << flush;
     */

} // end MakeCalibrationHistograms
