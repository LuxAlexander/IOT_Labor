set t wxt noraise
set title "Temperatur (".ARG1.")" noenhanced
set key autotitle columnhead
set xlabel "Zeit in s"; set ylabel "Temperatur in °C"
set grid
end=0; bind "x" "end = 1"
while(end==0) {
plot ARG1 using 1:5 with lines lc rgb "red"
pause 0.5
}
replot
pause mouse close;
set term 'pngcairo' size 1024,600 # for svg: set term 'svg'
set output ARG1.strftime('%F_%H-%M-%S', time(0)).'.png'
replot