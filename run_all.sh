lfc /home/u-tajima/lf-test/src/DeadlineTest2.lf
docker build -t lf-test .

rm -r log_all/log_baseline
rm -r log_all/log_cgroup
rm -r log_all/log_container_runc
rm -r log_all/log_container_crun
rm -r log_all/log_container_youki

REPEAT=5

for i in $(seq 1 $REPEAT); do

    ./run_baseline.sh
    ./run_container_runc.sh
    ./run_container_crun.sh
    ./run_container_youki.sh
    ./run_cgroup.sh

done

python3 plot_all.py
