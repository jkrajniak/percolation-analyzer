#ifndef __PSF_READER_H__
#define __PSF_READER_H__

#include <string>
#include <vector>
#include <utility>

std::vector<std::pair<int, int>> read_psf_bonds(const std::string& psf_file);

#endif
