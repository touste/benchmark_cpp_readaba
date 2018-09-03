#include <string>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <algorithm>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <time.h>

using namespace std; 


int main() {
// Parse an abaqus inp file and store the points, elements and hinges into a simit database
// Inspired by the abaqus inp parser of libMesh

	// Initialize timer
	clock_t tStart = clock();

    string fname = string("100x100x2.inp");

	ifstream infile(fname); // The input file

    vector<vector<float>> ref_nodes;
    vector<vector<float>> ref_els;
	
	string line;
	char c;

	// Read line by line
	while (getline(infile, line)) {
		string upper(line);
		transform(upper.begin(), upper.end(), upper.begin(), ::toupper); // Convert to uppercase
		
		// 1.) Find nodes
		if (upper.find("*NODE") != string::npos)
		  {
			// Read until the next section or end of file  
			while (infile.peek() != '*' && infile.peek() != EOF) 
			{
				getline(infile, line); // Get current line
				line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end()); // Remove empty spaces
				stringstream ss(line); // Make a stream from the line
				int abaqus_node_id;
				float x1, y1, z1;
				ss >> abaqus_node_id >> c >> x1 >> c >> y1 >> c >> z1; // We assume nodeID, x, y, z
				
				ref_nodes.push_back({x1, y1, z1}); // Add a point to the vector containing the simit nodes
			}
		  }

		// 2.) Find elements
		else if (upper.find("*ELEMENT,") != string::npos)
		  {
			// Read until the next section or end of file  
			while (infile.peek() != '*' && infile.peek() != EOF) 
			{
				getline(infile, line); // Get current line
				line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end()); // Remove empty spaces
				stringstream ss(line); // Make a stream from the line
				int abaqus_el_id;
				float n1, n2, n3;
				ss >> abaqus_el_id >> c >> n1 >> c >> n2 >> c >> n3; // We assume elementID, n1, n2, n3
				
				ref_els.push_back({n1, n2, n3}); // Add a point to the vector containing the simit nodes
				
			}
			
		  }
	}

	infile.close();

	// Print total time taken
	printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

	
    return 0;

}