#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import csv

def autolabel(rects):
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x()+rect.get_width(), 1.05*height, height,
                ha='center', va='bottom')

success_dwave = open('arXiv-1305-5837v1/anc/success.txt', 'r')
success_dwave_csv = open('arXiv-1305-5837v1/anc/actual_success.csv', 'w')
for each in success_dwave:
    success_dwave_csv.write(each.split()[1] + '\n')
success_dwave_csv.close()

n_groups = 5
i = 0
while i<= 995:
    sa = np.genfromtxt('arXiv-1305-5837v1/anc/predicted_success.csv')[i:i+5]/100.0
    qa = np.genfromtxt('arXiv-1305-5837v1/anc/actual_success.csv')[i:i+5]

    fig, ax = plt.subplots()

    index = np.arange(n_groups)
    bar_width = 0.1
    opacity = 0.4

    rects1 = plt.bar(index, sa, bar_width, alpha=opacity, color='b', label='Simulated Annealing')
    rects2 = plt.bar(index + bar_width, qa, bar_width, alpha=opacity, color='r', label='Quantum Annealing')

    plt.xlabel('Benchmark')
    plt.ylabel('Success Probability')
    plt.title('Performance Benchmarking')

    benchmark = open('arXiv-1305-5837v1/anc/success.txt', 'r')
    benchmark_list = []
    for each in benchmark:
        benchmark_list.append('-'.join(each.split()[0].replace('.mat', '').split('-')[-3:]))
    benchmark.close()

    lab = benchmark_list[i:i+5]
    plt.xticks(index + bar_width, lab)
    plt.legend()
    plt.tight_layout()

    plt.savefig('probability_graphs/' + str(i+1) + '-' + str(i+5) + '.png')
    plt.close()
    i=i+5