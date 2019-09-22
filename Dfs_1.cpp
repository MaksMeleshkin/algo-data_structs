#include<iostream>
#include<vector>
typedef size_t Vertex;

class Graph{
public:
	Graph(Vertex count, bool is_directed){
		is_dir = is_directed;
		vertex_count = count;
	}

	virtual std::vector<Vertex> get_neighbors(Vertex V) const = 0;

	size_t get_vertex_count (){
		return vertex_count;
	}

	bool is_directed(){
		return is_dir;
	}

	virtual void add_edge(const Vertex &start, const Vertex &finish) = 0;

	size_t get_edge_count(){
		return edge_count;
	}

protected:
	bool is_dir;
	size_t vertex_count, edge_count = 0;
};


class GraphAdjList: public Graph{
private:
	std::vector<std::vector<Vertex>> adj_list;
public:
	GraphAdjList(Vertex count, bool is_directed): Graph(count, is_directed), adj_list(count, std::vector<Vertex>()){}
	
	virtual void add_edge(const Vertex &start, const Vertex &finish){
		++edge_count;
		if (is_dir) {
			adj_list[start].push_back(finish);
		}else{
			adj_list[start].push_back(finish);
			adj_list[finish].push_back(start);
		}
	}
	
	virtual std::vector<Vertex> get_neighbors(Vertex V) const{
		return adj_list[V];
	}
};

namespace GraphProcessing{
	enum VertexMark{white, grey, black};
	
	void dfs(std::vector<VertexMark> &vertex_mark, const Graph &g, const Vertex V, std::vector<std::vector<Vertex>> &components){
		vertex_mark[V] = grey;
		(*(components.end() - 1)).push_back(V);
		for (auto i: g.get_neighbors(V)) {
			if (vertex_mark[i] == white) {
				dfs(vertex_mark, g, i, components);
			}
		}
		vertex_mark[V] = black;
	}
	
	std::vector<std::vector<Vertex>> getConnectedComponents(Graph &g){   //  const????
		std::vector<std::vector<Vertex>> components;
		std::vector<VertexMark> vertex_mark(g.get_vertex_count(), white);
		for (Vertex v = 0; v < g.get_vertex_count(); ++v) {
			if (vertex_mark[v] == white) {
				components.push_back(std::vector<Vertex>());
				//std::cout << "Вызван Dfs от вершины " << v << std::endl;
				dfs(vertex_mark, g, v, components);
			}
		}
		return components;
	}
}

int main(){
	int n, s;
	std::cin >> n >> s;
	GraphAdjList g(n, false);
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			int temp;
			std::cin >> temp;
			if(temp){
				g.add_edge(i, j);
			}
		}
	}
	std::vector<std::vector<Vertex>> component(1, std::vector<Vertex>());
	std::vector<GraphProcessing::VertexMark> vertex_mark(n, GraphProcessing::white);
	GraphProcessing::dfs(vertex_mark, g, s - 1, component);
	for(auto i: component){
		std::cout << component[0].size();
	}
}
