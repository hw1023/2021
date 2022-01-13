#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <bitset>

using namespace std;

int main (int argc, char** argv) {
	ifstream config;
	config.open(argv[1]);

	int m;
	config >> m; 
 
	config.close();

	ofstream out;
	string out_file_name = string(argv[2]) + ".out";
	out.open(out_file_name.c_str());
	
	ifstream trace;
	trace.open(argv[2]);

	int Counter = pow(2, m); //counter starts at 11
	bitset<2> bitCounter[Counter]; //bitCounter.resize(Counter);
	for(int i=0;i<Counter;i++){
		bitCounter[i] = bitset<2>(11);
	}
	
	
	bitset<32> address;

	while (!trace.eof()) {
		unsigned long pc; bool taken;
		trace >> std::hex >> pc >> taken;
		bool prediction;
		prediction = true; // taken=1, not taken=0
		address = bitset<32>(pc);
		//cout<<pc<<' '<<taken<<'\n';
		int j=0;
		if(taken){
			for(int i=0;i<m;i++){
				j+=(address[i]*pow(2,i));
			}
			if(bitCounter[j]==11){	//Strongly taken
				prediction=true;
				bitCounter[j]=11;
			}else if(bitCounter[j]==10){
				prediction=true;
				bitCounter[j]=11;
			}else if(bitCounter[j]==01){
				prediction=false;
				bitCounter[j]=11;
			}else if(bitCounter[j]==00){
				prediction=false;
				bitCounter[j]=00;
			}
		}else{
			for(int i=0;i<m;i++){
				j+=(address[i]*pow(2,i));
			}
			if(bitCounter[j]==11){	//Strongly taken
				prediction=true;
				bitCounter[j]=10;
			}else if(bitCounter[j]==10){
				prediction=true;
				bitCounter[j]=00;
			}else if(bitCounter[j]==01){
				prediction=false;
				bitCounter[j]=00;
			}else if(bitCounter[j]==00){
				prediction=false;
				bitCounter[j]=00;
			}
		}
		if(!trace.eof()){
			out << prediction << endl;
		}
		//out << prediction << endl;
	}
	 
	trace.close();	
	out.close();
	return 0;
}
