import matplotlib.pyplot as plt
import re
import os
import csv
import numpy as np

CPU_LOADS = [0,1,2,3,4,5]
configs = [
    ("1", "log_all/log_test100"),
    ("2", "log_all/log_test200"),
    ("3", "log_all/log_test300"),
    ("4", "log_all/log_test400"),
    ("5", "log_all/log_test500")
]

# データ保持リスト
all_source, all_dest, all_sched, all_miss = [], [], [], []
all_lf_cpu_usage, all_pc_cpu_usage = [], []
labels = []
# 生のレイテンシデータを保持: {'Deadline': {N: [data...], ...}}
all_raw_sched_latency = {}
all_raw_dest_latency = {} # ← Destination Latency の生データ用

# -------- 共通グラフ描画関数 --------
def plot_metric(cpu_loads, all_data, title, ylabel, filename, ylim=None):
    plt.figure(figsize=(8,6))
    for data, label in zip(all_data, labels):
        plt.plot(cpu_loads, data, marker='o', label=label)
    plt.title(title)
    plt.xlabel("CPU Load (number of stress-ng --cpu N)")
    plt.ylabel(ylabel)
    if ylim:
        plt.ylim(*ylim)
    plt.grid(True)
    plt.legend(title="Deadline (μsec)")
    plt.tight_layout()
    os.makedirs("log_all/log_img_test", exist_ok=True)
    plt.savefig(f"log_all/log_img_test/{filename}.png")
    plt.close()

# -------- ログから各種平均値を抽出 --------
def parse_data(log_dir):
    pattern_source = re.compile(r"Source sends: \d+ at \d+ ns \(interval: (\d+) ns\)")
    pattern_dest = re.compile(r"Destination receives msg.*error:\s*(-?\d+)\s*ns")
    pattern_scheduler = re.compile(r"Scheduler latency: (\d+) ns")
    pattern_received = re.compile(r"Total received:\s*(\d+)")
    pattern_misses = re.compile(r"Deadline misses:\s*(\d+)")

    source_list_all, dest_list_all, sched_list_all, miss_rate_all = [], [], [], []

    for N in CPU_LOADS:
        source_list, dest_list, sched_list = [], [], []
        total_received, total_missed = 0, 0

        try:
            with open(f"{log_dir}/lf_output_cpu{N}.log") as f:
                for line in f:
                    if m := pattern_source.search(line):
                        source_list.append(int(m.group(1)) / 1e6)  # ms
                    if m := pattern_dest.search(line):
                        dest_list.append(int(m.group(1)) / 1e6)    # ms
                    if m := pattern_scheduler.search(line):
                        sched_list.append(int(m.group(1)) / 1e6)  # ms
                    if m := pattern_received.search(line):
                        total_received += int(m.group(1))
                    if m := pattern_misses.search(line):
                        total_missed += int(m.group(1))
        except FileNotFoundError:
            pass # ファイルがない場合は次のNへ

        source_avg = sum(source_list)/len(source_list) if source_list else 0.0
        dest_avg   = sum(dest_list)/len(dest_list)     if dest_list   else 0.0
        sched_avg  = sum(sched_list)/len(sched_list)   if sched_list  else 0.0
        miss_rate  = (total_missed/total_received*100.0) if total_received > 0 else 0.0

        source_list_all.append(source_avg)
        dest_list_all.append(dest_avg)
        sched_list_all.append(sched_avg)
        miss_rate_all.append(miss_rate)

    return source_list_all, dest_list_all, sched_list_all, miss_rate_all


# -------- 生の Latency データ収集関数 (Scheduler & Destination) --------
def collect_raw_latency_data(log_dir):
    """
    Scheduler latency と Destination latency の生データを収集する。
    """
    pattern_scheduler = re.compile(r"Scheduler latency: (\d+) ns")
    pattern_dest = re.compile(r"Destination receives msg.*error:\s*(-?\d+)\s*ns")
    
    raw_sched_data = {n: [] for n in CPU_LOADS}
    raw_dest_data = {n: [] for n in CPU_LOADS}

    for N in CPU_LOADS:
        try:
            with open(f"{log_dir}/lf_output_cpu{N}.log") as f:
                for line in f:
                    # Scheduler Latency
                    if m := pattern_scheduler.search(line):
                        raw_sched_data[N].append(int(m.group(1)) / 1e6) # ms
                    # Destination Latency
                    if m := pattern_dest.search(line):
                        raw_dest_data[N].append(int(m.group(1)) / 1e6) # ms
        except FileNotFoundError:
            pass
            
    return raw_sched_data, raw_dest_data


# -------- CPU使用率（LF, PC） --------
def parse_cpu_usage(log_dir):
    pattern_cpu_usage = re.compile(r"Percent of CPU this job got:\s*(\d+)")
    
    def parse_mpstat_avg(filename):
        usage_values = []
        try:
            with open(filename, "r") as f:
                for line in f:
                    if re.search(r"\s+all\s+", line):
                        tokens = line.split()
                        cpu_idle = float(tokens[-1])
                        usage_values.append(100.0 - cpu_idle)
        except FileNotFoundError:
            return 0.0

        return sum(usage_values)/len(usage_values) if usage_values else 0.0

    lf_usage_all, pc_usage_all = [], []

    for N in CPU_LOADS:
        lf_usages = []
        try:
            with open(f"{log_dir}/lf_time_cpu{N}.log") as f:
                for line in f:
                    if m := pattern_cpu_usage.search(line):
                        lf_usages.append(float(m.group(1)))
        except FileNotFoundError:
            pass

        lf_usage = sum(lf_usages)/len(lf_usages) if lf_usages else 0.0
        pc_usage = parse_mpstat_avg(f"{log_dir}/lf_mpstat_cpu{N}.log")

        lf_usage_all.append(lf_usage)
        pc_usage_all.append(pc_usage)

    return lf_usage_all, pc_usage_all


