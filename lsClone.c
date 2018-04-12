/*
*Author: Joseph Cutino & Brendon Murthum
*
*Project: Lab12
*
*Date: 4/11/2018
*/

//Includes
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//Function used to convert mode_t integer to a visual representation of permissions
void permissions(char *output, mode_t mode){
	sprintf(output,"%s%s%s%s%s%s%s%s%s%s",
			( (S_ISDIR(mode)) ? "d" : "-"),
			( (mode & S_IRUSR) ? "r" : "-"),
			( (mode & S_IWUSR) ? "w" : "-"),
			( (mode & S_IXUSR) ? "x" : "-"),
			( (mode & S_IRGRP) ? "r" : "-"),
			( (mode & S_IWGRP) ? "w" : "-"),
			( (mode & S_IXGRP) ? "x" : "-"),
			( (mode & S_IROTH) ? "r" : "-"),
			( (mode & S_IWOTH) ? "w" : "-"),
			( (mode & S_IXOTH) ? "x" : "-"));
}

int main(int argc, char *argv[])
{
	//Directory variables
    DIR *dirPtr;
    struct dirent *entryPtr;
	
	/**
	*Modes of operation:
	*	no params: list current dir
	*	dir: list contents for provided dir
	*	-n: modify output to print total count, permissions, size, creation time, and user id and group in numeric form
	*	-i: provides the inode number for each file/dir
	*/
	
	//Flags
	bool displayInode = false;
	bool displayDetails = false;
	bool directorySet = false;	
	
	//cycle through parameters and find which apply
	
	int i;
	//start at 1. Will not need parameter 1
	for(i=1; i < argc; i++){
		if(strcmp(argv[i],"-i")==0){
			displayInode = true;
		}else if(strcmp(argv[i],"-n")==0){
			displayDetails = true;
		}else if(!directorySet){
			//check if this parameter is a valid directory and open it
			if((dirPtr = opendir(argv[i])) == NULL){
				printf("Invalid Parameter provided.\n");
			}else{
				directorySet = true;
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
		char permis[100];
		char temp[1024];
		char time[100];
		char time2[100];
		struct stat statBuf;
	

		if(displayInode && displayDetails){
			
			if((stat(entryPtr->d_name, &statBuf)) < 0){
				//perror("Failed to Grab File Status\n");
			}
			//inode  permissions  //  user  //  group  //  size  //  time of last modification
			permissions(permis, statBuf.st_mode);
			sprintf(time,"%s",ctime(&statBuf.st_mtime));
			
			strncpy(time2,time+4,12);

			sprintf(temp,"%lu %s %d %d %ld %s %s",
					statBuf.st_ino,
					permis,
					statBuf.st_uid,
					statBuf.st_gid,
					statBuf.st_size,
					time2,
					entryPtr->d_name);
			
			printf("%s\n",temp);
			
		}else if(displayInode){
			
			printf("%lu  %s\n",entryPtr->d_ino,entryPtr->d_name);
			
		}else if(displayDetails){
			if((stat(entryPtr->d_name, &statBuf)) < 0){
				//perror("Failed to Grab File Status\n");
			}
			//permissions  //  user  //  group  //  size  //  time of last modification
			permissions(permis, statBuf.st_mode);
			sprintf(temp,"%s  %d  %d  %ld  %ld  %s",
					permis,
					statBuf.st_uid,
					statBuf.st_gid,
					statBuf.st_size,
					statBuf.st_ctime,
					entryPtr->d_name);
			printf("%s\n",temp);
		}else{
			//Just print file name
			printf("%s\n",entryPtr->d_name);
		}
	}
	
	//close referenced directory
    closedir(dirPtr);
    return 0;
}