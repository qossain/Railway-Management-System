#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <conio.h>
#include <unistd.h>
#include <unordered_map>
#include <ctime>
#include <chrono>
using namespace std;


// Constants for file names
const string USERS_FILE = "users.txt";
const string SECURITY_LOGS_FILE = "security_logs.txt";
const string PASSENGER_FILE = "passenger_info.txt";
const string MAINTENANCE_FILE = "maintenance_schedule.txt";

// Helper function for current timestamp
string getCurrentTimestamp() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string timestamp(dt);
    timestamp.erase(remove(timestamp.begin(), timestamp.end(), '\n'), timestamp.end());
    return timestamp;
}

// Helper function for hashing passwords
string hashPassword(const string& password) {
    size_t hash = std::hash<string>{}(password);
    stringstream ss;
    ss << hash;
    return ss.str();
}

// Login System Class
class LoginSystem {
private:
    unordered_map<string, pair<string, string>> users; // username -> (hashed password, role)

    void loadUsers() {
        ifstream file(USERS_FILE);
        if (file.is_open()) {
            string username, password, role;
            while (file >> username >> password >> role) {
                users[username] = make_pair(password, role);
            }
            file.close();
        } else {
            cerr << "Error: Unable to open users file.\n";
        }
    }

    void saveUser(const string& username, const string& hashedPassword, const string& role) {
        ofstream file(USERS_FILE, ios::app);
        if (file.is_open()) {
            file << username << " " << hashedPassword << " " << role << endl;
            file.close();
        } else {
            cerr << "Error: Unable to write to users file.\n";
        }
    }

public:
    LoginSystem() { loadUsers(); }

    void registerUser() {
        string username, password, role;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
        cout << "Enter Role (Admin/User): ";
        cin >> role;

        if (role != "Admin" && role != "User") {
            cout << "Invalid role. Please enter 'Admin' or 'User'.\n";
            return;
        }

        if (users.find(username) == users.end()) {
            string hashedPassword = hashPassword(password);
            users[username] = make_pair(hashedPassword, role);
            saveUser(username, hashedPassword, role);
            cout << "User registered successfully!\n";
        } else {
            cout << "Username already exists.\n";
        }
    }

    string login() {
        string username, password;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
        string hashedPassword = hashPassword(password);

        if (users.find(username) != users.end() && users[username].first == hashedPassword) {
            cout << "Login successful! Welcome, " << users[username].second << "!\n";
            return users[username].second;
        } else {
            cout << "Invalid credentials.\n";
            return "";
        }
    }
};

// Route Management Class
class RouteManagement {
private:
    vector<string> routes;

public:
    void addRoute() {
        string route;
        cout << "Enter route name: ";
        cin.ignore();
        getline(cin, route);

        if (route.empty()) {
            cout << "Route name cannot be empty.\n";
            return;
        }

        routes.push_back(route);
        cout << "Route added successfully!\n";
    }

    void viewRoutes() {
        if (routes.empty()) {
            cout << "No routes available.\n";
            return;
        }

        cout << "Available Routes:\n";
        for (int i = 0; i < routes.size(); ++i) {
            cout << i + 1 << ". " << routes[i] << "\n";
        }
    }

    void deleteRoute() {
        if (routes.empty()) {
            cout << "No routes available to delete.\n";
            return;
        }

        int index;
        viewRoutes();
        cout << "Enter the route number to delete: ";
        cin >> index;

        if (index > 0 && index <= routes.size()) {
            routes.erase(routes.begin() + index - 1);
            cout << "Route deleted successfully!\n";
        } else {
            cout << "Invalid route number.\n";
        }
    }
};

// Passenger Information Class
class PassengerInfo {
public:
    void addPassenger() {
        string name, id;
        cout << "Enter Passenger Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Passenger ID: ";
        cin >> id;

        ofstream file(PASSENGER_FILE, ios::app);
        if (file.is_open()) {
            file << name << " " << id << endl;
            cout << "Passenger added successfully!\n";
            file.close();
        } else {
            cerr << "Error: Unable to write to passenger file.\n";
        }
    }

