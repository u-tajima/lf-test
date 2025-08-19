#!/bin/bash

LF_BIN="./bin/DeadlineTest2"
CPU_LOADS=(2 4 6 8 10 12)
REPEAT=10

LOG_DIR="log_all/log_baseline"
mkdir -p "$LOG_DIR"

echo "Running baseline with --cpu 0"

mpstat 1 60 >> "$LOG_DIR/lf_mpstat_cpu0.log" &
MPSTAT_PID=$!

sleep 1

for i in $(seq 1 $REPEAT); do

    echo "--- Set $i ---" >> "$LOG_DIR/lf_output_cpu0.log"
    echo "--- Set $i ---" >> "$LOG_DIR/lf_time_cpu0.log"

    /usr/bin/time -v timeout 60s "$LF_BIN" \
        >> "$LOG_DIR/lf_output_cpu0.log" \
        2>> "$LOG_DIR/lf_time_cpu0.log"

done

kill $MPSTAT_PID
wait $MPSTAT_PID 2>/dev/null

echo "Completed baseline --cpu 0"

for N in "${CPU_LOADS[@]}"; do
    echo "Running baseline with --cpu $N"

    stress-ng --cpu $N --timeout 60s >> "$LOG_DIR/stress_cpu${N}.log" 2>&1 &
    STRESS_PID=$!

    mpstat 1 60 >> "$LOG_DIR/lf_mpstat_cpu${N}.log" &
    MPSTAT_PID=$!

    sleep 1

    for i in $(seq 1 $REPEAT); do

        echo "--- Set $i ---" >> "$LOG_DIR/lf_output_cpu${N}.log"
        echo "--- Set $i ---" >> "$LOG_DIR/lf_time_cpu${N}.log"

        /usr/bin/time -v timeout 60s "$LF_BIN" \
            >> "$LOG_DIR/lf_output_cpu${N}.log" \
            2>> "$LOG_DIR/lf_time_cpu${N}.log"
    done

    kill $STRESS_PID
    wait $STRESS_PID 2>/dev/null
    
    kill $MPSTAT_PID
    wait $MPSTAT_PID 2>/dev/null

    echo "Completed baseline --cpu $N"
done

echo "✅ All baseline experiments done."
