#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <strings.h>
#include <vector>

// While these could -- and probably should -- be classes, we will just use structures for the sake of space and simplicity.
typedef struct
{
	// File paths can be stored as either absolute paths, obviously
	// beginning the entry with a /, or as a path relative to the
	// project file's location
	std::string path;

	// Dependencies are stored as a list of file paths seperated by pipe symbols (|).
	std::string dependencies;
} ProjectFile;

typedef struct
{
	std::string name;
	bool expanded;
	std::vector<ProjectFile> files;
} ProjectGroup;

// The actual Project class used by Paladin is much more complicated
// because it also has some properties for maintaining state while the
// program is running, but this structure has all of the data that is used
// for building and maintaining a project.
typedef struct
{
	std::map<std::string,std::string>	properties;

	std::vector<std::string>	localIncludes;
	std::vector<std::string>	systemIncludes;
	std::vector<ProjectGroup>	groups;
	std::vector<std::string>	libraries;
} PaladinProject;

int
ReadPaladinProject(const char* path, PaladinProject &project)
{
	// Create an Input File Stream for reading the file
	std::ifstream file;

	file.open(path, std::ifstream::in);
	if(!file.is_open())
	{
		std::cout << "Couldn't open the file: " << path << std::endl;
		return -1;
	}

	// Empty the project's data to make sure we're not building
	// upon existing baggage.
	project.properties.clear();
	project.localIncludes.clear();
	project.systemIncludes.clear();
	project.groups.clear();

	while(!file.eof())
	{
		std::string data;

		std::getline(file, data);

		if(data.empty())
			continue;

		size_t pos = data.find('=');

		if(pos == std::string::npos)
			continue;

		std::string key = data.substr(0,pos);
		std::string val = data.substr(pos+1);
		
		if(key.compare("GROUP") == 0)
		{
			ProjectGroup group;
			group.name = val;
			project.groups.push_back(group);
		}
		else if(key.compare("EXPANDGROUP") == 0)
		{
			if(!project.groups.empty())
				project.groups.back().expanded == strcasecmp(val.c_str(), "yes");
		}
		else if(key.compare("SOURCEFILE") == 0)
		{
			ProjectFile nFile;
			nFile.path = val;
			project.groups.back().files.push_back(nFile);
		}
		else if(key.compare("DEPENDENCY") == 0)
			project.groups.back().files.back().dependencies = val;
		else if(key.compare("LIBRARY") == 0)
			project.libraries.push_back(val);
		else if(key.compare("SYSTEMINCLUDE") == 0)
			project.systemIncludes.push_back(val);
		else if(key.compare("LOCALINCLUDE") == 0)
			project.localIncludes.push_back(val);
		else
			project.properties[key] = val;
	}

	return 0;
}

int
main(int argc, char* argv[])
{
	std::string makefilePath = "Makefile";
	PaladinProject project;
	const std::string defaultMakefile = "Makefile";

	if(argc >= 2)
	{
		if(ReadPaladinProject(argv[1], project))
			return 1;
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " (PLD FILE) [OUTPUT MAKEFILE]\n";
		return 1;
	}

	std::ofstream file;
	
	if(argc > 2)
		makefilePath = argv[2];
	
	file.open(makefilePath,std::ifstream::out);

	if(!file.is_open())
	{
		std::cout << "Couldn't open the file: "
			<< makefilePath
			<< std::endl;
		return 1;
	}

	std::cout << "Generating a Makefile out of "
		<< project.properties["NAME"]
		<< "..." << std::endl;

	file << "CC = gcc" << std::endl;
	file << "CCFLAGS = " << project.properties["CCEXTRA"];

	if(project.properties["CCDEBUG"] == "yes")
		file << " -g";

	file << std::endl;

	file << "LDFLAGS = " << project.properties["LDEXTRA"];

	for(std::vector<std::string>::iterator i = project.systemIncludes.begin();
			i != project.systemIncludes.end();
			i++)
		file << " -I" << *i;

	for(std::vector<std::string>::iterator i = project.localIncludes.begin();
			i != project.localIncludes.end();
			i++)
		file << " -I" << *i;
	for(std::vector<std::string>::iterator i = project.libraries.begin();
			i != project.libraries.end();
			i++)
	{
		size_t pos = i->find_last_of('/');
		std::string lib = i->substr(pos+1);
		pos = lib.find_last_of('.');
		lib = lib.substr(0,pos);
		lib = lib.substr(3);
		file << " -l" << lib;
	}
	file << std::endl << std::endl;

	file << "default: "
		<< project.properties["TARGETNAME"]
		<< std::endl << std::endl;

	file << project.properties["TARGETNAME"] << ": " << std::endl;
	
	file << "\t$(CC) $(CCFLAGS) -o \"" << project.properties["TARGETNAME"] << "\"";

	for(std::vector<ProjectGroup>::iterator i = project.groups.begin();
			i != project.groups.end();
			i++)
	{
		if(i->name == "Source Files"){
			for(std::vector<ProjectFile>::iterator x = i->files.begin();
					x != i->files.end();
					x++)
			{
				file << " " << x->path;
				size_t pos = 1;
				while(pos != std::string::npos)
				{
					size_t prevPos = pos-1;
					pos = x->dependencies.find_first_of('|');
					if(pos != std::string::npos)
						file << " " 
							<< x->dependencies.substr(prevPos,pos);
					else
						file << " "
							<< x->dependencies;
				}
			}
		}
	}

	file << " ${LDFLAGS}" << std::endl;
		
	file.close();
	return 0;

}
