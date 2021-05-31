import cma
import subprocess
import numpy as np
from math import inf
from pathlib import Path
import matplotlib.pyplot as plt
from keyframe_page_handler import keyframe_page_handler

def write_keyframe(x):
    index = 0
    prep.write_params(x[index:index+prep.numsteps*19])
    index += prep.numsteps*19
    back.write_params(x[index:index+back.numsteps*19])
    index += back.numsteps*19
    front.write_params(x[index:index+front.numsteps*19])
    return True

def evaluate_new_keyframe(x):
    write_keyframe(x)
    errcounter = 0
    server_speed_locked = False
    max_time = 150 if server_speed_locked else 12
    while True:
        try:
            subprocess.Popen('rcssserver3d', stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            subprocess.run('./KickKeyframeOptimization_AgentTest', cwd='binaries/', stdout=subprocess.PIPE, timeout=max_time)
            subprocess.run('killall -9 rcssserver3d', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            interface = open('binaries/interface.txt', 'r')
            fitness = interface.readline()
            interface.close()
            break
        except subprocess.TimeoutExpired:
            subprocess.run('killall -9 rcssserver3d', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            errcounter = errcounter + 1
            if errcounter == 3:
                fitness = -5
                break
    print('fitness: %f'%(-1*float(fitness)))
    return -1*float(fitness)


def otimize_cmaes(function, sigma0, epsilon, m0):
    es = cma.CMAEvolutionStrategy(m0.flatten(), sigma0)
    diff = inf
    oldfitness = inf
    fitness_history = []
    best_fitness = inf
    best_sample = None

    counter = 0
    while diff > epsilon:
        counter = counter + 1
        print('Episódio %d'%counter)
        samples = es.ask()
        fitness = np.array([function(sample) for sample in samples])
        index_best = np.argmin(fitness)

        if fitness[index_best] < best_fitness:
        	best_fitness = fitness[index_best]
        	best_sample = samples[index_best]
        	best_file = open('best_sample.txt', 'w')
        	best_file.write(str(best_sample))
        	best_file.close()
        
        fitness_history.extend(fitness.tolist())
        es.tell(samples, fitness)
        diff = np.max(np.absolute(fitness - oldfitness))
        oldfitness = fitness        
        plt.plot(fitness_history)
        plt.savefig('fitness.png')
        plt.clf()
             
    return fitness_history


input_shape = (19, 11)
sigma0 = 0.007
epsilon = 1
function = evaluate_new_keyframe

path = str(Path.home()) + "/RoboViz/bin/"
subprocess.Popen('./roboviz.sh', cwd=path, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
prep = keyframe_page_handler('prepFrontKickFast')
back = keyframe_page_handler('moveLegBack')
front = keyframe_page_handler('moveLegFront')
m0 = prep.read_params()
m0.extend(back.read_params())
m0.extend(front.read_params())
m0 = np.array(m0)

fitnesses = otimize_cmaes(function, sigma0, epsilon, m0)
