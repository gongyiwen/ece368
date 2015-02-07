#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MAX_LINE_LENGTH 10000000
#include "project1-A.h" 
#include <time.h>


int main(int argc, char * argv[])
{
  float lamda_0;
  float lamda_1;
  float u;
  int num = 1;
  srand(time(NULL));
  float aritime;
  int i;
  float  wait0 = 0;
  float  wait1 = 0;
  float avg_wait0;
  float avg_wait1; 
  float Utilization;
  float ttst=0;
  float ttsimT = 0;
  float avg_qL = 0; 
  float  qL = 0;
  //Task * head0 = NULL;
  //Task * head1 = NULL;
  Task * task0 = NULL;
  Task * task1 = NULL;
  //Recognizing inputs
  //reading a txt file
  if(argc == 2)
  {
    float * atime = malloc(sizeof(float)*MAX_LINE_LENGTH);
    int * priority = malloc(sizeof(int)*MAX_LINE_LENGTH);
    float * svtime = malloc(sizeof(float)*MAX_LINE_LENGTH);

    FILE * fp;
    char* filename;
    filename = argv[1];
    fp = fopen(filename, "r");
    i=0;
    while(!feof(fp))//not reaching end of file
    {
        fscanf(fp,"%f %d %f",&atime[i],&priority[i],&svtime[i]);
        i++;

    }
      num = i;
      num = num/2;
      for(i=0; i<num*2; i++)
      {
        if(priority[i]==0)
        {
          task0 = insertmodel2(task0, atime[i],svtime[i]);
        }
        else
        {
          task1 = insertmodel2(task1, atime[i],svtime[i]);
        }
      }
      //head0 = task0;
      //head1 = task1;
      free(atime);
      free(priority);
      free(svtime);
      ttst =  caltostime(task0, task1 );
      ttsimT = multitaskQ(task0, task1, &wait0, &wait1,&qL);
      fclose(fp);
  }

  //model 1
  else if(argc == 5)
  {
    lamda_0 = atof(argv[1]);
    //printf("lamda_0 %f\n", lamda_0);
    lamda_1 = atof(argv[2]);
    //printf("lamda_1 %f\n", lamda_1);
    u = atof(argv[3]);
    //printf("u %f\n", u);
    num = atoi(argv[4]);
    //printf("num %d\n", num);

    //creating task with priority 0 
    task0 = createtask(lamda_0, u);
   
    aritime = 0;
    task0 ->iatime = 0;
    task0 ->aritime = 0;
    task0 ->stime = calstime(u);


    for (i=0 ; i<num-1 ; i++)
    {
      task0 = insertmodel1(task0, lamda_0, u, aritime);
    }
  //head0 point to head of task0
    task1 = createtask(lamda_1, u);
   
    aritime = 0;
    task1 ->iatime = 0;
    task1 ->aritime = 0;
    task1 ->stime = calstime(u);
     for (i=0 ; i<num-1 ; i++)
      {
        task1 = insertmodel1(task1, lamda_1, u, aritime);
      }
      //head0 = task0;
      //head1 = task1;
 
    ttsimT = multitaskQ(task0, task1, &wait0, &wait1, &qL);//task0, task1 will not be changed
    ttst = caltostime(task0, task1);
    //Calculate average Queue length
  }
  else
  {
    printf("Wrong inputs");
  }

  /*while (head0 != NULL)
  {
    printf("priority0\n");
    printf("aritime %f \n",head0->aritime);
    printf("stime %f \n",head0->stime);
    head0 = head0->next;
  }

  while (head1 != NULL)
  {
    printf("priority1\n");
    printf("aritime %f \n",head1->aritime);
    printf("stime %f \n",head1->stime);
    head1 = head1->next;
  }*/
  avg_qL = qL/(2*num);
  //Calculate average untilization 
  Utilization = ttst /ttsimT;
  //Calculate average wait time
  avg_wait0 = wait0 / num;
  avg_wait1 = wait1 / num;
  FILE* fout = fopen("proj1-a_output","w");
  printf("Total wait time for group 0 %f\n", wait0);
  printf("Total wait time for group 1 %f\n", wait1);
  printf("Total time  %f\n", ttsimT);
  printf("Total server busy time %f\n", ttst);
  printf("Total queue length %f\n", qL);
  fprintf(fout,"%f\n%f\n%f\n%f\n",avg_wait0, avg_wait1, avg_qL, Utilization);
  task_destroy(task0);
  task_destroy(task1);
  return 0;
}