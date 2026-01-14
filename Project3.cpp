
#include <bits/stdc++.h>


using namespace std;

//FLIGHT STRUCT
struct Flight {
    int id;
    string src, dest;
    double distance, time, cost;

    Flight() {}
    Flight(int i, const string &s, const string &d, double dist, double t, double c)
        : id(i), src(s), dest(d), distance(dist), time(t), cost(c) {}
};

//GRAPH CLASS
class Graph {
public:
    unordered_map<string,int> cityIndex;
    vector<string> indexToCity;
    vector<vector<pair<int,int>>> adj; // (neighborCityIndex, flightId)
    vector<Flight> flights;

    int addCity(const string &city) {
        if(cityIndex.count(city)) return cityIndex[city];
        int idx = (int)indexToCity.size();
        indexToCity.push_back(city);
        cityIndex[city] = idx;
        adj.push_back(vector<pair<int,int>>());
        return idx;
    }

    void addFlight(const Flight &f) {
        int fid = (int)flights.size();
        flights.push_back(f);
        int u = addCity(f.src);
        int v = addCity(f.dest);
        adj[u].push_back(make_pair(v, fid));
    }
};

//FLIGHT LOOKUP
class FlightLookup {
    unordered_map<string, vector<int>> lookup;
    Graph &G;

public:
    FlightLookup(Graph &g) : G(g) {}

    static string key(const string &s, const string &d) { return s + "->" + d; }

    void indexFlights() {
        lookup.clear();
        for(int i=0;i<(int)G.flights.size();i++) {
            Flight &f = G.flights[i];
            lookup[key(f.src,f.dest)].push_back(i);
        }
    }

    vector<Flight> getFlights(const string &s, const string &d) {
        vector<Flight> res;
        string k = key(s,d);
        if(!lookup.count(k)) return res;
        for(int id : lookup[k]) res.push_back(G.flights[id]);
        return res;
    }

    Flight* bestDeal(const string &s, const string &d) {
        vector<Flight> fl = getFlights(s,d);
        if(fl.empty()) return NULL;

        struct cmp { bool operator()(const Flight&a,const Flight&b){return a.cost>b.cost;} };
        priority_queue<Flight, vector<Flight>, cmp> pq;

        for(const auto &f : fl) pq.push(f);
        return new Flight(pq.top());
    }
};

//BOOKING SYSTEM
struct Booking {
    int id;
    int flightId;
    string name, src, dest;
};

class BookingManager {
    vector<Booking> list;
    stack<Booking> st;
    int nextId = 1;

public:
    // Load existing bookings 
    void loadFromList(const vector<Booking> &loaded) {
        list = loaded;
        int mx = 0;
        for(const auto &b: list) if(b.id>mx) mx=b.id;
        nextId = mx+1;
        while(!st.empty()) st.pop();
        for(const auto &b : list) st.push(b);
    }

    vector<Booking> exportList() const { return list; }

    int book(int flightId, const string &name, const string &src, const string &dest) {
        Booking b;
        b.id = nextId++;
        b.flightId = flightId;
        b.name = name;
        b.src = src;
        b.dest = dest;

        list.push_back(b);
        st.push(b);

        cout << "[Booked] ID " << b.id << " for " << b.name << "\n";
        return b.id;
    }

    void undo() {
        if(st.empty()) { cout << "Nothing to undo\n"; return; }

        Booking b = st.top(); st.pop();
        for(size_t i=0;i<list.size();i++){
            if(list[i].id == b.id) {
                list.erase(list.begin()+i);
                break;
            }
        }
        cout << "[Undo] Removed booking " << b.id << "\n";
    }

    void show() const {
        cout << "Current Bookings:\n";
        for(size_t i=0;i<list.size();i++)
            cout << "  ID "<< list[i].id << " | " << list[i].name
                 << " | " << list[i].src << " -> " << list[i].dest << "\n";
    }
};

