#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct MyRecord *Rec;
void print(int,Rec *);
Rec * Init(int ,Rec *);
int The_Record();
void Coordinator_Splitter(int,Rec*,int ,char*,char*);
char* GetLastname(Rec );