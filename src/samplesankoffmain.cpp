/*
 * samplesankoffmain.cpp
 *
 *  Created on: 9-nov-2018
 *      Author: P. Sashittal
 */

#include "utils.h"
#include "sankoff.h"
#include "basetree.h"
#include <fstream>
#include <lemon/arg_parser.h>

int main(int argc, char** argv)
{
  
    typedef Digraph::NodeMap<int> IntNodeMap;
    typedef Digraph::NodeMap<double> DoubleNodeMap;
    typedef Digraph::ArcMap<int> IntArcMap;
  
    std::string host_filename, ptree_filename, output_prefix;
    int rootLabel = 0;
    int nrUnsampledHosts = 0;
    bool nonbinaryTree = false;
    int sampleLimit = 11000;
  
    lemon::ArgParser ap(argc, argv);
    ap.refOption("r", "Root label (default: 0)", rootLabel);
    ap.refOption("u", "Number of unsampled hosts (default: 0)", nrUnsampledHosts);
    ap.refOption("b", "is the tree non binary (default: false)", nonbinaryTree);
    ap.refOption("l", "Number of samples (default: 11000)", sampleLimit);
    ap.other("<host>");
    ap.other("<ptree>");
    ap.other("<output_prefix>");
    ap.parse();

  
    if (ap.files().size() != 3)
    {
        std::cerr << "Error1: expected <host> <ptree> <output_prefix>" << std::endl;
        return 1;
    }

    host_filename = ap.files()[0];
    ptree_filename = ap.files()[1];
    output_prefix = ap.files()[2];
  
    std::ifstream host_file(host_filename.c_str());
    if (!host_file.good())
    {
        std::cerr << "Error2: failed opening '" << host_filename << "' for reading" << std::endl;
        return 1;
    }
  
    std::ifstream ptree_file(ptree_filename.c_str());
    if (!ptree_file.good())
    {
        std::cerr << "Error3: failed opening '" << ptree_filename << "' for reading" << std::endl;
        return 1;
    }

    BaseTree B;
    try
    {
        BaseTree::readInputWithHosts(host_file, ptree_file, nrUnsampledHosts, !nonbinaryTree, B);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << "Error4: " << e.what() << std::endl;
        return 1;
    }
    host_file.close();
    ptree_file.close();
  
    Sankoff solver(B, rootLabel - 1);
    solver.init();
    solver.solve(std::cout, true, 0);
	
	IntPairSetIntMap solutionMap;
    char buf[1024];
    for (int count = 0; count < sampleLimit; ++count)
    {
        solver.getSample();
		const IntNodeMap& ell = solver.getSolMap();
		
		IntPairSet sampleSolution;
		for (NodeIt vi(B.tree()); vi != lemon::INVALID; ++vi)
		{
			int i = B.getIndex(vi);
			
			sampleSolution.insert(std::make_pair(i, ell[vi]));
		}
		
		if (!solutionMap.count(sampleSolution))
		{
			solutionMap[sampleSolution] = 1;
		}
		else
		{
			++solutionMap[sampleSolution];
		}
    }
	
	int idx = 0;
	
	for (const auto& sol_count : solutionMap )
	{
		snprintf(buf, 1024, "%sidx%d_count%d.out", output_prefix.c_str(), idx, sol_count.second);
	
		std::ofstream outSample(buf);

		IntNodeMap ell(B.tree(), -1);
		for (const auto& i_s : sol_count.first)
		{
			Node vi = B.getNode(i_s.first);
			ell[vi] = i_s.second;
		}
		
		B.writePtree(outSample, ell);
		
		outSample.close();

		++idx;
	}
	
	
    return 0;
}
