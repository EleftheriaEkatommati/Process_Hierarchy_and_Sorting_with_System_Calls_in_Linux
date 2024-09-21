 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// Function to perform the quicksort
void swap(Rec *a, Rec *b) {
    Rec temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Rec arr[], int low, int high) {
    char pivot[SIZEofBUFF];
    strcpy(pivot, arr[high]->LastName);

    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (strcmp(arr[j]->LastName, pivot) < 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(Rec arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to perform the quicksort on an array of MyRecord
Rec* quickSortWrapper(Rec arr[], int n) {
    quickSort(arr, 0, n - 1);
    return arr;
}

int main() {
   
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
    
  
    records=quickSortWrapper(records, numRecords) ;

  
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
