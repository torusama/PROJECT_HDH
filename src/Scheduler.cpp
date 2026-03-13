#include "Scheduler.h"
#include <iostream>
#include <algorithm>
using namespace std;

Scheduler::Scheduler() {
    CurrentTime = 0;
    CurrentQueueIndex = 0;
}

void Scheduler::addQueue(Queue q) {
    Queues.push_back(q);
}

void Scheduler::addProcess(Process p) {
    Processes.push_back(p);
}

void Scheduler::distributeProcessesToQueues() {
    for(auto& p : Processes) {
        string qid = p.getQueueID();
        for (auto& q : Queues) {
            if (q.getQid() == qid) {
                q.addProcess(&p);
            }
        }
    }
}

bool Scheduler::allProcessesCompleted() const {
    // Kiểm tra tất cả process
    for (const Process& p : Processes) {
        if (!p.isCompleted()) {
            return false;  // Còn process chưa xong
        }
    }
    return true;  // Tất cả đã xong
}

void Scheduler::executeProcess(Process* p, Queue& q, int timeToRun)
{
    int startTime = CurrentTime;
    
    // Nếu là lần đầu process được CPU
    if (p->getStartTime() == -1) // mặc định là -1 nếu chưa chạy
    {
        p->setStartTime(CurrentTime); // đặt start time của process là current time
    }
    
    // Thực thi process
    p->execute(timeToRun); // hàm execute sẽ giảm thời gian chạy của process đi 1 lương timeToRun
    
    // Cập nhật thời gian hệ thống
    CurrentTime += timeToRun; // CPU đã chạy thêm 1 lượng timeToRun
    
    // Ghi vào timeline
    recordEvent(startTime, CurrentTime, q.getQid(), p->getPID());
    // VD:  startTime = 8
    //      CurrentTime = 11
    //      q.getQid() = Queue = Q1
    //      p->getPID() = Process = P3
    //      => timeline sẽ thêm: [8 - 11] Q1 P3
    
    // Nếu process đã hoàn thành
    if (p->isCompleted()) // BurstTime = remainingTime = 0
    {
        p->setCompletionTime(CurrentTime); // CompletionTime = CurrentTime
    }
}

void Scheduler::recordEvent(int start, int end, string queueID, string processID) {
    if (!Timeline.empty()) {
        ScheduleEvent& last = Timeline.back();
        if (last.QueueID == queueID && last.ProcessID == processID && last.EndTime == start) {
            last.EndTime = end;
            return;
        }
    }
    Timeline.push_back(ScheduleEvent(start, end, queueID, processID));
}

void Scheduler::runScheduling() {
    distributeProcessesToQueues();
    while (!allProcessesCompleted()) {

        Queue& currentQueue = Queues[CurrentQueueIndex];

        bool Ready = false;
        for (auto q : Queues) {
            if (q.hasReadyProcess(CurrentTime)) {
                Ready = true;
                break;
            }
        }
        if (Ready == false) {
            CurrentTime++;
            continue;
        }

        string policy = currentQueue.getPolicy();
        int timeSlice = currentQueue.getTimeSlice();
        int sliceRemaining = timeSlice;

        // Chạy trong phạm vi time slice của queue
        while (sliceRemaining > 0 && currentQueue.hasReadyProcess(CurrentTime)) {

            Process* currentP = currentQueue.getNextProcess(CurrentTime);
            if (currentP == nullptr) break;

            if (policy == "SRTN") {
                // Preemptive nên chạy 1 lần rồi lại quay về check tiếp
                executeProcess(currentP, currentQueue, 1);
                sliceRemaining--;

            } else { 
                // SJF non-preemptive
                int timeToRun = min(sliceRemaining, currentP->getRemainingTime());
                executeProcess(currentP, currentQueue, timeToRun);
                sliceRemaining -= timeToRun;
            }
        }

        // Xong 1 queue thì Round Robin
        CurrentQueueIndex = (CurrentQueueIndex + 1) % Queues.size();
    }

    for (Process& p : Processes) {
        p.calculateMetrics();
    }
}                                                                        

vector<ScheduleEvent> Scheduler::getTimeline() const {
    return Timeline;
}

