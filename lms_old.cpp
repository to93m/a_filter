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

void read_wave(const char* file_name, short *wave, int data_size){
  FILE *fin;
  fin = fopen(file_name, "rb");
  fread(wave, sizeof(short), data_size, fin);
  fclose(fin);
}

int main(){
  short *target_wave, *input_wave;
  double *filter_gain;
  struct stat sd;
  const char* file_name;
  double mu = 0.00000001, output, error;
  int L = 500;
  int interation_num = 10;

  file_name = "./data/ComplexSound.raw";
  stat(file_name, &sd);
  int target_size = sd.st_size / 2;
  target_wave = new short[target_size];
  read_wave(file_name, target_wave, target_size);

  file_name = "./data/SpeechData.raw";
  stat(file_name, &sd);
  int input_size = sd.st_size / 2;
  input_wave = new short[input_size];
  read_wave(file_name, target_wave, input_size);

  FILE *fp;
  filter_gain = new double[L];
  fp = fopen("filter_gain3.txt", "r");
  fread(filter_gain, sizeof(double), L, fp);
  fclose(fp);

  //filter_gain = new double[L];
  //for(int i = 0; i < L; i++) filter_gain[i] = 0.0;
  //filter_gain[0] = 1;

  for(int i = 0; i < interation_num; i++){
    for(int j = 0; j < input_size; j++){
      output = 0.0;
      for(int k = 0; k < L; k++){
        if(j - k >= 0) output += (short)input_wave[j - k] * (double)filter_gain[k];
      }

      error = (short)target_wave[j] - (double)output;
      //cout << error << endl;

      for(int n = 0; n < L; n++){
        if(j - n >= 0) filter_gain[n] += mu * (double)error * (short)target_wave[j - n];
      }
    }
  }

  //for(int i = 0; i < L; i++) cout << (double)filter_gain[i] << endl;
  FILE *fout;
  fout = fopen("filter_gain3.txt", "w");
  for(int i = 0; i < L; i++){
    fprintf(fout, "%f\n", filter_gain[i]);
  }
  fclose(fout);

  return 0;
}
