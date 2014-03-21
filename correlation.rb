#ruby read_raw.rb input_file_name
require './plot'

def read_wave(file_name, mode)
  wave ||= []
  File.open("./data/"+ file_name) {|file|
    if mode == "16"
      binary = file.read
      wave = binary.unpack('s*')

    elsif mode == "64" #64bitの収録音源は8バイトで読み込まないといけない
      binary = file.read
      wave = binary.unpack('d*') #d* は符号付きの浮動小数点を指定

    elsif mode == "text"
      file.each{|line|
        wave << line.chomp.to_f
      }
    end
  }
  return wave
end

def correlation(signal, signal2)
  sum = 0.0
  signal.length.times{|i|
    signal2.length.times{|j|
      sum += signal[i] * signal2[j]
    }
  }
  return sum
end

input = read_wave(ARGV[0], ARGV[1])
input2 = read_wave(ARGV[2], ARGV[3])

#input.length.times{|i|
#  puts input[i]
#  puts input2[i]
#}

result = correlation(input, input2)
p result

x_axis = (0..input.length).to_a
x_axis.collect!{|n| n /= 16000.0}

plot(x_axis, input)
plot(x_axis, input2)
