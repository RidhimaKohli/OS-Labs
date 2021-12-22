#include <bits/stdc++.h>
#include <cstring>
#include<unistd.h>
using namespace std;

typedef pair<string, string> dir_file;

typedef struct block
{
    char data[512];
} block;

typedef struct superblock
{
    block *freeb;
} superblock;

typedef struct directp
{
    block *b = NULL;
    int size;
    int blockno;
    bool is_dir_or_file; // 0 => dir, 1=>file
} directp;

typedef struct singleip
{
    directp *sidp[62] = {NULL};
    int size;
    int blockno;
    bool is_dir_or_file;
} singleip;

typedef struct doublyip
{
    singleip *dip[62] = {NULL};
    int size;
    bool is_dir_or_file;
    int blockno;
} doublyip;

// typedef struct dir_file{
//     const string name; // max size 13
//     const string inodeno; // max 2 digits
// }dir_file;

typedef struct inode
{
    string parent;
    string root;
    directp dp[5];
    singleip sip;
    doublyip dip;
    // info
    //  string last_modified_time;
    //  string owner;
    //  string permission;
} inode;

map<dir_file, int> fnmap;
vector<inode> inodes(32);
vector<int> freeinodes(32);
map<string, int> opcl;
superblock sb;
block *freeb = (block *)malloc(4000 * sizeof(block));
vector<int> freeblocks(4000);
// size 512 bytes or less, store using direct pointer
// if size>512 and <21*512, store using single pointer
// if size>21*512 ans <42*21*512 store using double pointer

// as sizeof single indirect pointer is 24 bytes, it can point to 21 blocks
// as sizeof of each block is 512 => 512/24 = 21.3333

void open_file(string name)
{
    dir_file nfile;
    map<string, int>::iterator itr;
    for (itr = opcl.begin(); itr != opcl.end(); ++itr)
    {
        if (name == (itr->first))
        {
            if (itr->second == 0)
                cout << "File exists already, opening file.." << endl;
            else
                cout << "File is already opened\n";
            opcl[name] = 1;
            return;
        }
    }
    inode i;
    int inode_no;
    for (int i = 0; i < inodes.size(); ++i)
    {
        if (freeinodes[i] == 0)
        {
            inode_no = i;
            break;
        }
    }
    freeinodes[inode_no] = 1;
    opcl[name] = 1; // denoting file is created and is open
    nfile.first = name;
    nfile.second = to_string(inode_no);
    fnmap[nfile] = inode_no; // stores inode number for reference in future

    puts("File created/opened.");
    cout << "The name of the file is " << name << " with inode " << inode_no << endl;
}

void close_file(string name)
{
    if (opcl[name] == 0)
    {
        cout << "File is already closed.\n";
        return;
    }
    cout<<"File "<<name<<" closed"<<endl;
    opcl[name] = 0;
}

int findfreeb()
{
    for (int i = 0; i < freeblocks.size(); ++i)
    {
        if (freeblocks[i] == 0)
        {
            return i;
        }
    }
}
void read_file(string name)
{
    if (opcl[name] == 0)
    {
        cout << "File is closed, please open it for reading.\n";
        return;
    }
    map<dir_file, int>::iterator itr;
    int flag = 0;
    // get inode number
    int inode_no;
    for (itr = fnmap.begin(); itr != fnmap.end(); ++itr)
    {
        if ((itr->first).first == name)
        {
            flag = 1;
            inode_no = fnmap[itr->first];
            break;
        }
    }
    if (flag == 0)
    {
        cout << "No such file exists.\n";
        return;
    }
    // reading part
    cout << "Reading Data" << endl;
    cout << "----------------------------------" << endl;
    inode i = inodes[inode_no];
    for (int j = 0; j < 512; ++j)
    {

        int a = i.dp[0].blockno;
        if (freeb[a].data[j] != '\0')
            cout << freeb[a].data[j];
        else
        {
            cout << endl;
            return;
        }
    }
    for (int j = 0; j < 512; ++j)
    {
        // get block
        int a = i.dp[1].blockno;
        if (freeb[a].data[j] != '\0')
            cout << freeb[a].data[j];
        else
        {
            cout << endl;
            return;
        }
    }
    for (int j = 0; j < 512; ++j)
    {
        // get block
        int a = i.dp[1].blockno;
        if (freeb[a].data[j] != '\0')
            cout << freeb[a].data[j];
        else
        {
            cout << endl;
            return;
        }
    }
    for (int j = 0; j < 512; ++j)
    {
        // get block
        int a = i.dp[1].blockno;
        if (freeb[a].data[j] != '\0')
            cout << freeb[a].data[j];
        else
        {
            cout << endl;
            return;
        }
    }
    for (int j = 0; j < 512; ++j)
    {
        // get block
        int a = i.dp[1].blockno;
        if (freeb[a].data[j] != '\0')
            cout << freeb[a].data[j];
        else
        {
            cout << endl;
            return;
        }
    }
    // singleip used
    singleip s = i.sip;
    for (int j = 0; j < 62; ++j)
    {
        int a = s.sidp[j]->blockno;
        for (int i = 0; i < 512; ++i)
        {
            if (freeb[a].data[j] != '\0')
                cout << freeb[a].data[j];
            else
            {
                cout << endl;
                return;
            }
        }
    }
}

