#include <iostream>
#include <vector>
#include <queue>
#include <utility>  // para usar pair

using namespace std;

typedef pair<int, int> Edge;  // Definimos Edge como un par (peso, nodo)

int prims(int V, vector<vector<Edge>>& graph, vector<pair<int, Edge>>& mst_edges) {
    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;
    vector<bool> int_MST(V, false);
    vector<int> parent(V, -1);  // Para guardar el nodo anterior en la ruta
    int total_weight = 0;

    pq.push(Edge(0, 0));  // Empezamos desde el nodo 0

    // Debug: Verificar estado inicial
    cout << "Iniciando Prim's Algorithm\n";

    while (!pq.empty()) {
        int weight = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        // Debug: Imprimir nodo procesado
        cout << "Procesando nodo: " << node << " con peso: " << weight << endl;

        if (int_MST[node]) continue;
        int_MST[node] = true;
        total_weight += weight;

        // Guardamos la arista que lleva al nodo actual desde el padre
        if (parent[node] != -1) {  // Evitamos la arista inicial con peso 0
            mst_edges.push_back({parent[node], Edge(weight, node)});
        }

        // Añadimos los nodos adyacentes al MST
        for (auto& edge : graph[node]) {
            int adjnode = edge.second;
            int adjweight = edge.first;

            if (!int_MST[adjnode]) {
                pq.push(Edge(adjweight, adjnode));
                parent[adjnode] = node;  // Guardamos de dónde viene el nodo
                // Debug: Imprimir nodo añadido a la cola de prioridad
                cout << "Añadiendo nodo adyacente: " << adjnode << " con peso: " << adjweight << endl;
            }
        }
    }

    return total_weight;
}

int sp(int n, vector<vector<int>>& dist) {
    int INF = 1e9;
    vector<vector<int>> dp(1 << n, vector<int>(n, INF));

    // Base case, camino directo desde el nodo 0 hasta otros
    for (int i = 1; i < n; i++) {
        dp[1 << i][i] = dist[0][i];
    }

    // Iterar sobre todos los subconjuntos del conjunto de nodos
    for (int m = 0; m < (1 << n); m++) {
        for (int i = 0; i < n; i++) {
            if ((m & (1 << i)) == 0) continue;  // Nodo no está en el subconjunto

            for (int j = 0; j < n; j++) {
                if ((m & (1 << j)) == 0) {
                    dp[m | (1 << j)][j] = min(dp[m | (1 << j)][j], dp[m][i] + dist[i][j]);
                }
            }
        }
    }

    // Encontrar el mínimo costo de regresar al nodo 0
    int result = INF;
    for (int i = 1; i < n; i++) {
        result = min(result, dp[(1 << n) - 1][i] + dist[i][0]);
    }

    return result;
}

// Convertir el grafo de representación de aristas a matriz de adyacencia
vector<vector<int>> convertToAdjMatrix(int n, vector<vector<Edge>>& graph) {
    vector<vector<int>> dist(n, vector<int>(n, 1e9));  // Inicializamos con valores grandes (infinito)

    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;  // Distancia del nodo a sí mismo es 0
        for (const auto& edge : graph[i]) {
            int adjnode = edge.second;
            int adjweight = edge.first;
            dist[i][adjnode] = adjweight;  // Añadimos la arista a la matriz de adyacencia
        }
    }

    return dist;
}

int main() {
    int n;
    cout << "Introduce el número de nodos: ";
    cin >> n;

    if (n <= 0) {
        cout << "Error: el número de nodos debe ser mayor que 0." << endl;
        return 1;
    }

    vector<vector<Edge>> graph(n);

    // Leer la matriz de adyacencia
    cout << "Introduce la matriz de adyacencia (0 si no hay conexión):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int weight;
            if (!(cin >> weight)) {
                cout << "Error leyendo la matriz de adyacencia. Verifica la entrada." << endl;
                return 1;
            }
            if (weight != 0) {
                graph[i].push_back(Edge(weight, j));
            }
        }
    }

    // Vector para almacenar las aristas del MST
    vector<pair<int, Edge>> mst_edges;

    int result = prims(n, graph, mst_edges);

    // Imprimir la ruta del árbol de expansión mínimo (MST)
    cout << "\nRuta a seguir en el árbol de expansión mínimo (MST):\n";
    for (const auto& edge : mst_edges) {
        int from_node = edge.first;
        int to_node = edge.second.second;
        int weight = edge.second.first;
        cout << "Neighborhood " << from_node << " ---( " << weight << " )---> Neighborhood " << to_node << endl;
    }
    cout << "\nLa distancia total de la fibra óptica es: " << result << " km\n";

    // Resolver el problema del viajante (TSP) con programación dinámica
    vector<vector<int>> dist = convertToAdjMatrix(n, graph);
    int min_cost = sp(n, dist);
    cout << "El costo mínimo para visitar todos los vecindarios y volver al origen es: " << min_cost << " km\n";

    return 0;
}
