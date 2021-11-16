#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "TFile.h"

class InputManager
{
public:
    InputManager(const char * hist_dir, const char * out_file = "out.root");
    ~InputManager(void);
    void GetFileList(int run_number);
    void ClearFileList();
    Int_t GetRunNumber();
    Int_t GetNumberOfSubruns();

    std::vector<TString> file_list;
    const char * data_dir;
    Int_t run_num;

private:

};

#endif
