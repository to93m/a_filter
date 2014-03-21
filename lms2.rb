require './plot.rb'
include Math

def read_wave(file_name, bit)
  wave ||= []
  File.open("./data/"+ file_name + ".raw") {|file|
    binary = file.read
    if bit == 2
      wave = binary.unpack('s*')

    elsif bit == 8
      wave = binary.unpack('d*')
    end
  }
  return wave
end

def convolution(wave1, wave2)
  out_wave ||= []
  wave1.length.times{|i|
    out_wave[i] = 0
    wave2.length.times{|j|
      if i - j >= 0
        out_wave[i] += wave1[i - j] * wave2[j]
      end
    }
  }
  return out_wave
end

tap_num = 500
iteration_num = 15

target_data = read_wave("Convo_white", 2)
input_sound = read_wave("Whitenoise", 2)
filter_gain = Array.new(tap_num, 0.0)
output ||= []
error ||= []
mu = 0.000000000000001


iteration_num.times{
  input_sound.length.times{|i|
    output[i] = 0
    tap_num.times{|j|
      if i - j >= 0
        output[i] += input_sound[i - j] * filter_gain[j]
      end
    }

    error[i] = target_data[i] - output[i]

    tap_num.times{|k|
      if i - k >= 0
        filter_gain[k] += mu * error[i] * target_data[i - k]
      end
    }
  }
}

x_filter = (0..filter_gain.length).to_a
#x_tar = (0..target_data.length).to_a

plot(x_filter, error)
#plot(x_tar, target_data)
