require './plot.rb'
include Math

N = 16000

def generate(sample_num)
omega_u = 2 * PI * 16000
omega_l = 0
output_wave = Array.new(sample_num, 0)
point ||= []

delta_omega = (omega_u - omega_l) / N
s = 1 / (omega_u - omega_l)
a_k = 4 * s * delta_omega

sample_num.times{|t|
  N.times{|k|
    omega_k = omega_l + (k - 1/2) * delta_omega

    num = rand(2 * PI)
    cos_num = cos(omega_k * t + num)
    output_wave[t] += a_k * cos_num
  }
  point << t
}

output_wave.max - output_wave.min / 2

plot(point, output_wave)

return output_wave
end

out_wave = generate(7000)

File.open("./LMS/"+ ARGV[0] +".raw", 'w') {|file|
  file.write(out_wave.pack("s*"))
}

