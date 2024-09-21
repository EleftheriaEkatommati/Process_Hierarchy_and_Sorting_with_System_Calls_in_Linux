#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>  // Include this for open and close functions
#include "Processes.h"
#define MAX_PATH_LENGTH 256

int main(int argc, char *argv[])
{
    int k;
    int fpb;  // Change from FILE* to int for file descriptor
    Rec *records = NULL;
    long lSize;
    int numOfrecords;
    char *sorting1;
    sorting1 = NULL;
    char *sorting2;
    sorting2 = NULL;

    char file1[MAX_PATH_LENGTH] = "./";
    char file2[MAX_PATH_LENGTH] = "./";
    for(int i=1; i<=7;i++)
    {

   
        if (strcmp(argv[i], "-i") == 0)
        {
            fpb = open(argv[i+1], O_RDONLY);  
            if (fpb == -1)
            {
                printf("Cannot open binary file\n");
                return 1;
            }

            // check number of records
            lSize = lseek(fpb, 0, SEEK_END);
            numOfrecords = (int)lSize / The_Record();
            printf("Records found in file: %d\n", numOfrecords);
            records = Init(numOfrecords, records);
            if (records == NULL)
            {
                printf("Memory allocation failed\n");
                close(fpb);  // Close the file descriptor before returning
                return 1;
            }

            // Set the file pointer back to the beginning of the file
            lseek(fpb, 0, SEEK_SET);

            // Read records into the dynamically allocated memory using read
            for (int i = 0; i < numOfrecords; i++)
            {
                read(fpb, records[i], The_Record());
            }
            printf("helloooo\n");
            // Now we have the records stored in the dynamically allocated memory

          

            close(fpb);  // Close the file descriptor
            print(numOfrecords, records);

            // Free dynamically allocated memory

        
        }

        if (strcmp(argv[i], "-k") == 0)
        {
            k = atoi(argv[i+1]);
        }
        if (strcmp(argv[i], "-e1") == 0)
        { 
            sorting1 = (char *)malloc(strlen(argv[i+1]) + 1); //Unify each sorting1 or sorting2 with "./" 
            strcpy(sorting1, argv[i+1]);
            strcat(file1, sorting1);
        }
        if (strcmp(argv[i], "-e2") == 0)
        {
            sorting2 = (char *)malloc(strlen(argv[i+1]) + 1);
            strcpy(sorting2, argv[i+1]);
            strcat(file2, sorting2);
        }
    }
    Coordinator_Splitter(k, records, numOfrecords, file1, file2);
    free(records);
    return 0;
}
