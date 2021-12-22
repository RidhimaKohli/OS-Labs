
#include <stdio.h>
#include <stdlib.h>



int i;
int j;

int getIndex(int val , int arr[] , int n){
    for(i=0; i<n; i++){
    if(val == arr[i]){
      return i;
    }
  }
}

int main(){
    int totalNumOfCylinders;
    printf("Enter the total number of cylinders : ");
    scanf("%d", &totalNumOfCylinders);

  int requestQueue[20];
  int head,  totalNumberOfRequests, larger;


  printf("%s", "Input number of requests : ");
  scanf("%d", &totalNumberOfRequests);
double *waitTime;
double *turnaroundTime;
double avgWaitTime;
double avgTurnaroundTime;

waitTime = (double *)malloc(totalNumberOfRequests * sizeof(double));
turnaroundTime = (double *)malloc(totalNumberOfRequests * sizeof(double));
 int headIsRequested = 0;

  printf("%s", "Enter head position : ");
  scanf("%d", &head);
 
 printf("%s\n", "Input requests : ");
  for(i=0; i<totalNumberOfRequests; i++){
    scanf("%d", &requestQueue[i]);
    if(requestQueue[i] == head){
      headIsRequested = 1;
    }
  }


if (headIsRequested == 0){
 
 int headPosition = totalNumberOfRequests;
  requestQueue[headPosition] = head; 
  totalNumberOfRequests++;
}
int lastIndex = totalNumberOfRequests-1;

  for(i=0; i<totalNumberOfRequests;i++){
    for(j=i; j<totalNumberOfRequests; j++){
      if(requestQueue[i]>requestQueue[j]){
                    larger = requestQueue[i];
                    requestQueue[i] = requestQueue[j];
                    requestQueue[j] = larger;
      }
    }
  }



int *processedRequests;
processedRequests = (int *)malloc(totalNumberOfRequests * sizeof(int));
for( i=0; i<totalNumberOfRequests; i++){
  processedRequests[i] = 0;
}

//get index of head in sorted queue
int head_Index = getIndex(head, requestQueue, totalNumberOfRequests);

int Total_Movement = 0;

double currentTime = 0;
double cylShiftTime = 0.001; //ms
double timeToProcess = 0.100; //ms
int prevCylinder = head;
int currentCylinder = head;
 
  if(abs(head-0) <= abs(head-totalNumOfCylinders)){ //to left

      for(j=head_Index; j>=0; j--){  
        // printf("%s\n", "Moving to left");
        // printf("current time : %f\n", currentTime);
        currentCylinder=requestQueue[j];
        Total_Movement += abs(currentCylinder-prevCylinder);
        // printf("movement : %d , cylShiftTime : %f\n" , Total_Movement, cylShiftTime);
        double timeTakenForShift = abs(currentCylinder-prevCylinder)*cylShiftTime;
       // printf("timeTakenForShift : %f\n", timeTakenForShift);
        currentTime = currentTime + timeTakenForShift;
        waitTime[j] = currentTime;

        printf("%d --> ",requestQueue[j]);

        prevCylinder = currentCylinder;
        if(requestQueue[j] == head && headIsRequested==1){
          turnaroundTime[j] = currentTime + timeToProcess;
        currentTime = currentTime  + timeToProcess;
    
        }
        else if(requestQueue[j]!=head){
                turnaroundTime[j] = currentTime + timeToProcess;
        currentTime = currentTime  + timeToProcess;
        }
     
        processedRequests[j] = 1;
       //  printf("current time : %f\n", currentTime);
      }

      prevCylinder=totalNumOfCylinders-1;
      currentCylinder=totalNumOfCylinders-1;

      for(j=lastIndex; j>head_Index; j--){

        //     printf("%s\n", "Moving to left");
        // printf("current time : %f\n", currentTime);

        currentCylinder=requestQueue[j];
        Total_Movement += abs(currentCylinder-prevCylinder);
        double timeTakenForShift = abs(currentCylinder-prevCylinder)*cylShiftTime;
        currentTime = currentTime + timeTakenForShift;
        waitTime[j] = currentTime;

        printf("%d --> ",requestQueue[j]);

        prevCylinder = currentCylinder;
        if(requestQueue[j] == head && headIsRequested==1){
          turnaroundTime[j] = currentTime + timeToProcess;
        currentTime = currentTime  + timeToProcess;
    
        }
        else if(requestQueue[j]!=head){
                turnaroundTime[j] = currentTime + timeToProcess;
        currentTime = currentTime  + timeToProcess;
        }
        processedRequests[j] = 1;
         
        // printf("current time : %f\n", currentTime);
      }

      }  // one direction left
      
      
      else { //other direction right

      for(j=head_Index+1; j<totalNumberOfRequests; j++){

        // printf("%s\n", "Moving to right");
        // printf("current time : %f\n", currentTime);
        currentCylinder=requestQueue[j];
        Total_Movement += abs(currentCylinder-prevCylinder);
        double timeTakenForShift = abs(currentCylinder-prevCylinder)*cylShiftTime;
        currentTime = currentTime + timeTakenForShift;
        waitTime[j] = currentTime;

          printf("%d --> ",requestQueue[j]);

        prevCylinder = currentCylinder;
           if(requestQueue[j] == head && headIsRequested==1){
          turnaroundTime[j] = currentTime + timeToProcess;
        currentTime = currentTime  + timeToProcess;
    
        }
        else if(requestQueue[j]!=head){
                turnaroundTime[j] = currentTime + timeToProcess;
        currentTime = currentTime  + timeToProcess;
        }
            processedRequests[j] = 1;
      }
      prevCylinder=0;
      currentCylinder=0;
      for(j=0; j<=head_Index; j++){
        // printf("%s\n", "Moving to right");
        //    printf("current time : %f\n", currentTime);
        currentCylinder=requestQueue[j];
        Total_Movement += abs(currentCylinder-prevCylinder);
        double timeTakenForShift = abs(currentCylinder-prevCylinder)*cylShiftTime;
        currentTime = currentTime + timeTakenForShift;
        waitTime[j] = currentTime;

          printf("%d --> ",requestQueue[j]);

        prevCylinder = currentCylinder;
                if(requestQueue[j] == head && headIsRequested==1){
          turnaroundTime[j] = currentTime + timeToProcess;
        currentTime = currentTime  + timeToProcess;
    
        }
        else if(requestQueue[j]!=head){
                turnaroundTime[j] = currentTime + timeToProcess;
        currentTime = currentTime  + timeToProcess;
        }
            processedRequests[j] = 1;
              //  printf("current time : %f\n", currentTime);
      }

  }
printf("end\n");
double totalReq = 0.0;
for(j=0;j<totalNumberOfRequests;j++){
    avgWaitTime += waitTime[j];
    avgTurnaroundTime += turnaroundTime[j];
    printf("For request %d :\n",requestQueue[j]);
    printf("Wait Time : %f\n",waitTime[j]);
    printf("Turnaround Time : %f\n",turnaroundTime[j]);
    totalReq = totalReq+1.0;

}

avgWaitTime = avgWaitTime/totalReq;
avgTurnaroundTime = avgTurnaroundTime/totalReq;

printf("Average Wait Time : %f\n",avgWaitTime);
printf("Average Turnaround Time : %f\n",avgTurnaroundTime);


  printf("\nTotal movement of cylinders %d", Total_Movement);

  return 0;
}