    void viewPassengers() {
        ifstream file(PASSENGER_FILE);
        if (!file.is_open()) {
            cerr << "Error: Unable to open passenger file.\n";
            return;
        }

        string name, id;
        cout << "Passenger Information:\n";
        while (file >> name >> id) {
            cout << "Name: " << name << ", ID: " << id << "\n";
        }

        file.close();
    }
};

// Maintenance Schedule Class
class MaintenanceSchedule {
public:
    void addSchedule() {
        string schedule;
        cout << "Enter Maintenance Schedule: ";
        cin.ignore();
        getline(cin, schedule);

        ofstream file(MAINTENANCE_FILE, ios::app);
        if (file.is_open()) {
            file << getCurrentTimestamp() << " - " << schedule << endl;
            cout << "Maintenance schedule added successfully!\n";
            file.close();
        } else {
            cerr << "Error: Unable to write to maintenance file.\n";
        }
    }

    void viewSchedules() {
        ifstream file(MAINTENANCE_FILE);
        if (!file.is_open()) {
            cerr << "Error: Unable to open maintenance file.\n";
            return;
        }

        string line;
        cout << "Maintenance Schedules:\n";
        while (getline(file, line)) {
            cout << line << "\n";
        }

        file.close();
    }
};

// Security and Surveillance Class
class SecuritySurveillance {
private:
    vector<string> logs;

    // Function to get the current timestamp
    string getCurrentTimestamp() {
        auto now = chrono::system_clock::now();
        time_t timeNow = chrono::system_clock::to_time_t(now);
        string timestamp = ctime(&timeNow);
        timestamp.pop_back();  // Remove the newline character
        return timestamp;
    }

public:
    // Log the action with timestamp
    void logAction(const string& action) {
        string entry = getCurrentTimestamp() + " - " + action;
        logs.push_back(entry);
        ofstream file(SECURITY_LOGS_FILE, ios::app);
        if (file.is_open()) {
            file << entry << endl;
            file.close();
        } else {
            cerr << "Error: Unable to write to security logs file.\n";
        }
    }

    // View the logged actions
    void viewLogs() {
        ifstream file(SECURITY_LOGS_FILE);
        if (!file.is_open()) {
            cerr << "Error: Unable to open security logs file.\n";
            return;
        }

        string line;
        cout << "Security Logs:\n";
        while (getline(file, line)) {
            cout << line << "\n";
        }

        file.close();
    }
};
// Structs for handling data
struct Crew {
    string id;
    string name;
    string address;
    string CNIC;
    string dutyTiming;
    string shift;
    string trainAssigned;
    bool present;
};

struct Train {
    string trainName;
    string timing;
    int firstClassCapacity;
    int acClassCapacity;
    int economyClassCapacity;
    int lowEconomyClassCapacity;
    string luggageCapacity;
};

struct Complaint {
    string name;
    int rating;
    string description;
};

// Functions for file handling
void storeInFile(const string& fileName, const string& data) {
    ofstream file(fileName, ios::app);
    if (file.is_open()) {
        file << data << endl;
        file.close();
    } else {
        cerr << "Error opening file!" << endl;
    }
}

vector<Train> readTrainsFromFile(const string& fileName) {
    vector<Train> trains;
    ifstream file(fileName);
    string line;

    while (getline(file, line)) {
        Train train;
        stringstream ss(line);

        // Use ',' as the delimiter
        getline(ss, train.trainName, ',');
        getline(ss, train.timing, ',');
        ss >> train.firstClassCapacity;
        ss.ignore();
        ss >> train.acClassCapacity;
        ss.ignore();
        ss >> train.economyClassCapacity;
        ss.ignore();
        ss >> train.lowEconomyClassCapacity;
        ss.ignore();
        ss >> train.luggageCapacity;

        // Trim leading and trailing spaces from train name and timing
        train.trainName = train.trainName.substr(0, train.trainName.find_last_not_of(" \t\n\r\f\v") + 1);
        train.timing = train.timing.substr(0, train.timing.find_last_not_of(" \t\n\r\f\v") + 1);

        trains.push_back(train);
    }

    file.close();
    return trains;
}


void writeTrainsToFile(const string& fileName, const vector<Train>& trains) {
    ofstream file(fileName, ios::trunc);
    for (const auto& train : trains) {
        file << train.trainName << ", " << train.timing << ", "
             << train.firstClassCapacity << ", " << train.acClassCapacity << ", "
             << train.economyClassCapacity << ", " << train.lowEconomyClassCapacity << ", "
             << train.luggageCapacity << endl;
    }
    file.close();
}

