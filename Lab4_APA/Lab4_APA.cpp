#include <iostream>
#include <iomanip>
#include <stack>
#include <vector>
#include <iomanip>

#define INF INT_MAX

using namespace std;

class aMatrix {
public:
    int** adiacentMatrix = nullptr;
    int size;
    aMatrix() { size = 0; };
    aMatrix(int size) {
        this->size = size;
        adiacentMatrix = new int* [size];
        for (int i = 0; i < size; i++) {
            adiacentMatrix[i] = new int[size];
        }
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                adiacentMatrix[i][j] = INF;
            }
        }
    }
    void conexRar() {
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (abs(i - j) == 1 || abs(i - j) == size - 1) {
                    if (i < j) {
                        adiacentMatrix[i][j] = adiacentMatrix[j][i] = rand() % 100 + 1;
                    }
                }
            }
        }
    }
    void conexDens() {
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (i < j && abs(i - j) != size - 2) {
                    adiacentMatrix[i][j] = adiacentMatrix[j][i] = rand() % 100;
                }
            }
        }
    }
    void manualGraph() {
        int start;
        int end;
        int capacity;
        int edgeNr;
        start = end = -1;
        cout << "\nEnter nr. of edges: ";
        cin >> edgeNr;
        for (int i = 0; i < edgeNr; i++) {
            do {
                cout << "\nEnter start-end-weight separated by space(ex. 1 2 4): ";
                cin >> start >> end >> capacity;
            } while (start < 0 || end < 0);
            adiacentMatrix[start][end] = adiacentMatrix[end][start] = capacity;
        }
    }
    void display(int** m, int size) {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                std::cout << setw(4) << m[i][j];
            }
            std::cout << "\n";
        }
    }
};
class node {
public:
    int parent;
    int cost;
    bool areVisited;
    node() {
        parent = -1;
        cost = INF;
        areVisited = false;
    }
};

class graph :public aMatrix {
public:
    graph() {};
    graph(int size, const char type) {
        this->size = size;
        adiacentMatrix = new int* [size];
        for (int i = 0; i < size; i++) {
            adiacentMatrix[i] = new int[size] {};
        }
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                adiacentMatrix[i][j] = INF;
            }
        }
        if (type == 'r')
            conexRar();
        else if (type == 'd')
            conexDens();
        else
            manualGraph();
    }
};


class dijkstra {
public:
    node* n;
    graph* g;
    dijkstra(graph *g) {
        this->g = new graph;
        this->g = g;
        n = new node[g->size];
        
    }
    double minPath(int start);
    int findMinCost();
    void printPath(int start);
    void findPath(int temp, int start);
    double solve();
};

int dijkstra::findMinCost() {
    int minIndex = 0;
    int minCost = INF;
    for (int i = 0; i < g->size; i++) {
        if (n[i].cost < minCost && !n[i].areVisited) {
            minIndex = i;
            minCost = n[i].cost;
        }
    }
    return minIndex;
}

double dijkstra::minPath(int start) {
    clock_t begin, end;
    begin = clock();
    if (start < 0 || start >= g->size) {
        cout << "\nInexistent node!";
        return 0;
    }
    n[start].areVisited = true;
    n[start].cost = 0;
    node* temp = new node;

    for (int i = 0; i < g->size; i++) {
        n[i].cost = g->adiacentMatrix[start][i];
        if (n[i].cost != INF)
            n[i].parent = start;
    }
    int minIndex;
    for (int i = 1; i < g->size - 1; i++) {
        minIndex = findMinCost();
        n[minIndex].areVisited = true;
        for (int j = 0; j < g->size; j++) {
            if (!n[j].areVisited
                && g->adiacentMatrix[minIndex][j] != INF
                && n[minIndex].cost + g->adiacentMatrix[minIndex][j] < n[j].cost) {
                n[j].cost = n[minIndex].cost + g->adiacentMatrix[minIndex][j];
                n[j].parent = minIndex;
            }

        }
    }
    end = clock();
    return double(end - begin);
}

void dijkstra::findPath(int temp, int start) {
    if (temp == start) {
        cout << temp << " ";
        return;
    }
    findPath(n[temp].parent, start);
    cout << temp << " ";
}

void dijkstra::printPath(int start) {
    for (int i = 0; i < g->size; i++) {
        if (i != start) {
            cout << start << " -> " << i << ": ";
            findPath(i, start);
            cout<<setw(16) << "\t\t| cost: " << n[i].cost;
            cout << endl;
        }
    }
}

double dijkstra::solve() {
    clock_t begin, end;
    begin = clock();
    for (int i = 0; i < g->size; i++) {
       // cout << "\n\nStart: " << i<<endl;
        minPath(i);
       // printPath(i);
        delete[]n;
        n = new node[g->size];
    }
    end = clock();
    return double(end - begin);
}

class floyd {
public:
    int** adiacentMatrix;
    int** cost;
    int** next;
    int size;
    floyd(int **a,int size) {
        adiacentMatrix = a;
        cost = new int* [size];
        next = new int* [size];
        this->size = size;
        for (int i = 0; i < size; i++) {
            cost[i] = new int[size];
            next[i] = new int[size];
        }
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                cost[i][j] = adiacentMatrix[i][j];
                if (adiacentMatrix[i][j] != INF)
                    next[i][j] = i;
                else
                    next[i][j] = 0;
            }
        }
    }

    double minPath() {
        clock_t start, end;
        start = clock();
        for (int k = 0; k < size; k++) {
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    if (cost[i][j] > (cost[i][k] + cost[k][j])
                        && (cost[k][j] != INF
                        && cost[i][k] != INF)) {
                        cost[i][j] = cost[i][k] + cost[k][j];
                        next[i][j] = next[k][j];
                    }
                }
            }
        }
        end = clock();
        return double(end - start);
    }
    void printDistances() {

        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                if (cost[i][j] != INF && i!=j) {
                    cout << "\nMin. distance from " << i << " to " << j << " : " << cost[i][j];
                }
            }

            printf("\n");
        }
    }
    void printPath(int i, int j) {
        if (i == j)
            cout << i;
        else if (next[i][j] == 0)
            cout << "\nNonexistent path.";
        else {
            printPath(i, next[i][j]);
            cout<<" " << j;
        }
    }
};

int main()
{
    int size = 10;
    graph test={size,'d'};
    dijkstra test2(&test);
    cout << "\nDijkstra: " << test2.solve();
    floyd test1(test.adiacentMatrix,size);
    cout << "\nFloyd: " << test1.minPath();

}
