
class Sbox:
	def __init__(self, sbox):
		self.sbox = sbox
		self.SBOXSIZE = self.SboxSize()

	def SboxSize(self):
		"""	
		This function is used to calculate the size of a given sbox
		"""
		s = format(len(self.sbox), "b")
		num_of_1_in_the_binary_experission_of_the_len_of_sbox = s.count("1")
		assert num_of_1_in_the_binary_experission_of_the_len_of_sbox == 1
		return (len(s) - 1)

	def BitProduct(self, u, x):
		"""
		Return the value of the bitproduct function Pi_u(x)
		"""
		if (u & x) == u:
			return 1
		else:
			return 0

	def GetTruthTable(self, u):
		"""
		Retrieve the truth table of the boolean function Pi_u(y), where y = sbox(x)
		"""
		temp = [u for i in range(len(self.sbox))]
		table = map(self.BitProduct, temp, self.sbox)
		return table

	def ProcessTable(self, table):
		"""
		Process the truth table to get the ANF of the boolean function
		we use table size to calculate the SBOXSIZE
		"""
		for i in range(0, self.SBOXSIZE):
			for j in range(0, 2**i):
				for k in range(0, 2**(self.SBOXSIZE - 1 - i)):
					table[k + 2**(self.SBOXSIZE - 1 - i) + j*(2**(self.SBOXSIZE - i))] =\
	                table[k + 2**(self.SBOXSIZE - 1 - i) + j*(2**(self.SBOXSIZE - i))] ^\
	                table[k + j*(2**(self.SBOXSIZE - i))]

	def CreatANF(self):
		"""
		Return the ANF of the sbox, moreover, this function also return the ANF of boolean function which
		is the product of some coordinates of the sbox output
		"""
		ANF = [[]for i in range(0, len(self.sbox))]
		for i in range(1, len(self.sbox)): 
			table = self.GetTruthTable(i)
			self.ProcessTable(table)
			sqr = []
			for j in range(0, len(self.sbox)):
			    if table[j] != 0:
			        sqr.append(j)
			ANF[i] = sqr
		return ANF


	def PrintANF(self):
		"""
		this will print ANF of the given function
		"""	
		ANF = self.CreatANF()
		
		bit_size = self.SboxSize()			# 	defining bit_size of SBox 
		
		ANF_list = []
		ANF_index_list = []
		for i in range(1,bit_size+1):
			ANF_index_list.append(pow(2,i-1))
			
		for index in ANF_index_list:			#	taking only these bits cz this code is writen such like we can take 										multipication of bits also 
			ANF_list.append(ANF[index])

		for ANF_exp_index, ANF_exp in enumerate(ANF_list):	
			print("\n\ny" + str(ANF_exp_index) + " = "),
			for index_ANF, ele_ANF in enumerate(ANF_exp):
				if (ele_ANF == 0):		#  	print 1, if only 0 remains """
					print("1"),
					if (index_ANF != len(ANF_exp)-1):
						print(" + "),		
					continue
				
				bit_list = list(format(ele_ANF, "0"+str(bit_size)+"b"))

				for index, bit in enumerate(bit_list):
					if (bit == '1'):		# if bit == 1, then only it wiill print exp 
						print("x"+str(bit_size-1 - index)),

				if (index_ANF != len(ANF_exp)-1):	# atlast + will not be printed 
					print(" + "),		
		print("\n")	
		
		
		
