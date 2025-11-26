#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include <fstream>

using namespace std;

struct OpenReadingFrame {
    int startIndex;
    int stopIndex;
    string sequence;
};

int main() {
    cout << "-----------------------------------------------------------------" << endl;
    cout << "IMPORTANT NOTE for first-time users (especially on Ubuntu/Linux):" << endl;
    cout << "\nIf you are copy-pasting a multi-line sequence, your terminal" << endl;
    cout << "might execute it line-by-line, causing a mess." << endl;
    cout << "\n**TO FIX THIS (Permanent Solution):**" << endl;
    cout << "1. Open your terminal's config file. Run: nano ~/.bashrc" << endl;
    cout << "2. Add this exact line to the very end of the file:" << endl;
    cout << "    bind 'set enable-bracketed-paste on'" << endl;
    cout << "3. Save (Ctrl+O), exit (Ctrl+X), and restart your terminal." << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << endl;

    cout << "=======================================\n";
    cout << "   Biological Sequence Analyzer v2.0\n";
    cout << "=======================================\n\n";
    
    cout << "Please select your sequence type:\n";
    cout << "  1. Eukaryotic DNA MRNA\n";
    cout << "  2. Prokaryotic DNA&MRNA \n";
    cout << "  3. RNA\n";
    cout << "Enter your choice (1, 2, or 3): ";

    int choice;
    cin >> choice;

    string startCodon;
    vector<string> stopCodons;
    string promoterPattern;
    string promoterName;

    if (choice == 1) {
        startCodon = "ATG";
        stopCodons = {"TAA", "TAG", "TGA"};
        promoterPattern = "TATAAA";
        promoterName = "TATA Box";
        cout << "\n[Mode: Eukaryotic DNA]\n";
    } else if (choice == 2) {
        startCodon = "ATG";
        stopCodons = {"TAA", "TAG", "TGA"};
        promoterPattern = "TATAAT";
        promoterName = "Pribnow Box";
        cout << "\n[Mode: Prokaryotic DNA]\n";
    } else if (choice == 3) {
        startCodon = "AUG";
        stopCodons = {"UAA", "UAG", "UGA"};
        promoterPattern = "";
        promoterName = "N/A";
        cout << "\n[Mode: RNA]\n";
    } else {
        cerr << "Error: Invalid choice.\n";
        return 1;
    }

    cout << "\nSelect Input Method:\n";
    cout << "  1. Load from .txt/.fasta file\n";
    cout << "  2. Paste manually (Terminal)\n";
    cout << "Enter choice (1 or 2): ";
    
    int inputMethod;
    cin >> inputMethod;

    string line;
    stringstream sequenceStream;
    string sequence;

    if (inputMethod == 1) {
        cout << "Enter the filename (e.g., sequence.txt): ";
        string filename;
        cin >> filename;

        ifstream file(filename);
        
        if (!file.is_open()) {
            cerr << "Error: Could not open file '" << filename << "'. Check the name and try again.\n";
            return 1;
        }

        while (getline(file, line)) {
            if (line.empty() || line[0] == '>') {
                continue;
            }
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
            sequenceStream << line;
        }
        file.close();

    } else if (inputMethod == 2) {
        cout << "\nPlease paste your sequence and press Ctrl+D (Linux/Mac) or Ctrl+Z (Windows) when done:\n\n";
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while (getline(cin, line)) {
            if (line.empty() || line[0] == '>') {
                continue;
            }
            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
            sequenceStream << line;
        }
    } else {
        cerr << "Error: Invalid input method.\n";
        return 1;
    }
    
    sequence = sequenceStream.str();

    if (sequence.empty()) {
        cerr << "No sequence provided. Exiting." << endl;
        return 1;
    }

    cout << "\nSequence loaded (" << sequence.length() << " bp). Running analysis..." << endl;

    transform(sequence.begin(), sequence.end(), sequence.begin(), ::toupper);

    vector<int> promoterLocations;
    vector<OpenReadingFrame> foundORFs;

    if (!promoterPattern.empty()) {
        for (int i = 0; i <= (int)sequence.length() - (int)promoterPattern.length(); ++i) {
            if (sequence.substr(i, promoterPattern.length()) == promoterPattern) {
                promoterLocations.push_back(i);
            }
        }
    }

    for (int i = 0; i <= (int)sequence.length() - 3; ++i) {
        if (sequence.substr(i, 3) == startCodon) {
            for (int j = i + 3; j <= (int)sequence.length() - 3; j += 3) {
                string currentCodon = sequence.substr(j, 3);
                bool isStopCodon = false;

                for (const string& stop : stopCodons) {
                    if (currentCodon == stop) {
                        isStopCodon = true;
                        break;
                    }
                }

                if (isStopCodon) {
                    OpenReadingFrame orf;
                    orf.startIndex = i;
                    orf.stopIndex = j;
                    orf.sequence = sequence.substr(i, (j + 3) - i);
                    foundORFs.push_back(orf);
                    break;
                }
            }
        }
    }

    cout << "\n\n=======================================\n";
    cout << "          ANALYSIS COMPLETE\n";
    cout << "=======================================\n\n";

    if (!promoterName.empty() && promoterName != "N/A") {
        cout << "--- " << promoterName << " Results ---\n";
        if (promoterLocations.empty()) {
            cout << "No potential " << promoterName << " sequences found.\n";
        } else {
            cout << "Found " << promoterLocations.size() << " potential " << promoterName << "(s) at index:\n";
            for (int loc : promoterLocations) {
                cout << "  > " << loc << "\n";
            }
        }
        cout << "\n";
    }

    cout << "--- ORF Results ---\n";
    if (foundORFs.empty()) {
        cout << "No complete ORFs found.\n";
    } else {
        cout << "Found " << foundORFs.size() << " potential ORF(s):\n\n";
        for (const OpenReadingFrame& orf : foundORFs) {
            cout << "----------------------------------------\n";
            cout << "  Start Index: " << orf.startIndex << "\n";
            cout << "  Stop Index:  " << orf.stopIndex << "\n";
            cout << "  Length: " << orf.sequence.length() << " bp\n";
            cout << "  Sequence: " << orf.sequence << "\n";
            cout << "----------------------------------------\n\n";
        }
    }

    return 0;
}
