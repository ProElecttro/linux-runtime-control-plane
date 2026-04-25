#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <unistd.h> // coz using sleep()

using namespace std;

int main(){
	int pid;
	cout << "Enter the PID : ";
	cin >> pid;

	while(true){
		string path = "/proc/" + to_string(pid) + "/stat";
		ifstream file(path);

		if(!file.is_open()){
			cout << "failed to open the file, is PID correct?";
			return 1;
		}

		string line;
		getline(file, line);

		stringstream ss(line);
		string temp;
		vector<string> tokens;

		while(ss >> temp){
			tokens.push_back(temp);
		}

		long major_faults = stol(tokens[9]);
		long minor_faults = stol(tokens[11]);

		cout << "major faults : " << major_faults << " | minor faults : " << minor_faults <<endl;


		sleep(1);
	}

	return 0;
}
