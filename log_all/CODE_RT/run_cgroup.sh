#!/bin/bash

LF_BIN="./bin/DeadlineTest2"
CPU_LOADS=(1 2 3 4 5)

LOG_DIR="log_all/log_cgroup"
CGROUP_BASE="/sys/fs/cgroup/system.slice"
LF_CG="${CGROUP_BASE}/lf_test.scope"

mkdir -p "$LOG_DIR"

# cgroupディレクトリ作成（再利用）
sudo mkdir -p "$LF_CG"

echo "max 100000" | sudo tee "$LF_CG/cpu.max" > /dev/null

for N in "${CPU_LOADS[@]}"; do
    echo "▶ Running cgroup test with --cpu $N"

    stress-ng --cpu $N --timeout 60s >> "$LOG_DIR/stress_cpu${N}.log" 2>&1 &
    STRESS_PID=$!

    mpstat 1 60 >> "$LOG_DIR/lf_mpstat_cpu${N}.log" & MPSTAT_PID=$!

    sleep 1

    /usr/bin/time -v timeout 60s chrt --fifo 90 "$LF_BIN" \
        >> "$LOG_DIR/lf_output_cpu${N}.log" \
        2>> "$LOG_DIR/lf_time_cpu${N}.log" &
    LF_PID=$!
    echo $LF_PID | sudo tee "$LF_CG/cgroup.procs" > /dev/null
    wait $LF_PID

    kill $STRESS_PID 2>/dev/null; wait $STRESS_PID 2>/dev/null
    kill $MPSTAT_PID 2>/dev/null; wait $MPSTAT_PID 2>/dev/null

    echo "✅ Completed run --cpu $N"
done

echo "▶ Running cgroup test with --cpu 0"

mpstat 1 60 >> "$LOG_DIR/lf_mpstat_cpu0.log" & MPSTAT_PID=$!

sleep 1

/usr/bin/time -v timeout 60s chrt --fifo 90 "$LF_BIN" \
    >> "$LOG_DIR/lf_output_cpu0.log" \
    2>> "$LOG_DIR/lf_time_cpu0.log" &
LF_PID=$!
echo $LF_PID | sudo tee "$LF_CG/cgroup.procs" > /dev/null
wait $LF_PID

kill $MPSTAT_PID 2>/dev/null; wait $MPSTAT_PID 2>/dev/null

echo "✅ Completed run --cpu 0"

echo "✅ All cgroup experiments done."
