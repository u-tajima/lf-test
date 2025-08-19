#!/bin/bash

LF_BIN="./bin/DeadlineTest2"
CPU_LOADS=(2 4 6 8 10 12)
REPEAT=10

LOG_DIR="log_all/log_cgroup"
CGROUP_BASE="/sys/fs/cgroup"

LF_CG="${CGROUP_BASE}/lf_test"
STRESS_CG="${CGROUP_BASE}/stress_test"

mkdir -p "$LOG_DIR"

# cgroupディレクトリ作成（再利用）
sudo mkdir -p "$LF_CG"
sudo mkdir -p "$STRESS_CG"

# cpu.max 設定（両方に50%の制限）
echo "700000 100000" | sudo tee "$LF_CG/cpu.max" > /dev/null
echo "700000 100000" | sudo tee "$STRESS_CG/cpu.max" > /dev/null

echo "▶ Running cgroup test with --cpu 0"

mpstat 1 60 >> "$LOG_DIR/lf_mpstat_cpu0.log" & MPSTAT_PID=$!

sleep 1

for i in $(seq 1 $REPEAT); do

    echo "--- Set $i ---" >> "$LOG_DIR/lf_output_cpu0.log"
    echo "--- Set $i ---" >> "$LOG_DIR/lf_time_cpu0.log"

    /usr/bin/time -v timeout 60s "$LF_BIN" \
        >> "$LOG_DIR/lf_output_cpu0.log" \
        2>> "$LOG_DIR/lf_time_cpu0.log" &
    LF_PID=$!
    echo $LF_PID | sudo tee "$LF_CG/cgroup.procs" > /dev/null
    wait $LF_PID

done

kill $MPSTAT_PID 2>/dev/null; wait $MPSTAT_PID 2>/dev/null

echo "✅ Completed run --cpu 0"

for N in "${CPU_LOADS[@]}"; do
    echo "▶ Running cgroup test with --cpu $N"

    stress-ng --cpu $N --timeout 60s >> "$LOG_DIR/stress_cpu${N}.log" 2>&1 &
    STRESS_PID=$!
    echo $STRESS_PID | sudo tee "$STRESS_CG/cgroup.procs" > /dev/null

    mpstat 1 60 >> "$LOG_DIR/lf_mpstat_cpu${N}.log" & MPSTAT_PID=$!

    sleep 1

    for i in $(seq 1 $REPEAT); do

        echo "--- Set $i ---" >> "$LOG_DIR/lf_output_cpu${N}.log"
        echo "--- Set $i ---" >> "$LOG_DIR/lf_time_cpu${N}.log"

        /usr/bin/time -v timeout 60s "$LF_BIN" \
            >> "$LOG_DIR/lf_output_cpu${N}.log" \
            2>> "$LOG_DIR/lf_time_cpu${N}.log" &
        LF_PID=$!
        echo $LF_PID | sudo tee "$LF_CG/cgroup.procs" > /dev/null
        wait $LF_PID
    done

    kill $STRESS_PID 2>/dev/null; wait $STRESS_PID 2>/dev/null
    kill $MPSTAT_PID 2>/dev/null; wait $MPSTAT_PID 2>/dev/null

    echo "✅ Completed run --cpu $N"
done

echo "✅ All cgroup experiments done."
