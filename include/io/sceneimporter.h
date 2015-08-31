/**
 * @file sceneimporter.h
 * @version 0.1
 *
 * @section LICENSE
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
 */

#ifndef __FDL_SCENE_IMPORTER_H
#define __FDL_SCENE_IMPORTER_H

#include <string>
#include <set>
#include <exception>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "core/common.h"
#include "io/importerbase.h"
#include "core/vector.hpp"

namespace fdl {

	class SceneImporter : public ImporterBase {
	public:
		SceneImporter();
		~SceneImporter();
		virtual void load(const std::string &filename);
		virtual void save(const std::string &filename);

		/**
		 * Xml import functions
		 */
		double GetDt() {return pt.get<double>("scene.settings.<xmlattr>.dt");}
		double GetDx() {return pt.get<double>("scene.settings.<xmlattr>.dx");}
		int GetGridX() {return pt.get<int>("scene.settings.grid.<xmlattr>.x");}
		int GetGridY() {return pt.get<int>("scene.settings.grid.<xmlattr>.y");}
		int GetGridZ() {return pt.get<int>("scene.settings.grid.<xmlattr>.z");}
		std::vector<int> GetGridDims();
		bool GetPngOut() {return pt.get<bool>("scene.settings.png-out");}
		bool GetDf3Out() {return pt.get<bool>("scene.settings.df3-out");}
		bool GetGridIn() {return pt.get<bool>("scene.settings.grid-in");}
		std::string GetOutputPrefix() {return pt.get<std::string>("scene.settings.output-prefix");}
		std::string GetGridPrefix() {return pt.get<std::string>("scene.settings.grid-prefix");}
		std::string GetXmlOutputPrefix() {return pt.get<std::string>("scene.settings.xml-output-prefix");}
		std::string GetGridInputfile() {return pt.get<std::string>("scene.settings.grid-inputfile");}
		double GetCGTol() {return pt.get<double>("scene.settings.solver.<xmlattr>.tolerance");}
		int GetCGMaxIter() {return pt.get<int>("scene.settings.solver.<xmlattr>.maxIterations");}
		int GetMaxStep() {return pt.get<int>("scene.settings.max-step");}
		fdl::Vector3f GetSourceSize();
		fdl::Vector3f GetSourcePos();
		fdl::Vector3f GetSourceForce();
		fdl::Vector3f GetField();

		/**
		 * Xml export functions
		 */
		void PutDt(double dt) {pt.put("scene.settings.<xmlattr>.dt", dt);}
		void PutDx(double dx) {pt.put("scene.settings.<xmlattr>.dx", dx);}
		void PutGridDims(std::vector<int>);
		void PutPngOut(bool png_out) {pt.put("scene.settings.png-out", png_out);}
		void PutDf3Out(bool df3_out) {pt.put("scene.settings.df3-out", df3_out);}
		void PutGridIn(bool grid_in) {pt.put("scene.settings.grid-in", grid_in);}
		void PutOutputPrefix(std::string output_prefix) {pt.put("scene.settings.output-prefix", output_prefix);}
		void PutGridPrefix(std::string grid_prefix) {pt.put("scene.settings.grid-prefix", grid_prefix);}
		void PutXmlOutputPrefix(std::string xml_output_prefix) {pt.put("scene.settings.xml-output-prefix", xml_output_prefix);}
		void PutGridInputfile(std::string grid_inputfile) {pt.put("scene.settings.grid-inputfile", grid_inputfile);}
		void PutCGTol(double cg_tol) {pt.put("scene.settings.solver.<xmlattr>.tolerance", cg_tol);}
		void PutCGMaxIter(int cg_max_iter) {pt.put("scene.settings.solver.<xmlattr>.maxIterations", cg_max_iter);}
		void PutMaxStep(int max_step) {pt.put("scene.settings.max-step", max_step);}
		void PutSourceSize(fdl::Vector3f);
		void PutSourcePos(fdl::Vector3f);
		void PutSourceForce(fdl::Vector3f);
		void PutField(fdl::Vector3f);

	};

}	// namespace fdl

#endif	// __FDL_SCENE_IMPORTER_H
