import os
import sys
import subprocess
import itertools
from zsim_logparse import *
import time, datetime

def GenerateConfig(template, param_names, param_list):
    for i in range(len(param_names)):
        template = template.replace('{' + param_names[i] + '}', str(param_list[i]))

    return template

template_filename = sys.argv[1]
template = open(template_filename, 'r').read()

params = {
    'num_threads': [1, 2, 4, 8, 16, 32, 64, 128],
    'l1i_size': [32 * 1024], # 32KB
    'l1d_size': [64 * 1024], # 64 KB
    'l2_size': [512 * 1024], # 512 KB
    'l3_size': [1024 * 1024], # 1 MB
    'peak_bw': [1024, 4 * 1024, 16 * 1024, 64 * 1024, 256 * 1024],
}

param_names = ['num_threads', 'l1i_size', 'l1d_size', 'l2_size', 'l3_size', 'peak_bw']
param_lists = list(itertools.product(*[params[key] for key in param_names]))
runs = range(len(param_lists))

if not os.path.exists('tests/'):
    os.mkdir('tests')
timestamp = time.time()
time_format = datetime.datetime.fromtimestamp(timestamp).strftime('%Y-%m-%d_%H-%M-%S')
print("Simulation Runs started at "+time_format)
run_dir = 'tests/' + time_format
os.mkdir(run_dir)

procs = []

with open(run_dir+'/run_details.txt','w') as f:
    f.write('Run\tParameters\n')
    for run,param_list in zip(runs,param_lists):
        f.write('%d\t%s\n'%(run,param_list))
        uname = 'test_' + str(run)
        workdir = run_dir + '/' + uname
        os.mkdir(workdir)

        config_text = GenerateConfig(template, param_names, param_list)
        with open(workdir + '/config.cfg', 'w') as config_f:
            config_f.write(config_text)

        procs.append(subprocess.Popen(['zsim', 'config.cfg'], cwd=workdir, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL))
        #procs[-1].wait()

for proc in procs:
    proc.wait()

#p = str(procs[0].stdout.readlines())
#print('p is ', p)

stats = {
    'max_core_cycles': MaxCoreCycles,
    # 'avg_cont_cycles': AvgContCycles
}
stat_names = ['max_core_cycles']

print(', '.join(param_names + stat_names))
for run,param_list in zip(runs,param_lists):
    uname = 'test_' + str(run)
    workdir = run_dir + '/' + uname
    logout = workdir + '/zsim.out'
    print(logout+"\n")

    out_data = list(param_list) + [
        stats[stat_name](logout)
        for stat_name in stat_names
    ]

    print(', '.join([str(data) for data in out_data]))
