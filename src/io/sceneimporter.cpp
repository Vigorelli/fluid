#include <string>

#include "io/sceneimporter.h"

/*
// Example scene file...
<scene>
	<settings dt="0.1" dx="0.01">
		<grid x="50" y="50" z="50" />
		<output-format>PNG</output-format>
		<output-name>density_export_</output-name>
		<solver tolerance="0.00001" maxIterations="100" />
	</settings>
	<source>
		<pos x="1.0" y="0.0" z="0.5" />
		<size w="1.0" h="1.0" d="1.0" />
	</source>
	<field />
    <log-level>2</log-level>
</scene>
*/

namespace fdl {

SceneImporter::SceneImporter() {}

SceneImporter::~SceneImporter() {}

void SceneImporter::load(const std::string& filename)
{
	read_xml(filename, pt);
}

void SceneImporter::save(const std::string& filename)
{
//	Write property tree to XML file
	write_xml(filename, pt);
}

std::vector<int> SceneImporter::GetGridDims(){
	std::vector<int> m_grid_dims(3);
	m_grid_dims[0] = GetGridX();
	m_grid_dims[1] = GetGridY();
	m_grid_dims[2] = GetGridZ();

	return m_grid_dims;
}

void SceneImporter::PutGridDims(std::vector<int> grid_dims){
	pt.put("scene.settings.grid.<xmlattr>.x", grid_dims[0]);
	pt.put("scene.settings.grid.<xmlattr>.y", grid_dims[1]);
	pt.put("scene.settings.grid.<xmlattr>.z", grid_dims[2]);
}

fdl::Vector3f SceneImporter::GetSourceSize(){
	fdl::Vector3f source_size;
	source_size[0]=pt.get<float>("scene.source.size.<xmlattr>.w");
	source_size[1]=pt.get<float>("scene.source.size.<xmlattr>.h");
	source_size[2]=pt.get<float>("scene.source.size.<xmlattr>.d");
	return source_size;
}

fdl::Vector3f SceneImporter::GetSourcePos(){
	fdl::Vector3f source_pos;
	source_pos[0]=pt.get<float>("scene.source.pos.<xmlattr>.x");
	source_pos[1]=pt.get<float>("scene.source.pos.<xmlattr>.y");
	source_pos[2]=pt.get<float>("scene.source.pos.<xmlattr>.z");
	return source_pos;
}

fdl::Vector3f SceneImporter::GetSourceForce(){
	fdl::Vector3f source_force;
	source_force[0]=pt.get<float>("scene.source.force.<xmlattr>.x");
	source_force[1]=pt.get<float>("scene.source.force.<xmlattr>.y");
	source_force[2]=pt.get<float>("scene.source.force.<xmlattr>.z");
	return source_force;
}

fdl::Vector3f SceneImporter::GetField(){
	fdl::Vector3f field;
	field[0]=pt.get<float>("scene.field.<xmlattr>.x");
	field[1]=pt.get<float>("scene.field.<xmlattr>.y");
	field[2]=pt.get<float>("scene.field.<xmlattr>.z");
	return field;
}

void SceneImporter::PutSourceSize(fdl::Vector3f source_size){
	pt.put("scene.source.size.<xmlattr>.w", source_size[0]);
	pt.put("scene.source.size.<xmlattr>.h", source_size[1]);
	pt.put("scene.source.size.<xmlattr>.d", source_size[2]);
	return;
}

void SceneImporter::PutSourcePos(fdl::Vector3f source_pos){
	pt.put("scene.source.pos.<xmlattr>.x", source_pos[0]);
	pt.put("scene.source.pos.<xmlattr>.y", source_pos[1]);
	pt.put("scene.source.pos.<xmlattr>.z", source_pos[2]);
	return;
}

void SceneImporter::PutSourceForce(fdl::Vector3f source_force){
	pt.put("scene.source.force.<xmlattr>.x", source_force[0]);
	pt.put("scene.source.force.<xmlattr>.y", source_force[1]);
	pt.put("scene.source.force.<xmlattr>.z", source_force[2]);
	return;
}

void SceneImporter::PutField(fdl::Vector3f field){
	pt.put("scene.field.<xmlattr>.x", field[0]);
	pt.put("scene.field.<xmlattr>.y", field[1]);
	pt.put("scene.field.<xmlattr>.z", field[2]);
	return;
}

} //namespace fdl
