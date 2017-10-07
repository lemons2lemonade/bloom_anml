#include "automata.h"
#include "bloom_automata.h"
#include <vector>
#include <random>
#include <iostream>
#include <array>
#include <stack>
#include <chrono>
#define FROM_INPUT_STRING false

using namespace std;

void createSymbolArray(uint32_t length, vector<unsigned char> &randomChars) 
{
  
  //randomChars.resize(length);
  for(uint32_t i = 0; i < length; i++) {
    randomChars.push_back( (unsigned char)i);
  }

}

void randomizeSymbolArray(uint32_t numSymbols, vector<unsigned char> &orderedArray) {

  vector<unsigned char> temp;
  for(unsigned char c : orderedArray) {
    temp.push_back(c);
  }

  for(uint32_t i = 0; i < numSymbols; i++) {
    uint32_t randIndex = ((uint32_t)rand()) % temp.size();
    orderedArray[i] = temp[randIndex];
    if(orderedArray[i] > 255)
      cout << "WRONG!!!: " << i << endl;
    temp.erase(temp.begin() + randIndex);
  }
}

/*
TODO: 
1. Be able to create number of MCs based on number automata chips desired 
    - writing a new function to do this
2. Simulate all single-core CPU bloom experiments inside main
    - create this code inside main and use couts 
    A. Implement timing code to store the time taken for each experiment


 */

