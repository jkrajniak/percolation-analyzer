#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

std::vector<std::pair<int, int>> read_psf_bonds(const std::string& psf_file) {
    std::vector<std::pair<int, int>> bonds;
    std::ifstream file(psf_file);
    
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open PSF file: " + psf_file);
    }

    std::string line;
    bool reading_bonds = false;

    while (std::getline(file, line)) {
        if (line.find("!NBOND") != std::string::npos) {
            reading_bonds = true;
            continue;
        }

        if (reading_bonds) {
            std::istringstream iss(line);
            std::vector<int> bond_data;
            int atom;

            while (iss >> atom) {
                bond_data.push_back(atom);
            }

            for (size_t i = 0; i < bond_data.size(); i += 2) {
                int atom_a = bond_data[i];
                int atom_b = bond_data[i + 1];
                if (atom_a < atom_b) {
                    bonds.emplace_back(atom_a, atom_b);
                }
            }

            if (bond_data.empty()) {
                break;
            }
        }
    }

    file.close();
    return bonds;
}
