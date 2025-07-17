

# 必要なライブラリをインポート
import os
import numpy as np
import casadi
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
from matplotlib import patches


# 出力フォルダの作成
os.makedirs('images', exist_ok=True)

# フォント設定（Linux用）
plt.rcParams['font.family'] = 'IPAexGothic'  # japanize_matplotlib推奨フォント

# CasADiで最適化問題を解く
x = casadi.SX.sym('x')
y = casadi.SX.sym('y')
nlp = {
    'x': casadi.vertcat(x, y),
    'f': x**3 + x**2 + 8*x + 4*y**2 + 3*x*y,
    'g': x**2 + y**2 - 1  # 単位円制約
}
S = casadi.nlpsol('S', 'ipopt', nlp)

# 最適化の実行
r = S(x0=[0, 1], lbg=0, ubg=0)
x_opt = np.array(r['x'])
print('x_opt: ', x_opt)

# 等高線の描画用データを作成
X_, Y_ = np.arange(-2, 2.01, 0.01), np.arange(-2, 2.01, 0.01)
X, Y = np.meshgrid(X_, Y_)
Z = X**3 + X**2 + 8*X + 4*Y**2 + 3 * X * Y

levs = np.linspace(-20, 50, 20)

# 描画
fig, ax = plt.subplots(figsize=(8, 6))
ax.contour(X, Y, Z, levels=levs)
ax.scatter(x_opt[0], x_opt[1], c="red", label="最適解")
circle = plt.Circle((0, 0), 1, fill=False, linestyle='dashed', label="制約: 単位円")
ax.add_patch(circle)
ax.set_title("非線形計画問題の解")
ax.legend()
fig.colorbar(ax.contour(X, Y, Z, levels=levs))

# 画像保存と表示
plt.savefig("images/chap3_NLP_2D.png")
plt.show()
