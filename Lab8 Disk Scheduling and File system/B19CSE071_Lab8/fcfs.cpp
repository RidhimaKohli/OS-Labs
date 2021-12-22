/*
Disk Scheduling

Input :
 Number of cylinders
 100
 Cylinder 0 -->99
 Sequence of requests with arrival time
 12 0
 34 5
 7 8
 9 3
 78 20
 .
 .
 .
FCFS, SSTF, SCAN, C-SCAN

*/
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
int numOfCyclinders;
struct request
{
    int id;
    int arrivalTime;
    int cylinderNumber;
    int waitTime;
    int tat;
};


bool sortByArrTime(request a, request b)
{
    return a.arrivalTime < b.arrivalTime;
}

/*

what we require
current cylinder
total time till now = current time
wait time = total time till now - time of arrival of current cylinder
turnaround time 
i to i+1 cylinder - 0.001 seconds

*/
int FCFS(vector<request>& Requests){
    int currTime = 0;
    sort(Requests.begin(), Requests.end(),sortByArrTime);
    int currentCylinder = 0;
    int totalDistanceCovered = 0;
    int totalTime = 0;
    for(int i = 0; i < Requests.size(); i++){
        if (currTime < Requests[i].arrivalTime){
            currTime = Requests[i].arrivalTime;
        }
        
        
    totalDistanceCovered += abs(currentCylinder - Requests[i].cylinderNumber);
    currTime+=abs(currentCylinder - Requests[i].cylinderNumber);
    currentCylinder = Requests[i].cylinderNumber;
    Requests[i].waitTime = (currTime - Requests[i].arrivalTime);
    totalTime += Requests[i].arrivalTime;
    currTime+=100;
    Requests[i].tat = (currTime - Requests[i].arrivalTime);




}
return totalDistanceCovered;
}


int main(){
    int numOfRequests;
    vector<request> Requests;
    cout << "Enter number of cylinders: " << endl;
    cin >> numOfCyclinders;
    cout << "Enter number of requests: " << endl;
    cin >> numOfRequests;
    cout << "Enter your requests (arrival-time request-number): " << endl;
    int i=0;
    while(i<numOfRequests){
        int req , at;
        cin >> at >> req;
        struct request req_i;
        req_i.id = i;
        req_i.cylinderNumber = req;
        req_i.arrivalTime = at;
        Requests.push_back(req_i);
        i++;
    }
 
    int total;
    total = FCFS(Requests);
    float avgw=0; float avgt=0;
    for (int i=0; i<numOfRequests; i++){
        cout << "id: " << Requests[i].id << ", wt: " << Requests[i].waitTime  << ", tat: " << Requests[i].tat << endl;
        avgw += Requests[i].waitTime;
        avgt = Requests[i].tat;
    }
    avgw/=numOfRequests;
    avgt/=numOfRequests;
    cout << "total distance covered: " << total << endl;
    cout << "avg waiting time: " << avgw << endl;
    cout << "avg turnaround time: " << avgt << endl;

    return 0;
}