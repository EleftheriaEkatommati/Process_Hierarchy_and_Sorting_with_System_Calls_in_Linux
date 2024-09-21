#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/times.h> 
#include <signal.h>
#include "Processes.h"
#define SIZEofBUFF 20
#define SSizeofBUFF 6

int signals_count=0;

 struct MyRecord{
	int  	custid;
	char 	LastName[SIZEofBUFF];
	char 	FirstName[SIZEofBUFF];
	char	postcode[SSizeofBUFF];
};


int The_Record()
{   int num;
     num=sizeof(struct MyRecord);
    return num;    
}
Rec * Init(int num,Rec *matrix)
{
    matrix = malloc(num * sizeof(Rec));
    for (int i = 0; i < num; i++){

            matrix[i]=malloc(sizeof(struct MyRecord));
        }
    return matrix;
}


void print(int num, Rec * matrix)
{
    for (int i = 0; i < num; i++) 
    {
        printf("%d %s %s %s\n", matrix[i]->custid, matrix[i]->LastName, matrix[i]->FirstName, matrix[i]->postcode);
    }


}
char* GetLastname(Rec pointer){
    return pointer->LastName;

}



void Sorter(Rec* matrix,int input_0,int input_1,int output_0,int output_1,int Numrec,int index,int flag,char*file1,char*file2)
{ 

    write(input_1,&index,sizeof(int));
    write(input_1,&Numrec,sizeof(int));
    for (int i = 0; i < Numrec; ++i) {
        write(input_1, matrix[i], sizeof(struct MyRecord));
    }

    close(input_1);

        // Redirect stdin to read from the input pipe
    dup2(input_0, STDIN_FILENO);

        // Close the unused end of the input pipe
    close(input_0);

        // Close the read end of the output pipe
    close(output_0);
    
        // Redirect stdout to write to the output pipe
    dup2(output_1, STDOUT_FILENO);

        // Close the unused end of the output pipe
    close(output_1);


   if(flag==1)
   {
    //execlp("./sorting1", "./sorting1", NULL);
    execlp(file1,file1, NULL);
   }
   else if(flag==0)
   {
    //execlp("./sorting2", "./sorting2", NULL);
    execlp(file2, file2, NULL);
   }
   
    // If exec fails
    perror("execlp");
    exit(EXIT_FAILURE);
}



