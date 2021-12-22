#include <bits/stdc++.h>
#include <iostream>

using namespace std;
typedef long long ll;

vector<pair<int,int> >tlb;       //page,frame
vector<int> freeFrameLinkedList; //free frames 

struct FPlink{
    int frame_no;
    int page_linked;
};

// S: to store recently used pages
typedef struct Node{
    int data;
    struct Node* next;
    struct Node* prev;
}NODE;

struct FPlink* FP;
vector<pair<int,int> >page_table; //frames valid, stores valid invalid bit in second position
int *refString;                  //reference string


// S: variables for page fault treatment
NODE* head, *prevN, *curr, *tail, *ctail;  // S: pointers to keep track of LRU
int numOfFaults = 0; 
int total = 0;
map<int, NODE*> mp;  // S: LRU map

void printlist(NODE* h){
    cout << " Current state of LRU: \n";
    NODE* c = h;
    while (c!=NULL){
        cout << c->data << " ";
        c = c->next;
    }
    cout<< endl;

}

// S: whenever a new page is referred, we have to put it into LRU
void addNode(int page){
    if (total == 0){
        curr = (NODE*) malloc(sizeof(NODE));
        curr->data = page;
        mp[page] = curr;

        // adjust pointers
        head = curr;
        head->next = NULL;
        head->prev = NULL;
        tail = head;
        total ++ ;
    }
    else{
        // make new node
        curr = (NODE*) malloc(sizeof(NODE));
        curr->data = page;
        mp[page] = curr;

        // make it head
        curr->next = head;
        curr->prev = NULL;
        head->prev = curr;
        head = curr;
        total++;
    }
    cout << "added " << head->data;
    printlist(head);
}

// S: bring a node on top
void updatelru(int page){
    curr = mp[page];
    // if found at head, do nothing
    if (curr == head){
        return;
    }
    if (curr == tail){      // if found at tail, change tail to the previous element
        if (curr->prev){    // it will be changed only if previous of tail is not NULL
            tail = curr->prev;
            tail->next = NULL;
        }
    }

    // connect previous to next and vice versa if they exist
    if (curr->prev){
        curr->prev->next = curr->next;
    }
    if (curr->next){
        curr->next->prev = curr->prev;
    }

    // make current element as new head
    curr->prev = NULL;
    curr->next = head;
    head->prev = curr;
    head = curr;
    cout << "\nPut " << head->data << " at top\n";
    printlist(head);
    // no fault, no change in total variable
}

//S: page replacement
void deleteTail(int page, int frames){
    // make new node
    curr = (NODE*) malloc(sizeof(NODE));
    curr->data = page;

    int tailframe;
    for(int frame_num=0;frame_num<frames;frame_num++){ //page table check
        if(page_table[frame_num].second == 1){ //valid frame found
            if(FP[frame_num].page_linked==tail->data){ //valid frame is linked to required page
                FP[frame_num].page_linked = -1;
                tailframe = frame_num;
                break;
            }
        }
    }
    FP[tailframe].page_linked = page;

    cout << "\nDeleted " << tail->data;
    mp.erase(tail->data);   // remove old tail from hash table    

    // make it head
    curr->next = head;
    curr->prev = NULL;
    if (head)
        head->prev = curr;
    head = curr;
    
    // shift tail pointer to the previous node and delete old tail
    ctail = tail->prev;
    if (ctail){
        ctail->next = NULL;
        free(tail);
        tail = ctail;
    }
    else{
        free(tail);
        tail = head;
    }
    mp[page] = curr;   // add new page to hash table
    printlist(head);
}

void processRS(int pro_num ,int frames,int tlbsize,int page_num ){
    int foundInTLB = 0;

    // int numOfFaults = 0;   // S: num of page faults 
    // int total = 0;        // S: num of pages stored in LRU

    for(int i=0;i<tlbsize;++i){
        if(tlb[i].first==page_num){
            //hit
            foundInTLB = 1;
            cout << "Process : " <<pro_num << " for page reference " <<page_num << " , TLB hit with frame no. " << tlb[i].second << endl;
        }
    }

    if(foundInTLB==0){ // miss , now check page_table
        cout<<"Process : "<<pro_num<<" for page reference "<<page_num<<" , TLB miss -> Now check page_table\n";
        int foundInPage_Table=0;
        for(int frame_num=0;frame_num<frames;frame_num++){ //page table check

            if(page_table[frame_num].second == 1){ //valid frame found
                if(FP[frame_num].page_linked==page_num){ //valid frame is linked to required page
                    foundInPage_Table=1;
                    cout<<"TLB Miss with no page fault\n";
                    cout<<"Process : "<<pro_num<<" for page reference "<<page_num<<" , TLB miss -> page table valid -> with frame no. "<<frame_num<<endl;
                    updatelru(page_num);
                }         
            }

            if(foundInPage_Table==1){ 
                break;
            }
        }


        if(foundInPage_Table==0){  //no such frame with that page : two cases 1- free frame available , 2 - PAGE FAULT
                  
            /* 
            Case 1
            if size of freeFrameLinkedList is not 0 yet 
                    cout<<"TLB Miss with no page fault , free frame is available\n";
                    insert requested page : page_num into struct FP at corresponding next_free_frame_num :  next_free_frame_num = freeFrameLinkedList[0];
                    FP[next_free_frame_num].page_linked = page_num
                    and make validity of that next_free_frame_num in page_table to 1 : page_table[next_free_frame_num].second = 1
                    Also delete that next_free_frame_num from freeFrameLinkedList since its not free anymore
                    it = freeFrameLinkedList.begin() ;
                    freeFrameLinkedList.erase(it);
            Case 2
            else if size of freeFrameLinkedList is 0 now , Page Fault !!!!!!
                      cout<<"TLB Miss : Page Fault ....Performing Page Replacement with LRU\n";
                      LRU ---> least ref page number ----> VictimFrame ( get this from page table since it must be full now)
                      Fp[VictimFrame].page_linked = page_num //new ref page
                      page_table[VictimFrame ] --> must be already valid 1
            
            cout<<"Page table and frames have been updated\n";
            Display the updated page_table , freeFrameLinkedList , FP ( which has linkages )
            //TLB remains same
            */
            if(freeFrameLinkedList.size()!=0){   //S: this should also be a page fault,
                cout<<"TLB Miss with no page fault , free frame is available\n";
                //get first free frame
                int next_free_frame_num = freeFrameLinkedList[0];
                FP[next_free_frame_num].page_linked = page_num; //link frame to page requested
                page_table[next_free_frame_num].second = 1; //make it valid
                vector<int>::iterator it;
                cout << "Used the new frame " << next_free_frame_num << endl;
                it = freeFrameLinkedList.begin() ;
                freeFrameLinkedList.erase(it);
                addNode(page_num);
            }
            else if (freeFrameLinkedList.size()==0){ //S: page fault
                cout << "Page fault!\n";
                numOfFaults++;
                deleteTail(page_num, frames);
            }
        }
    }
}

