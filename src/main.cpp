#include <iostream>
#include "Scheduler.h"

using namespace std;

int main(int argc, char* argv[]) // argc: số lượng tham số truyền vào, argv: mảng chứa các tham số truyền vào dạng chuỗi
{
    if (argc < 3)
    {
        cout << "Usage: 24127318_24127435.exe input.txt output.txt\n"; // Kiểm tra cú pháp dòng lệnh
        return 1;
    }

    string inputFile  = argv[1]; // Gán file input/output
    string outputFile = argv[2];
    Scheduler s;

    s.read(inputFile);
    cout << "Input file reading complete !\n";

    s.runScheduling();
    cout << "Scheduling complete !\n";

    s.write(outputFile);
    cout << "Finished writing the output file !\n";
    cout << "End of program.\n";

    return 0;
}