//CHECK-IN QUEUE 
struct Passenger { string name, route; };
class CheckQueue {
    queue<Passenger> q;
public:
    void arrive(Passenger p) {
        q.push(p);
        cout << p.name << " arrived for check-in.\n";
    }
    void process() {
        if(q.empty()) { cout << "No passengers.\n"; return; }
        Passenger p = q.front(); q.pop();
        cout << "Processed " << p.name << "\n";
    }
};

//HOTEL BST
struct Hotel { string name; double rating; };

struct HotelNode {
    Hotel h;
    HotelNode *left,*right;
    HotelNode(Hotel H) : h(H), left(NULL), right(NULL) {}
};

class HotelBST {
    HotelNode *root=NULL;

    HotelNode* insertNode(HotelNode* node, Hotel h) {
        if(!node) return new HotelNode(h);

        if(h.rating > node->h.rating) node->left = insertNode(node->left,h);
        else if(h.rating < node->h.rating) node->right = insertNode(node->right,h);
        else {
            if(h.name < node->h.name) node->left = insertNode(node->left,h);
            else node->right = insertNode(node->right,h);
        }
        return node;
    }

    void inorder(HotelNode* node, vector<Hotel> &out) {
        if(!node) return;
        inorder(node->right,out);
        out.push_back(node->h);
        inorder(node->left,out);
    }

    void freeNodes(HotelNode* node) {
        if(!node) return;
        freeNodes(node->left);
        freeNodes(node->right);
        delete node;
    }

public:
    ~HotelBST() { freeNodes(root); }

    void addHotel(const string &name, double rating) {
        root = insertNode(root, Hotel{name,rating});
    }

    vector<Hotel> exportAll() {
        vector<Hotel> all;
        inorder(root, all);
       reverse(all.begin(), all.end()); // highest rating first
        return all;
    }

    void topK(int k) {
        vector<Hotel> all = exportAll();
        cout << "Top Hotels:\n";
        for(int i=0;i<min(k,(int)all.size()); i++)
            cout << "  " << all[i].name << " (" << all[i].rating << ")\n";
    }
};

//FILE I/O 

// Trim helpers
static inline string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if(a==string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b-a+1);
}

vector<Flight> loadFlightsFromFile(const string &filename) {
    vector<Flight> out;
    ifstream fin(filename);
    if(!fin) return out; // file missing -> empty

    string line;
    Flight cur;
    bool inBlock = false;
    while(getline(fin,line)){
        line = trim(line);
        if(line.empty()) continue;
        if(line == "[FLIGHT]"){
            inBlock = true;
            cur = Flight();
            continue;
        }
        if(inBlock) {
            auto pos = line.find('=');
            if(pos==string::npos) continue;
            string key = trim(line.substr(0,pos));
            string val = trim(line.substr(pos+1));
            if(key=="id") cur.id = stoi(val);
            else if(key=="src") cur.src = val;
            else if(key=="dest") cur.dest = val;
            else if(key=="distance") cur.distance = stod(val);
            else if(key=="time") cur.time = stod(val);
            else if(key=="cost") cur.cost = stod(val);   
        }
    }
    // The above simple parser didn't push flights 
    fin.clear();
    fin.seekg(0);

    inBlock = false;
    cur = Flight();
    while(getline(fin,line)){
        line = trim(line);
        if(line.empty()) continue;
        if(line == "[FLIGHT]"){
            if(inBlock) out.push_back(cur);
            inBlock = true;
            cur = Flight();
            continue;
        }
        if(inBlock){
            auto pos = line.find('=');
            if(pos==string::npos) continue;
            string key = trim(line.substr(0,pos));
            string val = trim(line.substr(pos+1));
            if(key=="id") cur.id = stoi(val);
            else if(key=="src") cur.src = val;
            else if(key=="dest") cur.dest = val;
            else if(key=="distance") cur.distance = stod(val);
            else if(key=="time") cur.time = stod(val);
            else if(key=="cost") cur.cost = stod(val);
        }
    }
    if(inBlock) out.push_back(cur);

    fin.close();
    return out;
}

