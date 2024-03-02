import numpy as np
import casadi as cs

cf_fd = cs.Function.load('csfn_forward_dynamics.casadi')
print("[Forward Dynamics]")
print("cf_fd =",cf_fd)
zeoro7 = np.zeros(7)
print("cf_fd(0,0,0) =",cf_fd(zeoro7,zeoro7,zeoro7))

cf_id = cs.Function.load('csfn_inverse_dynamics.casadi')
print("\n[Inverse Dynamics]")
print("cf_id =",cf_id)
print("cf_id(0,0,0) =",cf_id(zeoro7,zeoro7,zeoro7))
