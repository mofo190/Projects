import numpy as np
from numpy import random
import matplotlib.pyplot as plt

sample = random.normal(loc = 10, scale = 10, size = 1000)

n = len(sample)

mean = np.sum(sample) / (n-1)

sqr_x = np.power(sample,2)
summ_sqrx = np.sum(sqr_x)

x = summ_sqrx / (n-1)
y = np.power(mean,2)
std = (x - y)**0.5

thresh = mean + std

above = 0
below = 0


for x in sample:
    if x > thresh:
        above = above + 1
    elif x < thresh:
        below = below + 1
        
abvper = (above / n) * 100
belper = (below / n) * 100

y = np.array([abvper,belper])
da_labels = ['Above Threshold','Below Threshold']
da_explode = [0.2,0]
da_colours = ['red','blue']
plt.pie(y, colors = da_colours, labels = da_labels, explode = da_explode)
plt.title('Values Above/Below (Standard Deviation + Mean) Threshold')
plt.show()
