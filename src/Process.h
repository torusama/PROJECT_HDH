#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using namespace std;

class Process {
private:
    string ProcessID;
    int ArrivalTime;
    int BurstTime;
    int RemainingTime;
    string QueueID;
    int CompletionTime;
    int WaitingTime;
    int TurnaroundTime;
    int StartTime;
public:
    Process();
    Process(std::string id, int arrival, int burst, std::string queue);

    string getPID() const;
    int getArrivalTime() const;
    int getBurstTime() const;
    int getRemainingTime() const;
    string getQueueID() const;
    int getCompletionTime() const;
    int getTurnaroundTime() const;
    int getWaitingTime() const;
    int getStartTime() const;
    
    void setRemainingTime(int time);
    void setCompletionTime(int time);
    void setStartTime(int time);

    void execute(int timeUnits);           // Thực thi process trong timeUnits
    bool isCompleted() const;              
    void calculateMetrics();               

    void print() const;

};

#endif