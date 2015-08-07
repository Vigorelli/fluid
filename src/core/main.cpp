/**
 * @file main.cpp
 * @author Caleb Johnston
 * @version 0.1
 *
 * @section LICENSE
 *
 * FDL - Fluid Dynamics Library
 * Copyright (C) 2011 by Caleb Johnston
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 * Root class for all functionality
 */

#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cfloat>
#include <cmath>

#include "core/common.h"
//#include "core/glutrender.h"
#include "core/fluidsolver.h"
#include "core/particle.hpp"
#include "core/particlesystem.h"
#include "core/grid.hpp"
#include "io/pngexporter.h"
#include "io/df3exporter.h"
#include "io/gridexporter.h"
#include "io/sceneimporter.h"
#include "logger/logger.h"
#include "logger/stdiowriter.h"
#include "logger/syslogwriter.h"

#ifndef TARGET_VERSION_MAJOR
#define TARGET_VERSION_MAJOR 99
#endif

#ifndef TARGET_VERSION_MINOR
#define TARGET_VERSION_MINOR 99
#endif

/**
 * A helper function to simplify the CLI processing
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " ")); 
    return os;
}

/**
 * Main function, process inputs
 *
 * @return integer status of the program of range [0,1]
 */
int main(int argc, char **argv)
{	
	fdl::Logger out;
	// figure out how to automate this output...
	std::cout << "FDL version " << TARGET_VERSION_MAJOR << "." << TARGET_VERSION_MINOR << " of " << __DATE__ << " at " << __TIME__ << std::endl;
	std::cout << "The source code to FDL is covered by the GNU GPL." << std::endl;
	std::cout << "See the LICENSE file for the conditions of the license." << std::endl;
	
	fdl::SyslogWriter* syslogger = new fdl::SyslogWriter();
	fdl::StdOutWriter* stdlogger = new fdl::StdOutWriter();
	stdlogger->setFormat("%H:%M:%S");
	fdl::Logger::setIdentity("FDL");
	// fdl::Logger::RegisterWriter(syslogger);
	fdl::Logger::registerWriter(stdlogger);
	fdl::Logger::setLevel(fdl::Logger::WARN | fdl::Logger::ERROR);
	
	/**
	 * Default values (same order of example.xml)
	 */
	double dt = 0.1;				// time infinitesimal
	double dx = 0.01;				// space infinitesimal
	std::vector<int> grid_dims(3, 50);		// grid number of cells (n_x, n_y, n_z)
	bool png_out=true, df3_out=true, grid_in=false;	// activating io formats
	std::string output_prefix = "density_export_";	// output image filename prefix
	std::string grid_prefix = "grid_export_";	// output grid filename prefix
	std::string xml_output_prefix = "safepoint.xml";// output xml filename prefix
	std::string grid_inputfile;			// grid input filename
	double cg_tol = 1e-5;			 	// conjugate gradient tolerance
	int cg_max_iter = 100;				// conjugate gradient max iterations
	int max_step = 1000;				// max number of fluidsolver step

	/*SOURCE DEFAULT VALUES
	TODO: method for source in fluidsolver (read from xml)
	*/

	/**
	 * Scene importer to read xml input file (if needed) and to write xml output file
	 */
	fdl::SceneImporter* scene = new fdl::SceneImporter();

	/**
	 * Options for ./fdl
	 */
	try {
		int opt;
		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "produce help message")
			("input-file,I", po::value<std::string>(), "input scene file")
			("output-format,O", po::value<std::string>(), "output format")
			("output-name,N", po::value<std::string>(&output_prefix), "output file name PREFIX")
			("grid,G", po::value< std::vector<int> >(&grid_dims)->multitoken(), "[ X Y Z ]")
			("solver,L", po::value< std::vector<std::string> >(), "[ PCG | CG | Jacobi | ocl_cg | ocl_jacobi ]")
			("solver-tol", po::value<double>(&cg_tol), "linear solver convergence tolerance")
			("integration,A", po::value< std::vector<std::string> >(), "[ euler | verlet | runge-kutta2 | runge-kutta4 ]")
			("interp", po::value< std::vector<std::string> >(), "[ lerp | hat | gaussian | catmull-rom ]")
			("timestep,T", po::value<double>(), "timestep update.")
			("cell-width,D", po::value<double>(), "Width of a single cell.")
			("vorticity", po::value<int>(&opt)->default_value(0), "Apply vortex computations.")
			("wavelet,W", po::value<std::string>(), "[wavelet turbulence?]")
			("renderer,R", po::value<std::string>(), "[renderers]")
			("verbose,V", po::value<int>(&opt)->default_value(0), "logging level");
	
	        po::variables_map vm;        
	        po::store(po::parse_command_line(argc, argv, desc), vm);
	        po::notify(vm);

		if (vm.count("help")) {
			std::cout << "Usage: options_description [options]" << std::endl;
			std::cout << desc << std::endl;
			return 0;
		}
	
		if (vm.count("solver-tol")) {
			std::cout << "The linear solver residual is: " << cg_tol << std::endl;
		}

		if (vm.count("cell-width")) {
			dx = vm["cell-width"].as< double >();
			std::cout << "The cell width is: " << dx << std::endl;
		}

		if (vm.count("timestep")) {
			dt = vm["timestep"].as< double >();
			std::cout << "The update time step is: " << dt << std::endl;
		}

		if (vm.count("input-file")) {
			std::cout << "Input file name is: " << vm["input-file"].as<std::string>() << std::endl;
			std::string input_file = vm["input-file"].as<std::string>();
			scene->load(input_file);

			dt = scene->GetDt();
			png_out = scene->GetPngOut();
			df3_out = scene->GetDf3Out();
			grid_in = scene->GetGridIn();
			grid_prefix = scene->GetGridPrefix();
			xml_output_prefix = scene->GetXmlOutputPrefix();
			cg_tol = scene->GetCGTol();
			cg_max_iter = scene->GetCGMaxIter();
			max_step = scene->GetMaxStep();

			if(png_out || df3_out) {
				output_prefix = scene->GetOutputPrefix();
			}

			if(grid_in) {
				grid_inputfile = scene->GetGridInputfile();
			}
			else {
				dx = scene->GetDx();
				grid_dims = scene->GetGridDims();
			}
		}

		if (vm.count("output-name")) {
			std::cout << "Output name is: " << vm["output-name"].as<std::string>() << std::endl;
			output_prefix = vm["output-name"].as<std::string>();
		}

		if (vm.count("output-format")) {
			//TODO: be able to distinguish between png and df3, to make true the right flag
			std::cout << "Output format is: " << vm["output-format"].as<std::string>() << std::endl;
		}

		if (vm.count("verbose")) {
			int level = vm["verbose"].as<int>();
			std::string levelLabel = "";
			if(level <= 0){
				levelLabel = fdl::Logger::loggerLevelAsString(fdl::Logger::INFO);
				fdl::Logger::pushLevel(fdl::Logger::INFO);
			}
			if(level <= 1){
				levelLabel = fdl::Logger::loggerLevelAsString(fdl::Logger::DEV);
				fdl::Logger::pushLevel(fdl::Logger::DEV);
			}
			if(level <= 2){
				levelLabel = fdl::Logger::loggerLevelAsString(fdl::Logger::DEBUG);
				fdl::Logger::pushLevel(fdl::Logger::DEBUG);
			}

			INFO() << "Verbose logging enabled. Level is " << levelLabel << " (" << level << ")";
		}
	}

	catch(std::exception& e) {
		ERROR() << " * error: " << e.what();
		return 1;
	}
	catch(...) {
		ERROR() << "Exception of unknown type!";
	}
	
	//fdl::GlutRender::init(argc, argv);
	fdl::Grid* macGrid = new fdl::Grid(grid_dims[0], grid_dims[1], grid_dims[2], dx);
	//	static const char * pbrt = "grid_export_0015.grid";
	//	fdl::Grid* macGrid = fdl::GridExporter::read(pbrt);
	
	fdl::FluidSolver* fs = new fdl::FluidSolver(macGrid);
	fs->setCGTolerance((float)cg_tol);
	fs->setCGMaxIter(cg_max_iter);
	fdl::PngExporter* pngOut = new fdl::PngExporter(output_prefix);
	fdl::GridExporter* gridOut = new fdl::GridExporter(grid_prefix);
	fdl::Df3Exporter* df3Out = new fdl::Df3Exporter(output_prefix);
	//fdl::ParticleSystem* ps = new fdl::ParticleSystem();
	//ps->init(macGrid);
	//	std::ofstream myfile;
	//	myfile.open("average_densities_cg2.txt");


//	float average;
	
	for(int count=0; count<max_step; count++){
		//		fs->step(dt);
		//	ps->step(dt);
		pngOut->start(*macGrid);
		gridOut->start(*macGrid);
		df3Out->start(*macGrid);
		scene->save(xml_output_prefix);
		fs->step(dt);
		//		average = 0;
		/*		float* densityvector = macGrid->getDensityArray();
		 for(int i=0; i<macGrid->getNumberOfGridCells(); i++){
			average += 	densityvector[i];
		 }
		 myfile<< count <<"	" << average << "\n";
		 count++;
		 delete [] densityvector;*/
	}
	//	myfile.close();

	delete scene;

	return 0;
}
