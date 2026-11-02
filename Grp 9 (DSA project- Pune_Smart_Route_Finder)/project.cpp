
/*NAME:
   Afroze Shaikh - 23281
   Shravani Sonawane - 23282
   Rohini Ahire - 23272 
   Shravani Deshmukh */
   

// SMART ROAD ROUTE FINDER - PUNE
// Dijkstra's Algorithm: shortest distance / fastest estimated time
// Note: This is a college demonstration with sample road weights,
// not a live navigation map. Add verified road data for real use.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <algorithm>
#include <cctype>
using namespace std;

const int INF = numeric_limits<int>::max();

struct RouteState {
    int cost;
    vector<int> path;
};

struct RouteCompare {
    bool operator()(const RouteState& a, const RouteState& b) const {
        return a.cost > b.cost; // lowest-cost route is processed first
    }
};

struct Edge {
    int to;
    int distance;   // sample distance in km
    int time;       // sample estimated time in minutes
    bool open;
};

class RoadFinder {
private:
    vector<string> places;
    vector<vector<Edge>> graph;

    string lower(string s) {
        transform(s.begin(), s.end(), s.begin(),
                  [](unsigned char c) { return tolower(c); });
        return s;
    }

    int findPlace(string name) {
        name = lower(name);
        for (int i = 0; i < (int)places.size(); i++)
            if (lower(places[i]) == name) return i;
        return -1;
    }

    void addPlace(string name) {
        places.push_back(name);
        graph.push_back({});
    }

    void addRoad(int a, int b, int km, int minutes) {
        graph[a].push_back({b, km, minutes, true});
        graph[b].push_back({a, km, minutes, true});
    }

    void showPath(int source, int dest, vector<int>& parent,
                  int total, bool byTime) {
        if (source != dest && parent[dest] == -1) {
            cout << "\nNo route found. Roads may be closed or disconnected.\n";
            return;
        }
        vector<int> path;
        for (int v = dest; v != -1; v = parent[v]) {
            path.push_back(v);
            if (v == source) break;
        }
        reverse(path.begin(), path.end());

        cout << "\n========== PUNE ROUTE RESULT ==========\n";
        for (int i = 0; i < (int)path.size(); i++) {
            cout << places[path[i]];
            if (i + 1 < (int)path.size()) cout << " -> ";
        }
        cout << "\nIntermediate stops: " << (path.size() > 1 ? path.size() - 2 : 0);
        if (byTime) cout << "\nEstimated travel time: " << total << " minutes\n";
        else cout << "\nTotal distance: " << total << " km\n";
        cout << "=======================================\n";
    }

    // Lists up to 5 different simple routes in increasing distance/time order.
    // The priority queue explores lower-cost partial routes first.
    void showAvailableRoutes(int source, int dest, bool byTime) {
        priority_queue<RouteState, vector<RouteState>, RouteCompare> pq;
        pq.push({0, vector<int>(1, source)});
        int shown = 0;
        const int MAX_ROUTES = 5;
        const int MAX_EXPANSIONS = 100000;
        int expansions = 0;

        cout << "\nAVAILABLE ROUTES (ordered by "
             << (byTime ? "estimated time" : "distance") << ")\n";

        while (!pq.empty() && shown < MAX_ROUTES && expansions < MAX_EXPANSIONS) {
            RouteState current = pq.top();
            pq.pop();
            int u = current.path.back();
            expansions++;

            if (u == dest) {
                cout << shown + 1 << ". ";
                for (int i = 0; i < (int)current.path.size(); i++) {
                    cout << places[current.path[i]];
                    if (i + 1 < (int)current.path.size()) cout << " -> ";
                }
                cout << " | ";
                if (byTime) cout << current.cost << " minutes\n";
                else cout << current.cost << " km\n";
                shown++;
                continue;
            }

            for (const Edge& e : graph[u]) {
                if (!e.open) continue;
                // Avoid repeating a location in the same route.
                if (find(current.path.begin(), current.path.end(), e.to) != current.path.end())
                    continue;
                RouteState next;
                next.cost = current.cost + (byTime ? e.time : e.distance);
                next.path = current.path;
                next.path.push_back(e.to);
                pq.push(next);
            }
        }

        if (shown == 0) cout << "No available route found.\n";
        else if (shown == 1) cout << "Only one route was found in this road network.\n";
        cout << "\nThe first route has the lowest total "
             << (byTime ? "estimated travel time" : "distance")
             << " among the routes displayed.\n";
    }

