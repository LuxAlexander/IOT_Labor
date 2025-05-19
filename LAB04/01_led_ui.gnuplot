# No shebang since you're using `gnuplot -c`
# Set up plot appearance
set title "LED UI-Kennlinie" noenhanced
set key autotitle columnhead top left
set ylabel "Spannung in V"
set xlabel "Strom in mA"
set grid

# Output to PNG file with timestamped filename
set terminal pngcairo size 1024,600
set output sprintf("%s.png", strftime("%F_%H-%M-%S", time(0)))

# Plot from file(s) and color(s) provided via ARG1
plot for [i=1:words(ARG1):2] word(ARG1, i) using 4:1 with lines linecolor \
rgb word(ARG1, i+1) title word(ARG1, i) noenhanced

# End of script
