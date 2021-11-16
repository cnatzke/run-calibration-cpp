#ifndef MAKE_HISTOGRAMS_H
#define MAKE_HISTOGRAMS_H

#include "InputManager.h"

class MakeHistograms
{
public:
    MakeHistograms(InputManager* inputs);
    ~MakeHistograms();
    void MakeCalibrationHistograms();

private:

    InputManager *inputs = NULL;
};

#endif
