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
sstf, SSTF, SCAN, C-SCAN

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
    bool done;
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
int sstf(vector<request>& Requests){
    int currTime = 0;   // works as the clock
    sort(Requests.begin(), Requests.end(),sortByArrTime);
    int currentCylinder = 0;
    int totalDistanceCovered = 0;
    int totalTime = 0;
    int i=0;
    // while no all requests have been served
    while (i < Requests.size()){
        int ind =0;     // take the first request as a starting point
        int dist;
        int atleastOne = false;     // this flag checks if atleast one request is present at the moment or not
        for (int j=0; j<Requests.size(); j++){
            dist = abs(currentCylinder - Requests[j].cylinderNumber);
            if (Requests[j].done == true){  // ignore the finished requests
                // cout << Requests[j].id << " was found true" << endl;
                continue;
            }
            if ((Requests[j].done==false) && (atleastOne==false)){   // there is atleast one unfinished
                atleastOne = true;
                ind = j;
            }
            if ((atleastOne) && (dist <  abs(currentCylinder-Requests[ind].cylinderNumber)) && (Requests[j].arrivalTime <= currTime) &&(Requests[j].done==false)){ // check if there is a request closer to the current cylinder head or not
                ind = j;
            }
        }
        int last;
        // if there is no request, shift the current time to the next earliest request so that can be queued
        if (!atleastOne){
            for (int j=0; j<Requests.size(); j++){
                if (Requests[j].done == false){
                    last = j;
                    break;
                }
            }
            currTime = Requests[last].arrivalTime;
        }
        // else, mark the closest request as done and update wt and tat
        else{
            totalDistanceCovered += abs(currentCylinder - Requests[ind].cylinderNumber);
            currTime+=abs(currentCylinder - Requests[ind].cylinderNumber);
            Requests[ind].waitTime = (currTime - Requests[i].arrivalTime);
            totalDistanceCovered += abs(currentCylinder - Requests[i].cylinderNumber);
            currentCylinder = Requests[ind].cylinderNumber;
            totalTime += Requests[ind].arrivalTime;
            currTime+=100;
            Requests[ind].tat = (currTime - Requests[i].arrivalTime);
            Requests[ind].done = true;
            cout << "finished r.no. " << Requests[ind].id << " with at " << Requests[ind].arrivalTime <<  endl;
            i++;
        }
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
        req_i.done = false;
        Requests.push_back(req_i);
        i++;
        
    }
    for (int i=0; i<numOfRequests; i++){
        cout << "id: " << Requests[i].id << ", at: " << Requests[i].arrivalTime << ", cn: " << Requests[i].cylinderNumber << endl;
    }
    cout << "inp done" << endl;
    int total;
    total = sstf(Requests);
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