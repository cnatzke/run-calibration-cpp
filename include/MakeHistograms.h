#ifndef MAKE_HISTOGRAMS_H
#define MAKE_HISTOGRAMS_H

#include "InputManager.h"
#include "TH1.h"

class MakeHistograms
{
public:
    MakeHistograms(InputManager* inputs);
    ~MakeHistograms();
    void MakeCalibrationHistograms(const char*);

private:
    InputManager *inputs = NULL;
    Int_t num_crystals = 64;
    TH1F *histograms[64];

};

#endif
