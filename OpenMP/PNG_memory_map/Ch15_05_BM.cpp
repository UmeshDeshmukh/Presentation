//------------------------------------------------
//               Ch15_05_BM.cpp
//------------------------------------------------

#include <iostream>
#include "Ch15_05.h"
#include "BmThreadTimer.h"
#include "OS.h"

using namespace std;

void ConvertRgbToGs_BM(void)
{
    cout << "\nRunning benchmark function ConvertRgbToGs_BM - please wait\n";

    const char* fn_rgb = "../beach_sunset.png";

    ImageMatrix im_rgb(fn_rgb, PixelType::Rgb32);
    int im_h = im_rgb.GetHeight();
    int im_w = im_rgb.GetWidth();
    size_t num_pixels = im_h * im_w;
    ImageMatrix im_gs1(im_h, im_w, PixelType::Gray8);
    ImageMatrix im_gs2(im_h, im_w, PixelType::Gray8);
	ImageMatrix im_gs3(im_h, im_w, PixelType::Gray8);
    RGB32* pb_rgb = im_rgb.GetPixelBuffer<RGB32>();
    uint8_t* pb_gs1 = im_gs1.GetPixelBuffer<uint8_t>();
    uint8_t* pb_gs2 = im_gs2.GetPixelBuffer<uint8_t>();
	uint8_t* pb_gs3 = im_gs3.GetPixelBuffer<uint8_t>();

    const size_t num_it = 500;
    const size_t num_alg = 7;
    BmThreadTimer bmtt(num_it, num_alg);   
    
    for (size_t i = 0; i < num_it; i++){bmtt.Start(i, 0);
        ConvertRgbToGsCpp(pb_gs1, pb_rgb, num_pixels, g_Coef);
        bmtt.Stop(i, 0);}
		
    for (size_t i = 0; i < num_it; i++){bmtt.Start(i, 1);
        ConvertRgbToGs_(pb_gs2, pb_rgb, num_pixels, g_Coef);
		bmtt.Stop(i, 1);}
	for (size_t i = 0; i < num_it; i++){bmtt.Start(i, 2);
        ConvertRgbToGsCpp_omp(pb_gs3, pb_rgb, num_pixels, g_Coef,2);
        bmtt.Stop(i, 2);}
	for (size_t i = 0; i < num_it; i++){bmtt.Start(i, 3);
        ConvertRgbToGsCpp_omp(pb_gs3, pb_rgb, num_pixels, g_Coef,4);
        bmtt.Stop(i, 3);}
	for (size_t i = 0; i < num_it; i++){bmtt.Start(i, 4);
        ConvertRgbToGsCpp_omp(pb_gs3, pb_rgb, num_pixels, g_Coef,8);
        bmtt.Stop(i, 4);}
	for (size_t i = 0; i < num_it; i++){bmtt.Start(i, 5);
        ConvertRgbToGsCpp_omp(pb_gs3, pb_rgb, num_pixels, g_Coef,16);
        bmtt.Stop(i, 5);}
	for (size_t i = 0; i < num_it; i++){bmtt.Start(i, 6);
        ConvertRgbToGsCpp_omp(pb_gs3, pb_rgb, num_pixels, g_Coef,32);
        bmtt.Stop(i, 6);}
		
    

    string fn = bmtt.BuildCsvFilenameString("ConvertRgbToGs_BM_7new");
    bmtt.SaveElapsedTimes(fn, BmThreadTimer::EtUnit::MicroSec, 7);
    cout << "Benchmark times save to file " << fn << '\n';
}
