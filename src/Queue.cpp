#include "Queue.h"
Queue::Queue() {
    QueueID = "";
    TimeSlice = 0;
    Policy = "";
}

Queue::Queue(string qid,int time,string policy):QueueID(qid),TimeSlice(time),Policy(policy){}

string Queue::getQid() {
    return QueueID;
}

int Queue::getTimeSlice(){
    return TimeSlice;
}

string Queue::getPolicy(){
    return Policy;
}

int Queue::getProcessCount() {
    return Processes.size();
}

void Queue::addProcess(Process* p) {
    Processes.push_back(p);
}
bool Queue::isEmpty() const {
    return Processes.empty();
}

bool Queue::hasReadyProcess(int currentTime) {
    for (auto p : Processes) {
        if (p->getArrivalTime() <= currentTime && p->getRemainingTime() > 0) {
            return true;
        }
    }
    return false;
}

Process* Queue::getNextProcess(int currentTime) {
    Process* p = NULL;
    if (Policy == "SJF") {
        int shortestCPU = INT_MAX;
        for (auto a : Processes) {
            if (a->getArrivalTime() <= currentTime && a->getRemainingTime() > 0) {
                if(a->getBurstTime() < shortestCPU) {
                    shortestCPU = a->getBurstTime();
                    p = a;
                }
            }
        }
    }
    else if (Policy == "SRTN") {
        int shortestRTN = INT_MAX;
        for (auto a : Processes) {
            if (a->getArrivalTime() <= currentTime && a->getRemainingTime() > 0) {
                if (a->getRemainingTime() < shortestRTN) {
                    shortestRTN = a->getRemainingTime();
                    p = a;
                }
            }
        }
    }
    return p;
}