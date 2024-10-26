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
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <name_of_system>" << std::endl;
        return 1;
    }

    const std::string name_of_system = argv[1];
    mol::MolecularGraph mol_graph;
    read_pdb(name_of_system+".pdb", mol_graph);

    std::vector<std::pair<int, int>> bonds = read_psf_bonds(name_of_system+".psf");
    for (const auto& bond : bonds) {
        mol_graph.add_bond(bond.first, bond.second);
    }
    std::cout << "Read PDB file: " << name_of_system+".pdb" << std::endl;
    std::cout << "Read PSF file: " << name_of_system+".psf" << std::endl;

    std::cout << "number of bonds: " << bonds.size() << std::endl;

    std::cout << "Convert molecular graph to percolation graph" << std::endl;
    // Retrieve the Percolation Graph associated with the molecular graph
    percolation::PercolationGraph percolation_graph = mol_graph.get_percolation_graph();

        // Now calculate the percolation info
    std::vector<percolation::ComponentInfo> percolation_data = percolation_graph.get_component_percolation_info();

    // Let us output the percolation info
    std::cout << "Results of the percolation analysis:" << std::endl;
    std::cout << "The system has a total of " << percolation_data.size() << " disjoint molecules" << std::endl;
    
    // Find the largest molecule
    size_t largest_molecule_index = 0;
    size_t largest_molecule_size = percolation_data[0].vertices.size();
    for (size_t c_index = 1; c_index < percolation_data.size(); c_index++) {
        if (percolation_data[c_index].vertices.size() > largest_molecule_size) {
            largest_molecule_index = c_index;
            largest_molecule_size = percolation_data[c_index].vertices.size();
        }
    }

    std::cout << "Largest molecule index: " << largest_molecule_index << std::endl;
    std::cout << "Largest molecule size: " << largest_molecule_size << std::endl;
    std::cout << "Largest molecule percolation dimension: " << percolation_data[largest_molecule_index].percolation_dim << std::endl;

    return 0;
}
