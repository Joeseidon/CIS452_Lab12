#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void permissions(char *output, mode_t mode){
	/*printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");*/
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
    DIR *dirPtr;
    struct dirent *entryPtr;
	
	//Flags
	bool displayInode = false;
	bool displayDetails = false;
	bool directorySet = false;

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
		struct stat statBuf;
		
		//NEED TO RESET STRING BUFFERS

		if(displayInode && displayDetails){
			
			if((stat(entryPtr->d_name, &statBuf)) < 0){
				perror("Failed to Grab File Status\n");
			}
			//inode  permissions  //  user  //  group  //  size  //  time of last modification
			permissions(permis, statBuf.st_mode);
			sprintf(time,"%s",ctime(&statBuf.st_ctime));
			//printf("\n%s\n",&time[4]);
			sprintf(temp,"%lu  %s  %d  %d  %ld  %s  %s",
					statBuf.st_ino,
					permis,
					statBuf.st_uid,
					statBuf.st_gid,
					statBuf.st_size,
					&time[4],
					entryPtr->d_name);
			
			printf("%s\n",temp);
			
		}else if(displayInode){
			//add inode value to the output string 
			/*if((sprintf(temp,"%lu",entryPtr->d_ino)) <= 0){
				perror("Inode access failed");
			}*/
			
			printf("%lu  %s\n",entryPtr->d_ino,entryPtr->d_name);
			
		}else if(displayDetails){
			if((stat(entryPtr->d_name, &statBuf)) < 0){
				perror("Failed to Grab File Status\n");
			}
			//permissions  //  user  //  group  //  size  //  time of last modification
			permissions(permis, statBuf.st_mode);
			sprintf(temp,"%s  %d  %d  %ld  %ld  %s\n",
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
		
		/*if(displayInode){
			//add inode value to the output string 
			if((sprintf(temp,"%lu",entryPtr->d_ino)) <= 0){
				perror("Inode access failed");
			}
			strcat(fileData,temp);
		}
		if(displayDetails){
			
			strcat(fileData,temp);
		}
		
		
		
		//print output string
		printf("%s\n",fileData);	*/	
	}
	
	//close referenced directory
    closedir(dirPtr);
    return 0;
}