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