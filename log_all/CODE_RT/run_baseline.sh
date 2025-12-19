#!/bin/bash

LF_BIN="./bin/DeadlineTest2"
CPU_LOADS=(1 2 3 4 5)

LOG_DIR="log_all/log_baseline"
mkdir -p "$LOG_DIR"

for N in "${CPU_LOADS[@]}"; do
    echo "Running baseline with --cpu $N"

    stress-ng --cpu $N --timeout 60s >> "$LOG_DIR/stress_cpu${N}.log" 2>&1 &
    STRESS_PID=$!

    mpstat 1 60 >> "$LOG_DIR/lf_mpstat_cpu${N}.log" &
    MPSTAT_PID=$!

    sleep 1

    /usr/bin/time -v timeout 60s chrt --fifo 90 "$LF_BIN" \
        >> "$LOG_DIR/lf_output_cpu${N}.log" \
        2>> "$LOG_DIR/lf_time_cpu${N}.log"

    kill $STRESS_PID
    wait $STRESS_PID 2>/dev/null
    
    kill $MPSTAT_PID
    wait $MPSTAT_PID 2>/dev/null

    echo "Completed baseline --cpu $N"
done

echo "Running baseline with --cpu 0"

mpstat 1 60 >> "$LOG_DIR/lf_mpstat_cpu0.log" &
MPSTAT_PID=$!

sleep 1

/usr/bin/time -v timeout 60s chrt --fifo 90 "$LF_BIN" \
    >> "$LOG_DIR/lf_output_cpu0.log" \
    2>> "$LOG_DIR/lf_time_cpu0.log"

kill $MPSTAT_PID
wait $MPSTAT_PID 2>/dev/null

echo "Completed baseline --cpu 0"

echo "✅ All baseline experiments done."
