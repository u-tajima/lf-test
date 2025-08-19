lfc /home/tajima/lf-test/src/DeadlineTest2.lf
docker build -t lf-test .

./run_cgroup.sh
./run_baseline.sh
./run_container_runc.sh
./run_container_crun.sh
./run_container_youki.sh

python3 plot_all.py