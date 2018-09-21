#include <iostream>
#include <fstream>
#include "Graph.h"
#include "TraceLog.h"


Graph::Graph(int ini_, int max_, int var_, int faults_, std::string dir_, std::string out_) {
	dir=dir_;
	out=out_;
	ini=ini_;
	max=max_;
	var=var_;
	faults=faults_;
}

std::string Graph::getFileName(int i) {
	std::stringstream os;
	os << i;
	std::string statFile;
	statFile.append(dir);
	statFile.append("/stat_");
	statFile.append(os.str());
	statFile.append(".dat");
	return statFile;
}

void Graph::save() {
	for(int i=ini; i<=max; i+=var) {
		TraceLog trace;
		trace.setStatFile(getFileName(i));
		trace.load();
		writePercent(i, trace.getDecision(), "decision-xgraph");
		writeValue(i, trace.getConsensusTime(), "consensustime-xgraph");
		writePercent(i, trace.getSinkNodes(), "sinknodes-xgraph");
		writeValue(i, trace.getSinkTime(), "sinktime-xgraph");
		writeCountValue(i, trace.getAgreement(), "agreement-xgraph");
		writeValue(i, trace.getRounds(), "rounds-xgraph");

		writePercent(i, trace.getPD(), "pd-xgraph");
		writePercentNF(i, trace.getPDNodes(), "pdnodes-xgraph");
		writePercentNF(i, trace.getCollectNodes(), "collectnodes-xgraph");
		writePercent(i, trace.getCollect(), "collect-xgraph");
		writePercent(i, trace.getSink(), "sink-xgraph");
	}
}


//no faults
void Graph::writePercentNF(int n, cStdDev stat, std::string fileName) {
	double valuePercent = stat.getMean()*100/(n);
	double stddevPercent = stat.getStddev()*100/(n);
	write(n, valuePercent, stddevPercent, stat.getCount(), fileName);
}

void Graph::writePercent(int n, cStdDev stat, std::string fileName) {
	double valuePercent = stat.getMean()*100/(n-faults);
	double stddevPercent = stat.getStddev()*100/(n-faults);
	write(n, valuePercent, stddevPercent, stat.getCount(), fileName);
}

void Graph::writeValue(int n, cStdDev stat, std::string fileName) {
	write(n, stat.getMean(), stat.getStddev(), stat.getCount(), fileName);
}

void Graph::writeCountValue(int n, cStdDev stat, std::string fileName) {
	double value = stat.getSum()*100/stat.getCount();
	write(n, value, stat.getStddev(), stat.getCount(), fileName);
}

void Graph::write(int n, double value, double stddev, int samples, std::string fileName) {
	double stderror = stddev/sqrt(samples);
	//confidence interval - level 95% (stderror = confidence interval)
	stderror = stderror * 1.96;
	std::string file;
	file.append(out);
	file.append("/");
	file.append(fileName);
	std::ofstream myfile;
  	myfile.open (file.c_str(), std::ios::out | std::ios::app | std::ios::ate);
  	myfile << n << " " << value << " " << stderror << endl;
  	myfile.close();
}


int main(int argc, const char* argv[]) {
	if(argc<4) {
		printf("Usage: ");
		printf("graph [INI_NODES] [MAX_NODES] [VAR_NODES] [F] [DIR]\n");
		return 1;
	}
	int faults = 0;
	int dir_ix = 4;
	if(argc>3) {
		faults = atoi(argv[4]);
		dir_ix = 5;
	}
	int ini = atoi(argv[1]);
	int max = atoi(argv[2]);
	int var = atoi(argv[3]);
	std::string dir;
	dir.append(argv[dir_ix]);
	std::string out;
	out.append(argv[dir_ix]);

	Graph graph(ini, max, var, faults, dir, out);
	graph.save();
}

