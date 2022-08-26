import gurobipy as gp
from gurobipy import GRB

# initialize a model
m = gp.Model("MILP_example")

# add variables in the model 
x0 = m.addVar(vtype=GRB.BINARY, name="x0")
x1 = m.addVar(vtype=GRB.BINARY, name="x1")
x2 = m.addVar(vtype=GRB.BINARY, name="x2")
x3 = m.addVar(vtype=GRB.BINARY, name="x3")

x4 = m.addVar(vtype=GRB.INTEGER, name="x4")
x5 = m.addVar(vtype=GRB.INTEGER, name="x5")
x6 = m.addVar(vtype=GRB.INTEGER, name="x6")
x7 = m.addVar(vtype=GRB.INTEGER, name="x7")

d0 = m.addVar(vtype=GRB.BINARY, name="d0")

# set the objective function in the model i.e. here Minimize x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7
m.setObjective(x0+x1+x2+x3+x4+x5+x6+x7 , GRB.MINIMIZE)	 

# add constrants in the model
m.addConstr(x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 >=  5*d0)
m.addConstr(d0 - x0 >= 0)
m.addConstr(d0 - x1 >= 0)
m.addConstr(d0 - x2 >= 0)
m.addConstr(d0 - x3 >= 0)
m.addConstr(d0 - x4 >= 0)
m.addConstr(d0 - x5 >= 0)
m.addConstr(d0 - x6 >= 0)
m.addConstr(d0 - x7 >= 0)

m.addConstr(x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 >= 1 )

# Optimize model
m.optimize()

# to write the milp model
m.write("MILP_example.lp")

# This is to write the milp solution file. Depending upon the extension it will take the lp file or the sol file. There are other extensions also like this, see gurobi's site for that.
m.write("MILP_example.sol")


# to print the values using python
print("x0 : " + str(x0.x))
print("x1 : " + str(x1.x))
print("x2 : " + str(x2.x))
print("x3 : " + str(x3.x))

print("\n")
print("x4 : " + str(x4.x))
print("x5 : " + str(x5.x))
print("x6 : " + str(x6.x))
print("x7 : " + str(x7.x))
