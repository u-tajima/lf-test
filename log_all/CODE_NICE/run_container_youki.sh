#!/bin/bash

CPU_LOADS=(1 2 3 4 5)

mkdir -p log_all/log_container_youki

for N in "${CPU_LOADS[@]}"; do
    echo "Running container (youki) with --cpu $N"

    stress-ng --cpu $N --timeout 60s >> log_all/log_container_youki/stress_cpu${N}.log 2>&1 &
    STRESS_PID=$!

    mpstat 1 60 >> log_all/log_container_youki/lf_mpstat_cpu${N}.log &
    MPSTAT_PID=$!

    sleep 1

    echo "--- Set ${N} ---" >> log_all/log_container_youki/lf_output_cpu${N}.log
    echo "--- Set ${N} ---" >> log_all/log_container_youki/lf_time_cpu${N}.log

    docker run --rm \
        --runtime=youki \
        --entrypoint bash \
        --cap-add SYS_NICE \
        --ulimit rtprio=99 \
        --ulimit nice=-20 \
        --security-opt seccomp=unconfined \
        -v "$(pwd)/log_all/log_container_crun:/mnt/host_log_all/log_container_crun" \
        lf-test \
        -c "/usr/bin/time -v nice -n -10 /usr/local/bin/DeadlineTest2" \
        >> log_all/log_container_youki/lf_output_cpu${N}.log \
        2>> log_all/log_container_youki/lf_time_cpu${N}.log

    kill $STRESS_PID
    wait $STRESS_PID 2>/dev/null
    kill $MPSTAT_PID
    wait $MPSTAT_PID 2>/dev/null

    echo "Completed container (youki) --cpu $N"
done

echo "Running container (youki) with --cpu 0"

mpstat 1 60 >> log_all/log_container_youki/lf_mpstat_cpu0.log &
MPSTAT_PID=$!

sleep 1

echo "--- Set ${N} ---" >> log_all/log_container_youki/lf_output_cpu0.log
echo "--- Set ${N} ---" >> log_all/log_container_youki/lf_time_cpu0.log

docker run --rm \
    --runtime=youki \
    --entrypoint bash \
    --cap-add SYS_NICE \
    --ulimit rtprio=99 \
    --ulimit nice=-20 \
    --security-opt seccomp=unconfined \
    -v "$(pwd)/log_all/log_container_crun:/mnt/host_log_all/log_container_crun" \
    lf-test \
    -c "/usr/bin/time -v nice -n -10 /usr/local/bin/DeadlineTest2" \
    >> log_all/log_container_youki/lf_output_cpu0.log \
    2>> log_all/log_container_youki/lf_time_cpu0.log

kill $MPSTAT_PID
wait $MPSTAT_PID 2>/dev/null

echo "Completed container (youki) --cpu 0"

echo "All youki container experiments done."
