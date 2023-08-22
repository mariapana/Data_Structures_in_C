# Graphs
A graph is a collection of nodes (vertices) with edges between (some of) them.

<img src="https://www.boardinfinity.com/blog/content/images/2023/01/Graphs-in-DSA.png" alt=Graph width=30%>

## Representation
* Adjacency Matrix (1 if nodes are adjacent, otherwise 0)
* Adjacency List (vector of lists containing node neighbours)

![Undirected graph](https://notes.shichao.io/clrs/figure_22.1.png)

![Directed graph](https://notes.shichao.io/clrs/figure_22.2.png)

| Space Complexity | Adj Matrix | Adj List |
| --- | --- | --- |
| avg. case | O(V^2) | O(V + E) |
| worst case | O(V^2) | O(V + E) |

<br>

| Time Complexity | Adj Matrix | Adj List |
| --- | --- | --- |
| Adding a vertex | O(V^2) | O(1) |
| Removing a vertex | O(V^2) | O(V + E) |
| Adding an edge | O(1) | O(1) |
| Removing an edge | O(1) | O(E) |
| Edge query | O(1) | O(V) |
| Finding neighbours | O(V) | O(V) |
| Traversal | slow | faster |

<br>

obs 1: Undirected graph <--> symmetric adjacency matrix

obs 2: Adjacency lists are usually prefered, especially for sparse graphs.

obs 3: There are some situations when the adjacency matrix is prefered (Floyd-Warshall, Complement, Transpose)

<br>

## Searching

<img src="https://www.freelancinggig.com/blog/wp-content/uploads/2019/02/BFS-and-DFS-Algorithms.png" alt=BFSvsDFS width=50%>

## BFS - Breadth-First Search
_Complexity_: **O(V + E)**

_Main idea_: start at the given node (lvl 0); visit neighbours of given node (lvl 1); visit unvisited neighbours of nodes on previous level (lvl 2) etc.

* We go **WIDE**, level by level, visiting the siblings before the children
* Uses a queue (FIFO)

_Pseudocode_:
```
create a queue Q 
mark v as visited
Q.enqueue(v)

while Q is not empty:
    x = Q.dequeue()
    for y in Neighbors(x):
        if (y has not been marked as visited) then
            mark y as visited
            Q.enqueue(y)

```

<img src="https://www.algotree.org/images/BFS_Example_Run.svg" alt=BFSexplained width=60%>

obs: BFS is best used for searching vertices close to the source (ex: determining the shortest path, checking if a graph
is bipartite etc.)

obs: BFS is a bit slower than DFS and requires more memory.

<br>

## DFS - Depth-First Search
_Complexity_: **O(V + E)**

_Main idea_: start at the given node; explore each branch completely before moving on to the next branch

* We go **DEEP**, subtree by subtree, visiting all children before the siblings
* Uses a stack (LIFO) / recursion

_Pseudocode_ (recursive DFS):
```
DFS(G, u)
    u.visited = true
    for each neighbour v ∈ G.Adj[u]
        if v.visited == false
            DFS(G,v)
     
init() {
    for each u ∈ G
        u.visited = false
    for each u ∈ G
       DFS(G, u)
}
```

<img src="https://algotree.org/images/DFS_Example_Run.svg" alt=DFSexplained width=60%>

<br>

obs: **Timed DFS** - find the arrival and departure time of its vertices in DFS

```
TIME = 0
dfs (v):
    arrival_time[v] = TIME ++
    visited[v] = true
    for u in adj[v]:
            if visited[u] == false
                    then dfs(u)

    departure_time[v] = TIME ++
```

Applications of finding Arrival and Departure Time:
* Topological sorting in a DAG (Directed Acyclic Graph).
* Finding 2/3–(edge or vertex)–connected components.
* Finding bridges in graphs.
* Finding biconnectivity in graphs.
* Detecting cycle in directed graphs.
* Tarjan’s algorithm to find strongly connected components, and many more…

Obs: CC - DFS + BFS; Cycle - DFS + BFS; Shortest path - BFS; Bipartite - BFS; Topological Sort - DFS

<br>

### Terminology
____________________
**Directed** graph

**Undirected** graph

<br>

**Adjacent nodes**

**Incident edges**

<br>

**Conex** graph: undirected graph; for each pair of vertices, there exists at least one single path which joins them

**Complete** graph: directed graph: every pair of distinct vertices is connected by a pair of unique edges (one in each direction)

<br>

**Bipartite** graph: its nodes can be divided into two different sets U and V such that every edge connects a node from U with a node in V

<br>

**Partial graph**: same nodes, fewer edges

**Subgraph**: fewer nodes, same edges connecting the remaining nodes

<br>

**Clique**: subset of nodes of an undirected graph such that the described subgraph is complete

<br>

**Connected Components**: subgraph of an undirected graph, in which every vertex is reachable from every other vertex

**Strongly Connected Components**: subgraph of a directed graph, in which every vertex is reachable from every other vertex

<br>

**Cycle**: the path described ends in the same point it began

**Eulerian Cycle**: goes through all edges exactly once (ex: the 7 bridges problem)

**Hamiltonian Cycle**: goes through all nodes exactly once (!simple cycle: no repetitions)

<br>

**Graph Complement/Inverse**: fill missing edges required to form a complete graph and remove all edges that were previously there

**Graph Transpose/Reverse**: change orientation of the edges

<br>

**Topological Sort**: (!only for DAGs) placing the nodes along a horizontal line so that all edges are directed from left to right (there are no eges pointing to the parents, quite like a genealogical tree)
