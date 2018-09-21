#ifndef GRAPH_H_
#define GRAPH_H_

#include <omnetpp.h>

class Graph
{
  private:
	std::string dir;
	std::string out;
	int max;
	int ini;
	int var;
	int faults;

	std::string getFileName(int);
	void writePercent(int, cStdDev, std::string);
	void writePercentNF(int, cStdDev, std::string);
	void writeValue(int, cStdDev, std::string);
	void writeCountValue(int, cStdDev, std::string);
	void write(int, double, double, int, std::string);

  public:
	Graph(int, int, int, int, std::string, std::string);
	void save();

};

#endif /*GRAPH_H_*/