# -------- 箱ひげ図描画関数の定義 (共通) --------
def plot_boxplot(raw_data_map, title, ylabel, filename):
    plt.figure(figsize=(10, 6))
    
    x_labels = [f"N={n}" for n in CPU_LOADS]
    num_series = len(labels)
    num_loads = len(CPU_LOADS)
    
    total_width = 0.8
    single_width = total_width / num_series
    
    for i, label in enumerate(labels):
        # N=0, N=1, ... のデータリストのリストを作成
        data_list = [raw_data_map[label][n] for n in CPU_LOADS]
        
        # 箱の中心位置を計算
        positions = [x - (total_width / 2) + (i * single_width) + (single_width / 2) for x in range(num_loads)]
        
        # 箱ひげ図を作成
        bp = plt.boxplot(data_list, 
                         positions=positions, 
                         widths=single_width * 0.9, 
                         patch_artist=True, 
                         showfliers=False)
        
        # 色を設定（Matplotlibのデフォルトカラーサイクルを使用）
        color = plt.cm.Dark2(i % plt.cm.Dark2.N)

        for box in bp['boxes']:
            box.set_facecolor(color)
        
        # 凡例のためにダミーのプロットを作成
        plt.plot([], c=color, label=label)

    # 軸とタイトルの設定
    plt.xticks(range(num_loads), x_labels)
    plt.title(title)
    plt.xlabel("CPU Load (number of stress-ng --cpu N)")
    plt.ylabel(ylabel)
    plt.grid(True)
    plt.legend(title="Deadline (μsec)")
    plt.tight_layout()
    os.makedirs("log_all/log_img_test", exist_ok=True)
    plt.savefig(f"log_all/log_img_test/{filename}_boxplot.png")
    plt.close()


# -------- データ収集ループ --------
for label, path in configs:
    # 既存の平均値データを収集
    src, dst, sch, miss = parse_data(path)
    lf, pc = parse_cpu_usage(path)

    all_source.append(src)
    all_dest.append(dst)
    all_sched.append(sch)
    all_miss.append(miss)
    all_lf_cpu_usage.append(lf)
    all_pc_cpu_usage.append(pc)
    labels.append(label)

    # 生の Latency データを収集
    raw_sched, raw_dest = collect_raw_latency_data(path)
    all_raw_sched_latency[label] = raw_sched
    all_raw_dest_latency[label] = raw_dest # ← Destination Latency の生データを保存

# -------- グラフ出力 --------
plot_metric(CPU_LOADS, all_source, "Source Period Jitter", "Jitter avg (ms)", "combined_source_jitter")
plot_metric(CPU_LOADS, all_dest, "Destination Latency", "Latency avg (ms)", "combined_dest_latency")
plot_metric(CPU_LOADS, all_sched, "Scheduler Latency", "Scheduler avg (ms)", "combined_scheduler_latency")
plot_metric(CPU_LOADS, all_miss, "Deadline Miss Rate", "Miss Rate (%)", "combined_deadline_miss")
plot_metric(CPU_LOADS, all_lf_cpu_usage, "LF App CPU Usage", "CPU Usage (%)", "combined_lf_cpu_usage")
plot_metric(CPU_LOADS, all_pc_cpu_usage, "PC Total CPU Usage", "CPU Usage (%)", "combined_pc_cpu_usage")

# -------- 箱ひげ図出力 (Scheduler & Destination) --------
plot_boxplot(all_raw_sched_latency, 
             "Scheduler Latency Distribution", 
             "Latency (ms)", 
             "combined_scheduler_latency")

plot_boxplot(all_raw_dest_latency, # ← Destination Latency の箱ひげ図
             "Destination Latency Distribution (End-to-End)", 
             "Latency (ms)", 
             "combined_dest_latency")


print("✅ 8つの統合グラフを出力しました (うち2つはLatencyの箱ひげ図)。log_all/log_img_test/ を確認してください。")

# -------- CSV出力 (既存部分) --------
metrics = [
    ("Source_Jitter_avg_ms", all_source),
    ("Dest_Latency_avg_ms", all_dest),
    ("Scheduler_Latency_avg_ms", all_sched),
    ("Deadline_Miss_Rate_pct", all_miss),
    ("LF_CPU_Usage_pct", all_lf_cpu_usage),
    ("PC_CPU_Usage_pct", all_pc_cpu_usage),
]

os.makedirs("log_all/log_csv_test", exist_ok=True)

# メトリックごとに1ファイルで出力
for metric_name, all_data in metrics:
    csv_file = f"log_all/log_csv_test/{metric_name}.csv"
    with open(csv_file, mode="w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["CPU_Load"] + labels)
        for i, load in enumerate(CPU_LOADS):
            row = [load] + [data[i] for data in all_data]
            writer.writerow(row)

print("📊 メトリクスごとにCSVとして保存しました → log_all/log_csv_test/")