void saveFlightsToFile(const string &filename, const vector<Flight> &flights) {
    ofstream fout(filename);
    if(!fout) { cerr << "Failed to write flights file\n"; return; }
    for(const auto &f: flights){
        fout << "[FLIGHT]\n";
        fout << "id=" << f.id << "\n";
        fout << "src=" << f.src << "\n";
        fout << "dest=" << f.dest << "\n";
        fout << "distance=" << f.distance << "\n";
        fout << "time=" << f.time << "\n";
        fout << "cost=" << f.cost << "\n";
        fout << "\n";
    }
    fout.close();
}

vector<Booking> loadBookingsFromFile(const string &filename) {
    vector<Booking> out;
    ifstream fin(filename);
    if(!fin) return out;
    string line;
    Booking cur;
    bool inBlock = false;
    
    while(getline(fin,line)){
        line = trim(line);
        if(line.empty()) continue;
        if(line == "[BOOKING]"){
            if(inBlock) out.push_back(cur);
            inBlock = true;
            cur = Booking();
            continue;
        }
        if(inBlock){
            auto pos = line.find('=');
            if(pos==string::npos) continue;
            string key = trim(line.substr(0,pos));
            string val = trim(line.substr(pos+1));
            if(key=="id") cur.id = stoi(val);
            else if(key=="flightId") cur.flightId = stoi(val);
            else if(key=="name") cur.name = val;
            else if(key=="src") cur.src = val;
            else if(key=="dest") cur.dest = val;
        }
    }
    if(inBlock) out.push_back(cur);
    fin.close();
    return out;
}

void saveBookingsToFile(const string &filename, const vector<Booking> &bookings) {
    ofstream fout(filename);
    if(!fout) { cerr << "Failed to write bookings file\n"; return; }
    for(const auto &b: bookings){
        fout << "[BOOKING]\n";
        fout << "id=" << b.id << "\n";
        fout << "flightId=" << b.flightId << "\n";
        fout << "name=" << b.name << "\n";
        fout << "src=" << b.src << "\n";
        fout << "dest=" << b.dest << "\n";
        fout << "\n";
    }
    fout.close();
}

vector<Hotel> loadHotelsFromFile(const string &filename) {
    vector<Hotel> out;
    ifstream fin(filename);
    if(!fin) return out;
    string line;
    Hotel cur;
    bool inBlock = false;
    while(getline(fin,line)){
        line = trim(line);
        if(line.empty()) continue;
        if(line=="[HOTEL]"){
            if(inBlock) out.push_back(cur);
            inBlock = true;
            cur = Hotel();
            continue;
        }
        if(inBlock){
            auto pos = line.find('=');
            if(pos==string::npos) continue;
            string key = trim(line.substr(0,pos));
            string val = trim(line.substr(pos+1));
            if(key=="name") cur.name = val;
            else if(key=="rating") cur.rating = stod(val);
        }
    }
    if(inBlock) out.push_back(cur);
    fin.close();
    return out;
}

void saveHotelsToFile(const string &filename, const vector<Hotel> &hotels) {
    ofstream fout(filename);
    if(!fout) { cerr << "Failed to write hotels file\n"; return; }
    for(const auto &h: hotels){
        fout << "[HOTEL]\n";
        fout << "name=" << h.name << "\n";
        fout << "rating=" << h.rating << "\n";
        fout << "\n";
    }
    fout.close();
}

