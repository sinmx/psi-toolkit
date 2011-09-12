#ifndef SERVER_RUNNER_HDR
#define SERVER_RUNNER_HDR

#include <boost/program_options.hpp>

#include "index_site.hpp"
#include "pipe_site.hpp"

class ServerRunner {

public:
    ServerRunner(int argc, char* argv[]);
    int run();

private:
	PsiServer* psiSever_;

	boost::program_options::variables_map options_;
	boost::program_options::options_description optionsDescription_;
	void setOptionsDescription();
	boost::program_options::variables_map parseOptions(int argc, char * argv[]);

	int executeOptions();
};

#endif