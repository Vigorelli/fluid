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

    // Get filename and store it in m_file variable. Note that 
    // we specify a path to the value using notation where keys 
    // are separated with dots (different separator may be used 
    // if keys themselves contain dots). If debug.filename key is 
    // not found, exception is thrown.
//    m_file = pt.get<std::string>("debug.filename");
//	m_dt = pt.get<float>("scene.settings.<xmlattr>.dt");
//	std::cout << "sorrate m_dt " << m_dt << std::endl;

    // Get debug level and store it in m_level variable. This is
    // another version of get method: if debug.level key is not 
    // found, it will return default value (specified by second 
    // parameter) instead of throwing. Type of the value extracted 
    // is determined by type of second parameter, so we can simply 
    // write get(...) instead of get<int>(...).
//    m_level = pt.get("scene.log-level", 0);

    // Iterate over debug.modules section and store all found 
    // modules in m_modules set. get_child() function returns a 
    // reference to child at specified path; if there is no such 
    // child, it throws. Property tree iterator can be used in 
    // the same way as standard container iterator. Category 
    // is bidirectional_iterator.
    //BOOST_FOREACH(ptree::value_type &v, pt.get_child("debug.modules"))
    //    m_modules.insert(v.second.data());

//	return true;

}

void SceneImporter::save(const std::string& filename)
{
    // Put log filename in property tree
//    pt.put("debug.filename", m_file);

    // Put debug level in property tree
//    pt.put("debug.level", m_level);

    // Iterate over modules in set and put them in property 
    // tree. Note that put function places new key at the
    // end of list of keys. This is fine in most of the
    // situations. If you want to place item at some other
    // place (i.e. at front or somewhere in the middle),
    // this can be achieved using combination of insert 
    // and put_value functions
    //BOOST_FOREACH(const std::string &name, m_modules)
    //    pt.put("debug.modules.module", name, true);
    
    // Write property tree to XML file
    write_xml(filename, pt);

//	return true;
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
	source_size[0]=pt.get<int>("scene.source.size.<xmlattr>.w");
	source_size[1]=pt.get<int>("scene.source.size.<xmlattr>.h");
	source_size[2]=pt.get<int>("scene.source.size.<xmlattr>.d");
	return source_size;
}

fdl::Vector3f SceneImporter::GetSourcePos(){
	fdl::Vector3f source_pos;
	source_pos[0]=pt.get<int>("scene.source.pos.<xmlattr>.x");
	source_pos[1]=pt.get<int>("scene.source.pos.<xmlattr>.y");
	source_pos[2]=pt.get<int>("scene.source.pos.<xmlattr>.z");
	return source_pos;
}

fdl::Vector3f SceneImporter::GetSourceForce(){
	fdl::Vector3f source_force;
	source_force[0]=pt.get<int>("scene.source.force.<xmlattr>.x");
	source_force[1]=pt.get<int>("scene.source.force.<xmlattr>.y");
	source_force[2]=pt.get<int>("scene.source.force.<xmlattr>.z");
	return source_force;
}

fdl::Vector3f SceneImporter::GetField(){
	fdl::Vector3f field;
	field[0]=pt.get<int>("scene.field.<xmlattr>.x");
	field[1]=pt.get<int>("scene.field.<xmlattr>.y");
	field[2]=pt.get<int>("scene.field.<xmlattr>.z");
	return field;
}

} //namespace fdl