//MAIN MENU 
int main() {
    Graph G;
    FlightLookup lookup(G);
    BookingManager bm;
    CheckQueue cq;
    HotelBST hotels;

    const string flightsFile = "flights.txt";
    const string bookingsFile = "bookings.txt";
    const string hotelsFile = "hotels.txt";

    vector<Flight> loadedFlights = loadFlightsFromFile(flightsFile);
    for(const auto &f: loadedFlights) G.addFlight(f);
    lookup.indexFlights();

    vector<Hotel> loadedHotels = loadHotelsFromFile(hotelsFile);
    for(const auto &h: loadedHotels) hotels.addHotel(h.name, h.rating);

    vector<Booking> loadedBookings = loadBookingsFromFile(bookingsFile);
    bm.loadFromList(loadedBookings);

    cout << "Smart Travel Planner (with file persistence)\n";
    while(true) {
        cout << "\n========= TRAVEL PLANNER MENU =========\n";
        cout << "1. Add Flight\n";
        cout << "2. Show Direct Flights\n";
        cout << "3. Best Deal (Cheapest Direct Flight)\n";
        cout << "4. Book Flight\n";
        cout << "5. Undo Last Booking\n";
        cout << "6. Show Bookings\n";
        cout << "7. Passenger Check-In\n";
        cout << "8. Process Check-In\n";
        cout << "9. Add Hotel\n";
        cout << "10. Show Top Hotels\n";
        cout << "0. Exit\n";
        cout << "Choose: ";

        int ch;
        if(!(cin >> ch)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        cin.ignore();

        if(ch == 0) break;

        if(ch == 1) {
            string s, d;
            double dist=0, t=0, c=0;

            cout << "Source City: ";
            getline(cin, s);

            cout << "Destination City: ";
            getline(cin, d);

            cout << "Distance (km): ";
            cin >> dist;

            cout << "Time (hrs): ";
            cin >> t;

            cout << "Cost: ";
            cin >> c; cin.ignore();

            int id = (int)G.flights.size();
            Flight newf(id, s, d, dist, t, c);
            G.addFlight(newf);
            lookup.indexFlights();

            // persist
            saveFlightsToFile(flightsFile, G.flights);

            cout << "Flight Added Successfully.\n";
        }

        else if(ch == 2) {
            string s,d;
            cout << "Source: ";
            getline(cin, s);
            cout << "Destination: ";
            getline(cin, d);

            lookup.indexFlights();
            auto fl = lookup.getFlights(s,d);

            if(fl.empty()) cout << "No direct flights.\n";
            else {
                cout << "Direct Flights:\n";
                for(auto &f: fl)
                    cout << "  Flight ID:" << f.id << " | Cost: " << f.cost << " | " << f.src << "->" << f.dest << "\n";
            }
        }

        else if(ch == 3) {
            string s,d;
            cout << "Source: "; getline(cin,s);
            cout << "Destination: "; getline(cin,d);

            lookup.indexFlights();
            Flight* f = lookup.bestDeal(s,d);
            if(f == NULL) cout << "No flights found.\n";
            else {
                cout << "Best Deal Cost = " << f->cost << " | Flight ID: " << f->id << "\n";
                delete f; // we allocated in bestDeal
            }
        }

        else if(ch == 4) {
            int id; string name,src,dest;

            cout << "Flight ID: ";
            cin >> id; cin.ignore();

            cout << "Passenger Name: ";
            getline(cin, name);

            cout << "Source: ";
            getline(cin, src);

            cout << "Destination: ";
            getline(cin, dest);

            int newId = bm.book(id,name,src,dest);

            saveBookingsToFile(bookingsFile, bm.exportList());
        }

        else if(ch == 5) {
            bm.undo();
            saveBookingsToFile(bookingsFile, bm.exportList());
        }
        else if(ch == 6) bm.show();

        else if(ch == 7) {
            string name, route;
            cout << "Passenger Name: ";
            getline(cin, name);
            cout << "Flight Route (e.g., Delhi->Mumbai): ";
            getline(cin, route);
            cq.arrive(Passenger{name,route});
        }

        else if(ch == 8) cq.process();

        else if(ch == 9) {
            string name; double rating;
            cout << "Hotel Name: ";
            getline(cin, name);
            cout << "Rating: ";
            cin >> rating; cin.ignore();
            hotels.addHotel(name,rating);
            // persist hotels
            saveHotelsToFile(hotelsFile, hotels.exportAll());
        }

        else if(ch == 10) hotels.topK(5);

        else cout << "Invalid option.\n";
    }
    saveFlightsToFile(flightsFile, G.flights);
    saveBookingsToFile(bookingsFile, bm.exportList());
    saveHotelsToFile(hotelsFile, hotels.exportAll());

    cout << "Exiting. Data saved to files (flights.txt, bookings.txt, hotels.txt).\n";
    return 0;
}
