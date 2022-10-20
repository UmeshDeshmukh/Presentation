#include<iostream>
//#include<AudioFile.h>
//#include<fftw3.h>
#include<math.h>
#include<string>
#include<cstdio>
#include<omp.h>
#include<inttypes.h>

#define REAL 0
#define IMAG 1
#define no_pxls 2400*1600

using namespace std;
//AudioFile<double> audioFile;
//string path = "/mnt/d/DSP/AudioSrc/";
//string out_path = "/mnt/d/DSP/CPP/FFTW3_App/";
typedef struct pixel{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};
struct pxlArrys{
	uint8_t r[no_pxls];
	uint8_t g[no_pxls];
	uint8_t b[no_pxls];
	uint8_t a[no_pxls];
};
typedef struct bw_pixel{
	uint8_t val;
};
struct pixel in_img[no_pxls];
struct bw_pixel out_img[no_pxls];
struct pxlArrys pArrays;
void init2(){
	for(int i=0;i<no_pxls;i++){
		int val = rand()%256;
		pArrays.r[i] = val;
		pArrays.g[i] = val;
		pArrays.b[i] = val;
	}
}
void init(){
	for(int i=0;i<no_pxls;i++){
		int val = rand()%256;
		in_img[i].r = val;
		in_img[i].g = val;
		in_img[i].b = val;
	}
}

double omp_version()
{   
    double start =0.0, end =0.0;
	start = omp_get_wtime();
	
	omp_set_num_threads(2); 
	#pragma omp parallel 
	{
	#pragma omp for 	
	for(int j=0;j<no_pxls;j++){
		//printf("From thread %d\n",omp_get_thread_num());
		float val_f =in_img[j].r*0.2126+
		             in_img[j].g*0.7152+
		             in_img[j].b*0.0722;
		if(val_f<0.0) val_f = 0.0;
        if(val_f>255.0) val_f = 255.0;		
		out_img[j].val = (uint8_t)val_f;
	}
	}
	end = omp_get_wtime();
	return end-start;
}
double scalar_ver()
{   
    double start =0.0, end =0.0;
	start = omp_get_wtime();
	for(int j=0;j<no_pxls;j++){
		//printf("From thread %d\n",omp_get_thread_num());
		float val_f =in_img[j].r*0.2126+
		             in_img[j].g*0.7152+
		             in_img[j].b*0.0722;
		if(val_f<0.0) val_f = 0.0;
        if(val_f>255.0) val_f = 255.0;		
		out_img[j].val = (uint8_t)val_f;
	}
	
	end = omp_get_wtime();
	return end-start;
}
double omp_version2()
{   
    double start =0.0, end =0.0;
	start = omp_get_wtime();
	
	omp_set_num_threads(4); 
	#pragma omp parallel 
	{
	#pragma omp for 	
	for(int j=0;j<no_pxls;j++){
		//printf("From thread %d\n",omp_get_thread_num());
		float val_f =pArrays.r[j]*0.2126+
		             pArrays.g[j]*0.7152+
		             pArrays.b[j]*0.0722;
		if(val_f<0.0) val_f = 0.0;
        if(val_f>255.0) val_f = 255.0;		
		out_img[j].val = (uint8_t)val_f;
	}
	}
	end = omp_get_wtime();
	return end-start;
}
double scalar_ver2()
{   
    double start =0.0, end =0.0;
	start = omp_get_wtime();
	for(int j=0;j<no_pxls;j++){
		//printf("From thread %d\n",omp_get_thread_num());
		float val_f =pArrays.r[j]*0.2126+
		             pArrays.g[j]*0.7152+
		             pArrays.b[j]*0.0722;
		if(val_f<0.0) val_f = 0.0;
        if(val_f>255.0) val_f = 255.0;		
		out_img[j].val = (uint8_t)val_f;
	}
	
	end = omp_get_wtime();
	return end-start;
}
void check_mismatch(){
	int flag = 0;
	for(int k=0;k<no_pxls;k++){
		if(k != out_img[k].val) 
		{
			printf("Value mismatch at %d\n",k);
			flag = -1;
			break;
		}
				
	}
	if(flag == 0)printf("No value mismatched.\n");
}
int main(){
	
	double exe_time1 = 0.0,exe_time2=0.0;	
	
	init2();
	
	for(int j=0;j<250;j++){
	scalar_ver2();
	}
	for(int j=0;j<250;j++){
	exe_time1 += scalar_ver2();
	}	
	exe_time1 = exe_time1/250.0;
	check_mismatch();
	printf("Scalar2 execution time: %f\n",exe_time1);
	return 0;
}