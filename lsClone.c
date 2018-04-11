#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#incldue <string.h>
#include <stdbool.h>


int main(int argc, char *argv[])
{
    DIR *dirPtr;
    struct dirent *entryPtr;
	
	//Flags
	bool displayInode = FALSE;
	bool displayDetails = FALSE;
	bool directorySet = FALSE;

	/**
	*Modes of operation:
	*	no params: list current dir
	*	dir: list contents for provided dir
	*	-n: modify output to print total count, permissions, size, creation time, and user id and group in numeric form
	*	-i: provides the inode number for each file/dir
	*/
	
	
	//cycle through params and find which apply
	
	int i;
	//start at 1. Will not need parameter 1
	for(i=1; i < argc; i++){
		if(strcmp(argv[i],"-i")==0){
			displayInode = TRUE;
		}else if(strcmp(argv[i],"-n")==0){
			displayDetails = TRUE;
		}else if(!directorySet){
			//check if this parameter is a valid directory and open it
			if((dirPtr = opendir(argv[i])) == NULL){
				printf("Invalid Parameter provided.\n");
			}else{
				directorySet = TRUE;
			}
		}else{
			printf("Invalid Parameter Provided.\n");
		}
	}
	
	if(dirPtr == NULL){
		//directory not provided. Use default (current location)
		dirPtr = opendir(".");
	}
	
	//cycle through files & sub directories in the current location
    while ((entryPtr = readdir(dirPtr))){
		//create a string per file. This string will be modified based on the provided parameters 
		char *fileData;
		char temp[255];
		struct stat statBuf;
		
		if(displayInode){
			//add inode value to the output string 
			sprintf(temp,"%l",entryPtr->d_ino);
			strcat(fileData,temp);
		}
		if(displayDetails){
			if((stat(entryPtr, &statBuf)) < 0){
				perror("Failed to Grab File Status\n");
			}
			//append output string with formated file data
			sprintf("");
		}
		
		//append output string with file name
		strcat(fileData,entryPtr->d_name);
		
		//print output string
		printf("%s\n",fileData);		
	}
	
	//close referenced directory
    closedir(dirPtr);
    return 0;
}