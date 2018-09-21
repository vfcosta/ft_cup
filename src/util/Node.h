
#ifndef NODE_H
#define NODE_H

class NeighborDetector;

class Node
{
  public:
    Node();
    Node(int);
    Node(int, double);
    int getId();
    int getValue();
    double getTime();
    void setId(int);
    void setValue(int);
    void setTime(double);
    bool operator==(const Node &) const;
    int getCounter();
    void setCounter(int);
    int getParent();
    void setParent(int);
    NeighborDetector *getNeighbors() {return this->neighbors;}

  private:
	int parent;
	int counter;
  	int id;
  	double time;
  	int value;
  	int timeout;
  	NeighborDetector *neighbors;
};

#endif
