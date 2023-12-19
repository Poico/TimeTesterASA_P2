/*************************************************************
 * IST - ASA 23/24 - Projecto 2 - TugaNet instance generator *
 *************************************************************/
#include <iostream>
#include <vector>
#include <list>
#include <unordered_set>

using namespace std;

// Returns a random value between [0, m-1]
#define randomValue(m) (rand() % m)

typedef struct { int u, v; } Edge;

int _V, _E;
int _maxE, _numNets, _minV, _maxV;

list<int>* _g;               // Adjacency list
unordered_set<int>* _edges;  // Adjacency set
vector<int> _netVert;        // #V in each Network
vector<int> _initNetID;      // #V in each Network
vector<int> _mapVertNet;     // Map each vertex into SCC ID
vector<int> _vertexMap;      // Map vertex into new ID

//-----------------------------------------------------------------------------

void printUsage(char *progname) {
  cerr << "Usage: " << progname << " <V> <E> <SubN> <m> <M> <seed>" << endl;
  cerr << "  V: number of individuals in the social network" << endl;
  cerr << "  E: number of connections between individuals" << endl;
  cerr << "  SubN: number of sub-networks" << endl;
  cerr << "  m: minimal number of individuals per sub-network (optional, default: 1)" << endl;
  cerr << "  M: maximal number of individuals per sub-network (optional, default: 10)" << endl;
  cerr << "  seed: random seed generator (opcional)" << endl;
  exit(1);
}

inline void addEdge(int u, int v) {
  _g[u].push_back(v);
  _edges[u].insert(v);
  _g[v].push_back(u);
  _edges[v].insert(u);
  _E++;
}

void parseArgs(int argc, char **argv) {
  int seed = 0;

  if (argc < 4 || argc > 7) {
    cerr << "ERROR: Wrong number of arguments" << endl;
    printUsage(argv[0]);
  }

  sscanf(argv[1], "%d", &_V);
  sscanf(argv[2], "%d", &_maxE);
  sscanf(argv[3], "%d", &_numNets);
  if (_numNets > _V) {
    cerr << "ERROR: SubN cannot be bigger than V" << endl;
    printUsage(argv[0]);
    exit(0);
  }

  if (argc >= 5) sscanf(argv[4], "%d", &_minV);
  else _minV = 1;
  if (argc >= 6) sscanf(argv[5], "%d", &_maxV);
  else _maxV = 10;
  if (argc >= 7) sscanf(argv[6], "%d", &seed);
  if (_maxV < _minV) {
    cerr << "ERROR: m cannot be bigger than M" << endl;
    exit(0);
  }

  if (_numNets * _minV > _V) {
    cerr << "ERROR: V cannot be smaller than SubN * m" << endl;
    printUsage(argv[0]);
    exit(0);
  }

  if (_numNets * _maxV < _V) {
    cerr << "ERROR: V cannot be bigger than SubN * M" << endl;
    printUsage(argv[0]);
    exit(0);
  }

  if (argc == 7) {
    sscanf(argv[7], "%d", &seed);
    srand(seed);
  } else { // pseudo-random seed
    srand((unsigned int)time(NULL));
  }
}

void initDataStructures() {
  _E = 0;
  _g = new list<int>[_V];
  _edges = new unordered_set<int>[_V];
  _netVert.resize(_numNets, 0);
  _initNetID.resize(_numNets, 0);
  _mapVertNet.resize(_V, 0);
  _vertexMap.resize(_V);
  for (int i = 0; i < _V; i++) _vertexMap[i] = i;
}

void vertexDistribution() {
  int n = 0;
  for (int i = 0; i < _numNets; i++) {
    _netVert[i] = _minV;
    n += _minV;
  }

  while (n < _V) {
    int scc = randomValue(_numNets);
    while (_netVert[scc] >= _maxV)
      scc = (scc + 1) % _numNets;
    _netVert[scc]++;
    n++;
  }

  n = 0;
  for (int i = 0; i < _numNets; i++) {
    _initNetID[i] = n;
    for (int j = 0; j < _netVert[i]; j++) {
      _mapVertNet[n] = i;
      n++;
    }
  }
}

void addInternalSCCEdges() {
  int m = 0, M = 0;
  for (int i = 0; i < _numNets; i++) {
    m = M;
    M += _netVert[i];

    // If it's a single vertex, skip
    if (_netVert[i] == 1) continue;

    int r = randomValue(3);
    if (r == 0) { // Add a cycle
      for (int j = m; j < M-1; j++)
        addEdge(j, j+1);
      if (_netVert[i] > 2) addEdge(M-1, m);
    } else if (r == 1) { // Add line
      for (int j = m; j < M-1; j++) addEdge(j, j+1);
    } else if (r == 2) { // Add tree
      int children = randomValue(3)+1;
      int parent = m;
      for (int j = m+1; j < M; j++) {
        addEdge(parent, j);
        children--;
        if (children == 0) {
          children = randomValue(3)+1;
          parent++;
        }
      }
    }
  }
}

void addRemainingEdges() {
  int r = _maxE - _E;
  int tries = 10*r; // factor of 10

  while (r > 0) {
    int net = randomValue(_numNets); // select a random network
    if (_netVert[net] == 1) // if it's a single vertex, skip
      continue;

    int u = randomValue(_netVert[net]) + _initNetID[net];
    int v = randomValue(_netVert[net]) + _initNetID[net];

    // Do not allow self-loops
    if (u == v) continue;

    if (_edges[u].find(v) == _edges[u].end()) { // edge does not exist
      addEdge(u, v);
      r--;
    } else {
      tries--;
      if (tries == 0) break;
    }
  }
}

int main(int argc, char *argv[]) {
  parseArgs(argc, argv);
  initDataStructures();

  vertexDistribution();
  addInternalSCCEdges();
  addRemainingEdges();

  // Shuffle Vertex IDs
  for (int i = 0; i < _V; i++) {
    int u = randomValue(_V);
    int v = randomValue(_V);
    int aux = _vertexMap[u];
    _vertexMap[u] = _vertexMap[v];
    _vertexMap[v] = aux;
  }

  // Print network
  printf("%d %d\n", _V, _E);
  for (int i = 0; i < _V; i++) {
    for (list<int>::iterator iter = _g[i].begin(); iter != _g[i].end(); iter++) {
      if (_vertexMap[i] < _vertexMap[(*iter)])
        printf("%d %d\n", _vertexMap[i]+1, _vertexMap[(*iter)]+1);
    }
  }

  return 0;
}