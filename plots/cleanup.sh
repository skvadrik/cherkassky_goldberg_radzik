
problems="acyc_neg acyc_pos grid_phard grid_ssquare grid_swide rand_4 rand_p acyc_p2n grid_nhard grid_slong grid_ssquare_s rand_1:4 rand_len"

for problem in $problems; do
    rm "$problem.gnuplot" "plot_$problem.png" "plot_${problem}_logscale.png"
done
