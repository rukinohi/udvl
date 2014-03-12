#include "sat.h"

#include <stdexcept>
#include <algorithm>
#include <iterator>

namespace sat {

DimacsWriter::DimacsWriter(const std::string &name, std::ios_base::open_mode mode)
{
	m_filename = name;
	m_f.open(m_filename);
	if (!m_f.good())
		throw std::runtime_error("Failed to open output file");
}

void DimacsWriter::writeLiteral(int lit)
{
	m_f << lit << " ";
}

void DimacsWriter::finishClause()
{
	m_f << "0" << std::endl;
}

void DimacsWriter::writeClause(const std::vector<int> &clause)
{
	for(auto lit : clause) {
		writeLiteral(lit);
	}
	finishClause();
}

void DimacsWriter::writeClause(const std::initializer_list<int> &clause)
{
	for(auto lit : clause) {
		writeLiteral(lit);
	}
	finishClause();
}

void DimacsWriter::writeImpl(int left, int right)
{
	writeClause({-left, right});
}

bool DimacsWriter::closed()
{
	return !m_f.is_open();
}

void DimacsWriter::close()
{
	m_f.close();
}



SatSolver::SatSolver(const std::string &solverPath)
{
	// TODO solverPath
}

std::string SatSolver::getSolverPath()
{
	// TODO
	return "minisat";
}

SatSolver::Result SatSolver::solve(const std::string &theoryFile, const std::string &outputFile)
{
	system((getSolverPath() + " " + theoryFile + " " + outputFile + " >solver_out.txt").c_str());

	Result res;

	std::ifstream fi;
	fi.open(outputFile);
	if (!fi.good()) {
		return res;
	}

	std::string sat;
	fi >> sat;

	if (sat == "SAT") {
		res.sat = true;
		std::copy(std::istream_iterator<int>(fi),
				std::istream_iterator<int>(),
				std::back_inserter<std::vector<int> >(res.vars));
		// get rid of the 0 at the end
		if (res.vars.back() == 0) {
			res.vars.pop_back();
		}
	}

	return res;
}

SatSolver::Result SatSolver::solve(DimacsWriter *theoryWriter, const std::string &outputFile)
{
	if (!theoryWriter->closed())
	{
		theoryWriter->close();
	}
	return solve(theoryWriter->filename(), outputFile);
}

} //namespace sat
