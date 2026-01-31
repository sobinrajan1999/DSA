# Union-Find (Disjoint Set Union) - Complete DSA Documentation

## Table of Contents
1. [Introduction](#introduction)
2. [Core Concepts](#core-concepts)
3. [Implementation in C++](#implementation-in-cpp)
4. [Optimization Techniques](#optimization-techniques)
5. [Time and Space Complexity](#time-and-space-complexity)
6. [Applications](#applications)
7. [Advantages and Disadvantages](#advantages-and-disadvantages)
8. [Interview Questions and Answers](#interview-questions-and-answers)
9. [Coding Problems for Practice](#coding-problems-for-practice)
10. [Quick Reference](#quick-reference)

---

## Introduction

**Union-Find** (also called **Disjoint Set Union** or **DSU**) is a data structure that efficiently keeps track of elements partitioned into disjoint (non-overlapping) sets. It provides near-constant-time operations to:

- **Find**: Determine which set an element belongs to
- **Union**: Merge two sets into one
- **Connected**: Check if two elements are in the same set

### Why Union-Find?

Union-Find is particularly useful when you need to:
- Track connected components in a graph
- Detect cycles in undirected graphs
- Implement Kruskal's Minimum Spanning Tree algorithm
- Solve dynamic connectivity problems

---

## Core Concepts

### Fundamental Operations

1. **MakeSet(x)**: Creates a new set containing only element x
2. **Find(x)**: Returns the representative (root) of the set containing x
3. **Union(x, y)**: Merges the sets containing x and y

### Internal Representation

- Each set is represented as a **tree structure**
- The **root** of each tree is the representative of that set
- Each element stores a pointer to its **parent** in the tree
- Two elements are in the same set if they have the **same root**

### Visual Example

```
Initial state (each element is its own set):
0   1   2   3   4

After Union(0, 1):
  0       2   3   4
 /
1

After Union(2, 3):
  0       2       4
 /       /
1       3

After Union(0, 2):
    0           4
   / \
  1   2
     /
    3
```

---

## Implementation in C++

### Basic Implementation (Without Optimizations)

This is the simplest version to understand the concept:

```cpp
class UnionFind {
private:
    vector<int> parent;
    
public:
    // Constructor: Initialize each element as its own parent
    UnionFind(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;  // Each element is initially its own parent
        }
    }
    
    // Find: Returns the root of the set containing x
    int find(int x) {
        if (parent[x] == x) {
            return x;  // x is the root
        }
        return find(parent[x]);  // Recursively find the root
    }
    
    // Union: Merge sets containing x and y
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX != rootY) {
            parent[rootX] = rootY;  // Make one root the parent of the other
        }
    }
    
    // Check if x and y are connected
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};
```

**Time Complexity**: O(n) in worst case for both Find and Union
**Problem**: Trees can become skewed (like a linked list), leading to O(n) operations

---

### Optimized Implementation (With Path Compression and Union by Rank)

This version uses two critical optimizations:

```cpp
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;        // Rank for union by rank
    int components;          // Number of disjoint sets
    
public:
    // Constructor
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);   // Initialize all ranks to 0
        components = n;       // Initially, n separate components
        
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    // Find with Path Compression
    int find(int x) {
        if (parent[x] != x) {
            // Path compression: make parent point directly to root
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    // Union by Rank
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) {
            return false;  // Already in the same set
        }
        
        // Attach smaller rank tree under root of higher rank tree
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;  // Increase rank only when ranks are equal
        }
        
        components--;
        return true;
    }
    
    // Check if x and y are connected
    bool connected(int x, int y) {
        return find(x) == find(y);
    }
    
    // Get number of disjoint components
    int getComponents() {
        return components;
    }
    
    // Get size of the set containing x
    int getSetSize(int x) {
        int root = find(x);
        int count = 0;
        for (int i = 0; i < parent.size(); i++) {
            if (find(i) == root) {
                count++;
            }
        }
        return count;
    }
};
```

**Time Complexity**: O(α(n)) per operation, where α is the inverse Ackermann function
- α(n) is extremely slow-growing and effectively constant for all practical values
- For n < 10^600, α(n) ≤ 4

**Space Complexity**: O(n)

---

### Alternative: Union by Size

Instead of rank, you can track the actual size of each set:

```cpp
class UnionFind {
private:
    vector<int> parent;
    vector<int> size;  // Size of each set
    int components;
    
public:
    UnionFind(int n) {
        parent.resize(n);
        size.resize(n, 1);  // Each set initially has size 1
        components = n;
        
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);  // Path compression
        }
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) {
            return false;
        }
        
        // Attach smaller tree to larger tree
        if (size[rootX] < size[rootY]) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        } else {
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
        }
        
        components--;
        return true;
    }
    
    int getSetSize(int x) {
        return size[find(x)];
    }
    
    int getComponents() {
        return components;
    }
};
```

---

## Optimization Techniques

### 1. Path Compression

**What it does**: During the Find operation, makes every node on the path point directly to the root.

**How it works**:
```cpp
int find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);  // Recursively find root and compress path
    }
    return parent[x];
}
```

**Visual Example**:
```
Before path compression (finding 4):
    0
   /
  1
 /
2
/
3
/
4

After path compression:
    0
   /|\\\
  1 2 3 4
```

**Benefit**: Flattens the tree structure, making subsequent operations much faster.

### 2. Union by Rank

**What it does**: Always attaches the tree with smaller rank under the root of the tree with larger rank.

**How it works**:
- Maintain a "rank" (approximate tree height) for each root
- When uniting, the tree with smaller rank becomes a subtree of the other
- Only increase rank when both trees have equal rank

**Benefit**: Prevents trees from becoming too tall, keeping operations fast.

### 3. Union by Size (Alternative)

**What it does**: Attaches the smaller set to the larger set.

**Benefit**: Similar performance to union by rank, but easier to track actual set sizes.

---

## Time and Space Complexity

| Operation | Without Optimization | With Path Compression Only | With Both Optimizations |
|-----------|---------------------|---------------------------|------------------------|
| MakeSet | O(1) | O(1) | O(1) |
| Find | O(n) | O(log n) amortized | O(α(n)) amortized |
| Union | O(n) | O(log n) amortized | O(α(n)) amortized |
| Connected | O(n) | O(log n) amortized | O(α(n)) amortized |

**Space Complexity**: O(n) for storing parent and rank/size arrays

**Note**: α(n) is the inverse Ackermann function, which grows so slowly that it's ≤ 4 for any practical input size.

---

## Applications

### 1. Network Connectivity

Determine if two computers are connected in a network.

```cpp
bool areConnected(int computer1, int computer2, UnionFind& network) {
    return network.connected(computer1, computer2);
}
```

### 2. Kruskal's Minimum Spanning Tree

Union-Find is essential for efficiently detecting cycles:

```cpp
struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

vector<Edge> kruskalMST(vector<Edge>& edges, int n) {
    sort(edges.begin(), edges.end());
    UnionFind uf(n);
    vector<Edge> mst;
    
    for (auto& edge : edges) {
        if (uf.unite(edge.u, edge.v)) {
            mst.push_back(edge);  // No cycle created, add to MST
        }
    }
    
    return mst;
}
```

### 3. Connected Components in a Graph

Count the number of connected components:

```cpp
int countComponents(int n, vector<vector<int>>& edges) {
    UnionFind uf(n);
    for (auto& edge : edges) {
        uf.unite(edge[0], edge[1]);
    }
    return uf.getComponents();
}
```

### 4. Cycle Detection in Undirected Graphs

```cpp
bool hasCycle(int n, vector<vector<int>>& edges) {
    UnionFind uf(n);
    for (auto& edge : edges) {
        if (!uf.unite(edge[0], edge[1])) {
            return true;  // Edge connects already connected nodes = cycle
        }
    }
    return false;
}
```

### 5. Image Processing (Flood Fill)

Find connected regions of similar pixels.

### 6. Social Network Analysis

Group people into friend circles or communities.

### 7. Percolation Problems

Determine if a system percolates (physics/chemistry applications).

---

## Advantages and Disadvantages

### Advantages ✅

1. **Extremely Fast Operations**: Near O(1) amortized time with optimizations
2. **Simple to Implement**: Clean, concise code
3. **Space Efficient**: Only O(n) space required
4. **Versatile**: Can track additional information (sizes, etc.)
5. **Well-Studied**: Proven mathematical guarantees
6. **Dynamic**: Supports adding connections incrementally
7. **No Preprocessing**: Works on-the-fly as edges are added

### Disadvantages ❌

1. **No Deletion Support**: Cannot efficiently split sets or remove elements
2. **Static Size**: Number of elements must be known in advance
3. **Limited Query Types**: Only connectivity queries, not paths or distances
4. **No Edge Information**: Doesn't store which specific edges connect elements
5. **Undirected Only**: Doesn't handle directed graph relationships
6. **No Disconnect**: Once connected, cannot disconnect elements

---

## Interview Questions and Answers

### Q1: What is Union-Find and when would you use it?

**Answer**: Union-Find (Disjoint Set Union) is a data structure that efficiently tracks elements partitioned into disjoint sets. I would use it when I need to:
- Determine if two elements are in the same group/component
- Dynamically merge groups together
- Detect cycles in undirected graphs
- Implement algorithms like Kruskal's MST

The key advantage is that with path compression and union by rank optimizations, both find and union operations run in nearly constant time O(α(n)).

---

### Q2: Explain path compression. Why is it important?

**Answer**: Path compression is an optimization technique used in the Find operation. When we traverse from a node to find its root, we update all nodes along the path to point directly to the root.

**Code**:
```cpp
int find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);  // Compress path during recursion
    }
    return parent[x];
}
```

**Importance**: Without path compression, repeatedly finding the same element could take O(n) time if the tree is skewed. With path compression, the tree becomes flatter, making subsequent operations much faster.

**Example**: If we have chain 1→2→3→4→5, after `find(1)`, the structure becomes 1→5, 2→5, 3→5, 4→5 (all directly connected to root).

---

### Q3: What's the difference between union by rank and union by size?

**Answer**: Both are optimization techniques for the Union operation:

**Union by Rank**:
- Maintains an approximate tree height (rank)
- Attaches tree with smaller rank under tree with larger rank
- Only increases rank when both trees have equal rank
- Rank is an upper bound on tree height

**Union by Size**:
- Tracks actual number of elements in each set
- Attaches smaller set to larger set
- Updates size when merging

**Similarities**: Both achieve the same O(α(n)) time complexity

**Differences**:
- Union by size allows you to query set sizes easily
- Union by rank is slightly simpler theoretically
- In practice, performance is nearly identical

**My preference**: Union by size, because knowing set sizes is often useful in problems.

---

### Q4: Can you detect cycles in a directed graph using Union-Find?

**Answer**: No, Union-Find is only suitable for undirected graphs. For directed graphs, you need:
- DFS with color marking (white/gray/black)
- Topological sort
- Tarjan's algorithm for strongly connected components

**Why?**: Union-Find treats edges as bidirectional. In a directed graph, an edge from A to B doesn't imply B can reach A, but Union-Find would connect them in both directions.

---

### Q5: What's the time complexity of Union-Find with both optimizations?

**Answer**: With both path compression and union by rank (or size):
- **Time**: O(α(n)) per operation (amortized)
  - α(n) is the inverse Ackermann function
  - For all practical values (n < 10^600), α(n) ≤ 4
  - Effectively constant time

- **Space**: O(n) for parent and rank/size arrays

**Without optimizations**: O(n) per operation in worst case (skewed tree)

**With only path compression**: O(log n) amortized

---

### Q6: How would you find all elements in the same set as element x?

**Answer**:
```cpp
vector<int> getSetElements(int x, UnionFind& uf) {
    int root = uf.find(x);
    vector<int> elements;
    
    for (int i = 0; i < uf.parent.size(); i++) {
        if (uf.find(i) == root) {
            elements.push_back(i);
        }
    }
    
    return elements;
}
```

**Time Complexity**: O(n × α(n)) ≈ O(n)

**Note**: Union-Find isn't optimized for this query. If you frequently need to enumerate all elements in a set, consider maintaining additional data structures like a hash map from root to element lists.

---

### Q7: How do you implement "undo" in Union-Find?

**Answer**: Standard Union-Find doesn't support undo efficiently because path compression modifies the structure permanently. To support undo:

**Approach 1**: Disable path compression and maintain a stack of operations
```cpp
struct Operation {
    int node;
    int oldParent;
    int oldRank;
};

stack<Operation> history;

void undo() {
    if (history.empty()) return;
    Operation op = history.top();
    history.pop();
    parent[op.node] = op.oldParent;
    rank[op.oldParent] = op.oldRank;
}
```

**Approach 2**: Use persistent data structures (more complex)

**Trade-off**: Undo capability sacrifices the efficiency of path compression.

---

### Q8: Can you explain the inverse Ackermann function α(n)?

**Answer**: The inverse Ackermann function α(n) is an extremely slow-growing function.

**Ackermann function** A(m, n) grows faster than any primitive recursive function:
- A(0, n) = n + 1
- A(1, n) = n + 2
- A(2, n) = 2n + 3
- A(3, n) = 2^(n+3) - 3
- A(4, n) = 2^2^2^... (n+3 times)

**Inverse Ackermann** α(n) is the inverse: the number of times you must apply A to get from 4 to n.

**Practical values**:
- α(1) = 1
- α(3) = 2
- α(7) = 3
- α(2047) = 4
- α(2^2047) = 5

For any practical input size (even astronomical), α(n) ≤ 5, making it effectively constant.

---

### Q9: What's the difference between Union-Find and BFS/DFS for connectivity?

**Answer**:

| Aspect | Union-Find | BFS/DFS |
|--------|-----------|---------|
| **Dynamic edges** | Excellent - O(α(n)) per edge | Poor - must rerun O(V+E) |
| **One-time query** | Overkill | Better - O(V+E) once |
| **Multiple queries** | Excellent - O(α(n)) each | Poor - O(V+E) each time |
| **Find path** | Cannot | Can find actual path |
| **Shortest path** | Cannot | BFS can |
| **Memory** | O(V) | O(V) |

**Use Union-Find when**: Many dynamic updates and connectivity queries
**Use BFS/DFS when**: Static graph, need actual paths, or one-time analysis

---

### Q10: How would you modify Union-Find to track the number of components?

**Answer**: Add a counter that decrements when successful unions occur:

```cpp
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;
    int components;  // Track number of components
    
public:
    UnionFind(int n) : components(n) {  // Initially n components
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        
        if (rootX == rootY) return false;
        
        // Merge logic...
        
        components--;  // Decrement when merging
        return true;
    }
    
    int getComponents() {
        return components;
    }
};
```

**Key insight**: Start with n components (each element alone). Every successful union reduces components by 1.

---

## Coding Problems for Practice

### Problem 1: Number of Connected Components (Easy)

**Problem**: Given `n` nodes labeled from `0` to `n-1` and a list of undirected edges, return the number of connected components.

**Example**:
```
Input: n = 5, edges = [[0,1], [1,2], [3,4]]
Output: 2
Explanation: 0-1-2 form one component, 3-4 form another
```

**Solution**:
```cpp
int countComponents(int n, vector<vector<int>>& edges) {
    UnionFind uf(n);
    
    for (auto& edge : edges) {
        uf.unite(edge[0], edge[1]);
    }
    
    return uf.getComponents();
}
```

**Time**: O(E × α(n)), **Space**: O(n)

---

### Problem 2: Redundant Connection (Medium)

**Problem**: Given a graph that started as a tree with `n` nodes, one additional edge was added. Find the edge that can be removed to make it a tree again. If multiple answers exist, return the last one in the input.

**Example**:
```
Input: edges = [[1,2], [1,3], [2,3]]
Output: [2,3]
Explanation: Removing [2,3] leaves a valid tree
```

**Solution**:
```cpp
vector<int> findRedundantConnection(vector<vector<int>>& edges) {
    int n = edges.size();
    UnionFind uf(n + 1);  // Nodes are 1-indexed
    
    for (auto& edge : edges) {
        // If unite returns false, these nodes are already connected
        // This edge creates a cycle
        if (!uf.unite(edge[0], edge[1])) {
            return edge;
        }
    }
    
    return {};
}
```

**Time**: O(E × α(n)), **Space**: O(n)

---

### Problem 3: Accounts Merge (Medium)

**Problem**: Given a list of accounts where each account has a name and list of emails, merge accounts that share at least one email.

**Example**:
```
Input: accounts = [
  ["John", "john@mail.com", "john_work@mail.com"],
  ["John", "john@mail.com", "john_home@mail.com"],
  ["Mary", "mary@mail.com"]
]
Output: [
  ["John", "john@mail.com", "john_home@mail.com", "john_work@mail.com"],
  ["Mary", "mary@mail.com"]
]
```

**Solution**:
```cpp
vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
    int n = accounts.size();
    UnionFind uf(n);
    unordered_map<string, int> emailToId;
    
    // Map each email to its first occurrence account ID
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < accounts[i].size(); j++) {
            string email = accounts[i][j];
            if (emailToId.find(email) != emailToId.end()) {
                uf.unite(i, emailToId[email]);
            } else {
                emailToId[email] = i;
            }
        }
    }
    
    // Group emails by root account ID
    unordered_map<int, set<string>> merged;
    for (auto& [email, id] : emailToId) {
        int root = uf.find(id);
        merged[root].insert(email);
    }
    
    // Build result
    vector<vector<string>> result;
    for (auto& [id, emails] : merged) {
        vector<string> account;
        account.push_back(accounts[id][0]);  // Name
        account.insert(account.end(), emails.begin(), emails.end());
        result.push_back(account);
    }
    
    return result;
}
```

**Time**: O(N × K × α(N)) where N is accounts, K is emails per account
**Space**: O(N × K)

---

### Problem 4: Number of Provinces (Medium)

**Problem**: Given an `n x n` matrix `isConnected` where `isConnected[i][j] = 1` if cities `i` and `j` are directly connected, find the number of provinces (groups of connected cities).

**Example**:
```
Input: isConnected = [[1,1,0],
                      [1,1,0],
                      [0,0,1]]
Output: 2
Explanation: City 0 and 1 are connected (province 1), city 2 is alone (province 2)
```

**Solution**:
```cpp
int findCircleNum(vector<vector<int>>& isConnected) {
    int n = isConnected.size();
    UnionFind uf(n);
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (isConnected[i][j] == 1) {
                uf.unite(i, j);
            }
        }
    }
    
    return uf.getComponents();
}
```

**Time**: O(n² × α(n)), **Space**: O(n)

---

### Problem 5: Smallest String With Swaps (Medium)

**Problem**: Given a string `s` and pairs of indices that can be swapped any number of times, return the lexicographically smallest string.

**Example**:
```
Input: s = "dcab", pairs = [[0,3], [1,2]]
Output: "bacd"
Explanation: Swap 0,3 → "bcad", then 1,2 → "bacd"
```

**Solution**:
```cpp
string smallestStringWithSwaps(string s, vector<vector<int>>& pairs) {
    int n = s.length();
    UnionFind uf(n);
    
    // Connect all swappable indices
    for (auto& pair : pairs) {
        uf.unite(pair[0], pair[1]);
    }
    
    // Group indices by component
    unordered_map<int, vector<int>> components;
    for (int i = 0; i < n; i++) {
        components[uf.find(i)].push_back(i);
    }
    
    string result = s;
    
    // Sort characters within each component
    for (auto& [root, indices] : components) {
        string chars;
        for (int idx : indices) {
            chars += s[idx];
        }
        sort(chars.begin(), chars.end());
        sort(indices.begin(), indices.end());
        
        for (int i = 0; i < indices.size(); i++) {
            result[indices[i]] = chars[i];
        }
    }
    
    return result;
}
```

**Time**: O(n log n + E × α(n)), **Space**: O(n)

---

### Problem 6: Satisfiability of Equality Equations (Medium)

**Problem**: Given array of equations like "a==b" or "a!=b", determine if all equations can be satisfied.

**Example**:
```
Input: equations = ["a==b", "b!=a"]
Output: false
Explanation: a==b means they're equal, but b!=a contradicts this
```

**Solution**:
```cpp
bool equationsPossible(vector<string>& equations) {
    UnionFind uf(26);  // 26 letters
    
    // First, union all equalities
    for (auto& eq : equations) {
        if (eq[1] == '=') {
            uf.unite(eq[0] - 'a', eq[3] - 'a');
        }
    }
    
    // Then check if any inequalities are violated
    for (auto& eq : equations) {
        if (eq[1] == '!') {
            if (uf.connected(eq[0] - 'a', eq[3] - 'a')) {
                return false;  // Contradiction found
            }
        }
    }
    
    return true;
}
```

**Time**: O(n × α(26)) ≈ O(n), **Space**: O(26) = O(1)

---

### Problem 7: Minimize Malware Spread (Hard)

**Problem**: Given a graph and initial infected nodes, which single node should be removed to minimize the final infection spread?

**Example**:
```
Input: graph = [[1,1,0],
                [1,1,0],
                [0,0,1]], 
       initial = [0,1]
Output: 0
```

**Solution**:
```cpp
int minMalwareSpread(vector<vector<int>>& graph, vector<int>& initial) {
    int n = graph.size();
    UnionFind uf(n);
    
    // Build connected components
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (graph[i][j] == 1) {
                uf.unite(i, j);
            }
        }
    }
    
    // Count infected nodes in each component
    unordered_map<int, int> infectedCount;
    for (int node : initial) {
        infectedCount[uf.find(node)]++;
    }
    
    int result = *min_element(initial.begin(), initial.end());
    int maxSaved = 0;
    
    for (int node : initial) {
        int root = uf.find(node);
        
        // If this is the only infected node in its component
        if (infectedCount[root] == 1) {
            int componentSize = uf.getSetSize(root);
            if (componentSize > maxSaved || 
                (componentSize == maxSaved && node < result)) {
                maxSaved = componentSize;
                result = node;
            }
        }
    }
    
    return result;
}
```

**Time**: O(n² × α(n)), **Space**: O(n)

---

### Problem 8: Longest Consecutive Sequence (Medium - Creative Union-Find Use)

**Problem**: Given an unsorted array, find the length of the longest consecutive elements sequence.

**Example**:
```
Input: nums = [100, 4, 200, 1, 3, 2]
Output: 4
Explanation: [1, 2, 3, 4] is the longest consecutive sequence
```

**Solution**:
```cpp
int longestConsecutive(vector<int>& nums) {
    if (nums.empty()) return 0;
    
    unordered_map<int, int> parent;
    unordered_map<int, int> size;
    
    // Initialize each number as its own set
    for (int num : nums) {
        parent[num] = num;
        size[num] = 1;
    }
    
    function<int(int)> find = [&](int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };
    
    auto unite = [&](int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            parent[rootX] = rootY;
            size[rootY] += size[rootX];
        }
    };
    
    // Union consecutive numbers
    for (int num : nums) {
        if (parent.count(num + 1)) {
            unite(num, num + 1);
        }
    }
    
    int maxLen = 0;
    for (auto& [num, _] : parent) {
        maxLen = max(maxLen, size[find(num)]);
    }
    
    return maxLen;
}
```

**Time**: O(n × α(n)), **Space**: O(n)

---

### Problem 9: Swim in Rising Water (Hard)

**Problem**: Given an `n x n` grid where each cell has an elevation, find the minimum time needed to swim from top-left to bottom-right. At time `t`, water level is `t`, and you can only swim to cells with elevation ≤ `t`.

**Solution**:
```cpp
int swimInWater(vector<vector<int>>& grid) {
    int n = grid.size();
    UnionFind uf(n * n);
    
    vector<pair<int, pair<int, int>>> cells;  // {elevation, {row, col}}
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cells.push_back({grid[i][j], {i, j}});
        }
    }
    
    sort(cells.begin(), cells.end());
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    for (auto& [elevation, pos] : cells) {
        int r = pos.first, c = pos.second;
        int curr = r * n + c;
        
        // Connect to adjacent cells with lower elevation
        for (int d = 0; d < 4; d++) {
            int nr = r + dx[d], nc = c + dy[d];
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && 
                grid[nr][nc] <= elevation) {
                uf.unite(curr, nr * n + nc);
            }
        }
        
        // Check if start and end are connected
        if (uf.connected(0, n * n - 1)) {
            return elevation;
        }
    }
    
    return -1;
}
```

**Time**: O(n² log n²), **Space**: O(n²)

---

### Problem 10: Minimize Hamming Distance After Swap Operations (Medium)

**Problem**: Given two arrays `source` and `target`, and a list of swap indices, minimize the Hamming distance after performing any number of swaps.

**Solution**:
```cpp
int minimumHammingDistance(vector<int>& source, vector<int>& target, 
                           vector<vector<int>>& allowedSwaps) {
    int n = source.size();
    UnionFind uf(n);
    
    for (auto& swap : allowedSwaps) {
        uf.unite(swap[0], swap[1]);
    }
    
    // Group indices by component
    unordered_map<int, vector<int>> components;
    for (int i = 0; i < n; i++) {
        components[uf.find(i)].push_back(i);
    }
    
    int hamming = 0;
    
    // For each component, count optimal matching
    for (auto& [root, indices] : components) {
        unordered_map<int, int> sourceCount, targetCount;
        
        for (int idx : indices) {
            sourceCount[source[idx]]++;
            targetCount[target[idx]]++;
        }
        
        int matches = 0;
        for (auto& [val, count] : sourceCount) {
            matches += min(count, targetCount[val]);
        }
        
        hamming += indices.size() - matches;
    }
    
    return hamming;
}
```

**Time**: O(n × α(n)), **Space**: O(n)

---

## Quick Reference

### When to Use Union-Find

✅ **Use Union-Find when you see these patterns:**
- "Connected components"
- "Detect cycles" (undirected graphs)
- "Group elements" based on relationships
- "Minimum spanning tree"
- "Dynamic connectivity" with incremental edge additions
- "Merge sets" operations

❌ **Don't use Union-Find for:**
- Directed graphs (use DFS/Tarjan's)
- Finding shortest paths (use BFS/Dijkstra)
- Removing edges/disconnecting (standard UF doesn't support this)
- When you need actual paths, not just connectivity

### Implementation Checklist

```cpp
class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;     // or size
    int components;        // optional but useful
    
public:
    // ✅ Initialize parent[i] = i
    UnionFind(int n);
    
    // ✅ Implement path compression
    int find(int x);
    
    // ✅ Implement union by rank/size
    bool unite(int x, int y);
    
    // ✅ Helper function
    bool connected(int x, int y);
    
    // ✅ Track components (optional)
    int getComponents();
};
```

### Common Mistakes to Avoid

1. ❌ Forgetting path compression
2. ❌ Initializing parent[i] = -1 instead of parent[i] = i
3. ❌ Not checking if already connected before union
4. ❌ Using Union-Find for directed graphs
5. ❌ Not handling 0-indexed vs 1-indexed problems
6. ❌ Forgetting to decrease component count in unite()

### Complexity Cheat Sheet

| Optimization | Find | Union | Notes |
|-------------|------|-------|-------|
| None | O(n) | O(n) | Avoid this |
| Path Compression only | O(log n) | O(log n) | Amortized |
| Union by Rank only | O(log n) | O(log n) | Worst case |
| Both | O(α(n)) | O(α(n)) | ⭐ Best - use this! |

**α(n) ≈ constant** for all practical purposes

### LeetCode Practice Progression

**Easy** → **Medium** → **Hard**

1. 547. Number of Provinces
2. 684. Redundant Connection
3. 721. Accounts Merge
4. 200. Number of Islands (alternative to BFS/DFS)
5. 1319. Number of Operations to Make Network Connected
6. 1202. Smallest String With Swaps
7. 990. Satisfiability of Equality Equations
8. 1584. Min Cost to Connect All Points (Kruskal's MST)
9. 128. Longest Consecutive Sequence (creative use)
10. 778. Swim in Rising Water

---

**End of Documentation**

*Last updated: January 2026*
*For interview preparation and future reference*