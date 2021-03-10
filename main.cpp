#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <list>

using namespace std;

bool isEularian(vector<int> &eularianChecker,vector<int> &eularianChecker2, int numOfVertex)	// checks if the graph is eularian or not
{
	for(int i=0; i<numOfVertex; i++)					// traverse vectors due to their size
	{
		if(eularianChecker[i]!=eularianChecker2[i])		// checks if all vertices have same ingoing and outgoing vertices number or not
		{
			return false;								// returns false means the circuit is not eularian
		}
	}
	return true;										// returns true means the circuit is eularian 
}

void circuitMerge(list<int>&eularianCircuit, vector<int>&tour, int index)	// adds the partial tour to eularian circuit
{
	list<int>::iterator it= eularianCircuit.begin();	// treverse through eularian circuit till it comes the index where tour will be added
	advance(it, index);									// traverse forward
	eularianCircuit.erase(it);							// erases the vertex at the index to avoid double writing
	it=eularianCircuit.begin();							// sets iterator to begin to avoid confusion
	advance(it, index);									// traverse forward due to index of the vertex where tour will be added

	int size = tour.size();								// keeps size of the tour
	for(auto i{0}; i<size; i++)							// traverse through tour due to its size
	{
		int a = tour[i];								

		eularianCircuit.insert(it,a);					// adds elements of the tour to eularian circuit
	}
}

int unusedVertexFinder( list<int>&eularianCircuit, queue<int> *&graphTable, int &index)		// finds first non-used vertex in the eularian circuit
{
	int i=0;

	for(list<int>::iterator it= eularianCircuit.begin(); it!= eularianCircuit.end(); ++it)	// traverse through eularian circuit to find first non-used vertex
	{	
		if(!graphTable[*it].empty())	// checks whether the queue of the vertex is empty or not
		{		

			index=i;					// sets index as index of the first non-used vertex on the eularian circuit 
			
			return *it;					// returns this vertex
		}
		i++;
	}
	return -1;							// if there is no non-used vertex returns -1
}

int edgeCounter=0;	// counts total number of outgoing vertices 

int main(int argc, const char* argv[])
{

	int index=0;					// keeps the index of the vertex where tour will be added to eularian circuit
	
	ifstream read(argv[1]);			// reads input from input file
	ofstream write(argv[2]);		// writes output to output file
	
	int numOfVertex;				// total number of vertices
	read>>numOfVertex;

	queue<int> *graphTable= new queue<int>[numOfVertex];	// array of queues which keep outgoing vertices of vertices
	list<int> eularianCircuit;								// keeps eularian circuit
	vector<int> eularianChecker;							// keeps outgoing vertices numbers of vertices
	vector<int> eularianChecker2;							// keeps ingoing vertices numbers of vertices

	for(int i=0; i<numOfVertex; i++)		// for loop for initialize eularinChecker vectors
	{
		eularianChecker.push_back(0);
		eularianChecker2.push_back(0);
	}

	bool Eularian=true;						// keeps if the graph is eularian or not
	
	for(int i=0; i<numOfVertex; i++)		// reads all vertices respectively and adds their outgoing vertices to corresponding queues
	{
		int vertexId;						// id of the vertex
		read>>vertexId;	
		
		int numOfOutgoing=0;				// number of the outgoing vertices of the vertex
		read>>numOfOutgoing;
		
		edgeCounter+=numOfOutgoing;			// sums up number of outgoing vertices

		eularianChecker[i]=numOfOutgoing;	// sets eularian checker of the vertex as number of outgoing vertices

		for(int j=0; j<numOfOutgoing; j++)	// traverse outgoing vertices of the vertex
		{	

	 		int outgoingVertexID;			// id of the outgoing vertex
	 		read>>outgoingVertexID;		
			
			eularianChecker2[outgoingVertexID]++;	// increments the ingoing counter of the vertex
	 		graphTable[i].push(outgoingVertexID);	// adds outgoing vertex to current vertex's outgoing queue
	 		
	 		if(eularianChecker2[i]>eularianChecker[i])	// checks if there are a vertex which has larger ingoing vertex than outgoing vertex
	 		{

				Eularian=false;	// sets eularian as false
				i=numOfVertex;	// to exit from loops
				break;
	 		}		
	 	}
	}
	
	if(Eularian)	// if eularian is correct, checks other neccesities for graph to be eularian
	{
		Eularian=isEularian(eularianChecker, eularianChecker2, numOfVertex);	// sets eularian as is eularian 
	}

	if(Eularian)	// if is eularian is true it starts operations
	{	
		int firstVertex;									// takes first vertex from input file
		read>>firstVertex;
		
		eularianCircuit.push_back(firstVertex);				// pushes first vertex to eularian circuit
		int currentVertexID=firstVertex;					// sets current vertex as first vertex
		vector<int> tour;									// keeps tours

		while(eularianCircuit.size()<=edgeCounter)			// continues to process while size of the eularian circuit less than or equals to total number of outgoing vertices
		{

			tour.clear();									// clears the tour in every loop

			tour.push_back(currentVertexID);				// adds current vertex to the tour
			
			while(graphTable[currentVertexID].size()!=0)	// if current vertex has non-used vertices, continues
			{	

				tour.push_back(graphTable[currentVertexID].front());	// adds the first vertex of the current vertex's queue to the tour
				int a= currentVertexID;									// keeps current vertex id 				
				currentVertexID=graphTable[a].front();					// sets current vertex as the first vertex of the current vertex's queue 
				graphTable[a].pop();									// substracts the current vertex from queue

			}

			circuitMerge(eularianCircuit,tour, index);					// adds tour to eularian circuit
			currentVertexID= unusedVertexFinder(eularianCircuit, graphTable, index);	// sets current vertex as the vertex id coming from unused vertex finder method
			
			if(unusedVertexFinder(eularianCircuit, graphTable, index)==-1)				// if there is no unused vertex, method returns -1 and breaks
			{
				break;
			}
		}

	}else
	{
		write<<"no path";	// if circuit is not eularian writes no path to output file
	}

	for(list<int>::iterator it=eularianCircuit.begin(); it!= eularianCircuit.end(); it++)	// traverse eularian circuit and wirtes it to output file
	{
		write<< *it<<" ";
	}

	delete [] graphTable;

	return 0 ;
}