void updateTrainCapacity(const string& trainName, int classType) {
    vector<Train> trains = readTrainsFromFile("train_capacity.txt");
    bool updated = false;

    for (auto& train : trains) {
        if (train.trainName == trainName) {
            if (classType == 1 && train.firstClassCapacity > 0)
                train.firstClassCapacity--;
            else if (classType == 2 && train.acClassCapacity > 0)
                train.acClassCapacity--;
            else if (classType == 3 && train.economyClassCapacity > 0)
                train.economyClassCapacity--;
            else if (classType == 4 && train.lowEconomyClassCapacity > 0)
                train.lowEconomyClassCapacity--;
            else
                cout << "No seats available in selected class!" << endl;

            updated = true;
        }
    }

    writeTrainsToFile("train_capacity.txt", trains);

    if (updated)
        cout << "Train capacity updated successfully!" << endl;
    else
        cout << "Train not found or invalid class!" << endl;
}
// Train Checking Module
void trainChecking() {
    vector<Train> trains = readTrainsFromFile("train_capacity.txt");

    if (trains.empty()) {
        cout << "No trains found in the system." << endl;
        return;
    }

    cout << "Available Trains:\n";
    for (const auto& train : trains) {
        cout << "Train: " << train.trainName << ", Timing: " << train.timing << ", First Class: " 
             << train.firstClassCapacity << ", AC Class: " << train.acClassCapacity << ", Economy: " 
             << train.economyClassCapacity << ", Low Economy: " << train.lowEconomyClassCapacity 
             << ", Luggage: " << train.luggageCapacity << endl;
    }
}

// Payments Module
string getHiddenInput() {
    string input;
    char ch;
    while ((ch = _getch()) != '\r') {  // '\r' is the Enter key
        if (ch == '\b') {  // Backspace
            if (!input.empty()) {
                cout << "\b \b";  // Erase the last character on the console
                input.pop_back(); // Remove the last character from the string
            }
        } else {
            input.push_back(ch);
            cout << '*';  // Display '*' for each entered character
        }
    }
    cout << endl;
    return input;
}
// Function to trim whitespace from a string
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    size_t end = str.find_last_not_of(" \t\n\r\f\v");

    if (start == string::npos || end == string::npos)
        return "";  // Return an empty string if no non-whitespace characters found

    return str.substr(start, end - start + 1);
}

// Payments Module with password prompt
void handlePayment() {
    string trainName, timing, cardNumber;
    int classType;
    double price;
    string name;
    int option;

    vector<Train> trains = readTrainsFromFile("train_capacity.txt");

    cout << "Available Trains:\n";
    for (const auto& train : trains) {
        cout << "Train: " << train.trainName << ", Timing: " << train.timing << endl;
    }

    cout << "Enter the train name: ";
    cin.ignore(); // Clear buffer
    getline(cin, trainName);

    // Check if the train exists
    auto it = find_if(trains.begin(), trains.end(), [&](const Train& train) {
        return trim(train.trainName) == trim(trainName);
    });

    if (it == trains.end()) {
        cout << "No train found with the name \"" << trainName << "\". Payment process aborted.\n";
        return;
    }

    cout << "Enter train timing: ";
    getline(cin, timing);

    if (trim(it->timing) != trim(timing)) {
        cout << "No train found with the specified timing. Payment process aborted.\n";
        return;
    }

    cout << "Select Class (1. First, 2. AC, 3. Economy, 4. Low Economy): ";
    cin >> classType;

    price = 500.0 + (classType - 1) * 100.0;
    cout << "The ticket price is: Rs " << price << endl;

    cout << "Choose payment method:\n1. Cash\n2. Online\nOption: ";
    cin >> option;

    if (option == 1) {
        cout << "Proceed to the counter with your receipt." << endl;
        storeInFile("payments.txt", "Train: " + trainName + ", Timing: " + timing + ", Class: " + to_string(classType) + ", Payment: Cash");
        updateTrainCapacity(trainName, classType);
    } else if (option == 2) {
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter your card number: ";
        getline(cin, cardNumber); // Store card number

        cout << "Enter your card password: ";
        getHiddenInput(); // Get hidden input for the password but do not store it

        cout << "Payment of Rs " << price << " successful." << endl;
        storeInFile("payments.txt", "Train: " + trainName + ", Timing: " + timing + ", Name: " + name + ", Card Number: " + cardNumber + ", Class: " + to_string(classType) + ", Payment: Online");
        updateTrainCapacity(trainName, classType);
    } else {
        cout << "Invalid option." << endl;
    }
}


