
algs="gor1a lau lau1"
problems="acyc_neg acyc_pos grid_phard grid_ssquare grid_swide rand_4 rand_p acyc_p2n grid_nhard grid_slong grid_ssquare_s rand_1:4 rand_len"
logdir="../results.backup"
datadir="./data"

for problem in $problems; do
    for alg in $algs; do
        log="$logdir/$alg.$problem"
#        cat $log \
#            | grep -E "Nodes: ([0-9]+)|computation: ([.0-9]+)" -o \
#            | grep -E "[.0-9]+" -o \
#            | { while read l1 && read l2; do echo $l1 $l2; done } \
#            > "$datadir/$alg.$problem"
        cat $log \
            | grep -E "Nodes: ([0-9]+)|scans: ([0-9]+)" -o \
            | grep -E "[0-9]+" -o \
            | { while read l1 && read l2; do echo $((l1/2*2)) $l2; done } \
            > "$datadir/$alg.$problem"
    done
    cat template.gnuplot | sed -e "s/PROBLEM/$problem/g" > "$problem.gnuplot"
    gnuplot "$problem.gnuplot"
done
