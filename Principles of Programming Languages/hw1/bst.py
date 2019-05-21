import sys

class Node:
	def __init__(self):
		self.right = None
		self.left = None
		self.data = None

class Tree:
	def __init__(self):
		self.root = None
	
	def insert(self, val):
		node = Node()
		node.data = val
		if(self.root == None):
			self.root = node
		else:
			curr = self.root
			while(curr != None):
				prev = curr
				if(curr.data > val):
					curr = curr.left
				else:
					curr = curr.right
			if(prev.data > val):
				prev.left = node
			else:
				prev.right = node
	
	def query(self, val):
		if(self.root == None):
			print("not found")
		elif(self.root.data == val):
			print("found: root")
		else:
			str = ""
			curr = self.root
			while(curr != None):
				if(curr.data > val):
					str = str + "l "
					curr = curr.left
				elif(curr.data < val):
					str = str + "r "
					curr = curr.right
				else:
					print("found: " + str)
					return
			print("not found")
							

def main():
	tree = Tree()
	str = sys.stdin.readline()
	while(str != "" and str != "\n"):
		str = str.split()
		if(len(str) == 2):
			if(str[0] == 'i'):
				tree.insert(is_number(str[1]))
			elif(str[0] == 'q'):
				tree.query(is_number(str[1]))
			else:
				print("invalid command")
		else:
			print("invalid command")
		str = sys.stdin.readline()
		
		
def is_number(var):
	try:
		float(var)
		return float(var)
	except Exception:
		sys.stdout.write("invalid number")
		sys.exit();

main()