// Complaints Module
void handleComplaint() {
    Complaint complaint;

    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, complaint.name);
    cout << "Rate the department (1-5): ";
    cin >> complaint.rating;
    cin.ignore();
    cout << "Enter your complaint: ";
    getline(cin, complaint.description);

    storeInFile("complaints.txt", "Name: " + complaint.name + ", Rating: " + to_string(complaint.rating) + ", Complaint: " + complaint.description);
    cout << "Thank you for your feedback." << endl;
}

// Admin Requirements
// Crew Management Module
// Function to read crew from file
// Function to read crew from file
vector<Crew> readCrewFromFile(const string& fileName) {
    vector<Crew> crewList;
    ifstream file(fileName);
    string line;

    while (getline(file, line)) {
        Crew crew;
        stringstream ss(line);

        // Read crew details from the file (csv format)
        getline(ss, crew.id, ',');
        getline(ss, crew.name, ',');
        getline(ss, crew.address, ',');
        getline(ss, crew.CNIC, ',');
        getline(ss, crew.dutyTiming, ',');
        getline(ss, crew.shift, ',');
        getline(ss, crew.trainAssigned, ',');

        // Mark crew presence initially as false
        crew.present = false;

        crewList.push_back(crew);
    }

    file.close();
    return crewList;
}

// Function to write crew to file
void writeCrewToFile(const string& fileName, const vector<Crew>& crewList) {
    ofstream file(fileName, ios::trunc);
    for (const auto& crew : crewList) {
        file << crew.id << ", " << crew.name << ", " << crew.address << ", "
             << crew.CNIC << ", " << crew.dutyTiming << ", " << crew.shift << ", "
             << crew.trainAssigned << ", " << (crew.present ? "1" : "0") << endl;
    }
    file.close();
}

// Function to mark crew presence
void markCrewPresence() {
    vector<Crew> crewList = readCrewFromFile("crew.txt");

    if (crewList.empty()) {
        cout << "No crew members found." << endl;
        return;
    }

    cout << "Crew Members:\n";
    for (size_t i = 0; i < crewList.size(); ++i) {
        cout << i + 1 << ". " << crewList[i].name << " (ID: " << crewList[i].id << ")\n";
    }

    int index;
    cout << "Enter the number of the crew member to mark present (0 to exit): ";
    cin >> index;

    while (index > 0 && index <= static_cast<int>(crewList.size())) {
        crewList[index - 1].present = true;  // Mark as present
        cout << crewList[index - 1].name << " marked as present." << endl;
        storeInFile("attendance.txt", "Crew ID: " + crewList[index - 1].id + ", Name: " + crewList[index - 1].name + ", Status: Present");
        cout << "Enter the number of the next crew member to mark present (0 to exit): ";
        cin >> index;
    }

    cout << "Presence marking completed." << endl;

    // Write updated crew list back to file
    writeCrewToFile("crew.txt", crewList);
}

