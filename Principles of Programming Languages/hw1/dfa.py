import sys

class Graph:
	def __init__(self):
		self.vertices = []
		self.edges = []
		self.adj_mtrx = []
			
	def add_vertex(self, data):
		self.vertices.append(data)
		
	def add_edge(self, weight):
		self.edges.append(weight)
		
	def init_adj_mtrx(self):
		for row in range(len(self.vertices)):
			row_list = []
			for col in range(len(self.vertices)):
				row_list.append(0)
			self.adj_mtrx.append(row_list)
					
	def insert(self, src, edge, dest):
		src_loc = self.check_vertex(src)
		dest_loc = self.check_vertex(dest)
		if(src_loc > -1 and dest_loc > -1 and self.check_edge(edge)):
			if(self.adj_mtrx[src_loc][dest_loc] == 0):		
				self.adj_mtrx[src_loc][dest_loc] = edge			
				#self.adj_mtrx[dest_loc][src_loc] = edge
			else:
				sys.stdout.write("edge already exists\n")
				sys.exit(-1)
		else:
			sys.stdout.write("invalid graph input\n")
			sys.exit(0)
	
	def check_vertex(self, v):
		for idx, vertex in enumerate(self.vertices):
			if(v == vertex):
				return idx	
		return -1
		
	def check_edge(self, e):
		for edge in self.edges:
			if(e == edge):
				return True	
		return False

def get_dfa():
	dfa_graph = Graph()
	states = False
	symbols = False
	rules = False
	start = False
	finish = False
	p_start = ""
	p_finish = ""
	while(not (states and symbols and rules and start and finish)):
		str = sys.stdin.readline()
		if(len(str) != 0):
			str = str.split()
			if(str[0] == "states:" and states == False):
				states = True
				str.remove(str[0])
				for vertex in str:
					dfa_graph.add_vertex(vertex)
				dfa_graph.init_adj_mtrx()
			elif(str[0] == "symbols:" and symbols == False):
				symbols = True
				str.remove(str[0])
				for weight in str:
					dfa_graph.add_edge(weight)
			elif(str[0] == "begin_rules" and rules == False):
				rules = True
				str = sys.stdin.readline()
				str = str.split()
				while(len(str) != 0 and str[0] != "end_rules"):
					dfa_graph.insert(str[0], str[4], str[2])
					str = sys.stdin.readline()
					str = str.split()
			elif(str[0] == "start:" and start == False):
				start = True
				str.remove(str[0])
				p_start = []
				loc = dfa_graph.check_vertex(str[0])
				if(loc < 0):
					print("invalid start position")
					sys.exit(0)
				else:
					p_start.append(loc)
			elif(str[0] == "final:" and finish == False):
				finish = True
				p_finish = []
				str.remove(str[0])
				for value in str:
					loc = dfa_graph.check_vertex(value)
					if(loc < 0):
						print("invalid final position")
						sys.exit(0)
					else:
						p_finish.append(loc)
			else:
				print("invalid input") 
	return dfa_graph, p_start, p_finish
	
def str_validation(graph, start, finish):
	str = sys.stdin.readline()
	str = str.strip()
	while(len(str) != 0):
		curr_pos = start[0]
		for c in str:
			next_pos = find_value(graph, curr_pos, c)
			if(next_pos < 0):
				break
			else: 
				curr_pos = next_pos
		if next_pos in finish:
			print("accepted")
		else:
			print("rejected")
		str = sys.stdin.readline()
		str = str.strip()
					
def find_value(graph, curr_pos, c):
	for idx, value in enumerate(graph.adj_mtrx[curr_pos]):
		if(c == value):
			return idx
	return -1

def main():
	dfa_graph, start, finish = get_dfa()
	#print(start, "   ", finish)
	#print(dfa_graph.adj_mtrx)
	
	str_validation(dfa_graph, start, finish)

main()