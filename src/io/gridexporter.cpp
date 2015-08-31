#include <png.h>
#include "io/gridexporter.h"
#include "logger/logger.h"
#include <zlib.h>


namespace fdl {

/**
 * Constructor.
 *
 */	
GridExporter::GridExporter(std::string prefix) : ExporterBase(prefix)
{
	
}

/**
 * Destructor.
 *
 */
GridExporter::~GridExporter()
{
	
}

/**
 * Writes a file for the input grid.
 *
 * @param grid a reference to the grid to pull numbers from
 *
 */

void GridExporter::write()
{
	// m_writeMutex.lock();

	m_writingFile = true;
	
	int numCells = m_grid->getNumberOfGridCells();
	int sizeX = m_grid->getGridSizeX();
	int sizeY = m_grid->getGridSizeY();
	int sizeZ = m_grid->getGridSizeZ();
	
	float* density = m_grid->getDensityArray();
	float* velx = m_grid->getVelocityXArray();
	float* vely = m_grid->getVelocityYArray();
	float* velz = m_grid->getVelocityZArray();
	float dx = m_grid->getVoxelSize();
	exportDensity(m_filenameCounter, m_filenamePrefix, density, velx, vely, velz, sizeX, sizeY, sizeZ, dx);

	m_filenameCounter++;
	
	m_writingFile = false;
	
	// m_writeMutex.unlock();
}


/**
 * Exports a 3D density field as a grid file, in which are writed in the first line the value of dx, in the second line the number of cells in each direction, and
 * below the matrix with densities and velocity components
 *
 * @param counter number extension for the output file
 * @param prefix string filename to write to
 * @param field array of float densities
 * @param xRes resolution of the grid in the x dimension
 * @param yRes resolution of the grid in the y dimension
 * @param zRes resolution of the grid in the z dimension
 *
 */
void GridExporter::exportDensity(int counter, std::string prefix, float* field, float *velx, float *vely, float *velz, int xSize, int ySize, int zSize, float dx)
{
	char buffer[256];
	sprintf(buffer,"%04i", counter);
	std::string number = std::string(buffer);

	std::string filenameGrid = prefix + number + std::string(".grid.gz");
	DEV() << "Writing " << filenameGrid;

	std::fstream fout;
	fout.open(filenameGrid.c_str(), std::ios::out);

    gzFile file;
	file = gzopen(filenameGrid.c_str(), "wb1");
	if (file == NULL) {
		ERROR() << " Couldn't write file " << filenameGrid << "!";
		return;
	}

	// write file
	gzprintf(file, "%f\n", dx);
	gzprintf(file, "%i %i %i \n \n", xSize, ySize, zSize);

	for(int i = 0; i < xSize * ySize * zSize; i++){
		gzprintf(file, "%f ", field[i]);
		
		if(m_isCancelled){
			gzclose(file);
			delete[] field;
			return;
		}
	}
	gzprintf(file, " \n \n");

	 for(int i = 0; i < (xSize+1) * (ySize+1) * (zSize+1); i++){
		gzprintf(file, "%f ", velx[i]);
		if(m_isCancelled){
			gzclose(file);
			delete[] velx;
			return;
		}
	}
	gzprintf(file, " \n \n");

	 for(int i = 0; i < (xSize+1) * (ySize+1) * (zSize+1); i++){
		gzprintf(file, "%f ", vely[i]);
		
		if(m_isCancelled){
			gzclose(file);
			delete[] vely;
			return;
		}
	}
	gzprintf(file, " \n \n");
	
	for(int i = 0; i < (xSize+1) * (ySize+1) * (zSize+1); i++){
		gzprintf(file, "%f ", velz[i]);
		
		if(m_isCancelled){
			gzclose(file);
			delete[] velz;
			return;
		}
	}
	
	gzclose(file);
	delete [] field;
	delete [] velx;
	delete [] vely;
	delete [] velz;
}
//Reads the file above (previously decompressed)
Grid* GridExporter::load(std::string filenameGrid)
{
	
	DEV() << "Reading " << filenameGrid;
	
	
	fdl::Grid* grid = NULL;

	std::ifstream file;
	file.open(filenameGrid.c_str());

	if (file == NULL) {
		ERROR() << " Couldn't read file " << filenameGrid << "!";
		return grid;
	}
	int xSize;
	int ySize;
	int zSize;
	float dx;

	file >> dx;

	DEV() << "Reading " << dx;

	file >> xSize;
	file >> ySize;
	file >> zSize;
	
	DEV() << "Reading " << xSize << ySize << zSize;
	
	//Create Grid, all data will be stored here
	grid = new fdl::Grid(xSize, ySize, zSize, dx);
	float temp;
	
	//read density:
	for (int i = 0; i < xSize * ySize * zSize; i++){
		file >> temp;
		grid->setDensity(i, temp);
	}
	
	//read x velocities:
	for (int i = 0; i < (xSize + 1) * (ySize + 1) * (zSize + 1); i++){
		file >> temp;
		grid->setVelocityX(i, temp);
	}
	
	//read y velocities:
	for (int i = 0; i < (xSize + 1) * (ySize + 1) * (zSize + 1); i++){
		file >> temp;
		grid->setVelocityY(i, temp);
	}

	//read z velocities:
	for (int i = 0; i < (xSize + 1) * (ySize + 1) * (zSize + 1); i++){
		file >> temp;
		grid->setVelocityZ(i, temp);
	}
	
	file.close();
	
	return grid;
}

	
	
}	// namespace fdl
