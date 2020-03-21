for i in {1..5}; do
    qsub -N RunJob$i -l nodes=1:ppn=1,walltime=00:30:00 -d "$PWD" -F "$i" run.sh
done