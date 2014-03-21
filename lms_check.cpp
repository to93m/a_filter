//./data_p input_file target_file error_out_file gain_out_file 
#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

void read_wave(const char* file_name, short *wave, int data_size){
  FILE *fin;
  if((fin = fopen(file_name, "rb")) == NULL){
    cout << "Can't open " << file_name << endl;
    exit(EXIT_FAILURE);
  }
  fread(wave, sizeof(short), data_size, fin);
  fclose(fin);
}

int main(int argc, char *argv[]){
  short *target_wave, *input_wave;
  double *filter_gain, *error;
  struct stat sd;
  string name = "./data/", file_name;
  double mu = 0.001;
  int L = 10, count, iteration; //フィルタ長
  double output, msError, msE_max, ePrev, sum;

  int input_size = 10;
  input_wave = new short[10];
  input_wave[0] = 10;

  target_wave = new short[10];
  target_wave[5] = 1;

  filter_gain = new double[L + 1];
  error = new double[input_size + L + 2];
  for(int i = 0; i < L; i++) filter_gain[i] = 0.0; //フィルタの初期化

  for(int iteration = 0; iteration < 500; iteration++){ //iteration回数の設定
    for(int i = 0; i < input_size;i++){
      output = 0.0;
      for(int j = 0; j < L; j++){
        if(i - j >= 0){
          output += filter_gain[j] * (double)input_wave[i - j]; 
        }
      }
      error[i] = (double)target_wave[i] - output;

      for(int j = 0; j < L; j++){
        if(i - j >= 0){
          filter_gain[j] += 2.0 * mu * error[i] * (double)input_wave[i - j];
        }
      }
    }

    sum = 0.0;
    for(int j = 0; j < input_size; j++) sum += pow(error[j], 2);
    msError = sum / (double)input_size;

    if(iteration == 0) msE_max = msError;
    cout << iteration;
    printf(": %lf\n", 10.0*log10(msError/msE_max));
  }

  for(int i= 0; i < L; i++){
    printf("%f\n", filter_gain[i]);
  }

  delete [] input_wave;
  delete [] target_wave;
  delete [] filter_gain;
  delete [] error;

  return 0;
}