void write_file(string name, string data)
{
    if (opcl[name] == 0)
    {
        cout << "File is closed, please open it for writing.\n";
        return;
    }
    cout << "Writing data....\n";
    map<dir_file, int>::iterator itr;
    int flag = 0;
    // get inode number
    int inode_no;
    for (itr = fnmap.begin(); itr != fnmap.end(); ++itr)
    {
        if ((itr->first).first == name)
        {
            flag = 1;
            inode_no = fnmap[itr->first];
            break;
        }
    }
    if (flag == 0)
    {
        cout << "No such file exists.\n";
        return;
    }
    // writing part
    char dat[data.size() + 1];
    strcpy(dat, data.c_str());
    inode *ip = &inodes[inode_no];
    
    // inode of the file retrived
    int char_written = 0;
    int f = findfreeb();

    for (int j = 0; j < 512; ++j)
    {
        freeb[f].data[j] = dat[j];
        char_written++;
    }
    ip->dp[0].b = &freeb[f];
    ip->dp[0].blockno = f;
    freeblocks[f] = 1;
    f = findfreeb();
    int size = data.size();
    if (char_written < size)
    {
        for (int i = 0; i < 512; ++i)
        {
            freeb[f].data[i] = dat[i + 512];
            char_written++;
        }
        ip->dp[1].b = &freeb[f];
        ip->dp[1].blockno = f;
        freeblocks[f] = 1;
    }

    if (char_written < size)
    {
        f = findfreeb();
        for (int i = 0; i < 512; ++i)
        {
            freeb[f].data[i] = dat[i + 512 + 512];
            char_written++;
        }
        ip->dp[2].b = &freeb[f];
        ip->dp[2].blockno = f;
        freeblocks[f] = 1;
    }
    if (char_written < size)
    {
        f = findfreeb();
        for (int i = 0; i < 512; ++i)
        {
            freeb[f].data[i] = dat[i + 3 * 512];
            char_written++;
        }

        ip->dp[3].b = &freeb[f];
        ip->dp[3].blockno = f;
        freeblocks[f] = 1;
    }
    if (char_written < size)
    {
        f = findfreeb();
        for (int i = 0; i < 512; ++i)
        {
            freeb[f].data[i] = dat[i + 4 * 512];
            char_written++;
        }
        ip->dp[4].b = &freeb[f];
        ip->dp[4].blockno = f;
        freeblocks[f] = 1;
    }
    if (char_written < size)
    {
        // singleip used
        while (char_written < size)
        {
            f = findfreeb();
            int bno = 0;
            for (int i = 0; i < 512; ++i)
            {
                freeb[f].data[i] = dat[i + (bno + 4) * 512];
                char_written++;
            }
            ip->sip.sidp[bno]->b = &freeb[f];
            ip->sip.sidp[bno]->blockno = f;
            freeblocks[f] = 1;
        }
    }

    // alloting data to direct pointers first
    // initial 512 bytes to first block pointed by first direct pointer in inode
}

map<dir_file, pair<dir_file, dir_file> > proot;
map<string, dir_file> dir;
void make_dir(string name)
{
    if (dir.find(name) != dir.end())
    {
        cout << "Directory already exists" << endl;
        return;
    }
    int in;
    for (int i = 0; i < inodes.size(); ++i)
    {
        if (freeinodes[i] == 0)
        {
            in = i;
            break;
        }
    }
    dir_file file;
    file.first = name;
    file.second = to_string(in);
    fnmap[file] = in;
    dir[name] = file;
    dir_file root;
    dir_file par;
    root.first = "..";
    root.second = "100";
    par = root;
    proot[file].first = root; // stores root directory
    proot[file].second = par; // stores parent
}

void cur_dir()
{
    cout << "Currently in root directory" << endl;
}

void rm_dir(string name)
{
    dir_file dirname;
    dirname = dir[name];
    int inodenum = fnmap[dirname];
    freeinodes[inodenum] = 0; // freeing the inode number
    proot.erase(dirname);
    dir.erase(name);
}

