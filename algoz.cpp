#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <fstream>
#include <unordered_map>

using namespace std;
const int Size = 50;
string token[Size] = {};
string userInput;
string foundedVar[Size] = {};
bool Check = true;
string userEdt;

bool message = false;
bool Founded = false;
bool Counter = false;

// Variables
int countAND = 0;
int countOR = 0;
int countNOT = 0;
int countXOR = 0;
int countNAND = 0;
int countNOR = 0;

// Path profiling data
unordered_map<string, int> pathCount;
unordered_map<string, chrono::microseconds> pathExecutionTimes;

// Logging and profiling
ofstream logFile("event_log.txt");
chrono::steady_clock::time_point startTime;

void logEvent(const string& event) {
    logFile << "[LOG] " << event << endl;
}

void startExecutionTimer() {
    startTime = chrono::steady_clock::now();
    logEvent("Execution started.");
}

void endExecutionTimer() {
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
    logEvent("Execution completed in " + to_string(duration) + " ms.");
    cout << "Execution Time: " << duration << " ms" << endl;
}

void logGateUsage() {
    logEvent("Gate Usage Summary:");
    logEvent("AND gates used: " + to_string(countAND));
    logEvent("OR gates used: " + to_string(countOR));
    logEvent("NOT gates used: " + to_string(countNOT));
    logEvent("XOR gates used: " + to_string(countXOR));
    logEvent("NAND gates used: " + to_string(countNAND));
    logEvent("NOR gates used: " + to_string(countNOR));
}

void recordPathStart(const string& path) {
    pathCount[path]++;
    logEvent("Path started: " + path);
}

void recordPathEnd(const string& path, chrono::steady_clock::time_point startTime) {
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    pathExecutionTimes[path] += duration;
    logEvent("Path ended: " + path + " Duration: " + to_string(duration.count()) + " microseconds");
}

void displayPathProfile() {
    logEvent("Path Profile Analysis:");
    logFile << "--------------------------------------------------\n";
    logFile << "| Gate Type | Count | Execution Time (us)       |\n";
    logFile << "--------------------------------------------------\n";

    for (const auto& entry : pathCount) {
        auto duration = pathExecutionTimes[entry.first].count();
        logFile << "| " << entry.first << " | " << entry.second << "     | " << duration << " us             |\n";
        logEvent("Path: " + entry.first + " Count: " + to_string(entry.second) + " Total Execution Time: " + to_string(duration) + " us");
        cout << "Path: " << entry.first << " Count: " << entry.second << " Total Execution Time: " << duration << " us" << endl;
    }

    logFile << "--------------------------------------------------\n";
}


string andlogic(string a, string b) {
    auto startTime = chrono::steady_clock::now();
    recordPathStart("AND");
    int x = stoi(a);
    int y = stoi(b);
    int res = x * y;
    countAND++;
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    pathExecutionTimes["AND"] += duration;
    recordPathEnd("AND", startTime);
    return to_string(res);
}

string orlogic(string a, string b) {
    auto startTime = chrono::steady_clock::now();
    recordPathStart("OR");
    int x = stoi(a);
    int y = stoi(b);
    countOR++;
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    pathExecutionTimes["OR"] += duration;
    recordPathEnd("OR", startTime);
    return to_string(x + y > 0 ? 1 : 0);
}

string nandlogic(string a, string b) {
    auto startTime = chrono::steady_clock::now();
    recordPathStart("NAND");
    int x = stoi(a);
    int y = stoi(b);
    countNAND++;
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    pathExecutionTimes["NAND"] += duration;
    recordPathEnd("NAND", startTime);
    return to_string(!(x && y));
}

string norlogic(string a, string b) {
    auto startTime = chrono::steady_clock::now();
    recordPathStart("NOR");
    int x = stoi(a);
    int y = stoi(b);
    countNOR++;
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    pathExecutionTimes["NOR"] += duration;
    recordPathEnd("NOR", startTime);
    return to_string(!(x || y));
}

