#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>

using namespace std;

// Shared data
static int result = 0;

// global lock variable(Mutex)
mutex gLock;

// Calculate the sum of a portion of the array
void calculateSum(int threadID, int sum_till, int num_of_threads, vector<int> data)
{
    int start = threadID * (sum_till / num_of_threads);
    int end = (threadID + 1) * (sum_till / num_of_threads);

    int localSum = 0;
    for (int i = start; i < end; i++)
    {
        localSum += data[i];
    }

    // Lock and update the shared global variable(result)
    lock_guard<mutex> lock(gLock);
    result += localSum;
}

int main()
{
    // Opening the input file
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cout<<"[ERROR]: Cannot open input file."<<endl;
        return 1;
    }

    string tempInput = "";
    vector<int> v;

    // Reading content of input file
    while (getline(inputFile, tempInput)) {
        v.push_back(stoi(tempInput));
    }

    int sum_till = v[0];
    int num_of_threads = v[1];

    // Closing the input file
    inputFile.close();

    vector<int> data(sum_till);
    // Initialize the data array with some values
    for (int i = 0; i < sum_till; ++i)
    {
        data[i] = i + 1;
    }

    try
    {
        // Create threads
        thread threads[num_of_threads];
        for (int i = 0; i < num_of_threads; i++)
        {
            // Calling calculateSum function for all threads
            threads[i] = thread(calculateSum, i, sum_till, num_of_threads, data);
        }

        // Joining all the threads
        for (int i = 0; i < num_of_threads; i++)
        {
            threads[i].join();
        }
    }
    catch (...)
    {
        cout << "[ERROR]: Thread allocation was unsuccessful\n";
    }

    // Print the final result
    cout<<"Sum of first "<<sum_till<<" natural number is: "<<result<<endl;

    return 0;
}
