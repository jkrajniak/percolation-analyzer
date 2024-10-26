#include <string>
#include <fstream>
#include <iostream>

#include "vec.hpp"
#include "molecular-graph.hpp"
#include "pdb_reader.hpp"

/**
 * @brief Read a PDB file and convert it into a molecular graph
 * 
 * @param pdb_file 
 * @param mol_graph 
 * @return int 
 */
int read_pdb(const std::string &pdb_file, mol::MolecularGraph &mol_graph)
{
    std::ifstream file(pdb_file);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << pdb_file << std::endl;
        return 1;
    }

    std::string line;
    std::vector<vec<double>> atom_positions;
    std::vector<vec<double>> basis;
    bool basis_set = false;

    // Initialize basis with zeros
    basis.resize(3);
    for (size_t i = 0; i < 3; ++i) {
        basis[i] = vec<double>(0.0);
    }

    while (std::getline(file, line)) {
        if (line.substr(0, 6) == "CRYST1") {
            // Parse unit cell parameters
            basis[0][0] = std::stod(line.substr(6, 9));
            basis[1][1] = std::stod(line.substr(15, 9));
            basis[2][2] = std::stod(line.substr(24, 9));
            basis_set = true;
        } else if (line.substr(0, 4) == "ATOM") {
            // Parse atom positions
            double x = std::stod(line.substr(30, 8));
            double y = std::stod(line.substr(38, 8));
            double z = std::stod(line.substr(46, 8));
            atom_positions.push_back(vec<double>(x, y, z));
        }
    }

    file.close();

    if (!basis_set) {
        std::cerr << "Error: CRYST1 record not found in PDB file" << std::endl;
        return 1;
    }

    mol_graph.set_atom_count(atom_positions.size());
    mol_graph.set_basis(basis);

    std::cout << "Atom count: " << atom_positions.size() << std::endl;
    std::cout << "Basis:" << std::endl;
    for (size_t i = 0; i < basis.size(); ++i) {
        std::cout << basis[i].x << " " << basis[i].y << " " << basis[i].z << std::endl;
    }

    for (size_t i = 1; i < atom_positions.size()+1; i++) {
        int r = mol_graph.set_atom_position(i, atom_positions[i-1]);
        if (!r) {
            std::cerr << "Error: Failed to set atom position for atom " << i << std::endl;
            return 1;
        }
    }

    return 0;
}