// Updated manageCrew function
void manageCrew() {
    vector<Crew> crewList = readCrewFromFile("crew.txt");
    Crew crew;
    int option;

    cout << "1. Add Crew\n2. Remove Crew\n3. Mark Crew Presence\nOption: ";
    cin >> option;

    if (option == 1) {
        cout << "Enter Crew Details:\n";
        cout << "ID (6 digits): ";
        cin >> crew.id;

        // Check for duplicate ID
        auto duplicate = find_if(crewList.begin(), crewList.end(), [&](const Crew& c) {
            return c.id == crew.id;
        });
        if (duplicate != crewList.end()) {
            cout << "Error: A crew member with this ID already exists. Please use a unique ID." << endl;
            return;
        }

        cout << "Name: ";
        cin.ignore();
        getline(cin, crew.name);
        cout << "Address: ";
        getline(cin, crew.address);
        cout << "CNIC: ";
        getline(cin, crew.CNIC);
        cout << "Duty Timing: ";
        getline(cin, crew.dutyTiming);
        cout << "Shift: ";
        getline(cin, crew.shift);
        cout << "Train Assigned: ";
        getline(cin, crew.trainAssigned);

        // Add crew to the list
        crewList.push_back(crew);

        // Save to file in formatted output
        writeCrewToFile("crew.txt", crewList);

        cout << "Crew member added successfully." << endl;
    } else if (option == 2) {
        string crewId;
        cout << "Enter the ID of the crew member to remove: ";
        cin.ignore();
        getline(cin, crewId);

        // Remove crew by ID
        auto originalSize = crewList.size();
        crewList.erase(remove_if(crewList.begin(), crewList.end(), [&](const Crew& c) {
            return c.id == crewId;
        }), crewList.end());

        if (crewList.size() == originalSize) {
            cout << "No crew member with ID " << crewId << " found." << endl;
        } else {
            writeCrewToFile("crew.txt", crewList);
            cout << "Crew member removed successfully." << endl;
        }
    } else if (option == 3) {
        markCrewPresence();
    } else {
        cout << "Invalid option." << endl;
    }
}



// Train Capacity Management
void manageTrainCapacity() {
    vector<Train> trains = readTrainsFromFile("train_capacity.txt");
    Train train;
    int option;

    cout << "1. Add Train\n2. Remove Train\nOption: ";
    cin >> option;
    cin.ignore(); // Clear the newline character left in the buffer by cin

    if (option == 1) {
        cout << "Enter Train Details:\n";
        cout << "Train Name: ";
        getline(cin, train.trainName); // Use getline to get the full name
        cout << "Timing: ";
        getline(cin, train.timing);    // Use getline for timing

        cout << "First Class Capacity: ";
        cin >> train.firstClassCapacity;
        cout << "AC Class Capacity: ";
        cin >> train.acClassCapacity;
        cout << "Economy Class Capacity: ";
        cin >> train.economyClassCapacity;
        cout << "Low Economy Class Capacity: ";
        cin >> train.lowEconomyClassCapacity;

        cout << "Luggage Capacity (e.g., '9 tons', '15 kg', '500 grams'): ";
        cin.ignore(); // Clear the buffer
        getline(cin, train.luggageCapacity); // Get luggage capacity with units

        // Add the train to the list and write to the file
        trains.push_back(train);
        writeTrainsToFile("train_capacity.txt", trains);

        cout << "Train added successfully." << endl;
    } else if (option == 2) {
        string trainName;
        cout << "Enter the name of the train to remove: ";
        getline(cin, trainName); // Get the full name of the train

        // Remove the train from the vector
        trains.erase(remove_if(trains.begin(), trains.end(), [&](const Train& t) {
            return t.trainName == trainName;
        }), trains.end());

        writeTrainsToFile("train_capacity.txt", trains);
        cout << "Train removed successfully." << endl;
    } else {
        cout << "Invalid option." << endl;
    }
}

const int MAX_BOOKINGS = 100;
const int MAX_INVENTORY = 100;
const int MAX_PILOTS = 50;
const int MAX_TRAIN_TIMINGS = 4;

string bookings[MAX_BOOKINGS];
string inventory[MAX_INVENTORY];
string pilotSchedules[MAX_PILOTS][10];  // Max 10 duties per pilot
string trainTimings[MAX_TRAIN_TIMINGS] = {"Train A: 08:00 AM", "Train B: 12:00 PM", "Train C: 04:00 PM", "Train D: 08:00 PM"};
int inventoryQuantities[MAX_INVENTORY] = {0};
int totalBookings = 0;
int totalPilots = 0;
int totalInventoryItems = 0;
int totalTrainTimings = 0;

// Helper functions for file handling
void loadBookings() {
    ifstream file("bookings.txt");
    if (!file) {
        cout << "No bookings file found. Starting with empty bookings.\n";
        return;
    }
    string customerName;
    int id = 0;
    while (getline(file, customerName) && id < MAX_BOOKINGS) {
        bookings[id++] = customerName;
        totalBookings++;
    }
    file.close();
}

