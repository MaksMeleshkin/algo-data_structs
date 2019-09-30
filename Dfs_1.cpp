#include <iostream>
#include <vector>
#include <queue>

class Graph {
public:
	typedef size_t Vertex;
	Graph(size_t count, bool is_directed) {
		is_dir = is_directed;
		vertex_count = count;
	}

	virtual std::vector<Vertex> get_neighbors(Vertex v) const = 0;

	size_t get_vertex_count() const {
		return vertex_count;
	}

	bool is_directed() {
		return is_dir;
	}

	virtual void add_edge(const Vertex &start, const Vertex &finish) = 0;

	size_t get_edge_count() const {
		return edge_count;
	}

protected:
	bool is_dir;
	size_t vertex_count, edge_count = 0;
};


class GraphAdjList: public Graph {
private:
	std::vector<std::vector<Vertex>> adj_list;
public:
	GraphAdjList(size_t count, bool is_directed): Graph(count, is_directed), adj_list(count, std::vector<Vertex>()) {}
	
	virtual void add_edge(const Vertex &start, const Vertex &finish) {
		++edge_count;
		adj_list[start].push_back(finish);
		if (!is_dir) {
			adj_list[finish].push_back(start);
		}
	}
	
	virtual std::vector<Vertex> get_neighbors(Vertex v) const {
		return adj_list[v];
	}
};

class GraphAdjMatrix: public Graph {
private:
	std::vector<std::vector<bool>> adj_matrix;
public:
	GraphAdjMatrix (size_t count, bool is_directed): Graph(count, is_directed), adj_matrix(count, std::vector<bool>(count, false)) {}

	virtual void add_edge(const Vertex &start, const Vertex &finish) {
		++edge_count;
		adj_matrix[start][finish] = true;
		if (!is_dir) {
			adj_matrix[finish][start] = true;
		}
	}

	virtual std::vector<Vertex> get_neighbors(Vertex v) const {
		std::vector<Vertex> result;
		for (Vertex i = 0; i < vertex_count; ++i) {
			if (adj_matrix[v][i]) {
				result.push_back(i);
			}
		}
		return result;
	}
};

namespace GraphProcessing {
	namespace{
		typedef size_t Vertex;
		enum VertexMark {
			white,
			grey,
			black
		};
	
		void dfs_1(std::vector<VertexMark> &vertex_mark, const Graph &g, const Vertex v, std::vector<std::vector<Vertex>> &components) {
			vertex_mark[v] = grey;
			(*components.rbegin()).push_back(v);
			for (Vertex i: g.get_neighbors(v)) {
				if (vertex_mark[i] == white) {
					dfs_1(vertex_mark, g, i, components);
				}
			}
			vertex_mark[v] = black;
		}

		void dfs_2(std::vector<VertexMark> &vertex_mark, const Graph &g, const Vertex v, std::vector<Vertex> &result) {
			vertex_mark[v] = grey;
			for (Vertex i: g.get_neighbors(v)) {
				if (vertex_mark[i] == white) {
					dfs_2(vertex_mark, g, i, result); 
				}
			}
			result.push_back(v);
		}

		bool dfs(std::vector<VertexMark> &vertex_mark, const Graph &g, const Vertex v) {
			vertex_mark[v] = grey;
			for (Vertex i: g.get_neighbors(v)) {
				if (vertex_mark[i] == white) {
					if (!dfs(vertex_mark, g, i)) {
						return 0;
					} 
				}
				if (vertex_mark[i] == grey) {
					return 0;
				}
			}
			vertex_mark[v] = black;
			return 1;
		}
	}

	std::vector<Vertex> top_sort(const Graph &g) {
		std::vector<VertexMark> vertex_mark(g.get_vertex_count(), white);
		std::vector<Vertex> result;
		for (size_t i = 0; i < g.get_vertex_count(); ++i) {
			if (vertex_mark[i] == white) {
				dfs_2(vertex_mark, g, i, result);
			}
		}
		return result;
	}

	void bfs (const Graph &g, Vertex v, std::vector<size_t> &distance, std::vector<int> &parent) {
		std::vector<VertexMark> vertex_mark(g.get_vertex_count(), white);
		std::queue<Vertex> vertex_queue;
		vertex_mark[v] = grey;
		vertex_queue.push(v);
		while (!vertex_queue.empty()) {
			Vertex u = vertex_queue.front();
			vertex_queue.pop();
			for (Vertex i: g.get_neighbors(u)) {
				if (vertex_mark[i] == white) {
					vertex_mark[i] = grey;
					distance[i] = distance[u] + 1;
					parent[i] = u;
					vertex_queue.push(i);
				}
			}
			vertex_mark[u] = black;
		}
	}
	
	std::vector<Vertex> shortest_path(const Graph &g, const Vertex start, const Vertex finish) {
		const int NO_PARENT = -1;
		std::vector<size_t> distance(g.get_vertex_count(), 0);
		std::vector<int> parent(g.get_vertex_count(), NO_PARENT);
		std::vector<Vertex> result;
		bfs(g, start, distance, parent);
		if (start == finish) {
			result.push_back(start);
			return result;
		}
		if (parent[finish] == NO_PARENT) {
			return result;
		}
		size_t current = finish;
		while (start != current) {
			result.push_back(current);
			current = parent[current];
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
				dfs_1(vertex_mark, g, v, components);
			}
		}
		return components;
	}

	bool check_acyclicity(const Graph &g) {
		std::vector<VertexMark> vertex_mark(g.get_vertex_count(), white);
		for (Vertex i = 0; i < g.get_vertex_count(); ++i) {
			if (vertex_mark[i] == white) {
				if (!dfs(vertex_mark, g, i)) {
					return 0;
				}
			}
		}
		return 1;
	}

	std::vector<Vertex> getComponent(const Graph &g, const Vertex v) {
		std::vector<std::vector<Vertex>> component;
		component.push_back(std::vector<Vertex>());
		std::vector<VertexMark> vertex_mark(g.get_vertex_count(), white);
		dfs_1(vertex_mark, g, v, component);
		std::vector<Vertex> result = component[0];
		return result;
	}

}

int main(){
	int n, s;
	std::cin >> n >> s;
	GraphAdjList g(n, false);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			bool flag;
			std::cin >> flag;
			if (flag) {
				g.add_edge(i, j);
			}
		}
	}
	std::cout << GraphProcessing::getComponent(g, s - 1	).size();
}
