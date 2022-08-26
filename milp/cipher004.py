
# x_i_16 ,x_i_15,....x_i_0 denote the input to the i-th round.
# x_(i+1)_16 ,x_(i+1)_15,....x_(i+1)_0 denote the output to the i-th round.

from gurobipy import *

class Cipher004:
	def __init__(self, Round):
		self.Round = Round

		# state size of cipher004 is 16 and number of sbox is 4
		self.blocksize = 16
		self.sbox= 4

		# creatinf a lp file. We will call gurobi depending upon the lp file
		self.filename_model = "Cipher004_" + str(self.Round) + ".lp"
		fileobj = open(self.filename_model, "w")
		fileobj.close()
 

	def CreateObjectiveFunction(self):
		# Create objective function of the MILP model

		fileobj = open(self.filename_model, "w")
		fileobj.write("Minimize\n")
		eqn = []
		for i in range(0,self.Round):
			for j in range(0,self.sbox):
				eqn.append("a" + "_" + str(i) + "_" + str(j))
		temp = " + ".join(eqn)
		fileobj.write(temp)
		fileobj.write("\n")
		fileobj.close()


	def CreateVariables(n):
		# Generate the variables used in the model for round 'n'.

		array = []
		for i in range(0,16):
			array.append(("x" + "_" + str(n) + "_" + str(i)))
		return array


	def CreateSboxVariables(n):
		# Generate the sbox variables used in the model for round 'n'.
		array = []
		for i in range(0,4):
			array.append(("a" + "_" + str(n) + "_" + str(i)))
		return array


	def ConstraintsBySbox(self, variable1, variable2, sbox_var):
		# Generate the constraints by sbox layer.

		# adding the constrants x_i_0 - a_i <= 0
		fileobj = open(self.filename_model,"a")
		for sbox in range(0,4):
			for bit in range(0,4):
				fileobj.write(str(variable1[(4*sbox) + bit]) + " - " + str(sbox_var[sbox]) + " <= 0" + "\n")

		# adding the constrants x_i_0 + x_i_1 + x_i_2 + x_i_3 - a_i >= 0  for each sbox
		for sbox in range(0,4):
			eqn = []
			for bit in range(0,4):
				eqn.append(variable1[(4*sbox) + bit])
			temp = " + ".join(eqn)
			fileobj.write(temp + " - " + sbox_var[sbox] + " >= 0")
			fileobj.write("\n")

		# adding the constrants 4 x_i+1_0 + 4 x_i+1_1 + 4 x_i+1_2 + 4 x_i+1_3 - x_i_0 - x_i_1 - x_i_2 - x_i_3 >= 0  for each sbox
		for sbox in range(0,4):
			eqn = []
			for bit in range(0,4):
				eqn.append("4 " + variable2[(4*sbox) + bit])
			temp1 = " + ".join(eqn)
			eqn = []
			for bit in range(0,4):
				eqn.append(variable1[(4*sbox) + bit])
			temp2 = " - ".join(eqn)
			fileobj.write(temp1 + " - " + temp2 + " >= 0")
			fileobj.write("\n")

		# adding the constrants 4 x_i_0 + 4 x_i_1 + 4 x_i_2 + 4 x_i_3 - x_i+1_0 - x_i+1_1 - x_i+1_2 - x_i+1_3 >= 0  for each sbox
		for sbox in range(0,4):
			eqn = []
			for bit in range(0,4):
				eqn.append("4 " + variable1[(4*sbox) + bit])
			temp1 = " + ".join(eqn)
			eqn = []
			for bit in range(0,4):
				eqn.append(variable2[(4*sbox) + bit])
			temp2 = " - ".join(eqn)
			fileobj.write(temp1 + " - " + temp2 + " >= 0")
			fileobj.write("\n")
		fileobj.close(); 


	def LinearLayer(variable):
		array = ["" for i in range(0,16)]

		# array_index is the permutation of cipher004
		array_index = [0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15] 
		for i in range(0, 16):
			array[array_index[i]] = variable[i]

		return array


	def Constraint(self):
		# Generate the constraints used in the MILP model.

		assert(self.Round >= 1)
		fileobj = open(self.filename_model, "a")
		fileobj.write("Subject To\n")
		fileobj.close()

		# create variables for 1st round. 
		variablein = Cipher004.CreateVariables(0)
		variableout = Cipher004.CreateVariables(1)
		sbox_var = Cipher004.CreateSboxVariables(0)

		if self.Round == 1:
			self.ConstraintsBySbox(variablein, variableout, sbox_var)
		else:
			self.ConstraintsBySbox(variablein, variableout, sbox_var)
			for i in range(1, self.Round):
				variablein = Cipher004.LinearLayer(variableout)
				variableout= Cipher004.CreateVariables(i + 1)
				sbox_var = Cipher004.CreateSboxVariables(i)
				self.ConstraintsBySbox(variablein, variableout, sbox_var)

		# adding the necessary condition, a_0_0 + a_0_1 + ... + a_round_4 >= 1
		fileobj = open(self.filename_model, "a")
		eqn = []
		for i in range(0, self.Round):
			for sbox in range(0,4):
				eqn.append("a_" + str(i) + "_" +  str(sbox))
			temp = " + ".join(eqn)
		fileobj.write(temp + " >= 1 \n")
		fileobj.close()


	def VariableBinary(self):
		# Specify the variable type.

		fileobj = open(self.filename_model, "a")
		fileobj.write("Binary\n")
		for i in range(0, (self.Round + 1)):
			for j in range(0,16):
				fileobj.write("x_" + str(i) + "_" + str(j))
				fileobj.write("\n")
			
		for i in range(0, (self.Round)):
			for j in range(0,4):
				fileobj.write("a_" + str(i) + "_" + str(j))
				fileobj.write("\n")
		fileobj.write("END")
		fileobj.close()


	def MakeModel(self):
		# Generate the MILP model for the given round number.

		self.CreateObjectiveFunction()
		self.Constraint()
		self.VariableBinary()

	def SolveModel(self):
		# Solve the MILP model to search the integral distinguisher of Present.

		m = read(self.filename_model)
		m.optimize()


