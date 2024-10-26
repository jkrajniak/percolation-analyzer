#include <iostream>
#include <cstdlib>
#include <cmath>
#include <random>

#include "molecular-graph.hpp"
#include "pdb_reader.hpp"
#include "psf_reader.hpp"

int main(int argc, char *argv[])
{
    std::cout << "PDB graph builder" << std::endl;

    // Get PDB file and PSF file from command line arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <pdb_file> <psf_file>" << std::endl;
        return 1;
    }

    const std::string pdb_file = argv[1];
    const std::string psf_file = argv[2];
    mol::MolecularGraph mol_graph;
    read_pdb(pdb_file, mol_graph);

    std::vector<std::pair<int, int>> bonds = read_psf_bonds(psf_file);
    for (const auto& bond : bonds) {
        mol_graph.add_bond(bond.first, bond.second);
    }
    std::cout << "Read PDB file: " << pdb_file << std::endl;
    std::cout << "Read PSF file: " << psf_file << std::endl;

    std::cout << "number of bonds: " << bonds.size() << std::endl;

    std::cout << "Convert molecular graph to percolation graph" << std::endl;
    // Retrieve the Percolation Graph associated with the molecular graph
    percolation::PercolationGraph percolation_graph = mol_graph.get_percolation_graph();

        // Now calculate the percolation info
    std::vector<percolation::ComponentInfo> percolation_data = percolation_graph.get_component_percolation_info();

    // Let us output the percolation info
    std::cout << "Results of the percolation analysis:" << std::endl;
    std::cout << "The system has a total of " << percolation_data.size() << " disjoint molecules" << std::endl;

    return 0;
}