void copy_file(string fone, string ftwo)
{
    map<dir_file, int>::iterator itr;
    int flag1 = 0;
    int flag2 = 0;
    int eof=0;
    // get inode number
    int inode_no;
    for (itr = fnmap.begin(); itr != fnmap.end(); ++itr)
    {
        if ((itr->first).first == fone)
        {
            flag1 = 1;
            inode_no = fnmap[itr->first];
            break;
        }
    }
    int inode_no2;
    for (itr = fnmap.begin(); itr != fnmap.end(); ++itr)
    {
        if ((itr->first).first == ftwo)
        {
            flag2 = 1;
            inode_no2 = fnmap[itr->first];
        }
    }
    if (flag1 == 0)
    {
        cout << "File " << fone << " does not exist.\n";
        return;
    }
    if (flag2 == 0)
    {
        cout << "File " << ftwo << " does not exist.\n";
        return;
    }
    // reading part
    string reads = "";
    inode i = inodes[inode_no];
    for (int j = 0; j < 512; ++j)
    {
        int a = i.dp[0].blockno;
        if (freeb[a].data[j] != '\0')
            reads.push_back(freeb[a].data[j]);
        else
        {
            eof=1;
            cout << endl;
            break;
        }
    }
    if(eof==0){
    for (int j = 0; j < 512; ++j)
    {
        // get block
        int a = i.dp[1].blockno;
        if (freeb[a].data[j] != '\0')
            reads.push_back(freeb[a].data[j]);
        else
        {
            eof==1;
            cout << endl;
            break;
        }
    }
    }
    if(eof==0){
    for (int j = 0; j < 512; ++j)
    {
        // get block
        int a = i.dp[1].blockno;
        if (freeb[a].data[j] != '\0')
            reads.push_back(freeb[a].data[j]);
        else
        {
            eof==1;
            cout << endl;
            return;
        }
    }
    }
    if(eof==0){
    for (int j = 0; j < 512; ++j)
    {
        // get block
        int a = i.dp[1].blockno;
        if (freeb[a].data[j] != '\0')
            reads.push_back(freeb[a].data[j]);
        else
        {
            eof==1;
            cout << endl;
            return;
        }
    }
    }
    if(eof==0){
    for (int j = 0; j < 512; ++j)
    {
        // get block
        int a = i.dp[1].blockno;
        if (freeb[a].data[j] != '\0')
            reads.push_back(freeb[a].data[j]);
        else
        {
            eof==1;
            cout << endl;
            return;
        }
    }
    }
    // singleip used
    singleip s = i.sip;
    if(eof==0){
    for (int j = 0; j < 62; ++j)
    {
        int a = s.sidp[j]->blockno;
        for (int i = 0; i < 512; ++i)
        {
            if (freeb[a].data[j] != '\0')
                reads.push_back(freeb[a].data[j]);
            else
            {
                eof==1;
                cout << endl;
                return;
            }
        }
    }
    }
    cout<<reads<<endl;
    cout<<"Copy data from "<<fone<<" -----------to--------"<<ftwo<<endl;
    write_file(ftwo, reads);
}

// Stores special entries "." and ".."

// void read_file()

int main()
{

    sb.freeb = freeb;
    cout<<"----------Enter The Command-------------"<<endl;
    // sb.freeb[511]=-1;
    // cout << sizeof(directp);
    // cout << endl
    //      << 5 + 62 + 62 * 62;
    // cout << endl;
    // open_file("f1");
    // open_file("f2");
    // write_file("f1", "hi");
    // write_file("f2", "there");
    // copy_file("f1","f2");
    // sleep(3);
    // read_file("f1");
    // read_file("f2");
    
    while (1)
    {
        string command;
        getline(cin, command);
        stringstream ss(command);
        string token;
        while (ss >> token)
        {
            if (token == "my_open")
            {
                string filename;
                ss >> filename;
                open_file(filename);
                cout<<"======================================="<<endl;
            }
            else if (token == "my_close")
            {
                string filename;
                ss >> filename;
                close_file(filename);
                cout<<"======================================="<<endl;
            }
            else if (token == "my_read")
            {
                string name;
                ss >> name;
                read_file(name);
                cout<<"======================================="<<endl;
            }
            else if (token == "my_write")
            {
                string name, data;
                ss >> name;
                ss >> data;
                string w;
                while (ss >> w)
                {
                    data += " ";
                    data += w;
                }
                cout << "Opening file with name : " << name;
                cout << endl
                     << data << endl;
                write_file(name, data);
                cout << endl;
                cout<<"======================================="<<endl;
            }
            else if (token == "my_mkdir")
            {
                string name;
                ss>>name;
                make_dir(name);
                cout<<"Directory created\n"<<endl;
                cout<<"======================================="<<endl;
            }
            else if (token == "my_chdir")
            {
                cur_dir();
                cout<<"======================================="<<endl;
            }
            else if (token == "my_rmdir")
            {
                string name;
                ss>>name;
                rm_dir(name);
                cout<<"Directory removed\n"<<endl;
                cout<<"======================================="<<endl;
            }
            else if (token == "my_copy")
            {
                string f1, f2;
                ss>>f1;
                ss>>f2;
                copy_file(f1,f2);
                cout<<"======================================="<<endl;
            }
        }
    }
    return 0;
}