    void dijkstra(int source, int dest, bool byTime) {
        int n = places.size();
        vector<int> dist(n, INF), parent(n, -1);
        priority_queue<pair<int,int>, vector<pair<int,int>>,
                       greater<pair<int,int>>> pq;
        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int cost = pq.top().first;
            int u = pq.top().second;
            pq.pop();
            if (cost != dist[u]) continue;

            for (const Edge& e : graph[u]) {
                if (!e.open) continue; // ignore closed roads
                int w = byTime ? e.time : e.distance;
                if (dist[u] + w < dist[e.to]) {
                    dist[e.to] = dist[u] + w;
                    parent[e.to] = u;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        if (dist[dest] == INF) {
            cout << "\nNo route available between these locations.\n";
            return;
        }
        showPath(source, dest, parent, dist[dest], byTime);
    }

public:
    RoadFinder() {
        // Major areas/landmarks around Pune and Pimpri-Chinchwad.
        // Names are searchable. Distances/times below are illustrative.
        string names[] = {
            "Swargate", "Pune Railway Station", "Shivajinagar",
            "Deccan Gymkhana", "Kothrud", "Warje", "Sinhagad Road",
            "Katraj", "Bibwewadi", "Hadapsar", "Magarpatta",
            "Kharadi", "Viman Nagar", "Pune Airport", "Yerawada",
            "Koregaon Park", "Bund Garden", "Camp", "Kondhwa",
            "Wanowrie", "Fatima Nagar", "Aundh", "Baner",
            "Balewadi", "Hinjawadi", "Wakad", "Pimple Saudagar",
            "Pimpri", "Chinchwad", "Akurdi", "Nigdi",
            "Bhosari", "Moshi", "Dighi", "Khadki",
            "Pashan", "Bavdhan", "Warje Bridge", "University Circle",
            "Pune University", "FC Road", "JM Road", "Shaniwar Wada",
            "Deccan", "Karve Nagar", "Dhankawadi", "NIBM Road",
            "Undri", "Fursungi", "Loni Kalbhor", "Mundhwa",
            "Keshav Nagar", "Chandan Nagar", "Wagholi", "Lohegaon",
            "Dhanori", "Vishrantwadi", "Dapodi", "Kasarwadi",
            "Phugewadi", "Talegaon Dabhade", "Lonavala", "Chakan",
            "Rajgurunagar", "Ravet", "Punawale", "Tathawade",
            "Sus", "Mahalunge", "Pune-Satara Road", "Pune-Solapur Road",
            "Pune-Nashik Road", "Saswad", "Jejuri", "Alandi",
            "Dehu Road", "Pune Camp", "Aga Khan Palace",
            "Shaniwar Peth", "Budhwar Peth", "Mandai",
            "Parvati", "Saras Baug", "Pune Zoo (Katraj)",
            "Dagdusheth Ganpati", "Raja Dinkar Kelkar Museum",
            "Pataleshwar Cave Temple", "COEP Technological University",
            "Ruby Hall Clinic", "Sassoon Hospital", "Deenanath Mangeshkar Hospital",
            "Sahyadri Hospital Deccan", "Noble Hospital", "Inamdar Hospital",
            "Sancheti Hospital", "Jehangir Hospital", "Command Hospital",
            "D Y Patil Hospital Pimpri", "Aditya Birla Hospital",
            "Pimpri-Chinchwad", "PCMC Bhavan", "Balgandharva Rang Mandir",
            "Phoenix Marketcity", "Seasons Mall", "Amanora Mall",
            "Westend Mall", "Xion Mall", "Balewadi High Street",
            "Pune International Exhibition and Convention Centre",
            "Rajiv Gandhi Infotech Park Phase 1", "Rajiv Gandhi Infotech Park Phase 2"
        };

        int n = sizeof(names) / sizeof(names[0]);
        for (int i = 0; i < n; i++) addPlace(names[i]);

        // Bidirectional sample road connections between areas.
        // These are simplified links, not exact turn-by-turn roads.
        addRoad(0, 1, 4, 14); addRoad(0, 7, 8, 22);
        addRoad(0, 8, 5, 16); addRoad(0, 17, 3, 10);
        addRoad(0, 80, 2, 7); addRoad(0, 81, 2, 8);
        addRoad(0, 82, 2, 7); addRoad(0, 68, 4, 12);
        addRoad(0, 69, 6, 18); addRoad(0, 3, 5, 15);
        addRoad(1, 2, 3, 10); addRoad(1, 14, 5, 16);
        addRoad(1, 16, 2, 7); addRoad(1, 88, 1, 5);
        addRoad(1, 89, 2, 7); addRoad(1, 90, 2, 8);
        addRoad(2, 3, 3, 9); addRoad(2, 34, 5, 15);
        addRoad(2, 38, 4, 12); addRoad(2, 39, 3, 9);
        addRoad(2, 40, 2, 7); addRoad(2, 41, 2, 7);
        addRoad(2, 42, 2, 7); addRoad(2, 77, 2, 6);
        addRoad(2, 84, 2, 7); addRoad(2, 85, 2, 7);
        addRoad(2, 86, 2, 7); addRoad(2, 87, 2, 7);
        addRoad(3, 4, 6, 18); addRoad(3, 43, 1, 4);
        addRoad(3, 44, 5, 15); addRoad(3, 45, 3, 9);
        addRoad(4, 5, 5, 15); addRoad(4, 36, 4, 12);
        addRoad(4, 37, 4, 12); addRoad(4, 66, 7, 18);
        addRoad(4, 67, 5, 14); addRoad(5, 6, 6, 18);
        addRoad(5, 37, 3, 9); addRoad(6, 81, 6, 18);
        addRoad(6, 7, 8, 24); addRoad(7, 8, 4, 12);
        addRoad(7, 45, 4, 12); addRoad(7, 83, 2, 7);
        addRoad(8, 9, 9, 25); addRoad(8, 18, 5, 15);
        addRoad(9, 10, 4, 12); addRoad(9, 48, 7, 18);
        addRoad(9, 49, 8, 20); addRoad(9, 50, 5, 14);
        addRoad(9, 97, 2, 7); addRoad(9, 98, 2, 7);
        addRoad(9, 99, 2, 7); addRoad(10, 11, 7, 18);
        addRoad(10, 51, 5, 13); addRoad(10, 52, 4, 12);
        addRoad(11, 12, 5, 12); addRoad(11, 52, 4, 10);
        addRoad(11, 53, 7, 17); addRoad(11, 54, 6, 15);
        addRoad(12, 13, 4, 10); addRoad(12, 14, 5, 13);
        addRoad(12, 53, 4, 10); addRoad(12, 55, 5, 13);
        addRoad(13, 55, 6, 16); addRoad(14, 15, 4, 12);
        addRoad(14, 56, 4, 11); addRoad(14, 76, 3, 9);
        addRoad(15, 16, 3, 8); addRoad(15, 17, 4, 12);
        addRoad(15, 50, 5, 13); addRoad(16, 17, 3, 9);
        addRoad(17, 18, 7, 18); addRoad(17, 75, 1, 4);
        addRoad(18, 19, 4, 12); addRoad(18, 46, 5, 14);
        addRoad(19, 20, 2, 7); addRoad(20, 9, 6, 17);
        addRoad(21, 22, 4, 10); addRoad(21, 35, 5, 14);
        addRoad(21, 38, 6, 15); addRoad(21, 23, 5, 13);
        addRoad(22, 23, 3, 8); addRoad(22, 107, 4, 10);
        addRoad(23, 24, 3, 8); addRoad(23, 67, 4, 11);
        addRoad(23, 108, 3, 8); addRoad(24, 25, 4, 11);
        addRoad(24, 65, 3, 8); addRoad(25, 26, 5, 13);
        addRoad(25, 63, 5, 13); addRoad(26, 27, 4, 10);
        addRoad(26, 99, 3, 8); addRoad(27, 28, 3, 8);
        addRoad(27, 31, 5, 13); addRoad(27, 100, 3, 8);
        addRoad(28, 29, 3, 8); addRoad(28, 30, 4, 10);
        addRoad(29, 30, 3, 8); addRoad(29, 64, 6, 15);
        addRoad(30, 31, 5, 13); addRoad(30, 32, 5, 12);
        addRoad(31, 32, 5, 13); addRoad(31, 33, 4, 10);
        addRoad(32, 33, 4, 10); addRoad(32, 73, 6, 15);
        addRoad(33, 34, 5, 13); addRoad(33, 56, 3, 8);
        addRoad(34, 35, 4, 10); addRoad(34, 38, 4, 10);
        addRoad(35, 36, 4, 10); addRoad(36, 37, 4, 10);
        addRoad(37, 38, 3, 8); addRoad(38, 39, 2, 6);
        addRoad(39, 40, 3, 8); addRoad(40, 41, 2, 6);
        addRoad(41, 42, 2, 6); addRoad(42, 77, 2, 6);
        addRoad(43, 44, 2, 6); addRoad(44, 45, 4, 10);
        addRoad(45, 46, 4, 10); addRoad(46, 47, 5, 12);
        addRoad(47, 48, 6, 15); addRoad(48, 49, 5, 12);
        addRoad(49, 50, 5, 12); addRoad(50, 51, 4, 10);
        addRoad(51, 52, 3, 8); addRoad(52, 53, 4, 10);
        addRoad(53, 54, 4, 10); addRoad(54, 55, 4, 10);
        addRoad(55, 56, 3, 8); addRoad(56, 57, 4, 10);
        addRoad(57, 58, 3, 8); addRoad(58, 59, 3, 8);
        addRoad(59, 60, 8, 18); addRoad(60, 61, 12, 25);
        addRoad(61, 62, 18, 35); addRoad(62, 63, 10, 22);
        addRoad(63, 64, 6, 14); addRoad(64, 65, 4, 10);
        addRoad(65, 66, 3, 8); addRoad(66, 67, 4, 10);
        addRoad(67, 68, 5, 12); addRoad(68, 69, 4, 10);
        addRoad(69, 70, 5, 12); addRoad(70, 71, 6, 14);
        addRoad(71, 72, 14, 30); addRoad(72, 73, 12, 25);
        addRoad(73, 74, 8, 18); addRoad(74, 75, 10, 22);
        addRoad(76, 12, 5, 13); addRoad(78, 79, 1, 4);
        addRoad(79, 80, 1, 4); addRoad(80, 81, 2, 6);
        addRoad(82, 83, 2, 6); addRoad(84, 85, 1, 4);
        addRoad(85, 86, 1, 4); addRoad(86, 87, 1, 4);
        addRoad(88, 89, 2, 6); addRoad(89, 90, 2, 6);
        addRoad(90, 91, 3, 8); addRoad(91, 92, 3, 8);
        addRoad(92, 93, 3, 8); addRoad(93, 94, 3, 8);
        addRoad(94, 95, 4, 10); addRoad(95, 96, 4, 10);
        addRoad(96, 97, 4, 10); addRoad(98, 99, 4, 10);
        addRoad(100, 101, 2, 6); addRoad(101, 102, 3, 8);
        addRoad(102, 103, 3, 8); addRoad(103, 104, 4, 10);
        addRoad(104, 105, 3, 8); addRoad(105, 106, 3, 8);
        addRoad(106, 107, 3, 8); addRoad(107, 108, 4, 10);
        addRoad(108, 109, 4, 10); addRoad(109, 110, 4, 10);
    }

    void showPlaces() {
        cout << "\nAVAILABLE PUNE LOCATIONS (" << places.size() << ")\n";
        for (int i = 0; i < (int)places.size(); i++)
            cout << i + 1 << ". " << places[i] << "\n";
    }

    void searchPlace() {
        string key;
        cout << "Enter location name or part of name: ";
        getline(cin, key);
        bool found = false;
        for (string p : places) {
            if (lower(p).find(lower(key)) != string::npos) {
                cout << "- " << p << "\n";
                found = true;
            }
        }
        if (!found) cout << "No matching location found.\n";
    }

    void findRoute(bool byTime) {
        string start, destName;
        cout << "\nEnter starting location: ";
        getline(cin, start);
        cout << "Enter destination location: ";
        getline(cin, destName);
        int s = findPlace(start), d = findPlace(destName);
        if (s == -1 || d == -1) {
            cout << "Location not found. Choose a name from View All Locations.\n";
            return;
        }
        if (s == d) {
            cout << "Source and destination are the same.\n";
            return;
        }
        showAvailableRoutes(s, d, byTime);
        cout << "\nSHORTEST ROUTE USING DIJKSTRA'S ALGORITHM\n";
        dijkstra(s, d, byTime);
        cout << "\nHow the shortest route is found:\n";
        cout << "1. Start at the source with cost 0.\n";
        cout << "2. Repeatedly choose the unvisited location with the lowest known cost.\n";
        cout << "3. Add each open road's distance/time and update a route if its cost is lower.\n";
        cout << "4. Reconstruct the path using parent locations.\n";
    }

    void showRoads() {
        cout << "\nROAD CONNECTIONS (illustrative sample data)\n";
        for (int i = 0; i < (int)graph.size(); i++)
            for (const Edge& e : graph[i])
                if (i < e.to)
                    cout << places[i] << " <-> " << places[e.to]
                         << " | " << e.distance << " km | " << e.time
                         << " min | " << (e.open ? "OPEN" : "CLOSED") << "\n";
    }

    void changeRoadStatus() {
        string a, b;
        cout << "Enter first location: ";
        getline(cin, a);
        cout << "Enter second location: ";
        getline(cin, b);
        int u = findPlace(a), v = findPlace(b);
        if (u == -1 || v == -1) {
            cout << "Location not found.\n";
            return;
        }
        for (Edge& e : graph[u]) {
            if (e.to == v) {
                e.open = !e.open;
                bool status = e.open;
                for (Edge& back : graph[v])
                    if (back.to == u) back.open = status;
                cout << "Road " << (status ? "reopened" : "closed")
                     << ". Search again to calculate an alternative route.\n";
                return;
            }
        }
        cout << "These locations do not have a direct connection in this sample network.\n";
    }

    void run() {
        int choice;
        do {
            cout << "\n====================================\n";
            cout << "      SMART ROAD ROUTE FINDER - PUNE\n";
            cout << "====================================\n";
            cout << "1. Show available routes + shortest-distance route\n";
            cout << "2. Show available routes + fastest-time route\n";
            cout << "3. Search a location\n";
            cout << "4. View all locations\n";
            cout << "5. View road connections\n";
            cout << "6. Close / reopen a road (simulation)\n";
            cout << "7. Exit\n";
            cout << "Enter your choice: ";
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a number.\n";
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: findRoute(false); break;
                case 2: findRoute(true); break;
                case 3: searchPlace(); break;
                case 4: showPlaces(); break;
                case 5: showRoads(); break;
                case 6: changeRoadStatus(); break;
                case 7: cout << "Thank you for using Pune Road Route Finder!\n"; break;
                default: cout << "Invalid choice.\n";
            }
        } while (choice != 7);
    }
};

int main() {
    RoadFinder app;
    app.run();
    return 0;
}
