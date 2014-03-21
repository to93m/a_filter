require "./plot"
require "sound_lib"
include Math

def read_text(file_name)
  text ||= []
  File.open(file_name){|file|
    file.each{|line|
      text << line.chomp.to_f
    }
  }
  return text
end

input = read_text(ARGV[0])

x_axis = (0..input.length-1).to_a
plot(x_axis, input)

if ARGV[1] == dft
  real ||= []
  image ||= []
  power ||= []
  real, image = dft(input)

  real.length.times{|i|
    power << sqrt(real[i] ** 2 + image[i] ** 2)
  }

  plot(x_axis, power)

else
end
