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

	int cooldown = 0;
	int count = 0;
	double avg_rate = 0.0;

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

		if(cooldown > 0){
			cooldown--;
		}

		if(!first){
			long major_rate = major_faults - prev_major;
			long minor_rate = minor_faults - prev_minor;

			cout << "Major / sec : " << major_rate << " | Minor / sec : " << minor_rate << endl;

			if(major_rate > avg_rate * 1.5 && cooldown == 0){
				cout << "HIGH MEMORY PRESSURE -> throttling process" << endl;
				kill(pid, SIGSTOP); //-> pause the process for some time. (coz creating high memory pressure)
				usleep(200000); // 200 milliseconds
				kill(pid, SIGCONT);

				cooldown = 3;
			} 

			avg_rate = (avg_rate * count + major_rate) / (count + 1);
			count++;
		}

		prev_major = major_faults;
		prev_minor = minor_faults;
		first = false;

		sleep(1);
	}

	return 0;
}
