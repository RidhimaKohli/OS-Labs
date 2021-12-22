/*
Write a program in C/C++ language to implement Banker's Algorithm for deadlock avoidance. Your
program will read the number of processes, number of resource type and the matrices (Available, maxNeed,
Allocation). Your program will also take an input for a process request (process no. and a request string
depicting the number of instances required for each resource type). The program will first check
whether the input state is safe or unsafe, and then, output whether the stated request can be fulfilled or
not.



*/


#include<iostream>
#include<bits/stdc++.h>
using namespace std;

int numOfProcess , numOfResourceType ;
int *available ;
int *availCopy;
int **maxNeed ;
int **allocation;
int **need;
int *Process;
int numOfProcessLeft;
int stateIsSafe;
int *totalResources;
vector<int> sequence;
void checkSafeState( ){

// int needGTavail ;
//     for ( int k = 0; k < 5; k++) {
//     for (int p_no = 0; p_no < numOfProcess; p_no++) {
//       if (Process[p_no] == 0) {
//         needGTavail = 0;
//         for (j = 0; j < m; j++) {
//           if (need[p_no][j] > avail[j]){
//             needGTavail = 1;
//             break;
//           }
//         }
  
//         if (needGTavail == 0) {
//           sequence.push_back(p_no);
//           for (int r_no = 0; r_no < numOfResourceType; r_no++)
//             available[r_no] =  available[r_no] + allocation[p_no][r_no]; //res freed by process
//             Process[p_no] = 1;
//         }
//       }
//     }
//     }
    int i,j;
    int notExec = 0;
    
    for(i=0;i<numOfProcess;i++){ // loop through all processes in need matrix
                    if(Process[i] == 0){
                        int canBeExec = 0;
                        for(j=0;j<numOfResourceType;j++){ // loop through resources in need
                            if(need[i][j]>available[j]){ // cannot execute 
                                cout<<"Process "<<i<<" needs more than available hence cannot be executed\n";
                                notExec++;
                                break;
                            }
                            else{
                                canBeExec++;

                            }
                        } // looped through resources

                // all resources met condition
                            if(canBeExec==numOfResourceType){
                                cout<<"Process "<<i<<" can be executed as need is less than or equal to available\n";
                                Process[i] = 1;
                                sequence.push_back(i);
                                // numOfProcessLeft--;
                            for(int r=0;r<numOfResourceType;r++){
                                available[r] = available[r] + allocation[i][r];
                                allocation[i][r] = 0;
                            }

                            }

                    }
    }//end of for loop for processes

    if(notExec==numOfProcessLeft){ //unsafe in the first round
        cout<<"Unsafe\n";
        stateIsSafe = 0;
    }
    else{
                        int flag = 0;
                        for(int i=0;i<numOfProcess;i++){ 
                            if(Process[i]==0){ // some process is still left for execution
                            flag++; // counts number of process left
                            
                            }
                        }
                    
        if(flag!=0){
            numOfProcessLeft = flag;
            checkSafeState(); // recursively call the function
        }
        else{
          cout<<"Safe\n";
          stateIsSafe = 1;
          cout<<"Sequence of processes : \n";
            for(int p=0;p<numOfProcess;p++){
                cout<<sequence[p]<<" ";
            }
cout<<endl;

        }
     
        
    }

}

void checkRequestedProcess(int processNo,int *resourceInstance){

    for(int i=0;i<numOfResourceType;i++){
        if(resourceInstance[i]>need[processNo][i]){
            cout<<"Requested process cannot be executed : Process demands more than its need \n";
            return;
        }
    }
  
  for(int i=0;i<numOfResourceType;i++){
    if(resourceInstance[i]>available[i]){
      cout<<"Requested process cannot be executed right now : Resources are not available \n Please wait for other processes";
      // execute other processes and keep checking
        if(stateIsSafe==0){
            cout<<"Request can not be executed as system is in UNSAFE state\n";
        }
        else{
                for(int i=0;i<numOfResourceType;i++){
                    if(totalResources[i]<resourceInstance[i]){
                        cout<<"Request can not be executed \n";
                        return;
                    }
                }
        }
     
    }
    else{
        cout<<"Requested process can be executed \n";
        return;
    }

 
  }








}

