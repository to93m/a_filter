#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <float.h>
#include <iomanip>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

int main(){
  short *base_wave, *out_wave;
  double *impulse;
  FILE *fin, *fin2, *fout;
  struct stat stat_data;

  fin = fopen("./data/ImpulseResponse.raw", "rb");
  stat("./data/ImpulseResponse.raw", &stat_data);
  int impulse_sample = stat_data.st_size / 8;
  impulse = new double[impulse_sample];
  fread(impulse, sizeof(double), impulse_sample, fin);
  fclose(fin);

  fin2 = fopen("./data/SpeechData.raw", "rb");
  stat("./data/SpeechData.raw", &stat_data);
  int base_sample = stat_data.st_size / 2;
  base_wave = new short[base_sample];
  fread(base_wave, sizeof(short), base_sample, fin2);
  fclose(fin2);

  double max = 0.0;
  for(int i = 0; i < 20; i++){
    if (max < impulse[i]) max = impulse[i];
  }

  for(int i = 0; i < impulse_sample; i++){
    if (impulse[i] != 0) impulse[i] = impulse[i] / max;
    else impulse[i] = 0;
  }

  //convolution
  out_wave = new short[impulse_sample + base_sample];
  double temp;
  for(int i = 0; i < base_sample; i++){
    out_wave[i] = 0;
    for(int j = 0; j < impulse_sample; j++){
      if(i - j >= 0){
        temp = (short)base_wave[i - j] * (double)impulse[j];      
        out_wave[i] += (short)temp;
      }
    }
  }  

  for(int i = 0; i < 10; i++){
    cout << (double)impulse[i] << endl;
  }

  fout = fopen("./data/test.raw", "wb");
  //for(int i = 0; i < base_sample + impulse_sample; i++){
    fwrite(out_wave, sizeof(short), (base_sample + impulse_sample), fout);
  //}

  fclose(fout);
  delete [] base_wave;
  delete [] impulse;
  delete [] out_wave;

  /*
     name = "./data/SpeechData.raw";
     ifstream fin(name, ios::in | ios::binary);

     ofstream fout;
     fout.open("./data/test.raw", ios::out|ios::binary|ios::trunc);

     i = 0;
     while(!fin.eof()){
     fin.read((char *) &base_wave[i], sizeof(short));
     fout.write((char *) &base_wave[i], sizeof(short));
     i++;
     }
     fin.close();
     fout.close();
     */
}
