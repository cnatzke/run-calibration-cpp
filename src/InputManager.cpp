//////////////////////////////////////////////////////////////////////////////////
// Handles file input
//
// Author:        Connor Natzke (cnatzke@triumf.ca)
// Creation Date: 08-11-2021
// Last Update:  08-11-2021
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "TSystemDirectory.h"
#include "TList.h"
#include "TFile.h"
#include "TRunInfo.h"

#include "InputManager.h"

//////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////
InputManager::InputManager(const char * hist_dir, const char * out_file)
{
    data_dir = hist_dir;
    out_file = out_file;

    std::cout << "Found data directory: " << data_dir << std::endl;

} // end constructor

//////////////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////////////
InputManager::~InputManager(void)
{
} // end destructor

//////////////////////////////////////////////////////////////////////////////////
// Reads in list of files in directory
//////////////////////////////////////////////////////////////////////////////////
std::vector<TString> InputManager::GetFileList(int run_number)
{
    const char *ext = ".root";
    TString base_name = Form("analysis%i", run_number);

    TSystemDirectory dir(data_dir, data_dir);
    TList *files = dir.GetListOfFiles();
    files->Sort();
    if (files) {
        TSystemFile *file;
        TString fname;
        TIter next(files);
        while ((file = (TSystemFile*) next())) {
            fname = file->GetName();
            if (!file->IsDirectory() && fname.EndsWith(ext) && fname.Contains(base_name)) {
                file_list.push_back(fname);
            }
        }
    }

    std::sort(file_list.begin(), file_list.end());
    return file_list;

} // end GetFileList

//////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////
void InputManager::ClearFileList()
{
    file_list.clear();
} // end ClearFileList