void saveBookings() {
    ofstream file("bookings.txt");
    for (int i = 0; i < MAX_BOOKINGS; i++) {
        if (bookings[i] != "") {
            file << bookings[i] << endl;
        }
    }
    file.close();
}

void loadInventory() {
    ifstream file("inventory.txt");
    if (!file) {
        cout << "No inventory file found. Starting with empty inventory.\n";
        return;
    }
    string itemName;
    int quantity;
    while (file >> itemName >> quantity) {
        inventory[totalInventoryItems] = itemName;
        inventoryQuantities[totalInventoryItems] = quantity;
        totalInventoryItems++;
    }
    file.close();
}

void saveInventory() {
    ofstream file("inventory.txt");
    for (int i = 0; i < totalInventoryItems; i++) {
        file << inventory[i] << " " << inventoryQuantities[i] << endl;
    }
    file.close();
}

void loadPilotSchedules() {
    ifstream file("pilotSchedules.txt");
    if (!file) {
        cout << "No pilot schedules file found. Starting with empty schedules.\n";
        return;
    }
    string pilotName, duty;
    int pilotIndex = 0, dutyIndex = 0;
    while (file >> pilotName) {
        getline(file, duty);  // Read full duty description
        pilotSchedules[pilotIndex][dutyIndex++] = duty;
        if (dutyIndex >= 10) {  // Move to next pilot if max duties reached
            pilotIndex++;
            dutyIndex = 0;
        }
        totalPilots++;
    }
    file.close();
}

void savePilotSchedules() {
    ofstream file("pilotSchedules.txt");
    for (int i = 0; i < totalPilots; i++) {
        for (int j = 0; j < 10 && pilotSchedules[i][j] != ""; j++) {
            file << pilotSchedules[i][j] << endl;
        }
    }
    file.close();
}

// Booking system
void bookResource(int id, string customerName) {
    if (id < 0 || id >= MAX_BOOKINGS || bookings[id] != "") {
        cout << "Resource is already booked or invalid ID.\n";
    } else {
        bookings[id] = customerName;
        totalBookings++;
        saveBookings();  // Save to file after booking
        cout << "Booking successful for " << customerName << " with booking ID: " << id << endl;
    }
}

void cancelBooking(int id) {
    if (id < 0 || id >= MAX_BOOKINGS || bookings[id] == "") {
        cout << "No booking found with ID: " << id << endl;
    } else {
        cout << "Booking cancelled for: " << bookings[id] << endl;
        bookings[id] = "";
        totalBookings--;
        saveBookings();  // Save to file after cancellation
    }
}

// Inventory management
void addItem(string item, int quantity) {
    bool found = false;
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (inventory[i] == item) {
            inventoryQuantities[i] += quantity;
            cout << quantity << " " << item << "(s) added to inventory.\n";
            found = true;
            break;
        }
        if (inventory[i] == "") {
            inventory[i] = item;
            inventoryQuantities[i] = quantity;
            totalInventoryItems++;
            cout << quantity << " " << item << "(s) added to inventory.\n";
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Inventory limit reached.\n";
    }
    saveInventory();  // Save to file after adding
}

void removeItem(string item, int quantity) {
    bool found = false;
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (inventory[i] == item) {
            if (inventoryQuantities[i] >= quantity) {
                inventoryQuantities[i] -= quantity;
                cout << quantity << " " << item << "(s) removed from inventory.\n";
            } else {
                cout << "Not enough " << item << " in inventory.\n";
            }
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Item not found in inventory.\n";
    }
    saveInventory();  // Save to file after removal
}

void showInventory() {
    cout << "Current Inventory:\n";
    for (int i = 0; i < totalInventoryItems; i++) {
        if (inventory[i] != "") {
            cout << inventory[i] << ": " << inventoryQuantities[i] << endl;
        }
    }
}

// Pilot duty management
void assignDuty(string pilotName, string duty) {
    if (totalPilots < MAX_PILOTS) {
        int pilotIndex = totalPilots++;
        pilotSchedules[pilotIndex][0] = duty;
        savePilotSchedules();  // Save to file after assigning duty
        cout << "Duty assigned to " << pilotName << ": " << duty << endl;
    } else {
        cout << "Pilot limit reached.\n";
    }
}

