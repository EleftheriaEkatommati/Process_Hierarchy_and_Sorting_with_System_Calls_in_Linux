#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZEofBUFF 20
#define SSizeofBUFF 6


 struct MyRecord{
	int  	custid;
	char 	LastName[SIZEofBUFF];
	char 	FirstName[SIZEofBUFF];
	char	postcode[SSizeofBUFF];
};

typedef struct MyRecord *Rec;

// Function to perform the bubble sort
Rec* bubbleSort(Rec* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Compare LastNames and swap if needed
            if (strcmp(arr[j]->LastName, arr[j+1]->LastName) > 0) {
                Rec temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}

int main(int argc,char *argv[]){
    int numRecords;
    int index;
   
 
   
    read(0,&index,sizeof(int));
    read(0,&numRecords,sizeof(int));
    Rec* records;
    records = malloc(numRecords* sizeof(Rec));
    for(int i=0; i<numRecords;i++)
    {   records[i] = malloc(sizeof(struct MyRecord));
        read(0,records[i],sizeof(struct MyRecord));
    }
    
  
   

    // Perform the bubble sort
    records=bubbleSort(records, numRecords);

 
    write(1,&index,sizeof(int));
    write(1,&numRecords,sizeof(int));

    for(int i=0; i<numRecords;i++)
    {   
        write(1,records[i],sizeof(struct MyRecord));
    }
    for(int i=0;i<numRecords;i++){
        free(records[i]);
    }
    free(records);

    return 0;
}
