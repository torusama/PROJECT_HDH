# CPU Scheduling Simulation

## Thông tin đồ án

Môn: Hệ điều hành - LAB 01

Sinh viên thực hiện:

- 24127318 – Võ Tấn An  
- 24127435 – Đoàn Võ Ngọc Lâm  

## Mô tả chương trình

Chương trình mô phỏng hệ thống lập lịch CPU sử dụng:

- Multi-level Queue
- Round Robin giữa các queue
- SJF (Shortest Job First)
- SRTN (Shortest Remaining Time Next)

Chương trình đọc dữ liệu từ **file input** và ghi kết quả ra **file output**.

## Cách biên dịch chương trình

Mở terminal tại thư mục chứa source code (thư mục src: cd src) và chạy:

```bash
g++ main.cpp Scheduler.cpp Queue.cpp Process.cpp -o 24127318_24127435.exe
```

## Cách chạy chương trình

Sau khi compile, chạy chương trình bằng lệnh:

```bash
.\24127318_24127435.exe input.txt output.txt
```

Trong đó:

- `input.txt` : file dữ liệu đầu vào
- `output.txt` : file kết quả chương trình ghi ra

## Format file input

Ví dụ `input.txt`:

```
3
Q1 8 SRTN
Q2 5 SJF
Q3 3 SJF

P1 0 12 Q1
P2 1 6 Q1
P3 2 8 Q2
P4 3 4 Q2
P5 4 10 Q3
```

Ý nghĩa:

- Dòng đầu: số lượng queue
- Mỗi queue: `QueueID TimeSlice Policy`
- Mỗi process: `ProcessID ArrivalTime BurstTime QueueID`

## Output

Chương trình sẽ tạo file `output.txt` gồm:

- CPU Scheduling Diagram (timeline thực thi)
- Thông tin của từng process:
  - Arrival Time
  - Burst Time
  - Completion Time
  - Turnaround Time
  - Waiting Time
- Average Turnaround Time
- Average Waiting Time
