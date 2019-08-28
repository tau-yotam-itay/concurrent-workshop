import subprocess

SL_EXEC = './concurrent_skiplist'
MQ_EXEC = './concurrent_multiqueue'

sl_stat = open('sl_stat','a+')
mq_stat = open('mq_stat','a+')
n_threads = 1
while n_threads <= 80:
    cmd = [SL_EXEC, '../example_input_output/edges', str(n_threads)]
    p = subprocess.Popen(cmd,stdout=sl_stat)
    p.wait()
    cmd = [MQ_EXEC, '../example_input_output/edges', str(n_threads)]
    p = subprocess.Popen(cmd,stdout=mq_stat)
    p.wait()
    if n_threads == 1:
        n_threads = 10
    else:
        n_threads += 10
sl_stat.close()
mq_stat.close()
