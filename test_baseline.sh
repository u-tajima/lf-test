#!/usr/bin/env bash
set -Eeuo pipefail

LF_BIN="./bin/DeadlineTest2"
CPU_LOADS=(2 4 6 8 10 12)
REPEAT=10

# ==== オプション ====
PIN_CPU="${PIN_CPU:--1}"          # 例: PIN_CPU=2 で LF を CPU2 に固定。-1 で無効
RT_PRIO="${RT_PRIO:-90}"          # SCHED_FIFO の優先度
RUN_SECS="${RUN_SECS:-60}"        # 1セットの実行秒数
WARMUP="${WARMUP:-1}"             # ウォームアップ回数（各Nの最初に捨てる）

LOG_DIR="log_all/log_baseline"
mkdir -p "$LOG_DIR"

cleanup() {
  # 残っているバックグラウンドを掃除
  local p
  for p in ${MPSTAT_PID:-} ${STRESS_PID:-}; do
    [[ -n "${p:-}" ]] && kill "$p" 2>/dev/null || true
    [[ -n "${p:-}" ]] && wait "$p" 2>/dev/null || true
  done
}
trap cleanup EXIT

log() { echo "[$(date '+%F %T')] $*" | tee -a "$LOG_DIR/run.log" ; }

# ==== 実験前スナップショット ====
sysdump="$LOG_DIR/system_snapshot.txt"
{
  echo "===== SYSTEM SNAPSHOT $(date -Is) ====="
  uname -a
  lscpu
  echo -e "\n--- Kernel cmdline ---"; cat /proc/cmdline
  echo -e "\n--- Clocksource ---"
  cat /sys/devices/system/clocksource/clocksource0/available_clocksource 2>/dev/null || true
  echo -n "current: "; cat /sys/devices/system/clocksource/clocksource0/current_clocksource 2>/dev/null || true
  echo -e "\n--- RT throttling ---"
  sysctl kernel.sched_rt_runtime_us kernel.sched_rt_period_us 2>/dev/null || true
  echo -e "\n--- CPU governors ---"
  grep -H . /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor 2>/dev/null | sort || true
  echo -e "\n--- THP / swap ---"
  grep -H . /sys/kernel/mm/transparent_hugepage/enabled 2>/dev/null || true
  sysctl vm.swappiness 2>/dev/null || true
  echo -e "\n--- irqbalance ---"; (systemctl is-active irqbalance 2>/dev/null || echo "unknown") | sed 's/^/status: /'
  echo -e "\n--- Time / NTP ---"; timedatectl 2>/dev/null || true
  echo -e "\n--- Container / Virt ---"; (systemd-detect-virt 2>/dev/null || true)
  echo "===== END SNAPSHOT ====="
} > "$sysdump"

log "Wrote system snapshot to $sysdump"

# ==== ヘルパ: 1回分のLF実行 ====
run_lf_once() {
  local tag="$1"           # 例: cpu0 or cpu6
  local idx="$2"           # セット番号（文字列でOK）
  local out_log="$LOG_DIR/lf_output_${tag}.log"
  local time_log="$LOG_DIR/lf_time_${tag}.log"
  local ps_log="$LOG_DIR/lf_ps_${tag}.log"

  echo "--- Set $idx ---" >> "$out_log"
  echo "--- Set $idx ---" >> "$time_log"
  echo "--- Set $idx ---" >> "$ps_log"

  # 実行コマンドを組み立て（必要なら taskset）
  cmd=(stdbuf -oL -eL /usr/bin/timeout "${RUN_SECS}s" chrt -f "$RT_PRIO")
  if [[ "$PIN_CPU" != "-1" ]]; then
    cmd=(taskset -c "$PIN_CPU" "${cmd[@]}")
  fi
  cmd+=("$LF_BIN")

  # 実行（/usr/bin/time -vでプロセスCPU使用率も採取）
  /usr/bin/time -v "${cmd[@]}" >>"$out_log" 2>>"$time_log" || true

  # 実際にRTクラスで走っていたか確認ログ
  ps -eo pid,ppid,cls,rtprio,pri,psr,comm | awk 'NR==1 || $7=="DeadlineTest2"' >> "$ps_log"
}

# ==== ウォームアップ関数 ====
warmup_if_needed() {
  local tag="$1"
  if (( WARMUP > 0 )); then
    log "Warmup ($WARMUP) for $tag"
    for w in $(seq 1 "$WARMUP"); do
      run_lf_once "$tag" "warmup-$w"
    done
  fi
}

# ==== mpstat 起動 ====
start_mpstat() {
  local tag="$1"
  mpstat 1 "$RUN_SECS" >> "$LOG_DIR/lf_mpstat_${tag}.log" &
  MPSTAT_PID=$!
}

# ==== N=0（stressなし） ====
log "Running baseline with --cpu 0 (no stress)"
start_mpstat "cpu0"
sleep 1
warmup_if_needed "cpu0"
for i in $(seq 1 $REPEAT); do
  run_lf_once "cpu0" "$i"
done
kill "$MPSTAT_PID" 2>/dev/null || true
wait "$MPSTAT_PID" 2>/dev/null || true
log "Completed baseline --cpu 0"

# ==== N>0 ====
for N in "${CPU_LOADS[@]}"; do
  log "Running baseline with --cpu $N"

  # stress-ng（ログは別ファイルへ）
  stress-ng --cpu "$N" --timeout "${RUN_SECS}s" >> "$LOG_DIR/stress_cpu${N}.log" 2>&1 &
  STRESS_PID=$!

  start_mpstat "cpu${N}"
  sleep 1

  warmup_if_needed "cpu${N}"

  for i in $(seq 1 $REPEAT); do
    run_lf_once "cpu${N}" "$i"
  done

  kill "$STRESS_PID" 2>/dev/null || true
  wait "$STRESS_PID" 2>/dev/null || true

  kill "$MPSTAT_PID" 2>/dev/null || true
  wait "$MPSTAT_PID" 2>/dev/null || true

  log "Completed baseline --cpu $N"
done

log "✅ All baseline experiments done."
