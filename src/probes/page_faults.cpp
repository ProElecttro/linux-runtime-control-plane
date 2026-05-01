#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <unistd.h> // coz using sleep()
#include <signal.h>

#include <algorithm> 

using namespace std;

void throttle(int pid, useconds_t duration){
	if (kill(pid, SIGSTOP) == -1) {
    		perror("SIGSTOP failed");
		return;
	}

	usleep(duration);

	if (kill(pid, SIGCONT) == -1) {
    		perror("SIGCONT failed");
		kill(pid, SIGCONT); // try once more
	}
} 

int main(){
	int pid;
	cout << "Enter the PID : ";
	cin >> pid;

	long prev_major = 0, prev_minor = 0;
	bool first = true;

	int cooldown = 0;
	int count = 0;
	double avg_rate = 0.0;

	while(true){
		string path = "/proc/" + to_string(pid) + "/stat";
		ifstream file(path);

		if(!file.is_open()){
			cout << "failed to open the file, is PID correct?";
			break;
		}

		string line;
		getline(file, line);

		stringstream ss(line);
		string temp;
		vector<string> tokens;

		while(ss >> temp){
			tokens.push_back(temp);
		}

		if (tokens.size() < 12) {
    			cout << "Unexpected /proc format\n";
   			 break;
		}

		long minor_faults = 0, major_faults = 0;

		try {
    			minor_faults = stol(tokens[9]);
    			major_faults = stol(tokens[11]);
		} catch (...) {
			cout << "Parse error\n";
    			break;
		}

		if(cooldown > 0){
			cooldown--;
		}

		if(!first){
			long major_rate = max(0L, major_faults - prev_major);
			long minor_rate = max(0L, minor_faults - prev_minor);

			cout << "Major / sec : " << major_rate << " | Minor / sec : " << minor_rate << endl;

			avg_rate = (avg_rate * count + major_rate) / (count + 1);
			count++;

			if(count < 5){
				cout << "Warming Up..." << endl;
			}else{
				double pressure = (avg_rate > 0) ? major_rate / avg_rate : 1.0;


				if(pressure > 2 && cooldown == 0){ // cooldown only limits the strong actions
					cooldown = 3;
					cout << "STRONG throttle | pressure: " << pressure << endl;
					throttle(pid, 500000);
				}else if(pressure > 1.5){
					cout << "MEDIUM throttle | pressure: " << pressure << endl;
					throttle(pid, 200000);
				}else if(pressure > 1.2){
					cout << "LIGHT throttle | pressure: " << pressure << endl;
					throttle(pid, 100000);
				}
			}
		}

		prev_major = major_faults;
		prev_minor = minor_faults;
		first = false;

		sleep(1);
	}

	return 0;
}

