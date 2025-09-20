#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// =================== Candidate Class ===================
class Candidate {
    int id;
    string name;
    string party;
    int votes;

public:
    Candidate() { votes = 0; }

    void input() {
        cout << "Enter Candidate ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // flush input buffer
        cout << "Enter Candidate Name: ";
        getline(cin, name);
        cout << "Enter Party Name: ";
        getline(cin, party);
        votes = 0;
    }

    void display() {
        cout << "ID: " << id
             << " | Name: " << name
             << " | Party: " << party
             << " | Votes: " << votes << endl;
    }

    int getID() { return id; }
    string getName() { return name; }
    string getParty() { return party; }
    int getVotes() { return votes; }
    void incrementVotes() { votes++; }

    // File handling: write and read candidate
    void saveToFile(ofstream &fout) {
        fout << id << " " << name << " " << party << " " << votes << "\n";
    }

    void loadFromFile(ifstream &fin) {
        fin >> id;
        fin.ignore();
        getline(fin, name, ' ');
        getline(fin, party, ' ');
        fin >> votes;
    }
};

// =================== Helper Functions ===================

// Add Candidate (Admin)
void addCandidate() {
    Candidate c;
    c.input();
    ofstream fout("candidates.txt", ios::app);
    fout << c.getID() << " " << c.getName() << " " << c.getParty() << " " << c.getVotes() << "\n";
    fout.close();
    cout << "✅ Candidate added successfully!\n";
}

// View all Candidates
void viewCandidates() {
    ifstream fin("candidates.txt");
    if(!fin) {
        cout << "No candidates found.\n";
        return;
    }

    int count = 0;
    int id, votes;
    string name, party;
    cout << "\n--- Candidate List ---\n";
    while(fin >> id >> ws && getline(fin, name, ' ') && getline(fin, party, ' ') >> votes) {
        count++;
        cout << "ID: " << id << " | Name: " << name << " | Party: " << party << " | Votes: " << votes << endl;
    }

    if(count == 0) cout << "No candidates found.\n";
    fin.close();
}

// View Results (Admin)
void viewResults() {
    ifstream fin("candidates.txt");
    if(!fin) {
        cout << "No candidates found.\n";
        return;
    }

    int count = 0;
    int id, votes;
    string name, party;
    cout << "\n--- Election Results ---\n";
    while(fin >> id >> ws && getline(fin, name, ' ') && getline(fin, party, ' ') >> votes) {
        count++;
        cout << "ID: " << id << " | Name: " << name << " | Party: " << party << " | Votes: " << votes << endl;
    }

    if(count == 0) cout << "No candidates found.\n";
    fin.close();
}

// Cast Vote (User)
void castVote() {
    int voterID;
    cout << "Enter Your Voter ID: ";
    cin >> voterID;

    // Check if voter already voted
    ifstream vfin("votes.txt");
    vector<int> votedIDs;
    int id;
    while(vfin >> id) votedIDs.push_back(id);
    vfin.close();

    for(int i : votedIDs) {
        if(i == voterID) {
            cout << "❌ You have already voted!\n";
            return;
        }
    }

    // Show candidates
    ifstream fin("candidates.txt");
    vector<Candidate> candidates;
    int cid, votes;
    string name, party;
    while(fin >> cid >> ws && getline(fin, name, ' ') && getline(fin, party, ' ') >> votes) {
        Candidate c;
        // Set candidate data
        c = Candidate();
        c.incrementVotes(); // set dummy increment for object (will overwrite later)
        candidates.push_back(c);
    }
    fin.close();

    cout << "\n--- Candidates ---\n";
    for(size_t i=0; i<candidates.size(); i++) {
        cout << i+1 << ". " << candidates[i].getName() << " | Party: " << candidates[i].getParty() << endl;
    }

    int choice;
    cout << "Enter Candidate Number to vote: ";
    cin >> choice;

    if(choice < 1 || choice > (int)candidates.size()) {
        cout << "Invalid choice!\n";
        return;
    }

    // Increment vote in file
    // Load all candidates, update chosen one, rewrite file
    vector<string> lines;
    ifstream fin2("candidates.txt");
    string line;
    while(getline(fin2, line)) lines.push_back(line);
    fin2.close();

    ofstream fout("candidates.txt");
    for(size_t i=0; i<lines.size(); i++) {
        if(i == (size_t)(choice-1)) {
            // Append +1 vote
            fout << lines[i] << " +1\n"; // simple method
        } else {
            fout << lines[i] << "\n";
        }
    }
    fout.close();

    // Record voter ID
    ofstream vout("votes.txt", ios::app);
    vout << voterID << "\n";
    vout.close();

    cout << "✅ Vote cast successfully!\n";
}

// =================== Panels ===================

// Admin Panel
void adminPanel() {
    int choice;
    do {
        cout << "\n--- Admin Panel ---\n";
        cout << "1. Add Candidate\n";
        cout << "2. View Candidates\n";
        cout << "3. View Results\n";
        cout << "4. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: addCandidate(); break;
            case 2: viewCandidates(); break;
            case 3: viewResults(); break;
            case 4: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 4);
}

// User Panel
void userPanel() {
    int choice;
    do {
        cout << "\n--- User Panel ---\n";
        cout << "1. View Candidates\n";
        cout << "2. Cast Vote\n";
        cout << "3. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: viewCandidates(); break;
            case 2: castVote(); break;
            case 3: cout << "Returning to main menu...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 3);
}

// =================== Main Function ===================
int main() {
    int choice;
    do {
        cout << "\n===== Smart Online Voting System =====\n";
        cout << "1. Admin Panel\n";
        cout << "2. User Panel\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: adminPanel(); break;
            case 2: userPanel(); break;
            case 3: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 3);

    return 0;
}