string notlogic(string a) {
    auto startTime = chrono::steady_clock::now();
    recordPathStart("NOT");
    bool x = stoi(a);
    countNOT++;
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    pathExecutionTimes["NOT"] += duration;
    recordPathEnd("NOT", startTime);
    return to_string(!x);
}

string xorlogic(string a, string b) {
    auto startTime = chrono::steady_clock::now();
    recordPathStart("XOR");
    int x = stoi(a);
    int y = stoi(b);
    countXOR++;
    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
    pathExecutionTimes["XOR"] += duration;
    recordPathEnd("XOR", startTime);
    return to_string((x + y) % 2);
}

void setArr() {
    for (int i = Size - 1; i > 0; i--) {
        if (token[i - 1].empty()) {
            token[i - 1] = token[i];
            token[i] = "";
        }
    }
}

bool isSingleAlphabet(const string& s) {
    return s.size() == 1 && isalpha(s[0]);
}

int countUniqueStrings(string arr[], int size) {
    unordered_map<string, bool> seenVariables;
    int count = 0;
    for (int i = 0; i < size; ++i) {
        if (isSingleAlphabet(arr[i]) && arr[i] != "e" && seenVariables.find(arr[i]) == seenVariables.end()) {
            seenVariables[arr[i]] = true; // Mark variable as seen
            ++count;
        }
    }
    return count;
}

void Handler(const vector<string>& missingVars) {
    if (!message) {
        cout << missingVars.size() << " variables are missing. Please enter them in separate lines." << endl;
        logEvent(to_string(missingVars.size()) + " variables are missing.");
        message = true;
    }

    for (const string& var : missingVars) {
        cout << "Enter value for " << var << ": ";
        cin >> userEdt;  // Input format: "1"
        string se(1, userEdt[0]); // Extract value

        // Replace variable with its value in tokens
        for (int j = 0; j < Size; j++) {
            if (token[j] == var) {
                token[j] = se;
            }
        }
    }
}

void Checker() {
    vector<string> missingVars;

    for (int i = 0; i < Size; i++) {
        if (token[i].length() == 1 && isSingleAlphabet(token[i]) && token[i] != "e") {
            // Add variable to the list of missing variables
            if (find(missingVars.begin(), missingVars.end(), token[i]) == missingVars.end()) {
                missingVars.push_back(token[i]);
            }
        }
    }

    if (!missingVars.empty()) {
        Handler(missingVars);
    }
}


