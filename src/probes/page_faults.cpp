#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <unistd.h> // coz using sleep()
#include <signal.h>

using namespace std;

int main(){
	int pid;
	cout << "Enter the PID : ";
	cin >> pid;

	long prev_major, prev_minor;
	bool first = true;

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

		if(!first){
			long major_rate = major_faults - prev_major;
			long minor_rate = minor_faults - prev_minor;

			cout << "Major / sec : " << major_rate << " | Minor / sec : " << minor_rate << endl;

			if(major_rate > 500){
				cout << "HIGH MEMORY PRESSURE -> throttling process" << endl;
				kill(pid, SIGSTOP); //-> pause the process for some time. (coz creating high memory pressure)
				sleep(1);
				kill(pid, SIGCONT);
			} 
		}

		prev_major = major_faults;
		prev_minor = minor_faults;
		first = false;

		sleep(1);
	}

	return 0;
}
