#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int main()
{
	//0 for cpu io bound
	//1 for io bound 

	int pid;
	int ROUNDS[2];
	ROUNDS[0]=10000;  
	ROUNDS[1]=4000;
	int prior[2];
	prior[0]=5;
	prior[1]=10;
	int sleeptime[2];
	sleeptime[0]=1;
	sleeptime[1]=3;
	int prob[2];
	prob[0]=30;
	prob[1]=70;

	int i,j=0;
	char CommandToRun[1024],add[1024];	


	for(i=0,j=0;i<4;i++)
	{ 

		if(i>1)
		{	
			j=1;
		}
		memset(&CommandToRun,0,1024);
		sprintf(CommandToRun,"/mnt/c/Users/Varsha/desktop/semesterv/OS/Labs/Lab5/Code/Code/process.out ");
		sprintf(add,"%d ",ROUNDS[j]);
		strcat(CommandToRun,add);
		sprintf(add,"%d ",prior[j]);
		strcat(CommandToRun,add);
		sprintf(add,"%d ",prob[j]);
		strcat(CommandToRun,add);
		sprintf(add,"%d ",sleeptime[j]);
		strcat(CommandToRun,add);
		printf("checking value of CommandToRun =%s \n",CommandToRun);

		pid=fork();
		if (pid<0){
			printf("cannot fork\n");
		}
		else if (pid >0){
			printf("in parent\n");
		}
		else if(pid==0)
		{
	
			system(CommandToRun);

		}


		
	}



	return 0;
}


