int solveGate(string gate, int Pos) {
    if (gate == "" || gate == " ") return 0;
    logEvent("Evaluating parameter: " + gate + " at position " + to_string(Pos));

    if (gate == "AND") {
        if (token[Pos + 1] == "0" || token[Pos + 1] == "1" && token[Pos + 2] == "0" || token[Pos + 2] == "1") {
            token[Pos] = andlogic(token[Pos + 1], token[Pos + 2]);
            token[Pos + 1] = "";
            token[Pos + 2] = "";

            if (token[Pos + 3] == "e") {
                token[Pos + 3] = "";
            }
        }
        else {
            logEvent("BAD input: missing input number for AND");
            cout << "result : BAD input : missing input number";
            exit(0);
        }
        setArr();
    }
    else if (gate == "OR") {
        if (token[Pos + 1] == "0" || token[Pos + 1] == "1" && token[Pos + 2] == "0" || token[Pos + 2] == "1") {
            token[Pos] = orlogic(token[Pos + 1], token[Pos + 2]);
            token[Pos + 1] = "";
            token[Pos + 2] = "";

            if (token[Pos + 3] == "e") {
                token[Pos + 3] = "";
            }
        }
        else {
            logEvent("BAD input: missing input number for OR");
            cout << "result : BAD input : missing input number";
            exit(0);
        }
        setArr();
    }
    else if (gate == "XOR") {
        if (token[Pos + 1] == "0" || token[Pos + 1] == "1" && token[Pos + 2] == "0" || token[Pos + 2] == "1") {
            token[Pos] = xorlogic(token[Pos + 1], token[Pos + 2]);
            token[Pos + 1] = "";
            token[Pos + 2] = "";

            if (token[Pos + 3] == "e") {
                token[Pos + 3] = "";
            }
        }
        else {
            logEvent("BAD input: missing input number for XOR");
            cout << "result : BAD input : missing input number";
            exit(0);
        }
        setArr();
    }
    else if (gate == "NAND") {
        if (token[Pos + 1] == "0" || token[Pos + 1] == "1" && token[Pos + 2] == "0" || token[Pos + 2] == "1") {
            token[Pos] = nandlogic(token[Pos + 1], token[Pos + 2]);
            token[Pos + 1] = "";
            token[Pos + 2] = "";

            if (token[Pos + 3] == "e") {
                token[Pos + 3] = "";
            }
        }
        else {
            logEvent("BAD input: missing input number for NAND");
            cout << "result : BAD input : missing input number";
            exit(0);
        }
        setArr();
    }
    else if (gate == "NOR") {
        if (token[Pos + 1] == "0" || token[Pos + 1] == "1" && token[Pos + 2] == "0" || token[Pos + 2] == "1") {
            token[Pos] = norlogic(token[Pos + 1], token[Pos + 2]);
            token[Pos + 1] = "";
            token[Pos + 2] = "";

            if (token[Pos + 3] == "e") {
                token[Pos + 3] = "";
            }
        }
        else {
            logEvent("BAD input: missing input number for NOR");
            cout << "result : BAD input : missing input number";
            exit(0);
        }
        setArr();
    }
    else if (gate == "NOT") {
        if (token[Pos + 1] == "0" || token[Pos + 1] == "1") {
            token[Pos] = notlogic(token[Pos + 1]);
            token[Pos + 1] = "";

            if (token[Pos + 2] == "e") {
                token[Pos + 2] = "";
            }
        }
        else {
            logEvent("BAD input: missing input number for NOT");
            cout << "result : BAD input : missing input number";
            exit(0);
        }
        setArr();
    }
    else if (gate == "e") {
        logEvent("Encountered end token at position " + to_string(Pos));
    }
    else if (gate == "1" || gate == "0" || gate == "") 
logEvent("input value");
    
    else {
        logEvent("Wrong Logic Circuit Description for gate: " + gate);
        cout << "Wrong Logic Circuit Description";
        exit(0);
    }
    return 0;
}
//int Size;
int main() {
   // while (true){
         countAND = 0;
         countOR = 0;
         countNOT = 0;
         countXOR = 0;
         countNAND = 0;
         countNOR = 0;
    logFile << "Logic Gate Simulator Log File" << endl;
    startExecutionTimer();

    cout << "Enter circuit description: " << endl;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, userInput);
     //Size = userInput.c_str().length();
    char text[1000];
    strcpy(text, userInput.c_str());
    cout << userInput <<'\n';
    cout << strlen(userInput.c_str()) << '\n';
    int x = 0;
    int y = 0;
    char str[1000] = {};
    for (int i = 0; i < strlen(text); i++) {
        while (isalpha(text[i]) || isdigit(text[i]) || text[i] == 'e') {
            str[x++] = text[i];
            i++;
        }
        str[x] = '\0';
        string s(str);
        token[y++] = s;
        x = 0;
    }
    setArr();

    Checker();

    for (int i = strlen(userInput.c_str()) - 1; i >= 0; i--) {
        solveGate(token[i], i);
    }

    string res = token[0];
    cout << "AND gates: " << countAND << endl;
    cout << "OR gates: " << countOR << endl;
    cout << "NOT gates: " << countNOT << endl;
    cout << "XOR gates: " << countXOR << endl;
    cout << "NAND gates: " << countNAND << endl;
    cout << "NOR gates: " << countNOR << endl;
    cout << "Result: " << res << endl;

    logGateUsage();
    displayPathProfile();
    endExecutionTimer();

    logFile.close(); //}
    return 0;
}
