#ifndef JOB_H
#define JOB_H

class Job{

private:
	int time;
	int status;
	int price;
	string desc;
	string location;
	string title;

public:
	int getTime(){ return time; } //Grab from computer clock
	int getPrice() { return price; }
	int getStatus(){ return status; }
	string getLocation(){ return location; }
	string getDesc(){ return desc; }
	string getTitle(){ return title; }
}
#endif
