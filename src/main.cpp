#include "include/net.h"
#include "include/trainingData.h"
#include <iostream>
#include <fstream>

using namespace std;

void showVectorVals(string label, vector<double> &v){
	cout << label << " ";
	for (unsigned i = 0; i < v.size(); ++i)
		cout << v[i] << " ";

	cout << endl;
}

void saveData(vector<int> &x, vector<double> &y, string fileName){
	ofstream datFile;
	datFile.open(fileName, fstream::trunc);
	if(!datFile.is_open()){
		cerr << "Error encountered while opening file for saving data\n";
		exit(1);
	}
	for(unsigned i = 0; i < x.size(); ++i)
		datFile << x[i] << "\t" << y[i] << endl;
}

int main(){

	TrainingData trainData("/tmp/trainingData.txt");

	// eg: {3,2,1}
	std::vector<unsigned> topology;
	trainData.getTopology(topology);

	Net net(topology);

	std::vector<double> inputVals, targetVals, resultVals;
	int trainingPass = 0;

	vector<int> epData;
	vector<double> errData;

	while (!trainData.isEof()) {
        	++trainingPass;
        	cout << endl << "Pass " << trainingPass;

        	// Get new input data and feed it forward:
        	if (trainData.getNextInputs(inputVals) != topology[0])
            		break;

        	showVectorVals(": Inputs:", inputVals);
        	net.feedForward(inputVals);

        	net.getResults(resultVals);
        	showVectorVals("Outputs:", resultVals);

       	 	// Train the net what the outputs should have been:
        	trainData.getTargetOutputs(targetVals);
        	showVectorVals("Targets:", targetVals);
        	assert(targetVals.size() == topology.back());

        	net.backProp(targetVals);

        	// Report how well the training is working, average over recent samples:
        	cout << "Net recent average error: " << net.getRecentAverageError() << endl;

		epData.push_back(trainingPass);
		errData.push_back(net.getRecentAverageError());
	}

	saveData(epData, errData, "/tmp/saveData.dat");
    	cout << endl << "Done" << endl;

}