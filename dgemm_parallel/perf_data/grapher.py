# cat result_str | grep 'grep' | awk '{printf "%d %f %f\n", $2, $3, $4}' > timeS
import numpy as np
import matplotlib.pyplot as plt

mat = np.loadtxt("1000_omp.dat")
xb = mat[:, 0]
yb = (mat[:, 1])

mat = np.loadtxt("2000_omp.dat")
xa = mat[:, 0]
ya = (mat[:, 1])

mat = np.loadtxt("3000_omp.dat")
xs = mat[:, 0]
ys = (mat[:, 1])
xs = xs.astype(np.int)

fig, ax = plt.subplots(figsize=(10, 5))
plt.xticks(np.arange(2,13,2))
plt.yticks(np.arange(0.5,13,0.5))
ax.grid(linestyle = ':')


ax.plot(xs, xs, linewidth=1, c = 'black', label = 'Linear speedup')
ax.plot(xb, yb, linewidth=1, c = 'red', label = 'n = 1000')
ax.plot(xa, ya, linewidth=1, c = 'blue', label = 'n = 2000')
ax.plot(xs, ys, linewidth=1, c = 'green', label = 'n = 3000')

ax.scatter(xs, xs, linewidth=0.3, marker='.', c = 'black')
ax.scatter(xb, yb, linewidth=0.3, marker='.', c = 'black')
ax.scatter(xa, ya, linewidth=0.3, marker='.', c = 'black')
ax.scatter(xs, ys, linewidth=0.3, marker='.', c = 'black')

ax.set_ylabel('Ускорение')
ax.set_xlabel('Количество потоков')
ax.legend()
# plt.show()
fig.savefig('omp')


mat = np.loadtxt("1000_pthr.dat")
xb = mat[:, 0]
yb = (mat[:, 1])

mat = np.loadtxt("2000_pthr.dat")
xa = mat[:, 0]
ya = (mat[:, 1])

mat = np.loadtxt("3000_pthr.dat")
xs = mat[:, 0]
ys = (mat[:, 1])
xs = xs.astype(np.int)

fig, ax = plt.subplots(figsize=(10, 5))
plt.xticks(np.arange(2,13,2))
plt.yticks(np.arange(0.5,13,0.5))
ax.grid(linestyle = ':')


ax.plot(xs, xs, linewidth=1, c = 'black', label = 'Linear speedup')
ax.plot(xb, yb, linewidth=1, c = 'red', label = 'n = 1000')
ax.plot(xa, ya, linewidth=1, c = 'blue', label = 'n = 2000')
ax.plot(xs, ys, linewidth=1, c = 'green', label = 'n = 3000')

ax.scatter(xs, xs, linewidth=0.3, marker='.', c = 'black')
ax.scatter(xb, yb, linewidth=0.3, marker='.', c = 'black')
ax.scatter(xa, ya, linewidth=0.3, marker='.', c = 'black')
ax.scatter(xs, ys, linewidth=0.3, marker='.', c = 'black')

ax.set_ylabel('Ускорение')
ax.set_xlabel('Количество потоков')
ax.legend()
fig.savefig('pthreads')


# mat = np.loadtxt("timeomp")
# xb = mat[:, 3]
# yb = (mat[:, 2])
#
# mat = np.loadtxt("timethr")
# xa = mat[:, 3]
# ya = (mat[:, 2])

# mat = np.loadtxt("timeS")
# xs = mat[:, 0]
# ys = (mat[:, 2])
# xs = xs.astype(np.int)

# fig, ax = plt.subplots(figsize=(10, 5))
# plt.xticks(np.arange(0,5,1))
# ax.grid(linestyle = ':')
#
# ax.plot(xb, yb, linewidth=1, c = 'green', label = 'dgemm openMP')
# ax.plot(xa, ya, linewidth=1, c = 'red', label = 'dgemm posix thread')
# ax.plot(xb, xb, linewidth=1, c = 'blue', label = 'Линейное ускорение')
#
# ax.set_ylabel('Ускорение')
# ax.set_xlabel('Количество потоков')
# ax.legend()
# plt.show()
# fig.savefig('main')
# # /////////////////////////////////////////////////////////////////
# mat = np.loadtxt("time")
# # xb = mat[:, 0]
# # yb = (mat[:, 2])
#
#
# fig, ax = plt.subplots(figsize=(10, 5))
# plt.xticks(np.arange(0,513,32))
# ax.grid(linestyle = ':')
# ax.plot(xb, mat, linewidth=1, c = 'blue', label = 'dgemm_opt_1')
#
# ax.set_ylabel('Время (с)')
# ax.set_xlabel('Размер матрицы')
#
# # plt.show()
# fig.savefig('timedg')
# # /////////////////////////////////////////////////////////////////
#
# mat1 = np.loadtxt("time")
# print(mat1)
# # xb = mat[:, 0]
# # yb = (mat[:, 0])
#
#
# fig, ax = plt.subplots(figsize=(10, 5))
# plt.xticks(np.arange(0,6145,1024))
# ax.grid(linestyle = ':')
# ax.plot(xb, mat1, linewidth=1, c = 'blue', label = 'dgemm_opt_1')
#
# # ax.set_ylabel('Ускорение')
# # ax.set_xlabel('Размер блока')
# ax.set_ylabel('Время (с)')
# ax.set_xlabel('Размер матрицы')
#
# plt.show()
# fig.savefig('timedgemm')
