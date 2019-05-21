import sys
rows = 0
cols = 0

def main():
	size_1 = getSize()
	matrix_1 = getMatrix()
	
	size_2 = getSize()
	matrix_2 = getMatrix()
	
	if(size_1[1] != size_2[0]):
		sys.stdout.write("invalid input")
		sys.exit()
		 
	final_matrix = matrix_mult(matrix_1, matrix_2)
	print_matrix(final_matrix)
	
	
def getSize():
	str = sys.stdin.readline()
	str = str.split()
	if(len(str) != 2):
		msg = "Invalid  matrix data entered!"
		error(msg)
	else:
		global rows 
		global cols
		
		msg = "Invalid size entered!"
		if(str[0].isdigit() and str[1].isdigit()):
			rows = int(str[0])
			cols = int(str[1])
			if(rows <= 0 or cols <= 0):
				error(msg)
		else:
			error(msg)
	return rows, cols


def getMatrix():
	matrix = [] 
	msg = "Invalid matrix data entered!"
	
	for i in range(rows):
		rowData = []
		str = sys.stdin.readline()
		str = str.split()
		if(len(str) != cols):
			error(msg)
		else:
			for j, entry in enumerate(str):
				val = is_number(entry)
				rowData.append(val)
			matrix.append(rowData)
	return matrix
			
			
def is_number(var):
	try:
		float(var)
		return float(var)
	except Exception:
		error("Invalid matrix data entered!")


def matrix_mult(matrix_1, matrix_2):
	final_matrix = []
	for row in range(len(matrix_1)):
		rowData = []
		for col in range(len(matrix_2[0])):
			sum = 0
			for i in range(len(matrix_1[1])):
				sum = sum + (matrix_1[row][i] * matrix_2[i][col])
			rowData.append(sum)
		final_matrix.append(rowData)
	return final_matrix
	

def print_matrix(matrix):
	
	#sys.stdout.write(str(len(matrix)) + " " + str(len(matrix[0])) + "\n")
	for row in range(len(matrix)):
		for col in range(len(matrix[0])):
			sys.stdout.write(str(matrix[row][col]) + " ")
		sys.stdout.write("\n")

def error(msg):
	sys.stdout.write("ERROR! " + str(msg))
	sys.exit()
	
	
main()