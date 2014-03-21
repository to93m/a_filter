require 'gnuplot'

def plot(x, y)
  Gnuplot.open do |gp|
    Gnuplot::Plot.new( gp ) do |plot|
      plot.term "x11"
      plot.xrange "[#{x[0]}:#{x[-1]}]"
      #plot.yrange "[-1:5]"
      plot.nokey

      plot.data << Gnuplot::DataSet.new( [x, y] ) do |ds|
        ds.with = "lines"
        ds.linewidth = 1
      end
    end
  end
end

def t_plot(x, y, title)
  Gnuplot.open do |gp|
    Gnuplot::Plot.new( gp ) do |plot|
      plot.set 'title', "#{title}"
      plot.xrange "[#{x[0]}:#{x[-1]}]"
      plot.nokey

      plot.data << Gnuplot::DataSet.new( [x, y] ) do |ds|
        ds.with = "lines"
        ds.linewidth = 1
      end
    end
  end
end

def d_plot(x, y, y2)
  Gnuplot.open do |gp|
    Gnuplot::Plot.new( gp ) do |plot|
      plot.xrange "[#{x[0]}:#{x[-1]}]"
      plot.nokey

      plot.data << Gnuplot::DataSet.new( [x, y] ) do |ds|
        ds.with = "lines"
        ds.linewidth = 1
      end

      plot.data << Gnuplot::DataSet.new( [x, y2] ) do |ds|
        ds.with = "lines"
        ds.linewidth = 1
      end
    end
  end
end

def o_plot(x, y, x_l, y_l, out_n)
  Gnuplot.open do |gp|
    Gnuplot::Plot.new( gp ) do |plot|
      plot.terminal 'postscript eps enhanced color solid "Times-Roman" 23'
      plot.set 'output', "./#{out_n}.eps"
      plot.xrange "[#{x[0]}:#{x[-1]}]"
      plot.nokey
      plot.xlabel "#{x_l}"
      plot.ylabel "#{y_l}"

      plot.data << Gnuplot::DataSet.new( [x, y] ) do |ds|
        ds.with = "lines"
        ds.linewidth = 2
      end
    end
  end
end
