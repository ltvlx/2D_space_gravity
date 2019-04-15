import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation, colors
from matplotlib.collections import LineCollection
from numpy import pi, sqrt
from itertools import combinations
import codecs
import random
random.seed(0)


fig, ax = plt.subplots(figsize=(6,6))


class Planet:
    def __init__(self, m, x0, x1, v0, v1, a0, a1):
        self.x = np.array([x0, x1])
        self.v = np.array([v0, v1])
        self.a = np.array([a0, a1])
        self.m = m

        self.r = 5 * sqrt(m)
        self.trace_x = [x0]
        self.trace_y = [x1]
        self.color = 'blue'


    def __str__(self):
        description = "Planet '%s', mass = %.1f\n"%(self.color, self.m)
        description += "   x = [%.2f, %.2f]\n"%(self.x[0], self.x[1])
        description += "   v = <%.2f, %.2f>\n"%(self.v[0], self.v[1])
        description += "   a = <<%.2f, %.2f>>\n"%(self.a[0], self.a[1])

        return description


    def distance(self, other):
        return sqrt((self.x[0] - other.x[0])**2 + (self.x[1] - other.x[1])**2)



class Space:
    planets = []
    G = 1.0
    dt = 0.1
    n_trace = 25
    time = 0.0

    def __init__(self, inp_fname='setup.txt'):
        cols = list(colors.CSS4_COLORS.keys())
        random.shuffle(cols)
        cols = iter(cols)

        with codecs.open(inp_fname, 'r') as fin:
            N = int(fin.readline())
            fin.readline()
            for _ in range(N):
                parameters = tuple(map(float, fin.readline().split()))
                PP = Planet(*parameters)
                PP.color = next(cols)
                self.planets.append(PP)
                # print(self.planets[-1])


    def make_iteration(self):
        self.time += self.dt
        self.__calculate_acceleration()
        self.__calculate_position()
        self.__calculate_velocity()



    def __calculate_acceleration(self):
        """
        <F> = <r> * G * m1 * m2 / r**3
        m1 * <a> = <F>
        <a> = <r> * G * m2 / r**3
        """
        # Reset the acceleration
        for p in self.planets:
            p.a = np.array([0.0, 0.0])


        for p1, p2 in combinations(self.planets, 2):
            dist = p1.distance(p2)
            if dist < 1e-8:
                continue

            # print(p1.color, p2.color)
            F = (self.G * p1.m * p2.m / (dist**3)) * np.array([p2.x[0] - p1.x[0], p2.x[1] - p1.x[1]])

            p1.a += F / p1.m
            p2.a -= F / p2.m
            

    def __calculate_velocity(self):
        """
        v = v0 + a * dt
        """
        for p in self.planets:
            p.v += p.a * self.dt
            

    def __calculate_position(self):
        """
        x = x0 + v0 * dt + a * (dt**2) / 2
        """
        for p in self.planets:
            p.x += p.v * self.dt + p.a * (self.dt**2) / 2
            
            p.trace_x.append(p.x[0])
            p.trace_y.append(p.x[1])


    def init_animation(self):
        ax.set_xlim(-10, 10)
        ax.set_ylim(-10, 10)
        for p in self.planets:
            ax.plot(p.x[0], p.x[1], 'o', color=p.color)


    def animate_iteration(self, t):
        ax.cla()
        ax.set_xlim(-10, 10)
        ax.set_ylim(-10, 50)

        # for _ in range(100):
        self.make_iteration()


        for p in self.planets:
            ax.plot(p.x[0], p.x[1], 'o', color=p.color, markersize=p.r)

            ax.arrow(p.x[0], p.x[1], p.v[0], p.v[1], color='black', zorder=3)

            points = np.array([p.trace_x[-self.n_trace:], p.trace_y[-self.n_trace:]]).T.reshape(-1, 1, 2)
            segments = np.concatenate([points[:-1], points[1:]], axis=1)
            
            lwidths = np.geomspace(1, 3, self.n_trace-1)
            lc = LineCollection(segments, linewidths=lwidths, color=p.color)
            ax.add_collection(lc)

        ax.annotate('t=%.2f'%(self.time), xy=(0, 1), xytext=(12, -12), va='top', xycoords='axes fraction', textcoords='offset points')
        



A = Space()



anim = animation.FuncAnimation(fig, A.animate_iteration, init_func=A.init_animation, frames=2000, interval=60)
plt.show()


