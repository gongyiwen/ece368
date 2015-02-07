/*
	Functions for project 1 A
	Do not modify this part of comments
	Teammates : Zhiyang Lin(lin382@purdue.edu) Yiwen Gong(gong32@purdue.edu)
*/
#include "project1-A.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_LINE_LENGTH 10000000

//model1 functions

//Model1 task structure

Task * createtask(float lamda, float u)
{
	Task * task = malloc(sizeof(Task));
	task -> iatime = caliatime(lamda);
	task -> aritime = 0;
	task -> stime = calstime(u);
	task -> tlq = 0;
	task -> next = NULL;
	return task;
}
Task * createnode(float atime, float stime)
{
  Task * queue = malloc(sizeof(Task));
  queue -> iatime = 0;
  queue -> aritime = atime;
  queue -> stime = stime;
  queue -> tlq = 0;
  queue -> next = NULL;
  return queue;
}

//Model2 creat queue
Task * insertmodel2(Task * head, float atime, float stime)
{	
   	if(head == NULL)
	{	
		head = createnode(atime, stime);
		return head;
	}
	head -> next = insertmodel2(head-> next , atime, stime);
	return head;
}

//Model1 tasks creation
Task * insertmodel1(Task * head, float lamda, float u, float aritime)
{
	
	if(head == NULL)
	{	

		head = createtask(lamda, u );
		head -> aritime = aritime + head -> iatime;
		return head;
	}
	head -> next = insertmodel1( head-> next ,lamda, u , head -> aritime);
	return head;
}
//function calculates waiting time and q length
float multitaskQ(Task * task0, Task * task1, float * wait0, float * wait1, float * qL)
{
	float currentT = 0;
	int i = 0;
	int * arrt = malloc(sizeof(int)*MAX_LINE_LENGTH);
	Task * head0 = task0;
	Task * head1 = task1;
	while(task0 != NULL &&  task1 !=NULL)		
	{
		if( task1->aritime <= currentT && task0->aritime > currentT )//group 1 has already arrived but not group 0 before previous job done
		{
			* wait1 += (currentT-task1->aritime);
			arrt[i++] = task1->aritime;
			task1->tlq = currentT;
			currentT += task1->stime;
			task1 = task1->next;
		}
		else if(task0->aritime <= currentT) // group 0 arrived before previous job done
		{
			* wait0 += (currentT-task0->aritime);
			arrt[i++] = task0->aritime;
			currentT += task0->stime;
			task0->tlq = currentT - task0->stime;
			task0 = task0->next;
		}
		else if( task1->aritime < task0->aritime)//group 1 arrived first in server's free time
		{
    		arrt[i++]= task1->aritime;
			currentT = task1->aritime + task1->stime;
			task1->tlq = currentT - task1->stime;
			task1 = task1->next;
		}
		else if(task1->aritime >= task0->aritime)//group 0 arrived dirst in server's free time
		{
    		arrt[i++] = task0->aritime;
			currentT = task0->aritime + task0->stime;
			task0->tlq = currentT - task0->stime;
			task0 = task0->next;
		}
		
	}
	while(task0 != NULL && task1 == NULL)//all of group1 have been served
	{
		if(task0->aritime <= currentT)
		{
			* wait0 += (currentT - task0->aritime);
    		arrt[i++] = task0->aritime;
			currentT += task0->stime;
			task0->tlq = currentT - task0->stime;
			task0 = task0->next;
		}
		else
		{
    		arrt[i++] = task0->aritime;
			currentT = task0->aritime + task0->stime;
			task0->tlq = currentT - task0->stime;
			task0 = task0->next;
		}

	}
	while(task1 != NULL && task0 == NULL)//all of group1 have been served
	{
		if( task1->aritime <= currentT)
		{
			*wait1 += (currentT - task1->aritime);
			arrt[i++] = task1->aritime;
			task1->tlq = currentT;
			currentT += task1->stime;
			task1 = task1->next;
		}
		else
		{
    		arrt[i++] = task1->aritime;
			currentT = task1->aritime + task1->stime;
			task1->tlq = currentT - task1->stime;
			task1 = task1->next;
		}
	}


	i = checka(arrt,i);
	int j;
	for(j = 0; j < i; j++)
	{
		* qL += qLength(head0,head1,arrt,j);
	}
	free(arrt);
	return currentT;
}

//cal q length
float qLength(Task * task0, Task * task1,int * arrt, int i)
{
	float queue = 0; 
	while(task0!= NULL)
	{
		if (task0->aritime <= arrt[i] && task0->tlq > arrt[i])
		{
			queue++;
		}
		task0= task0->next;
	}
	while(task1!= NULL)
	{
		if (task1->aritime <= arrt[i] && task1->tlq > arrt[i])
		{
			queue++;
		}
		task1= task1->next;
	}
	return queue;
}
//calculate in 
int checka(int * arrt, int size)
{
	int i,j,k;
	for (i = 0; i < size; i++) 
	{
      for (j = i + 1; j < size;j++) 
      {
         if (arrt[j] == arrt[i]) 
         {
            for (k = j; k < size; k++) 
            {
               arrt[k] = arrt[k + 1];
            }
            size--;
         } 
      }
   	}
   	return size;
}
float caliatime(float lamda)
{	
	float x = ((float) rand()/(RAND_MAX));
	float iatime = ( -1 / lamda ) * log( 1 - x );
	return ceil(iatime);
}

float  calstime(float u)
{
	float x = ((float) rand()/(RAND_MAX));
	float stime = ( -1 / u ) * log( 1 -x );
	return stime;
}

float  caltostime(Task * task0, Task * task1 )
{
	float ttst = 0;
	while(task0 != NULL)
	{
		ttst += task0->stime ;
		task0 = task0->next;
	}
	while(task1 != NULL)
	{
		ttst += task1->stime ;
		task1 = task1->next;
	}
	return ttst;
}

void task_destroy(Task * task)
{
	while(task != NULL)
	{
		Task * p= task -> next;
		free(task);
		task = p;
	}
}
