#include<iostream>
#include<cstdio>
#include<omp.h>

int t_across;
#pragma omp threadprivate(t_across)
int main(int argc,char *argv[]){
	int n_t,t_id =555;
	#pragma omp parallel private(t_id) shared(n_t)
	{   
	    t_across = omp_get_thread_num();
		t_id = omp_get_thread_num();
		
		#pragma omp single 
		{
		    for(int i=0;i<1000000000;i++);
            n_t  = omp_get_num_threads();
			printf("Printing from single t_id: %d\n",t_id);
		}
		#pragma omp single nowait
		{   
		    printf("Printing from nowait t_id: %d\n",t_id);
		}
		#pragma omp master
		{
			printf("Printing from master thread with t_id: %d\n",t_id);
		}
				
		printf("In thread %d of %d\n",t_id,n_t);
	}
	
	printf("Outside omp parallel region t_id: %d\n",t_id);
	printf("*****************************************\n");
	
	#pragma omp parallel
	{   
	    t_id = omp_get_thread_num();
	    printf("t_id: %d | t_across: %d\n",t_id,t_across);
	}
	return 0;
}