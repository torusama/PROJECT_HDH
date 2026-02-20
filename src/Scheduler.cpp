#include "Scheduler.h"
#include <iostream>
#include <algorithm>
using namespace std;

// ========== CONSTRUCTOR ==========
Scheduler::Scheduler() {
    CurrentTime = 0;
    CurrentQueueIndex = 0;
}

// ========== ADD DATA ==========
void Scheduler::addQueue(Queue q) {
    Queues.push_back(q);
}

void Scheduler::addProcess(Process p) {
    Processes.push_back(p);
}

// ========== HELPER FUNCTIONS ==========

void Scheduler::distributeProcessesToQueues() {
    // Duyệt qua tất cả process
    for (Process& p : Processes) {
        string qid = p.getQueueID();
        
        // Tìm queue có ID tương ứng
        for (Queue& q : Queues) {
            if (q.getQid() == qid) {
                q.addProcess(&p);  // Thêm pointer của process vào queue
                break;
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

int Scheduler::getNextQueueIndex(int startIndex) {
    int n = Queues.size();
    
    // Quét vòng tròn từ queue tiếp theo
    for (int i = 1; i <= n; i++) {
        int index = (startIndex + i) % n;
        
        // Nếu queue này có process ready
        if (Queues[index].hasReadyProcess(CurrentTime)) {
            return index;
        }
    }
    
    return -1;  // Không có queue nào ready
}

void Scheduler::executeProcess(Process* p, Queue& q, int timeToRun) {
    int startTime = CurrentTime;
    
    // Nếu là lần đầu process được CPU
    if (p->getStartTime() == -1) {
        p->setStartTime(CurrentTime);
    }
    
    // Thực thi process
    p->execute(timeToRun);
    
    // Cập nhật thời gian hệ thống
    CurrentTime += timeToRun;
    
    // Ghi vào timeline
    recordEvent(startTime, CurrentTime, q.getQid(), p->getPID());
    
    // Nếu process đã hoàn thành
    if (p->isCompleted()) {
        p->setCompletionTime(CurrentTime);
    }
}

void Scheduler::recordEvent(int start, int end, string queueID, string processID) {
    ScheduleEvent event(start, end, queueID, processID);
    Timeline.push_back(event);
}

// ========== MAIN SCHEDULING ==========
void Scheduler::runScheduling() {
    // Bước 1: Phân phối process vào các queue
    distributeProcessesToQueues();
    
    // Bước 2: Scheduling loop
    int safetyCounter = 0;  // Tránh infinite loop
    
    while (!allProcessesCompleted() && safetyCounter++ < 10000) {
        
        // Tìm queue tiếp theo có process ready (Round Robin giữa queue)
        int queueIndex = getNextQueueIndex(CurrentQueueIndex);
        
        // Nếu không có queue nào ready
        if (queueIndex == -1) {
            // Tăng thời gian lên 1 đơn vị (chờ process đến)
            CurrentTime++;
            continue;
        }
        
        // Lấy queue hiện tại
        Queue& currentQueue = Queues[queueIndex];
        
        // Lấy process tiếp theo từ queue (theo SJF hoặc SRTN)
        Process* selectedProcess = currentQueue.getNextProcess(CurrentTime);
        
        // Nếu không có process nào (không nên xảy ra vì đã check hasReadyProcess)
        if (selectedProcess == nullptr) {
            CurrentQueueIndex = (queueIndex + 1) % Queues.size();
            continue;
        }
        
        // Tính thời gian chạy = min(TimeSlice của queue, RemainingTime của process)
        int timeSlice = currentQueue.getTimeSlice();
        int remainingTime = selectedProcess->getRemainingTime();
        int timeToRun = min(timeSlice, remainingTime);
        
        // Thực thi process
        executeProcess(selectedProcess, currentQueue, timeToRun);
        
        // Chuyển sang queue tiếp theo (Round Robin)
        CurrentQueueIndex = (queueIndex + 1) % Queues.size();
    }
    
    // Bước 3: Tính metrics cho tất cả process
    for (Process& p : Processes) {
        p.calculateMetrics();
    }
    
    // Debug warning nếu vượt quá safety counter
    if (safetyCounter >= 10000) {
        cerr << "WARNING: Safety counter reached! Possible infinite loop." << endl;
    }
}

// ========== GETTERS ==========
vector<ScheduleEvent> Scheduler::getTimeline() const {
    return Timeline;
}

vector<Process> Scheduler::getProcesses() const {
    return Processes;
}

// ========== DEBUG ==========
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