#include<bits/stdc++.h>
using namespace std;

int Current_Directory;
string currDirName="";
int Current_File_Block;
int highestFAT_index_first_block=2;
string prompt = "> ";
typedef struct block1{
    int **FAT;
} block1;

 block1 BLOCK1;
map<string , int> Directory_Name_Map;
typedef struct Directory{
  map<string,int> files;
}Directory;
 Directory *DIRECTORY;


typedef struct superblock{
    long int blockSize;
    long int totalFileSystemSize;
    string freeByteVector;
    //pointer to directory block
    Directory *directory;
    int TotalNumberOfDirectories;
} superblock;
 superblock SUPER_BLOCK;

typedef struct fileBlock{
    char fileName[100];
    char fileContent[916]; // 1024 - 100 - 8
    int isOpen; // size 4 
    int read_write; //0 for read and 1 for write : size 4
} fileBlock;
fileBlock *FILE_BLOCK;

int getFreeBlock(){
    int i;
    for(i=0;i<SUPER_BLOCK.totalFileSystemSize;i++){
        if(SUPER_BLOCK.freeByteVector[i]=='0'){
            return i;
        }
    }
    return -1;
}
// void print FAT
void print_FAT(block1 BLOCK1 , int noOfBlocks){
    cout<<prompt<<"The FAT is "<<endl;
    cout<<prompt<<"Directory Index\t\t\tBlock\n";
    for(int i=0;i<noOfBlocks;++i){
        cout<<BLOCK1.FAT[i][0]<<" "<<BLOCK1.FAT[i][1]<<endl;
    }
}
void my_mkdir(string name){
    
    if(SUPER_BLOCK.TotalNumberOfDirectories==32){
        cout<<prompt<<"Memory full ! No more directories can be created\n";
        return;
    }
    else if(Directory_Name_Map.find(name)!=Directory_Name_Map.end()){
        cout<<prompt<<"Directory already exists\n";
        return;
    }
    else{
        Directory_Name_Map[name]=SUPER_BLOCK.TotalNumberOfDirectories;
        SUPER_BLOCK.TotalNumberOfDirectories++;
        cout<<prompt<<"Directory created\n";
        return;
    }
  

}
 void my_chdir(string Dirname){
         if(Directory_Name_Map.find(Dirname)==Directory_Name_Map.end()){
                    cout<<prompt<<"Directory does not exist\n";
                }
                else{
                    Current_Directory=Directory_Name_Map[Dirname];
                    cout<<"Directory changed to "<<Dirname<<"\n";
                    currDirName = Dirname;
                    prompt = " > " +Dirname+"/ > ";
                }
                return;
 }


string checkNext(int index , string s){
    int blockno = index;
     if(BLOCK1.FAT[blockno][1]!=-1){
     int nextBlock = BLOCK1.FAT[blockno][1];
     s+=FILE_BLOCK[nextBlock].fileContent;
        return s+=checkNext(nextBlock,s);
     }
     else {
         return "";
     }
}
 void my_read(int blockno){
     cout<<prompt<<"Reading file\n";
     string readLines = FILE_BLOCK[blockno].fileContent;
    readLines+=checkNext(blockno,readLines);

   cout<<prompt<<"File content is :\n"<<readLines<<endl;
   return;
 }

  string get_read(int blockno){
     cout<<prompt<<"Reading file\n";
     string readLines = FILE_BLOCK[blockno].fileContent;
    readLines+=checkNext(blockno,readLines);

   
   return readLines;
 }

 void my_write(int blockno){
     cout<<prompt<<"Writing file\n";
     cout<<prompt<<"Enter the content to be written\n";
    char content[916];
     cin>>content;
     if (content.size()>916){
         cout<<prompt<<"Block size exceeded .... distributing file in other block...\n";
         char temp[916];
         temp = content.substr(0,916);
         char remaining[916];
         remaining = content.substr(916);
            FILE_BLOCK[blockno].fileContent = temp;

            int freeBlock = getFreeBlock();
if (freeBlock==-1){
    cout<<prompt<<"Memory full ! No more blocks can be allocated , cant write full file\n";
    return;
}
cout<<".................. Linking blocks of File ..................\n";
            BLOCK1.FAT[blockno][1] = freeBlock;
           int nextBlock = freeBlock;
           SUPER_BLOCK.freeByteVector[freeBlock] = '1';
            FILE_BLOCK[next_block].fileContent = remaining;
           BLOCK1.FAT[nextBlock][1] = -1;

            FILE_BLOCK[next_block].fileContent = remaining;
         return;
     }
     FILE_BLOCK[blockno].fileContent=content;

     return;
 }