//TODO #1
void createBloomSetup(Automata ap, uint32_t numStates, uint32_t numChips, uint32_t numSymbols){

  //auto start = std::chrono::high_resolution_clock::now();
  uint32_t numBits = 17728 * numChips;
  uint32_t numMCs = numBits;
  //Automata ap;
  uint32_t mcId = 0;
  uint32_t count = 0;
  //generate all reporting nodes beforehand so that they exist
  // when referenced for edge creating in the double-for loop
  for (uint32_t j=0; j < numMCs; j++){
    for (uint32_t k = 0; k < numStates; k++){
      string starId = "S_" + to_string(k) + "_MC_"+to_string(mcId);
      if (j == 0 && k == 0){
	STE * ste = new STE(starId,"*","start-of-data");
	ap.rawAddSTE(ste);
	ste->setReporting(true);
	count++;
      }

      else if(count <= 2000){
	STE * ste = new STE(starId, "*", "none");
	ap.rawAddSTE(ste);
	ste->setReporting(true);
	count++;
      }
      else{
	STE * ste = new STE(starId, "*", "none");
	ap.rawAddSTE(ste);
      }
      cout << "Node ID Created: " << starId << endl;
    }
    mcId++;
  }
    
  //For each Markov Chain Set
  for(uint32_t a=0; a < numMCs; a++){
      
    // For each Markov chain "star" state
    for(uint32_t startState = 0; startState < numStates; startState++) {

      string starNodeId = "S_" + to_string(startState) + "_MC_"+to_string(a);
      //
      vector<unsigned char> alphArray;
      vector<vector<unsigned char>> symbolSetArray;

      // create contiguous symbols in array
      vector<unsigned char> randoms;
      createSymbolArray(numSymbols, randoms);
      randomizeSymbolArray(numSymbols, randoms);
	
      // Generate transition STEs
      for(uint32_t transitionState = 0; transitionState < numStates; transitionState++) {

	// T_#_#"
	string pathId = "T_" + to_string(startState) + "_" + to_string(transitionState) +
	  "_MC_"+to_string(a);
	STE * ste = new STE(pathId, "[]", "none");
	    
	// Add symbols to STE
	uint32_t offset = numSymbols/numStates;
	uint32_t startIndex = offset * transitionState;
	for(uint32_t index = startIndex; index < startIndex + offset; index++) {
	  ste->addSymbolToSymbolSet(randoms[index]);
	}
	  
	// Add STE to automata
	ap.rawAddSTE(ste);

	//Add outgoing edge paths
	string pathToOtherNode = "S_"+to_string(transitionState) + "_MC_"+to_string(a);
	//cout <<"Node ID Referenced: " <<pathToOtherNode << endl;
	cout << "MC: "+to_string(a)+ " , Path: "+ pathId + " -> "+pathToOtherNode << endl;
	ap.addEdge(ap.getElement(starNodeId), ap.getElement(pathId));
	ap.addEdge(ap.getElement(pathId), ap.getElement(pathToOtherNode));

      }
   
    }

  }
  /*
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  return elapsed.count();
  */
}

		 
int main(int argc, char* argv[]) {
  
  cout << "Starting Main..." <<endl;

  uint32_t numSymbols = 256; //Alphabet Size


  //Command line arguments
  /*
  int numChips = atoi(argv[1]); //let user set num of chips
  int numStates = atoi(argv[2]); //let user set num states per MC
  static_cast<uint32_t>(numStates);
  static_cast<uint32_t>(numChips);
  */
  uint32_t mcId = 0;


  //TODO #2, Experiments HERE:
  //Method Signature: createBloomSetup(Automata ap, uint32_t numStates, uint32_t numChips, uint32_t numSymbols)

  /*
  Automata ap1;
  createBloomSetup(ap1, 2, 1, numSymbols);
  ap1.automataToANMLFile("bloom_automata1.anml");
  
  Automata ap2;
  createBloomSetup(ap2, 2, 2, numSymbols);
  ap2.automataToANMLFile("bloom_automata2.anml");

  Automata ap3;
  createBloomSetup(ap3, 2, 3, numSymbols);
  ap3.automataToANMLFile("bloom_automata3.anml");

  Automata ap4;
  createBloomSetup(ap4, 2, 4, numSymbols);
  ap4.automataToANMLFile("bloom_automata4.anml");

  Automata ap5;
  createBloomSetup(ap5, 2, 5, numSymbols);
  ap5.automataToANMLFile("bloom_automata5.anml");

  Automata ap6;
  createBloomSetup(ap6, 2, 6, numSymbols);
  ap6.automataToANMLFile("bloom_automata6.anml");

  Automata ap7;
  createBloomSetup(ap7,2,7, numSymbols);
  ap7.automataToANMLFile("bloom_automata7.anml");

  
  Automata ap8;
  createBloomSetup(ap8,2,8, numSymbols);
  ap8.automataToANMLFile("bloom_automata8.anml");

  Automata ap9;
  createBloomSetup(ap9,2,9, numSymbols);
  ap9.automataToANMLFile("bloom_automata9.anml");

  Automata ap10;
  createBloomSetup(ap10,2,10, numSymbols);
  ap10.automataToANMLFile("bloom_automata10.anml");

  */
  Automata ap11;
  createBloomSetup(ap11,2,11, numSymbols);
  ap11.automataToANMLFile("bloom_automata11.anml");

  Automata ap12;
  createBloomSetup(ap12,2,12, numSymbols);
  ap12.automataToANMLFile("bloom_automata12.anml");

  Automata ap13;
  createBloomSetup(ap13,2,13, numSymbols);
  ap13.automataToANMLFile("bloom_automata13.anml");

  Automata ap14;
  createBloomSetup(ap14,2,14, numSymbols);
  ap14.automataToANMLFile("bloom_automata14.anml");

  Automata ap15;
  createBloomSetup(ap15,2,15, numSymbols);
  ap15.automataToANMLFile("bloom_automata15.anml");

  Automata ap16;
  createBloomSetup(ap16,2,16, numSymbols);
  ap16.automataToANMLFile("bloom_automata16.anml");

  
  Automata ap17;
  createBloomSetup(ap17,2,17, numSymbols);
  ap17.automataToANMLFile("bloom_automata17.anml");

  
  Automata ap18;
  createBloomSetup(ap18,2,18, numSymbols);
  ap18.automataToANMLFile("bloom_automata18.anml");
  

  /*
  //generate all reporting nodes beforehand so that they exist
  // when referenced for edge creating in the double-for loop
  for (uint32_t j=0; j < numMCs; j++){
    for (uint32_t k = 0; k < numStates; k++){
      string starId = "S_" + to_string(k) + "_MC_"+to_string(mcId);
      if (j == 0 && k == 0){
	STE * ste = new STE(starId,"*","start-of-data");
	ap.rawAddSTE(ste);
	ste->setReporting(true);
      }

      else{
	STE * ste = new STE(starId, "*", "none");
	ap.rawAddSTE(ste);
	ste->setReporting(true);
      }
      cout << "Node ID Created: " << starId << endl;
    }
    mcId++;
  }
    
  //For each Markov Chain Set
  for(uint32_t a=0; a < numMCs; a++){
      
    // For each Markov chain "star" state
    for(uint32_t startState = 0; startState < numStates; startState++) {

      string starNodeId = "S_" + to_string(startState) + "_MC_"+to_string(a);
      //
      vector<unsigned char> alphArray;
      vector<vector<unsigned char>> symbolSetArray;

      // create contiguous symbols in array
      vector<unsigned char> randoms;
      createSymbolArray(numSymbols, randoms);
      randomizeSymbolArray(numSymbols, randoms);
	
      // Generate transition STEs
      for(uint32_t transitionState = 0; transitionState < numStates; transitionState++) {

	// T_#_#"
	string pathId = "T_" + to_string(startState) + "_" + to_string(transitionState) + "_MC_"+to_string(a);
	STE * ste = new STE(pathId, "[]", "none");
	    
	// Add symbols to STE
	uint32_t offset = numSymbols/numStates;
	uint32_t startIndex = offset * transitionState;
	for(uint32_t index = startIndex; index < startIndex + offset; index++) {
	  ste->addSymbolToSymbolSet(randoms[index]);
	}
	  
	// Add STE to automata
	ap.rawAddSTE(ste);

	//Add outgoing edge paths
	string pathToOtherNode = "S_"+to_string(transitionState) + "_MC_"+to_string(a);
	//cout <<"Node ID Referenced: " <<pathToOtherNode << endl;
	cout << "MC: "+to_string(a)+ " , Path: "+ pathId + " -> "+pathToOtherNode << endl;
	ap.addEdge(ap.getElement(starNodeId), ap.getElement(pathId));
	ap.addEdge(ap.getElement(pathId), ap.getElement(pathToOtherNode));

      }
   
    }

  }

  */
}