void checkPilotEligibility(string pilotName) {
    bool found = false;
    for (int i = 0; i < MAX_PILOTS; i++) {
        if (pilotSchedules[i][0] != "") {
            cout << "Pilot has the following duties:\n";
            for (int j = 0; j < 10 && pilotSchedules[i][j] != ""; j++) {
                cout << pilotSchedules[i][j] << endl;
            }
            found = true;
        }
    }
    if (!found) {
        cout << "No duties found for " << pilotName << endl;
    }
}

// Train timings
// Function to load train timings from file
// Function to load train timings from file (only one definition)
void loadTrainTimings() {
    ifstream file("trainTimings.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line) && totalTrainTimings < MAX_TRAIN_TIMINGS) {
            trainTimings[totalTrainTimings++] = line;
        }
        file.close();
    } else {
        cout << "Error opening train timings file.\n";
    }
}

// Function to save train timings to file (only one definition)
void saveTrainTimings() {
    ofstream file("trainTimings.txt");
    if (file.is_open()) {
        for (int i = 0; i < totalTrainTimings; i++) {
            file << trainTimings[i] << endl;
        }
        file.close();
    } else {
        cout << "Error saving train timings to file.\n";
    }
}

// Function to display train timings
void displayTrainTimings() {
    cout << "Available Train Timings:\n";
    for (int i = 0; i < totalTrainTimings; i++) {
        cout << trainTimings[i] << endl;
    }
}

// Function to add new train timing
void addTrainTiming() {
    if (totalTrainTimings < MAX_TRAIN_TIMINGS) {
        string newTiming;
        cout << "Enter new train timing (e.g., Train E: 10:00 PM): ";
        cin.ignore();  // Clear the input buffer
        getline(cin, newTiming);

        // Add the new timing
        trainTimings[totalTrainTimings++] = newTiming;

        // Save the updated train timings to file
        saveTrainTimings();
        cout << "New train timing added and saved.\n";
    } else {
        cout << "Train timing limit reached.\n";
    }
}


void generateReport() {
    cout << "Generating Analytics Report...\n";
    cout << "Booking Summary: " << totalBookings << " Bookings in the past week.\n";
    cout << "Inventory Summary: " << totalInventoryItems << " items sold.\n";
    cout << "Pilot Summary: " << totalPilots << " pilots on duty.\n";
    cout << "Train Summary: " << MAX_TRAIN_TIMINGS << " trains on schedule.\n";
}

