#include "Process.h"
#include <iostream>
using namespace std;

int main() {
    cout << "\n========== TEST PROCESS CLASS ==========\n" << endl;
    
    // TEST 1: Khởi tạo Process
    cout << "TEST 1: Tạo process P1" << endl;
    Process p1("P1", 0, 12, "Q1");
    p1.print();
    
    // TEST 2: Execute một phần
    cout << "\nTEST 2: Execute P1 trong 5 time units" << endl;
    p1.setStartTime(0);  // Giả sử bắt đầu lúc 0
    p1.execute(5);
    p1.print();
    
    // TEST 3: Execute tiếp
    cout << "\nTEST 3: Execute P1 thêm 7 time units" << endl;
    p1.execute(7);
    p1.print();
    
    // TEST 4: Kiểm tra isCompleted
    cout << "\nTEST 4: Kiểm tra P1 đã hoàn thành chưa?" << endl;
    cout << "Is Completed: " << (p1.isCompleted() ? "YES" : "NO") << endl;
    
    // TEST 5: Set completion time và tính metrics
    cout << "\nTEST 5: Set completion time = 12 và tính metrics" << endl;
    p1.setCompletionTime(12);
    p1.calculateMetrics();
    p1.print();
    
    // TEST 6: Test process khác (theo ví dụ trong đề)
    cout << "\n\nTEST 6: Tạo process P2 từ đề bài" << endl;
    Process p2("P2", 1, 6, "Q1");
    p2.print();
    
    cout << "\nExecute P2 hoàn toàn (6 units)" << endl;
    p2.setStartTime(1);
    p2.execute(6);
    p2.setCompletionTime(7);
    p2.calculateMetrics();
    p2.print();
    
    // TEST 7: Verify kết quả với expected output từ đề
    cout << "\n========== VERIFY VỚI ĐỀ BÀI ==========\n" << endl;
    cout << "P2 Expected: Turnaround=6, Waiting=0" << endl;
    cout << "P2 Actual  : Turnaround=" << p2.getTurnaroundTime() 
         << ", Waiting=" << p2.getWaitingTime() << endl;
    
    if (p2.getTurnaroundTime() == 6 && p2.getWaitingTime() == 0) {
        cout << "✓ PASS!" << endl;
    } else {
        cout << "✗ FAIL!" << endl;
    }
    
    return 0;
}