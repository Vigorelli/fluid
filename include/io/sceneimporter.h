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

namespace fdl {

	class SceneImporter : public ImporterBase {
	public:
		SceneImporter();
		~SceneImporter();
		virtual void load(const std::string &filename);
		virtual void save(const std::string &filename);

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

	private:
		std::string m_file;
		int m_level;
		float m_dt;
		std::string m_name;
	};

}	// namespace fdl

#endif	// __FDL_SCENE_IMPORTER_H
