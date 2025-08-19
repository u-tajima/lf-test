#!/bin/bash

CPU_LOADS=(2 4 6 8 10 12)
REPEAT=5

mkdir -p log_all/log_container_youki

echo "Running container (youki) with --cpu 0"

mpstat 1 60 >> log_all/log_container_youki/lf_mpstat_cpu0.log &
MPSTAT_PID=$!

sleep 1

for i in $(seq 1 $REPEAT); do

    echo "--- Set $i ---" >> log_all/log_container_youki/lf_output_cpu0.log
    echo "--- Set $i ---" >> log_all/log_container_youki/lf_time_cpu0.log

    docker run --rm \
        --runtime=youki \
        --entrypoint bash \
        -v "$(pwd)/log_all/log_container_youki:/mnt/host_log_all/log_container_youki" \
        lf-test \
        -c "/usr/bin/time -v /usr/local/bin/DeadlineTest2" \
        >> log_all/log_container_youki/lf_output_cpu0.log \
        2>> log_all/log_container_youki/lf_time_cpu0.log

done

kill $MPSTAT_PID
wait $MPSTAT_PID 2>/dev/null

echo "Completed container (youki) --cpu 0"

for N in "${CPU_LOADS[@]}"; do
    echo "Running container (youki) with --cpu $N"

    stress-ng --cpu $N --timeout 60s >> log_all/log_container_youki/stress_cpu${N}.log 2>&1 &
    STRESS_PID=$!

    mpstat 1 60 >> log_all/log_container_youki/lf_mpstat_cpu${N}.log &
    MPSTAT_PID=$!

    sleep 1

    for i in $(seq 1 $REPEAT); do

        echo "--- Set $i ---" >> log_all/log_container_youki/lf_output_cpu${N}.log
        echo "--- Set $i ---" >> log_all/log_container_youki/lf_time_cpu${N}.log

        docker run --rm \
            --runtime=youki \
            --entrypoint bash \
            -v "$(pwd)/log_all/log_container_youki:/mnt/host_log_all/log_container_youki" \
            lf-test \
            -c "/usr/bin/time -v /usr/local/bin/DeadlineTest2" \
            >> log_all/log_container_youki/lf_output_cpu${N}.log \
            2>> log_all/log_container_youki/lf_time_cpu${N}.log
    done

    kill $STRESS_PID
    wait $STRESS_PID 2>/dev/null
    kill $MPSTAT_PID
    wait $MPSTAT_PID 2>/dev/null

    echo "Completed container (youki) --cpu $N"
done

echo "All youki container experiments done."
