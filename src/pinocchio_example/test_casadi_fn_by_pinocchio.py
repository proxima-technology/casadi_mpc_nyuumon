import numpy as np
import casadi as cs

M = cs.Function.load('csfn_mass_matrix.casadi')
print("[Mass Matrix]")
print("M =",M)
print("M(np.zeros(7)) =",M(np.zeros(7)))

b = cs.Function.load('csfn_bias_term.casadi')
print("\n[Bias Term]")
print("b =",b)
print("b(np.zeros(7),np.ones(7)) =",b(np.zeros(7),np.ones(7)))
