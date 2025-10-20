import matplotlib.pyplot as plt
import numpy as np

threads = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12])

time_1e6 = np.array([0.001831738, 0.000975452, 0.000835001, 0.000908644,
                     0.000955740, 0.000971425, 0.000957492, 0.000996461,
                     0.001007595, 0.001023551, 0.001067615, 0.001002216])

time_1e7 = np.array([0.018110411, 0.008323423, 0.008316808, 0.008053734,
                     0.008760594, 0.010115881, 0.010465109, 0.010501224,
                     0.009521683, 0.010600083, 0.010596632, 0.010818058])

time_1e8 = np.array([0.182832350, 0.082071916, 0.090501939, 0.108652922,
                     0.137530988, 0.128333309, 0.100446176, 0.091850681,
                     0.097469641, 0.096079841, 0.093758749, 0.093548843])

plt.figure(figsize=(10, 6))
plt.plot(threads, time_1e6, marker='o', linewidth=2, markersize=8, label='$10^6$')
plt.plot(threads, time_1e7, marker='s', linewidth=2, markersize=8, label='$10^7$')
plt.plot(threads, time_1e8, marker='^', linewidth=2, markersize=8, label='$10^8$')

plt.xlabel('Number of Threads', fontsize=12)
plt.ylabel('Time (seconds)', fontsize=12)
plt.title('Paraller Inner Product Perfomance', fontsize=14, fontweight='bold')
plt.grid(True, alpha=0.3, linestyle='--')
plt.legend(fontsize=11, title='Problem Size')
plt.xticks(threads)

plt.tight_layout()
plt.savefig('plot.png')