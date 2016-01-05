set title "75% ON"
set key top left
set xl "SNR[dB]"
set yl "Power Difference[dB]"
plot "75per.txt" using 1:2 w lp title "Conventional"
replot "75per.txt" using 1:3 w lp title "CUSUM"
set terminal png
set out "75per.png"
replot

