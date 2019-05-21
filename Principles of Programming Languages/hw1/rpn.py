import sys

class Stack:
	def __init__(self):
		self.top = None
		self.stack = []
	
	def isEmpty(self):
		return (len(self.stack) == 0)
		
	def push(self, element):
		self.stack.append(element)
		self.top = element
	
	def pop(self):
		if not(self.isEmpty()):
			element = self.stack.pop()
			if(self.isEmpty()):
				self.top = None
			else:
				self.top = self.stack[len(self.stack) - 1]
			return element
		else:
			print("Error! Stack is empty. Cannot pop().")
			return None
			
	def peek(self):
		return self.top
		
	def size(self):
		return len(self.stack)
			
			
def main():
	stack = Stack()
	str = sys.stdin.readline()
	while(str != "" and str != "\n"):
		str = str.rstrip()
		compute(str, stack)
		print(stack.top)
		str = sys.stdin.readline()

def compute(str, stack):
	if(str.isdigit()):
		stack.push(int(str))
	else:
		if(str == "+" and stack.size() >  1):
			num1 = stack.pop()
			num2 = stack.pop()
			stack.push(num2 + num1)
		elif(str == "-" and stack.size() >  1):
			num1 = stack.pop()
			num2 = stack.pop()
			stack.push(num2 - num1)
		elif(str == "*" and stack.size() >  1):
			num1 = stack.pop()
			num2 = stack.pop()
			stack.push(num2 * num1)
		elif(str == "/" and stack.size() >  1):
			num1 = stack.pop()
			num2 = stack.pop()
			stack.push(num2 / num1)
		elif(str == "~" and stack.size() > 0):
			num1 = stack.pop()
			stack.push(num1 * (-1))
		else:
			print("invalid operation")



main()