vector<Process> Scheduler::getProcesses() const {
    return Processes;
}

void Scheduler::print() const {
    cout << "========== SCHEDULER INFO ==========" << endl;
    cout << "Current Time: " << CurrentTime << endl;
    cout << "Total Queues: " << Queues.size() << endl;
    cout << "Total Processes: " << Processes.size() << endl;
    
    cout << "\n--- Timeline ---" << endl;
    for (const ScheduleEvent& e : Timeline) {
        cout << "[" << e.StartTime << " - " << e.EndTime << "] "
             << e.QueueID << " " << e.ProcessID << endl;
    }
    
    cout << "\n--- Processes ---" << endl;
    for (const Process& p : Processes) {
        cout << p.getPID() << ": "
             << "Arrival=" << p.getArrivalTime() << ", "
             << "Burst=" << p.getBurstTime() << ", "
             << "Completion=" << p.getCompletionTime() << ", "
             << "Turnaround=" << p.getTurnaroundTime() << ", "
             << "Waiting=" << p.getWaitingTime() << endl;
    }
    cout << "====================================" << endl;
}

void Scheduler::read(const string& fileName)
{
    ifstream fin(fileName);

    if(!fin.is_open())
    {
        cerr << "Can't open file !";
        return;
    }

    int n; // Biến đọc dòng quene
    fin >> n;

    for(int i = 0; i < n; i++)
    {
        string queueID, policy;
        int timeSlice;

        fin >> queueID >> timeSlice >> policy; // Đọc từng dòng queue add vào vector queue
        Queue q(queueID, timeSlice, policy);
        addQueue(q);
    }

    string processID, queueID;
    int arrival, burst;

    while(fin >> processID >> arrival >> burst >> queueID) // Đọc từng dòng Process cho đến khi hết file, add vào vector process
    {
        Process p(processID, arrival, burst, queueID);
        addProcess(p);
    }

    fin.close();
}

void Scheduler::write(const string& fileName)
{
    ofstream fout(fileName);
    if(!fout.is_open())
    {
        cerr << "Error writing file !";
        return;
    }

    // in sơ đồ tiến trình CPU
    fout << "================== CPU SCHEDULING DIAGRAM ==================\n\n";
    fout << left << setw(15) << "[Start - End]" << setw(10) << "Queue" << setw(10) << "Process" << "\n";
    fout << "-----------------------------------------------------\n";

    for(ScheduleEvent& t: Timeline) // in timeline entry
    {
        string timeRange = "[" + to_string(t.StartTime) + " - " + to_string(t.EndTime) + "]";
        fout << left << setw(15) << timeRange << setw(10) << t.QueueID << setw(10) << t.ProcessID << "\n";
    }

    fout << "\n================ PROCESS STATISTICS ================\n\n";
    fout << left << setw(12) << "Process" << setw(12) << "Arrival" << setw(12) << "Burst" << setw(12) << "Completion" << setw(12) << "Turnaround" << setw(12) << "Waiting" << "\n";
    fout << "--------------------------------------------------------------------\n";

    double sumTurnaround = 0;
    double sumWatting = 0;

    for(Process& p: Processes) // in tiến trình
    {
        fout << left << setw(12) << p.getPID() << setw(12) << p.getArrivalTime() << setw(12) << p.getBurstTime()
             << setw(12) << p.getCompletionTime() << setw(12) << p.getTurnaroundTime() << setw(12) << p.getWaitingTime() << "\n";
        
        // Tính tổng thời gian của Turnaround và Watting
        sumTurnaround += p.getTurnaroundTime();
        sumWatting += p.getWaitingTime();
    }

    // Tính thời gian trung bình của Turnaround và Watting
    double averageTT = sumTurnaround / Processes.size();
    double averageWT = sumWatting / Processes.size();
    fout << "--------------------------------------------------------------------\n";

    fout << "\nAverage Turnaround Time : " << fixed << setprecision(1) << averageTT << "\n"; // in 1 chữ số sau dấu phẩy
    fout << "Average Waiting Time    : " << fixed << setprecision(1) << averageWT << "\n";
    fout << "====================================================\n";

    fout.close();
}