#ruby convo.rb input_file_name 畳み込むもの
#ruby convo.rb SpeechData.raw 16 impulse_response_file.raw mode output_file_name
require './plot'

base_wave ||= []
impulse_response ||= []
x_axis ||= []

def read_wave(file_name, mode)
  wave ||= []
  File.open("./data/"+ file_name) {|file|
    if mode == "s"
      binary = file.read
      wave = binary.unpack('s*')

    elsif mode == "d" #64bitの収録音源は8バイトで読み込まないといけない
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

base_wave = read_wave(ARGV[0], ARGV[1])
impulse_response = read_wave(ARGV[2], ARGV[3])

impulse_response.length.times{|n|
  base_wave << 0
}

#max = impulse_response.max
#impulse_response.collect!{|n| n /= max} #最大を1にする
#max = base_wave.max
#base_wave.collect!{|n| n /= max} #最大を1にする

out_wave ||= []
base_wave.length.times{|i|
  out_wave[i] = 0
  impulse_response.length.times{|j|
    if i - j >= 0
      out_wave[i] += base_wave[i - j] * impulse_response[j]
    end
  }
}

x_axis = (0..base_wave.length).to_a
x_axis.collect!{|n| n /= 16000.0}
x_impulse = (0..7200).to_a
x_impulse.collect!{|n| n /= 16000.0}
x_base = (0..base_wave.length).to_a
x_base.collect!{|n| n /= 16000.0}

File.open("./data/"+ ARGV[4] +".raw", 'w') {|file| #rawファイルで書き出し
  file.write(out_wave.pack("s*"))
}
=begin
File.open("./data/"+ ARGV[4] +".txt", 'w') {|file| #textファイルで書き出し
  out_wave.length.times{|i|
    file.write "#{out_wave[i]}\n"
  }
}
=end

plot(x_base, out_wave)
#o_plot(x_base, base_wave, "time[s]", "Amplitude", "SpeechData_graph")
#o_plot(x_impulse, impulse_response, "time[s]", "Amplitude", "ImpulseResponse_graph")
#o_plot(x_axis, out_wave, "time[s]", "Amplitude", "Convolution_out")

#impulse_response.collect!{|n| n *= 30000}

#File.open("./LMS/impulse.raw", 'w') {|file|
#  file.write(impulse_response.pack("s*"))
#}