int main() {
    LoginSystem loginSystem;
    RouteManagement routeManager;
    PassengerInfo passengerInfo;
    MaintenanceSchedule maintenanceSchedule;
    SecuritySurveillance securityManager;

    int mainChoice;
    do {
        cout << "\n--- Railway Management System ---\n";
        cout << "1. Register User\n";
        cout << "2. Login\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1:
                loginSystem.registerUser();
                break;

            case 2: {
                string role = loginSystem.login();
                if (role == "Admin" || role == "User") {
                    int choice;
                    do {
                        cout << "\n--- Services Menu ---\n";
                        if (role == "Admin") {
                            cout << "1. Manage Routes\n";
                            cout << "2. Passenger Information\n";
                            cout << "3. Maintenance Schedule\n";
                            cout << "4. View Security Logs\n";
                            cout << "5. Crew Management\n";
                            cout << "6. Train Capacity Management\n";
                            cout << "7. Inventory Management\n";
                            cout << "8. Pilot Duty Check\n";
                            cout << "9. Analytics Reporting\n";
                        }
                        cout << "10. Train Checking\n";
                        cout << "11. Complaints\n";
                        cout << "12. Booking System\n";
                        cout << "13. Payments\n";
                        cout << "14. Train Timings\n";
                        cout << "0. Logout\n";
                        cout << "Enter choice: ";
                        cin >> choice;

                        switch (choice) {
                            case 1:
                                if (role == "Admin") {
                                    int routeChoice;
                                    cout << "1. Add Route\n2. View Routes\n3. Delete Route\nEnter choice: ";
                                    cin >> routeChoice;
                                    if (routeChoice == 1) routeManager.addRoute();
                                    else if (routeChoice == 2) routeManager.viewRoutes();
                                    else if (routeChoice == 3) routeManager.deleteRoute();
                                    else cout << "Invalid choice.\n";
                                } else cout << "Access denied.\n";
                                break;

                            case 2:
                                if (role == "Admin") {
                                    int passengerChoice;
                                    cout << "1. Add Passenger\n2. View Passengers\nEnter choice: ";
                                    cin >> passengerChoice;
                                    if (passengerChoice == 1) passengerInfo.addPassenger();
                                    else if (passengerChoice == 2) passengerInfo.viewPassengers();
                                    else cout << "Invalid choice.\n";
                                } else cout << "Access denied.\n";
                                break;

                            case 3:
                                if (role == "Admin") {
                                    int maintenanceChoice;
                                    cout << "1. Add Schedule\n2. View Schedules\nEnter choice: ";
                                    cin >> maintenanceChoice;
                                    if (maintenanceChoice == 1) maintenanceSchedule.addSchedule();
                                    else if (maintenanceChoice == 2) maintenanceSchedule.viewSchedules();
                                    else cout << "Invalid choice.\n";
                                } else cout << "Access denied.\n";
                                break;

                            case 4:
                            
                                if (role == "Admin") securityManager.viewLogs();
                                else cout << "Access denied.\n";
                                break;

                            case 5:
                                if (role == "Admin") manageCrew();
                                else cout << "Access denied.\n";
                                break;

                            case 6:
                                if (role == "Admin") manageTrainCapacity();
                                else cout << "Access denied.\n";
                                break;

                            case 7:
                                if (role == "Admin") {
                                    int inventoryChoice;
                                    string item;
                                    int quantity;
                                    cout << "1. Add Item\n2. Remove Item\n3. Show Inventory\nEnter choice: ";
                                    cin >> inventoryChoice;
                                    cin.ignore(); // Clear the buffer
                                    if (inventoryChoice == 1) {
                                        cout << "Enter item name: ";
                                        getline(cin, item);
                                        cout << "Enter quantity: ";
                                        cin >> quantity;
                                        addItem(item, quantity);
                                    } else if (inventoryChoice == 2) {
                                        cout << "Enter item name: ";
                                        getline(cin, item);
                                        cout << "Enter quantity to remove: ";
                                        cin >> quantity;
                                        removeItem(item, quantity);
                                    } else if (inventoryChoice == 3) showInventory();
                                    else cout << "Invalid choice.\n";
                                } else cout << "Access denied.\n";
                                break;

                            case 8:
                                if (role == "Admin") {
                                    int dutyChoice;
                                    string pilotName, duty;
                                    cout << "1. Assign Duty\n2. Check Pilot Eligibility\nEnter choice: ";
                                    cin >> dutyChoice;
                                    cin.ignore(); // Clear the buffer
                                    if (dutyChoice == 1) {
                                        cout << "Enter pilot name: ";
                                        getline(cin, pilotName);
                                        cout << "Enter duty: ";
                                        getline(cin, duty);
                                        assignDuty(pilotName, duty);
                                    } else if (dutyChoice == 2) {
                                        cout << "Enter pilot name: ";
                                        getline(cin, pilotName);
                                        checkPilotEligibility(pilotName);
                                    } else cout << "Invalid choice.\n";
                                } else cout << "Access denied.\n";
                                break;

                            case 9:
                                if (role == "Admin") generateReport();
                                else cout << "Access denied.\n";
                                break;

                            case 10:
                                trainChecking();
                                break;

                            case 11:
                                handleComplaint();
                                break;

                            case 12: {
                                int bookingId;
                                string customerName;
                                cout << "Enter booking ID: ";
                                cin >> bookingId;
                                cin.ignore(); // Clear the buffer
                                cout << "Enter customer name: ";
                                getline(cin, customerName);
                                bookResource(bookingId, customerName);
                                break;
                            }

                            case 13:
                                handlePayment();
                                break;

                            case 14:
                                displayTrainTimings();
                                break;

                            case 0:
                                cout << "Logging out.\n";
                                break;

                            default:
                                cout << "Invalid choice. Please try again.\n";
                        }
                    } while (choice != 0);
                } else cout << "Invalid role. Please try again.\n";
                break;
            }

            case 0:
                cout << "Exiting the system. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (mainChoice != 0);

    return 0;
}
