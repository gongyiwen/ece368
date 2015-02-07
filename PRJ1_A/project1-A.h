/*
Header file for project 1 A
Do not modify this part of comments
Teammates : Zhiyang Lin(lin382@purdue.edu) Yiwen Gong(gong32@purdue.edu)
*/
#ifndef project1_H
#define project1_H

//Model1 queue
typedef struct task_s
{
	float iatime;
	float aritime;
    float stime;
    float tlq;
    struct task_s * next;
} Task;

Task * createtask(float lamda, float u);

Task * createnode(float atime, float stime);

Task * insertmodel1(Task * head,  float lamda, float u, float aritime);

Task*  insertmodel2(Task * task, float atime, float stime);

float  multitaskQ(Task * task0, Task * task1, float * wait0, float * wait1, float * qL);

int checka(int * arrt, int size);

float  caliatime(float lamda0);

float  calstime(float u);

float  caltostime(Task * task0, Task * task1);

float qLength(Task * task0, Task * task1, int * arrt , int i);

void task_destroy(Task * task);

#endif 