#include<iostream>
#include<vector>

class Graph{
public:
	typedef size_t Vertex;
	Graph(Vertex count, bool is_directed){
		is_dir = is_directed;
		vertex_count = count;
	}
protected:
	bool is_dir;
	size_t vertex_count, edge_count;
	bool is_directed(){
		return is_dir;
	}
	virtual std::vector<Vertex> get_neighbors(Vertex V) const = 0;
	size_t get_vertex_count(){
		return vertex_count;
	}
	virtual void add_edge(const Vertex &start, const Vertex &finish) = 0;
};


class GraphAdjList: public Graph{
private:
	std::vector<std::vector<Vertex>> adj_list;
public:
	void add_edge(const Vertex &start, const Vertex &finish){
		++edge_count;
		if(is_dir){
			adj_list[start].push_back(finish);
		}else{
			adj_list[start].push_back(finish);
			adj_list[finish].push_back(start);
		}
	}
	
	std::vector<Vertex> get_neighbors(Vertex V){
		return adj_list[V];
	}
};

namespace GraphProcessing{
	enum VertexMark{white, grey, black};
	
	void dfs(std::vector<VertexMark> &vertex_mark, const GraphAdjList &g){
		
		for(){
			
		}
	}
	
	void DoDfs(std::vector<VertexMark> &vertex_mark, const GraphAdjList &g){
		for(int i = 0; i < g.adj_list.size(); ++i){
			if(vertex_mark[i] == "white"){
				dfs();
			}
		}
	}
	
	std::vector<vector<Vertex>> getConnectedComponents(const Graph &g){
		std::vector<vector<Vertex>> components;
		std::vector<VertexMark> vertex_mark(n, white);
	
	}
}

int main(){
	GraphAdjList G;
	int n;
	cin >> n;
		for(int i = 0; i < n; ++i){
			for(int j = 0; j < n; ++j){
				int temp;
				cin >> temp;
				if(temp){
					G.add_edge(temp, )
				}
			}
		}
	
	
}
