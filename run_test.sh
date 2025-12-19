REPEAT=1

lfc /home/yu/lf-test/src100/DeadlineTest2.lf

rm -r log_all/log_test100

for i in $(seq 1 $REPEAT); do

    ./test_baseline100.sh

done

lfc /home/yu/lf-test/src200/DeadlineTest2.lf

rm -r log_all/log_test200

for i in $(seq 1 $REPEAT); do

    ./test_baseline200.sh

done

lfc /home/yu/lf-test/src300/DeadlineTest2.lf

rm -r log_all/log_test300

for i in $(seq 1 $REPEAT); do

    ./test_baseline300.sh

done

lfc /home/yu/lf-test/src400/DeadlineTest2.lf

rm -r log_all/log_test400

for i in $(seq 1 $REPEAT); do

    ./test_baseline400.sh

done

lfc /home/yu/lf-test/src500/DeadlineTest2.lf

rm -r log_all/log_test500

for i in $(seq 1 $REPEAT); do

    ./test_baseline500.sh

done

python3 plot_test.py
