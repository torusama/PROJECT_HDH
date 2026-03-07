#include "Process.h"
#include <iostream>
#include <iomanip>
//Constructor===============================
Process::Process()
   : ProcessID(""),ArrivalTime(0),BurstTime(0),RemainingTime(0),
     QueueID(""),CompletionTime(0),WaitingTime(0),TurnaroundTime(0),StartTime(-1) {} 

Process::Process(string id, int arrival, int burst, string queue)
    : ProcessID(id), ArrivalTime(arrival), BurstTime(burst), 
      RemainingTime(burst), QueueID(queue), 
      CompletionTime(0), TurnaroundTime(0), WaitingTime(0), StartTime(-1) {
}
//Getters===================================
string Process::getPID() const {
    return ProcessID;
}
int Process::getArrivalTime() const {
    return ArrivalTime;
}
int Process::getBurstTime() const {
    return BurstTime;
}
int Process::getRemainingTime() const {
    return RemainingTime;
}
string Process::getQueueID() const {
    return QueueID;
}
int Process::getCompletionTime() const {
    return CompletionTime;
}
int Process::getTurnaroundTime() const {
    return TurnaroundTime;
}
int Process::getWaitingTime() const {
    return WaitingTime;
}
int Process::getStartTime() const {
    return StartTime;
}
//Setters===================================
void Process::setRemainingTime(int time) {
    RemainingTime = time;
}
void Process::setCompletionTime(int time) {
    CompletionTime = time;
}
void Process::setStartTime(int time) {
    StartTime = time;
}

void Process::execute (int timeUnits) {
    if(RemainingTime >= timeUnits) {
        RemainingTime -= timeUnits;
    }
    else {
        RemainingTime = 0;
    }
}

bool Process::isCompleted() const {
    if (RemainingTime == 0) return true;
    return false;
}

void Process::calculateMetrics() {
    TurnaroundTime = CompletionTime - ArrivalTime;
    WaitingTime = CompletionTime - ArrivalTime - BurstTime;
}

void Process::print() const {
    cout << "================================" << endl;
    cout << "Process ID       : " << ProcessID << endl;
    cout << "Queue ID         : " << QueueID << endl;
    cout << "Arrival Time     : " << ArrivalTime << endl;
    cout << "Burst Time       : " << BurstTime << endl;
    cout << "Remaining Time   : " << RemainingTime << endl;
    cout << "Start Time       : " << (StartTime == -1 ? "Not started" : to_string(StartTime)) << endl;
    cout << "Completion Time  : " << CompletionTime << endl;
    cout << "Turnaround Time  : " << TurnaroundTime << endl;
    cout << "Waiting Time     : " << WaitingTime << endl;
    cout << "Status           : " << (isCompleted() ? "Completed" : "Running/Waiting") << endl;
    cout << "================================" << endl;
}