Rec * mergeSortedArrays(Rec * arr1, int size1, Rec *arr2, int size2) {
    int i = 0, j = 0, k = 0;
    Rec *result_matrix;
    result_matrix=malloc((size1+size2) *sizeof(Rec));
    // Merge until we have elements in both arrays
    while (i < size1 && j < size2) {
        // Compare elements based on the second element (string) of each row
        if (strcmp(arr1[i]->LastName, arr2[j]->LastName) <= 0) {
            // If the element from arr1 is smaller or equal, copy it to the result array
            result_matrix[k] = arr1[i];
            i++;
        } else {
            // If the element from arr2 is smaller, copy it to the result array
            result_matrix[k] = arr2[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements from arr1, if any
    while (i < size1) {
        result_matrix[k] = arr1[i];
        i++;
        k++;
    }

    // Copy the remaining elements from arr2, if any
    while (j < size2) {
        result_matrix[k] = arr2[j];
        j++;
        k++;
    }
    return result_matrix;
}


void handle_custom_signal(int signum) {

        printf("Received custom signal %d from child process\n", signum);
        signals_count++;
    
} 

void Splitter(Rec* records, int k_sorters,int numofnode,int num_rec,int fd_write,char*file1,char*file2)
{
    
    int c,y;
    int child_pid;
    int index;
    int flag2;
    int flag_sorter_before;
    int flag_sorter_after;
    int start2,split2,rest2,start3;
    Rec*unsoterd_matrix;
    Rec ** new_matrixes;
    int *sizes;
    double t1 , t2 , cpu_time ;
    struct tms tb1 , tb2 ;
    double ticspersec ;
    flag_sorter_before=0;
    flag_sorter_after=1;
    rest2=0;
    index=0;
    flag2=0;
    y=0;
    start2=0;

    int pipefd_input[2];  
    int pipefd_output[2]; 

    if (signal(SIGUSR1, handle_custom_signal) == SIG_ERR) {
        perror("Error setting up signal handler");
        exit(EXIT_FAILURE);
    }
    

 
    if (pipe(pipefd_input) == -1 || pipe(pipefd_output) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if(num_rec%numofnode==0) // Check if we have a perfect division if not we calculate the mod and the rest we give it to the first sorter
    {
        split2=num_rec/numofnode;
    }
    else
    {
        split2=(int)num_rec/numofnode;
        rest2=num_rec%numofnode;
        flag2=1;
        
    }
    
    for(int i=numofnode; i>0;i--)
    {
        child_pid = fork();
        c=0;
        if(flag_sorter_before==0) //here we control whatsorter will be used for sorting
        {
            flag_sorter_after=1;
        }
        else if(flag_sorter_before==1)
        {
            flag_sorter_after=0;
        } 
        if (child_pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        ticspersec = ( double ) sysconf( _SC_CLK_TCK ) ; 
        t1 = ( double ) times (& tb1 ) ;
        unsoterd_matrix=malloc(num_rec*sizeof(Rec));
        if (child_pid == 0) {
            // This is the child process
           
           
            printf("I am insplitter\n");
             if(i==numofnode && flag2==1)
            {   
                start3=index; // here we store at waht cell we stop sorting
                start2=index+split2+rest2; // calculate the next new start
                
               
            }
            else
            {   
                start3=index;
                start2=index+split2;
                
            }
           
            for(int j=start3; j<start2;j++)
            {
                    unsoterd_matrix[c]=records[j];
                    c++;

            }
            index=start2;// store the new start for the next loop
            
            printf("This the matrix that the sorter will sort:\n");
            print(start2-start3,unsoterd_matrix);

            Sorter(unsoterd_matrix,pipefd_input[0],pipefd_input[1],pipefd_output[0],pipefd_output[1],start2-start3,index,flag_sorter_after,file1,file2);
          
            free(unsoterd_matrix);

            if (kill(getppid(), SIGUSR1) == -1) {
                perror("Error sending signal to parent process");
                exit(EXIT_FAILURE);
            }
            
            break;  // Exit the loop in the child
        } else {
           
            
            wait(NULL);
            t2 = (double)times(&tb2);
            cpu_time = (double) (( tb2.tms_utime + tb2.tms_stime) - (tb1.tms_utime + tb1.tms_stime ) ) ;
            printf ( " Run time was % lf sec ( REAL time ) although we used the CPU for % lf sec ( CPU time ).\n ",(t2 - t1) / ticspersec , cpu_time / ticspersec );
           
       
            flag_sorter_before=flag_sorter_after;
            
            int size;
            read(pipefd_output[0],&index,sizeof(int));// Read the sorted matrix from the output pipe
            read(pipefd_output[0],&size,sizeof(int));
            
            Rec *sorted_matrix;
            sorted_matrix=malloc(size*sizeof(Rec));
            for(int i=0; i<size; i++)
            {
                 sorted_matrix[i]=malloc(sizeof(struct MyRecord));
                 read(pipefd_output[0], sorted_matrix[i], sizeof(struct MyRecord));

            }

            printf("A sorter just finished \n"); 
           // printf("%d\n",size);
            //printf("%d\n",index);
           
           for (int i = 0; i < size; i++)
            {
                printf("ID: %d, LastName: %s, FirstName: %s, Postcode: %s\n",
                sorted_matrix[i]->custid, sorted_matrix[i]->LastName, sorted_matrix[i]->FirstName, sorted_matrix[i]->postcode);
            }
                
            if(i==numofnode)
            {   sizes=malloc(numofnode*sizeof(int));
                new_matrixes=malloc(sizeof(Rec *));
                new_matrixes[y]=sorted_matrix;
                sizes[0]=size;
                
            }
            else
            {   y=y+1;
                new_matrixes=realloc(new_matrixes,(y+1)*sizeof(Rec *));
                new_matrixes[y]=sorted_matrix;
                sizes[y]=size;
            }
             

            
            }
            

           //index=index+start2;

    }
    if(child_pid!=0) // Here we are doing the merging of the osrted arrays
    {   
        
        Rec *result_matrix;
        int before_size;
        
        for(int i=0;i<=y;i++) //we take the first two sorted matrixes and then we we merged them, then we take the next matrix and we marge it with the merged matrix we creat before,and we go on
        {
           
            if(y==0)
            {
                result_matrix=new_matrixes[0];
                break;
            }
           if(i==0)
           {
               
                result_matrix=mergeSortedArrays(new_matrixes[i],sizes[i],new_matrixes[i+1],sizes[i+1]);
                for(int k=0;k<sizes[i]+sizes[i+1];k++)
                {
                    printf("ID: %d, LastName: %s, FirstName: %s, Postcode: %s\n",
                    result_matrix[k]->custid, result_matrix[k]->LastName, result_matrix[k]->FirstName, result_matrix[k]->postcode);
                }
                before_size=sizes[i]+sizes[i+1]; 
                free(new_matrixes[i]);
                free(new_matrixes[i+1]);
                i=i+1;
                printf("MERGE:%d\n",i);
                
           }
           else
           {     
                result_matrix=mergeSortedArrays(new_matrixes[i],sizes[i],result_matrix,before_size);
                before_size=sizes[i]+before_size;
                for(int k=0;k<before_size;k++)
                {
                    printf("ID: %d, LastName: %s, FirstName: %s, Postcode: %s\n",
                    result_matrix[k]->custid, result_matrix[k]->LastName, result_matrix[k]->FirstName, result_matrix[k]->postcode);
                }
                printf("MERGE:%d\n",i);
                free(new_matrixes[i]);
                 
           }
           
            
        }
        

        close(pipefd_input[0]);
           

             //Close the write end of the output pipe
        close(pipefd_output[1]);

             //Close the write end of the input pipe to signal the end of input
        close(pipefd_input[1]);
    
        close(pipefd_output[0]);
        
        printf("Just Printing here\n");
        write(fd_write,&num_rec,sizeof(int));
        //write(fd_write,&before_size,sizeof(int));
        for (int i = 0; i < num_rec; ++i) {
            write(fd_write, result_matrix[i], sizeof(struct MyRecord));
        }
       
        close(fd_write);
       
        free(new_matrixes);
        for (int i = 0; i < num_rec; ++i) {
            free(result_matrix[i]);
        }
        free(result_matrix);
  

    }
   

}




void Coordinator_Splitter(int k_sorters,Rec*records,int NumofRec,char *file1,char*file2)
{
    int i,k;
    int child_pid,index; 
    int split,start,rest;
    int flag; 
    int y;
    int start2;
    int root_splitter[2];
    Rec *no_sorted_matrix;
    int *sizes; 
    Rec **new_matrixes;
    flag=0;start=0;rest=0;index=0;start2=0;
    y=0;
    
  
    if (signal(SIGUSR2, handle_custom_signal) == SIG_ERR) {
        perror("Error setting up signal handler");
        exit(EXIT_FAILURE);
    }
    
    pipe(root_splitter); 
    if(pipe(root_splitter) == -1 ) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if(NumofRec%k_sorters==0)
    {
        split=NumofRec/k_sorters; // precise division
    }
    else
    {
       split=(int)NumofRec/k_sorters;
       rest=NumofRec%k_sorters;
       flag=1; 
    }
    

    for (i = 0; i < k_sorters; i++) {
        child_pid = fork();
        k=0;
        
        if (child_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        if (child_pid == 0) 
        {
            // This is the child process
            
            

            if(i==0 && flag==1)
            {   
                start2=index;
                start=index+split+rest;
                printf("%d\n",start);
               
            }
            else
            {   
                start2=index;
                start=index+split;
                printf("%d\n",start);
            }
            no_sorted_matrix=malloc((start-start2)*sizeof(Rec));
            for(int j=start2; j<start;j++){
                    no_sorted_matrix[k]=records[j];
                    k++;

            }
            printf("I am in Coordinator\n");
            printf("-->The index of the record we stop sorting: %d\n",index);
            printf("Going in the splitter\n");
            printf("These ara how many reacords will be sorted:%d\n",start-start2); // this is how many records are going into the splitter
            print(start-start2,no_sorted_matrix);
            
            Splitter(no_sorted_matrix,k_sorters,k_sorters-i,start-start2,root_splitter[1],file1,file2);
            free(no_sorted_matrix);

            if (kill(getppid(), SIGUSR2) == -1) {
                perror("Error sending signal to parent process");
                exit(EXIT_FAILURE);
            }
            
            
            break;  // Exit the loop in the child
        } else { 
            
            wait(NULL);
            int num;
            
            // This is the parent process
            read(root_splitter[0],&num,sizeof(int));
            index=num+index;
            Rec *sorted_matrix;
           

            sorted_matrix=malloc(num*sizeof(Rec));
            for(int i=0; i<num; i++){
                 sorted_matrix[i]=malloc(sizeof(struct MyRecord));
                 read(root_splitter[0], sorted_matrix[i], sizeof(struct MyRecord));

            }
            if(i==0)
            {   sizes=malloc(num*sizeof(int));
                new_matrixes=malloc(sizeof(Rec *));
                new_matrixes[y]=sorted_matrix;
                sizes[0]=num;
                
            }
            else
            {   y=y+1;
                new_matrixes=realloc(new_matrixes,(y+1)*sizeof(Rec *));
                new_matrixes[y]=sorted_matrix;
                sizes[y]=num;
            }
            
            printf("Parent process with PID %d created child with PID %d\n", getpid(), child_pid);
        }
    }

    // Code after the loop will be executed by both parent and child processes
    if (child_pid != 0) {

        Rec *result_matrix;
        int before_size;
     
        for(int i=0;i<=y;i++)
        {
          
           if(i==0)
           {
                result_matrix=mergeSortedArrays(new_matrixes[i],sizes[i],new_matrixes[i+1],sizes[i+1]);
                for(int k=0;k<sizes[i]+sizes[i+1];k++)
                {
                    printf("ID: %d, LastName: %s, FirstName: %s, Postcode: %s\n",
                    result_matrix[k]->custid, result_matrix[k]->LastName, result_matrix[k]->FirstName, result_matrix[k]->postcode);
                }
                before_size=sizes[i]+sizes[i+1];
                free(new_matrixes[i]);
                free(new_matrixes[i+1]);
                i=i+1;
                printf("MERGE:%d\n",i);
                
           }
           else
           {     
                result_matrix=mergeSortedArrays(new_matrixes[i],sizes[i],result_matrix,before_size);
                before_size=sizes[i]+before_size;
                for(int k=0;k<before_size;k++)
                {
                    printf("ID: %d, LastName: %s, FirstName: %s, Postcode: %s\n",
                    result_matrix[k]->custid, result_matrix[k]->LastName, result_matrix[k]->FirstName, result_matrix[k]->postcode);
                }
                printf("MERGE:%d\n",i);
                free(new_matrixes[i]);
                
           }
           
            
        }


        close(root_splitter[0]);
           

             //Close the write end of the output pipe
        close(root_splitter[1]);
        printf("The total signals are :%d\n",signals_count);

             //Close the write end of the input pipe to signal the end of input
        
        free(new_matrixes);
        for (int i = 0; i < before_size; ++i) {
            free(result_matrix[i]);
        }
        free(result_matrix);
        printf("END\n");
        
  

    
    }
}