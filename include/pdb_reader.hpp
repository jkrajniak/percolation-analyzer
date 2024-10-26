#ifndef __PDB_READER_H__
#define __PDB_READER_H__

#include <string>

#include "molecular-graph.hpp"

int read_pdb(const std::string &pdb_file, mol::MolecularGraph &mol_graph);

#endif