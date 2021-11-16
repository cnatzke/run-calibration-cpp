#ifndef MAKE_HISTOGRAMS_H
#define MAKE_HISTOGRAMS_H

#include "InputManager.h"
#include "TH2.h"

class MakeHistograms
{
public:
    MakeHistograms(InputManager* inputs);
    ~MakeHistograms();
    TH2F* MakeCalibrationHistogram(const char*);

private:
    InputManager *inputs = NULL;
};

#endif
