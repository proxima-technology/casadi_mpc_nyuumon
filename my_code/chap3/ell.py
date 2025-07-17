import casadi
import numpy as np
import matplotlib.pyplot as plt

x = casadi.SX.sym('x')
y = casadi.SX.sym('y')

f = y**2 + x**2  # 最大化したいのでマイナス
g = y**2 - x**3 + 3*x - 1  # 制約：楕円曲線

nlp = {'x': casadi.vertcat(x, y),
        'f': f,
        'g': g
        }
solver = casadi.nlpsol('solver', 'ipopt', nlp)

sol = solver(x0=[1, 1], lbg=0, ubg=0)  # 初期値と制約g=0
x_opt = np.array(sol['x'])
print('x_opt:', x_opt)

# 可視化
X_, Y_ = np.linspace(-4, 4, 400), np.linspace(-4, 4, 400)
X, Y = np.meshgrid(X_, Y_)
F = Y**2 + X**2
C = Y**2 - X**3 + 3*X - 1

fig, ax = plt.subplots(figsize=(8, 6))
ax.contour(X, Y, C, levels=[0], colors='black')  # 制約曲線
ax.contour(X, Y, F, levels=20)  # 等高線
ax.plot(x_opt[0], x_opt[1], 'ro')
ax.set_title("Maximize xy on $y^2 = x^3 - 3x + 1$")
plt.show()
