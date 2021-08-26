#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h> 
 #include <dirent.h> 
#define clear() printf("\033[H\033[J")
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define HISTORY_MAX_SIZE 500
static char *history[HISTORY_MAX_SIZE];
static unsigned counter = 0;


// help
void helpfunc()
{	printf("\033[0;33m");
    puts("\n***>>>>>	Help Section   <<<<<***"
        "\nCommands :"
		"\n>hello"
		"\n>history"
		"\n>env"
        "\n>help"
        "\n>ls"
        "\n>exit"
        "\n>mkdir"
		"\n>rmdir"
        "\n>cd"
        "\n>pwd"
		"\n>clear");
  printf("\033[0m"); 
    return;
}

// welcome text
void welcome()
{
    clear();
    printf("\033[0;32m");
    printf("\n\n\n\n******************"
        "************************");
        printf("\033[0m");
    printf("\033[0;33m"); //Set the text to the color 
    printf("\n\n\n\t****Welcome****");
    printf("\033[0m"); //Resets the text to default color
    printf("\n\n\n\t<<---(⁰ v ⁰)--->>");
    printf("\033[0;32m");
    printf("\n\n\n\n*******************"
        "***********************");
    printf("\033[0m");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(2);

}


void execute_command(char** tokens){
	// fflush(stdout);
	// printf("%d",strcmp(tokens[0],"pwd"));cd

	if (strcmp(tokens[0],"ls")==0){
		struct dirent *de;
                DIR *dr = opendir(".");
                while ((de = readdir(dr)) != NULL){
                    printf("%s ", de->d_name); 
                }
                printf("\n");
                closedir(dr);
	}
	if(strcmp(tokens[0],"help")==0){
		helpfunc();
	}
	if(strcmp(tokens[0],"hello")==0){
		char* username;
		username = getenv("USER");
		printf("\033[0;34m");
        printf("\nHello %s.\n I am the shell. "
            "Please type the required command."
            "\nType help to see list of commands\n",
            username);
		printf("\033[0m");
	}
	if(strcmp(tokens[0],"env")==0){
		system("env");
	}
	else if(strcmp(tokens[0],"cd")==0){
		if (access(tokens[1], F_OK) == -1)
        printf("The specified file/directory does not exist.\n");

		chdir(tokens[1]);
	}
	else if(!strcmp(tokens[0],"pwd")){
		char pwd[PATH_MAX];
			if (getcwd(pwd, sizeof(pwd)) != NULL)
			{
				printf("%s\n", pwd);
			}
			else
			{
				perror("getcwd() error");
			}
	}
	else if (strcmp(tokens[0],"exit")==0){
		printf("\nGoodbye\n");
        exit(0);
	}
	else if (strcmp(tokens[0], "history")==0){
		if (tokens[1]==NULL){
			for (int i=0;i<counter;i++){
				printf("%d %s",i, history[i]);
			}
		}
		else{
			int hist_count = atoi(tokens[1]);
			for (int i=(counter-hist_count); i<counter; i++){
				printf("%d %s", i, history[i]);
			}
		}
	}
	else if (strcmp(tokens[0], "clear") == 0){
        clear();
    } 
	else if (strcmp(tokens[0], "mkdir") == 0){
		int dirCheck = mkdir(tokens[1], 0777);
		if (!dirCheck){
			printf("\033[0;34m");
            printf("Directory created !\n");
			printf("\033[0m");
		}
		else {
            printf("\033[0;31m");
            printf("Can't create directory :( There might be another file with the same name .\n");
			printf("\033[0m");
        }
	}
	else if((strcmp(tokens[0], "rmdir") == 0)){
		int delCheck = remove(tokens[1]);
		if (!delCheck){
            printf("\033[0;34m");
            printf("Directory Deleted !\n");
			printf("\033[0m");
        } //returns 0 if directory is del
        else {
            printf("\033[0;31m");
            printf("Can't delete directory :( \n");
			printf("\033[0m");
        }
	}
}

void add_to_history(char* line){
	if (line[0] == '\n'){
		return;
	}
	if (counter < HISTORY_MAX_SIZE) {
			history[counter++] = strdup( line );
	}
	else {
		free( history[0] );
		for (unsigned index = 1; index < HISTORY_MAX_SIZE; index++) {
			history[index - 1] = history[index];
		}
		history[HISTORY_MAX_SIZE - 1] = strdup( line );
	}
}

char **tokenize(char *line)
{
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

	for (i = 0; i < strlen(line); i++)
	{

		char readChar = line[i];

		if (readChar == ' ' || readChar == '\n' || readChar == '\t')
		{
			token[tokenIndex] = '\0';
			if (tokenIndex != 0)
			{
				tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0;
			}
		}
		else
		{
			token[tokenIndex++] = readChar;
		}
	}

	free(token);
	tokens[tokenNo] = NULL;
	return tokens;
}

int main(int argc, char *argv[])
{	
	welcome();
	char line[MAX_INPUT_SIZE];
	char **tokens;
	int i;

	FILE *fp;
	if (argc == 2)
	{
		fp = fopen(argv[1], "r");
		if (fp < 0)
		{
			printf("File doesn't exists.");
			return -1;
		}
	}

	while (1)
	{
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if (argc == 2)
		{ // batch mode
			if (fgets(line, sizeof(line), fp) == NULL)
			{ // file reading finished
				break;
			}
			line[strlen(line) - 1] = '\0';
		}
		else
		{ // interactive mode
			char pwd[PATH_MAX];
			if (getcwd(pwd, sizeof(pwd)) != NULL)
			{
				printf("%s",pwd);
			}
			else
			{
				perror("getcwd() error");
				return 1;
			}
			printf("$ ");
			scanf("%[^\n]", line);
			getchar();
		}
		// printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		//do whatever you want with the commands, here we just print them
		add_to_history(line);
		execute_command(tokens);

		// Freeing the allocated memory
		for (i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	return 0;
}