void my_open(string FileName){
         if(Directory[Current_Directory] .files.find(Filename)==Directory[Current_Directory] .files.end()){
                    cout<<prompt<<"File doesn't exist in this directory ....\n";
                    while( highestFAT_index_first_block!=fileBlock.size() && SUPER_BLOCK.freeByteVector[highestFAT_index_first_block]!='0'){
                        highestFAT_index_first_block++;
                    }
                    if (highestFAT_index_first_block==fileBlock.size() + 1 ){
                        cout<<prompt<<"Memory full\n";
                        return;
                    }
                     Directory[Current_Directory] .files[Filename] = highestFAT_index_first_block; // map to index in FAT
                     int blockNum = BLOCK1.FAT[highestFAT_index_first_block];
                     SUPER_BLOCK.freeByteVector[highestFAT_index_first_block] = '1';
                     highestFAT_index_first_block++;
                    FILE_BLOCK[blockNum].fileName = FileName;
                    FILE_BLOCK[blockNum].isOpen = 1;
                    prompt+=FileName+" > ";
                    cout<<prompt<<"File created and opened , Enter whether you want to read or write\n";
                    string command;
                                                getline(cin,command);
                                                stringstream ss(command);
                                                string token;
                                                
                                                    if(token=="my_read"){
                                                        
                                                         my_read(blockNum);
                                                    }
                                                    else if(token=="my_write"){

                                                        my_write(blockNum);


                                                    }

                        my_close();
                    cout<<prompt<<"File closed\n";

                }

                else{

                    int ind = Directory[Current_Directory].files[FileName];
                    int blockNum = BLOCK1.FAT[ind][1] ;
                    FILE_BLOCK[blockNum].isOpen = 1;
                    Current_File_Block = blockNum;
                     prompt+=FileName+" > ";
                    cout<<prompt<<"File created and opened , Enter whether you want to read or write\n";
                    string command;
                                                getline(cin,command);
                                                stringstream ss(command);
                                                string token;
                                                
                                                    if(token=="my_read"){
                                                        
                                                         my_read(blockNum);
                                                    }
                                                    else if(token=="my_write"){

                                                        my_write(blockNum);


                                                    }

                        my_close();
                    cout<<prompt<<"File closed\n";
                    
                }
                return ;
}

void my_close(){

FILE_BLOCK[Current_File_Block].isOpen = 0;
prompt = "> "+currDirName+" > ";
return;


}



void my_rmdir(string Dirname){
    if(Directory_Name_Map.find(Dirname)==Directory_Name_Map.end()){

        cout<<prompt<<"Directory does not exist\n";
        return;
    }
    else{
                int dirIndex = Directory_Name_Map[Dirname];
                if (Current_Directory == dirIndex){
                    Current_Directory = -1;
                    Current_File_Block = -1;
                    currDirName = "";
                }

            Directory_Name_Map.erase(Dirname);
            SUPER_BLOCK.TotalNumberOfDirectories--;
            cout<<prompt<<"Directory and all its content deleted\n";
            return;
        
    }



    }
void my_copy(string FileName,string Dirname){

        int dirIndex = Directory_Name_Map[Dirname];
      
            int fileIndex = Directory[dirIndex].files[FileName];
            int blockNum = BLOCK1.FAT[fileIndex][1];
            int freeBlock = getFreeBlock();
            if (freeBlock==-1){
                cout<<prompt<<"Memory full\n";
                return;
            }
            string copied = get_read(blockNum);
            FILE_BLOCK[freeBlock].fileContent = copied;
            BLOCK1.FAT[freeBlock][1] = blockNum;
            SUPER_BLOCK.freeByteVector[freeBlock] = '1';
        

    }