int main(){

cout<<"Enter number of process\n";
cin>>numOfProcess;
numOfProcessLeft = numOfProcess;
cout<<"Enter number of resource type\n";
cin>>numOfResourceType;

Process=(int*)malloc(sizeof(int)*numOfProcess);

for(int i=0;i<numOfProcess;i++){
    Process[i]=0;
}
available=(int*)malloc(sizeof(int)*numOfResourceType);

maxNeed = new int*[numOfProcess];
for(int i=0;i<numOfProcess;i++){
    maxNeed[i] = new int[numOfResourceType];
}



allocation = new int*[numOfProcess];
for(int i=0;i<numOfProcess;i++){
    allocation[i] = new int[numOfResourceType];
}

need = new int*[numOfProcess];
for(int i=0;i<numOfProcess;i++){
    need[i] = new int[numOfResourceType];
}


availCopy = (int*)malloc(sizeof(int)*numOfResourceType);
totalResources = (int*)malloc(sizeof(int)*numOfResourceType);
cout<<"Enter available\n";

    for(int j=0;j<numOfResourceType;j++){
        cin>>available[j];
        availCopy[j] = available[j];
    }


for(int i=0;i<numOfResourceType;i++){

    totalResources[i] = available[i];


}


for(int i=0;i<numOfProcess;i++){
    for(int j=0;j<numOfResourceType;j++){
        totalResources[j] = totalResources[j] + allocation[i][j];
    }
}

    cout<<"numOfProcesses: "<<numOfProcess<<"\n";
    cout<<"numOfResourceType: "<<numOfResourceType<<"\n";
//take matrix as input in maxNeed
cout<<"Enter max matrix :\n";
for(int i=0;i<numOfProcess;i++){
   
    for(int j=0;j<numOfResourceType;j++){
       
        cin>>maxNeed[i][j];
       
    }
}                                           

cout<<"Enter allocation values for resources\n";
for(int i=0;i<numOfProcess;i++){
    for(int j=0;j<numOfResourceType;j++){
        cin>>allocation[i][j];
    }
}
// input for a process request (process no. and a request string depicting the number of instances required for each resource type).
int processNo ;
int *resourceInstance;
resourceInstance = (int*)malloc(sizeof(int)*numOfResourceType);
cout<<"Enter process no.\n";
cin>>processNo;
cout<<"Enter resource instance\n";
for(int i=0;i<numOfResourceType;i++){
    cin>>resourceInstance[i];
}


for(int i = 0;i<numOfProcess;i++){
    for(int j = 0;j<numOfResourceType;j++){
        need[i][j] = maxNeed[i][j] - allocation[i][j];
    }
}
cout<<"Need matrix\n";
for(int i=0;i<numOfProcess;i++){
  
        for(int j=0;j<numOfResourceType;j++){
                cout<<need[i][j]<<" ";
        }
        cout<<endl;
        }
cout<<"---------------------------------\n";
cout<<"First we check for SAFE / UNSAFE state by looking at Need Matrix\n";

checkSafeState();

cout<<"---------------------------------\n\n";
cout<<"Now we will check for requested process\n";

cout<<"Process requested : "<<processNo<<"\n";

for(int i=0;i<numOfResourceType;i++){
    cout<< resourceInstance[i] <<" ";
}
cout<<endl;
cout<<"Original available resources : \n";
for(int i=0;i<numOfResourceType;i++){
 cout<<availCopy[i]<<" ";
}
cout<<endl;
checkRequestedProcess(processNo,resourceInstance);

    return 0;
}