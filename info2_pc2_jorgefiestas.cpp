#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

string calculate_stats();
string calculate_mem();

int main(){
	while(true){
		cout<<calculate_mem()<<endl;
		cout<<calculate_stats()<<endl;
		usleep(1000);
		break;
	}
	return 0;
}

string calculate_stats(){
	ifstream stats("/proc/stat");
	string line;
	getline(stats, line);
	int i = 0;
	string ans = "\tuser\t\tsystem\t\tidle\n";
	while(getline(stats, line)){
		if(line.find("cpu") == string::npos) break;
		stringstream ss(line);
		string cpu;
		double user, nice, system, idle, iowait, irq, softirq;
		ss>>cpu>>user>>nice>>system>>idle>>iowait>>irq>>softirq;
		double total = user+nice+system+idle+iowait+irq+softirq;
		ans += "CPU " + to_string(i) + "\t";
		ans += to_string(user/total*100) + "%\t";
		ans += to_string(system/total*100) + "%\t";
		ans += to_string(idle/total*100) + "%\n";
	}
	return ans;
}

string calculate_mem(){
	ifstream mem("/proc/meminfo");
	double total, free; 
	string line, lines;
	getline(mem,line);
	lines += line;
	getline(mem, line);
	lines += line;
	stringstream ss(lines);
	ss>>line>>total>>line>>free;
	return "Free Memory: " + to_string(free/total * 100) + "%\n";
}
