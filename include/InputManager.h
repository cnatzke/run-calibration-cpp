#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "TFile.h"

class InputManager
{
public:
    InputManager(const char * hist_dir, const char * out_file = "out.root");
    ~InputManager(void);
    std::vector<TString> GetFileList(int run_number);
    void ClearFileList();

    std::vector<TString> file_list;
    const char * data_dir;
    const char * out_file;

private:

};

#endif