void deleteLinkedList(NODE* head){
    NODE* c = head;
    NODE* n = NULL;
    
    while (c!=NULL){
        n = c->next;
        free(c);
        c = n;
    }
}

int main(){
    srand(5);
    int k , m , s , f;
    int  no_exec_pro=0;
    int ran;
    int curr_m;
    cout<<"Enter number of process k\n";
    cin>>k;
    int curr_proc;   // current process running
    map<int, int> processes;    // keep track of which processes have already run
    for (int i=1; i<=k; i++){
        processes[i] = 0;
    }



        cout<<"Enter Virtual address space : max number of pages :\n";
        cin>>m;
        cout<<"Enter Physical address space : number of frames :\n";
        cin>>f;
        cout<<"Enter size of TLB(should be less than number of frames):\n";
        cin>>s;
        if(s>f){
            cout<<"Invalid entry for size of TLB\nExiting the program....\n";
            exit(1);
        }
    while(no_exec_pro<k){

 curr_proc = 1+rand()%k;
        if (processes[curr_proc]==1){
            continue;
        }
        cout << "Current process is: " << curr_proc;

        curr_m = 1+rand()%m;
        cout << "\nCurrent process requires " << curr_m << " pages\n";
      
        int rfs_n = 2*curr_m;   // size of reference string is taken as twice of number of pages it requires
      
        // int rfs_n = 2*m;
        refString=(int*)malloc(rfs_n*sizeof(int));
        cout<<"Reference String Generated : ";
        for(int i=0;i<rfs_n;i++){
            refString[i] = rand()%m; //create ref string 
            cout<<refString[i]<<" ";
        }
        
        cout<<endl;
        

        FP=(struct FPlink*)malloc(sizeof(struct FPlink)*f);

        for(int fn = 0 ; fn<f;fn++){
            FP[fn].frame_no = fn;
            FP[fn].page_linked = -1; //initially no page linked
        }


        cout<<"Free Frames : ";
        
        //initialize free frame linked list of full size 
        for(int fi = s ; fi < f ; fi++){
            freeFrameLinkedList.push_back(fi);
            cout<<freeFrameLinkedList[fi-s]<<" ";
        }
        cout<<endl;


map<int,int> pgf; 
for(int i=0;i<m;i++){
    pgf[i]=-1;
}

cout<<"TLB : page - frame \n";
        for(int t=0;t<s;t++){    
            ran = rand()%m; //page number

        if(pgf[ran]==-1){
            tlb.push_back(make_pair(ran,t));
            pgf[ran]=t;
            cout<<tlb[t].first<<" - "<<tlb[t].second<<endl;
        }
        else{
            t--;
            continue;
        }
            
            // tlb.push_back(make_pair(rand()%m,t)); //create TLB - assign random values for every process
        
        }



        cout<<"\nPage Table : Page number - Validity (Only frames chosen for TLB are valid) \n";

        //create page table
        for(int t=0;t<f;t++){
            if(t>=s){
                page_table.push_back(make_pair(t,0));
            }
            else{
                page_table.push_back(make_pair(t,1));
            }
             //create page table : all frames invalid initially except TLB
            cout<<page_table[t].first<<" - "<<page_table[t].second<<endl;
        }
        

        // send ref string tlb ffll and page table for processing
        cout<<"Starting Process number : "<<no_exec_pro<<endl;
        for(int ri=0;ri<rfs_n;ri++){
            processRS(curr_proc,f,s,refString[ri]);
            // tlb.erase();
            // executed[id]=true;
        }
        // S: refreshing everything for new process
        tlb.clear();
        deleteLinkedList(head);
        head=NULL; curr=NULL; tail=NULL; ctail=NULL; prevN=NULL;
        mp.clear();
        total = 0;
        cout << "Number of faults is for process  "<<no_exec_pro<<" are "<< numOfFaults << endl;
        numOfFaults = 0;
        no_exec_pro++;
    }
    return 0;
}