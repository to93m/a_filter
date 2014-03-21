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
  double mu = 0.0000000000001;
  int L = 1024, count, iteration; //フィルタ長
  double output, msError, msE_max, ePrev, sum;
  FILE *fp;

//-----------------読み込み-------------------------------------
  file_name = name + argv[1];
  stat(file_name.c_str(), &sd);
  int input_size = sd.st_size / 2;
  input_wave = new short[input_size + L + 1];
  for(int i = 0; i < input_size + L; i++) input_wave[i] = 0;
  read_wave(file_name.c_str(), input_wave, input_size);

  file_name = name + argv[2];
  stat(file_name.c_str(), &sd);
  int target_size = sd.st_size / 2;
  target_wave = new short[input_size + L + 2];
  for(int i = 0; i < input_size + L; i++) target_wave[i] = 0;
  read_wave(file_name.c_str(), target_wave, target_size);

  filter_gain = new double[L + 1];
  error = new double[input_size + L + 2];
  for(int i = 0; i < L; i++) filter_gain[i] = 0.0; //フィルタの初期化

  if((fp = fopen(argv[3], "w")) == NULL){
    cout << "Can't open" << argv[3] << endl;
    exit(EXIT_FAILURE);
  }

//-----------------フィルタ更新------------------------------------
  for(int iteration = 0; iteration < 100; iteration++){ //iteration回数の設定
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
    fprintf(fp, "%lf\n", 10.0*log10(msError/msE_max));
  }

  FILE *fout;
  if((fout = fopen(argv[4], "w")) == NULL){
    cout << "Can't open" << argv[4] << endl;
    exit(EXIT_FAILURE);
  }
  for(int i= 0; i < L; i++){
    fprintf(fout, "%f\n", filter_gain[i]);
  }
  fclose(fout);
  fclose(fp);

  delete [] input_wave;
  delete [] target_wave;
  delete [] filter_gain;
  delete [] error;

  return 0;
}