int main(){
    
    long long int fileSystemSize ;
    cout<<"Enter the size of the file system in MB: ";
    cin>>fileSystemSize;
    fileSystemSize=fileSystemSize*1024*1024;
    cout<<"Enter the size of the block in kilo bytes: ";
    int blockSize;
    cin>>blockSize;
    blockSize=blockSize*1024;
    int noOfBlocks=fileSystemSize/blockSize;
    cout<<"-----------------------------------------------------------------------------\n";
    cout<<"The number of blocks in the file system is "<<noOfBlocks<<endl;
    cout<<"The size of the file system is "<<fileSystemSize<<endl;
    cout<<"The size of the block is "<<blockSize<<endl;
    cout<<"-----------------------------------------------------------------------------\n";

    //noofblocks - 0 : super block , 1 - block1 : fat , other blocks - files 
    SUPER_BLOCK.blockSize=blockSize;
    SUPER_BLOCK.totalFileSystemSize=fileSystemSize;
    SUPER_BLOCK.TotalNumberOfDirectories=0;
   
    DIRECTORY=new Directory[32];
    SUPER_BLOCK.directory=DIRECTORY;
    SUPER_BLOCK.freeByteVector="";
    for (int i=0;i<noOfBlocks;++i){
        SUPER_BLOCK.freeByteVector+="0";
    }
    SUPER_BLOCK.freeByteVector[0]='1'; //SUPERBLOCK ITSELF BLOCK 0
    SUPER_BLOCK.freeByteVector[1]='1'; // BLOCK 1 Contains FAT
    cout<<"The free byte vector is "<<SUPER_BLOCK.freeByteVector<<endl;
    cout<<"-----------------------------------------------------------------------------\n";

// 2 x noOfBlocks FAT

   
    BLOCK1.FAT=new int*[noOfBlocks];
    for(int i=0;i<noOfBlocks;++i){
        BLOCK1.FAT[i]=new int[2];
        BLOCK1.FAT[i][0]=i;
        BLOCK1.FAT[i][1]=-1;
    }
BLOCK1.FAT[0][1]=0; //SUPERBLOCK BLOCK 1
BLOCK1.FAT[1][1]=1; //FAT BLOCK 2
   

print_FAT(BLOCK1,noOfBlocks);
    


FILE_BLOCK=new fileBlock[noOfBlocks-2];
for (int i=0;i<FILE_BLOCK.size();i++){
    FILE_BLOCK[i].read_write = -1;
    FILE_BLOCK[i].isOpen = 0;
}

    while(1){
        cout<<"-------------------------------------------------------\n";
        cout<<prompt<<"Enter command\n";

        string command;
        getline(cin,command);
        stringstream ss(command);
        string token;
        while(ss>>token){
            if(token=="my_mkdir"){
                
                string Dirname;
                ss>>Dirname;
                my_mkdir(Dirname);
            }
            else if(token=="my_chdir"){
                string Dirname;
                ss>>Dirname;
                my_chdir(Dirname);


            }
            else if(token=="my_open"){
                string Filename;
                ss>>Filename;
                my_open(Filename);

            }
            else if(token=="my_close"){
                my_close();
            }
             else if(token=="my_read"){
                
                 cout<<"Open a file first\n";
            }
            else if(token=="my_write"){
                cout<<"Open a file first\n";

            }
            else if(token=="my_rmdir"){
                    string Dirname;
                    ss>>Dirname;
                    my_rmdir(Dirname);
                

            }
            else if(token=="my_copy"){ // mycopy file name , directory name to current directory
                    string Filename;
                    ss>>Filename;
                    string Dirname;
                    ss>>Dirname;
                    my_copy(Filename,Dirname);
            }
            else if(token=="exit"){
                exit(0);
            }
        }
    }


    return 0;
}