import numpy as np
import casadi
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.animation import FuncAnimation

def opt_by_solver(solver_name):
    try:
        x = casadi.SX.sym('x')
        y = casadi.SX.sym('y')
        qp = {'x':casadi.vertcat(x,y), \
                'f':x**2 + 2*y**2 - x - 2*y,\
                'g':casadi.vertcat(
                    x+y-3,
                    x-1,
                    3-y,
                    -x+3*y+1,
                    5-x-y
                )}
        S = casadi.qpsol('S',solver_name, qp)
        r = S(x0=[0, 1],lbg=[0,0,0,0,0], ubg=[0,np.inf,np.inf,np.inf,np.inf])
        x_opt = np.array(r['x'])
        print('x_opt: ', x_opt)
        return True
    except:
        return False

def main():
    solver_list = [
        #"cbc",
        #"clp",
        #"cplex",
        #"fatrop",
        #"gurobi",
        #"highs",
        #"hpipm",
        #"hpmpc",
        #"ooqp",
        #"osqp",
        #"proxqp",
        #"qpoases",
        #"sqic",
        #"superscs",
        #"ipqp",
        #"nlpsol",
        "qrqp" 
    ]
    ret_list = []
    for solver_name in solver_list:
        print(solver_name)
        ret = opt_by_solver(solver_name)
        ret_list.append(ret)

    return list(zip(solver_list,ret_list))

print(main())