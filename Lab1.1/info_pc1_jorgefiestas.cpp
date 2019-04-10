#include <bits/stdc++.h>
using namespace std;

int main(){
	ifstream cpuinfo("/proc/cpuinfo");
	string line;
	while(getline(cpuinfo, line)){
		if(line.find("model name") == string::npos) continue;
		cout<<line.substr(line.find(":") + 2)<<endl;
		break;
	}
	ifstream meminfo("/proc/meminfo");
	getline(meminfo, line);
	cout<<line<<endl;
	ifstream version("/proc/version");
	getline(version, line);
	cout<<line<<endl;
	ifstream uptime("/proc/uptime");
	getline(uptime, line);
	cout<<line.substr(0, line.find(" "))<<" seconds"<<endl;
	return 0;
}
