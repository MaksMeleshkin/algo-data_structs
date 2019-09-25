#include<iostream>
#include<vector>
#include<queue>
typedef size_t Vertex;

class Graph{
public:
	Graph (Vertex count, bool is_directed){
		is_dir = is_directed;
		vertex_count = count;
	}

	virtual std::vector<Vertex> get_neighbors (Vertex V) const = 0;

	size_t get_vertex_count () const {
		return vertex_count;
	}

	bool is_directed () {
		return is_dir;
	}

	virtual void add_edge (const Vertex &start, const Vertex &finish) = 0;

	size_t get_edge_count() const {
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
	
	virtual std::vector<Vertex> get_neighbors(Vertex V) const {
		return adj_list[V];
	}
};

class GraphAdjMatrix: public Graph{
private:
	std::vector<std::vector<bool>> adj_matrix;
public:
	GraphAdjMatrix (Vertex count, bool is_directed): Graph(count, is_directed), adj_matrix(count, std::vector<bool>(count, false)){}

	virtual void add_edge (const Vertex &start, const Vertex &finish) {
		++edge_count;
		if (is_dir) {
			adj_matrix[start][finish] = true;
		}else{
			adj_matrix[start][finish] = true;
			adj_matrix[finish][start] = true;
		}
	}

	virtual std::vector<Vertex> get_neighbors (Vertex V) const {
		std::vector<Vertex> result;
		for (int i = 0; i < vertex_count; ++i) {
			if (adj_matrix[V][i]) {
				result.push_back(i);
			}
		}
		return result;
	}
};

namespace GraphProcessing{
	enum VertexMark{white, gray, black};
	
	void dfs (std::vector<VertexMark> &vertex_mark, const Graph &g, const Vertex V, std::vector<std::vector<Vertex>> &components) {
		vertex_mark[V] = gray;
		(*(components.end() - 1)).push_back(V);
		for (auto i: g.get_neighbors(V)) {
			if (vertex_mark[i] == white) {
				dfs(vertex_mark, g, i, components);
			}
		}
		vertex_mark[V] = black;
	}

	void bfs (const Graph &g, Vertex V, std::vector<size_t> &distance, std::vector<int> &parent, std::vector<VertexMark> &vertex_mark) {
		std::queue<Vertex> Q;
		vertex_mark[V] = gray;
		Q.push(V);
		while (!Q.empty()) {
			Vertex u = Q.front();
			Q.pop();
			for (auto i: g.get_neighbors(u)) {
				if (vertex_mark[i] == white) {
					vertex_mark[i] = gray;
					distance[i] = distance[u] + 1;
					parent[i] = u;
					Q.push(i);
				}
			}
			vertex_mark[u] = black;
		}
	}
	
	std::vector<Vertex> shortest_path (Graph &g, Vertex start, Vertex finish) {
		std::vector<VertexMark> vertex_mark(g.get_vertex_count(), white);
		std::vector<size_t> distance(g.get_vertex_count(), 0);
		std::vector<int> parent(g.get_vertex_count(), -1);
		std::vector<Vertex> result;
		bfs(g, start, distance, parent, vertex_mark);
		if (start == finish) {
			result.push_back(start);
			return result;
		}
		if (parent[finish] == -1) {
			return result;
		}
		while (start != finish) {
			result.push_back(finish);
			finish = parent[finish];
		}
		result.push_back(start);
		return result;
	}	

	std::vector<std::vector<Vertex>> getConnectedComponents (const Graph &g) {
		std::vector<std::vector<Vertex>> components;
		std::vector<VertexMark> vertex_mark(g.get_vertex_count(), white);
		for (Vertex v = 0; v < g.get_vertex_count(); ++v) {
			if (vertex_mark[v] == white) {
				components.push_back(std::vector<Vertex>());
				dfs(vertex_mark, g, v, components);
			}
		}
		return components;
	}
}

int main(){
	int n;
	std::cin >> n;
	GraphAdjList g(n, false);
	bool** arr;
	arr = new bool*[n];
	for (int i = 0; i < n; ++i) {
		arr[i] = new bool[n];
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cin >> arr[i][j];
		}
	}

	for (int i = 0; i < n - 1; ++i) {
		for (int j = i; j < n; ++j) {
			if (arr[i][j]) {
				g.add_edge(i, j);
			}
		}
	}

	Vertex start, finish;
	std::cin >> start >> finish;
	std::vector<Vertex> res = GraphProcessing::shortest_path(g, start - 1, finish - 1);
	if (res.empty()) {
		std::cout << "-1";
		return 0;
	}
	std::cout << res.size() - 1 << std::endl;
	if (res.size() == 1) {
		return 0;
	}
	for (auto i = res.rbegin(); i != res.rend(); ++i) {
		std::cout << *i + 1 << " ";
	}
}