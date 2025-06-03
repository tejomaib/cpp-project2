#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <bitset>
using namespace std;

bool isValid(const string& ip) {
    stringstream ss(ip);
    string segment;
    int count = 0;
    while (getline(ss, segment, '.')) {
        if (segment.empty() || segment.size() > 3) return false;
        for (char ch : segment) {
            if (!isdigit(ch)) return false;
        }
        int num = stoi(segment);
        if (num < 0 || num > 255) return false;
        ++count;
    }
    return count == 4;
}

string ipToBinary(const string& ip) {
    stringstream ss(ip);
    string segment;
    string binaryIP;
    while (getline(ss, segment, '.')) {
        int num = stoi(segment);
        binaryIP += bitset<8>(num).to_string();
    }
    return binaryIP;
}

int main() {
    int numSubnets;
    cout << "Welcome! In this Routing Table Simulation, you will first be prompted to fill out network information in order to then find the respective subnets and next hops for any IP Address of your choice!";
    cout << "\nEnter the Number of Subnets (excluding the Default Gateway) in the Network: ";
    cin >> numSubnets;

    vector<string> subnetNumbers; 
    vector<string> subnetMasks;
    vector<string> nextHops;

    string value, mask, hop, gatewayHop;
    int defGateway;

    cin.ignore();

    for (int i = 1; i <= numSubnets; i++) {
        while (true) {
            cout << "\nFor Subnet " << i << ", Enter the Subnet Number (ex: X.X.X.X): ";
            getline(cin, value);
            if (isValid(value)) break;
            else cout << "Invalid format. Please try again.\n";
        }
        subnetNumbers.push_back(value);

        while (true) {
            cout << "For Subnet " << i << ", Enter the Subnet Mask (ex: X.X.X.X): ";
            getline(cin, mask);
            if (isValid(mask)) break;
            else cout << "Invalid format. Please try again.\n";
        }
        subnetMasks.push_back(mask);

        cout << "For Subnet " << i << ", Enter the Next Hop: ";
        getline(cin, hop);
        nextHops.push_back(hop);
    }

    cout << "\nIs the Default Gateway set? (1 for Yes, 0 for No): ";
    cin >> defGateway;
    if (defGateway == 1){
        cout << "\nFor the Default Gateway, Enter the Next Hop: ";
        getline(cin, gatewayHop);
    }

    cout << "\nRouting Table:\n";
    cout << left << setw(20) << "Subnet Number" << setw(20) << "Subnet Mask" << setw(20) << "Next Hop" << endl;
    cout << string(60, '-') << endl;

    for (int i = 0; i < numSubnets; i++) {
        cout << left << setw(20) << subnetNumbers[i]
             << setw(20) << subnetMasks[i]
             << setw(20) << nextHops[i] << endl;
    }

    if (defGateway == 1) {
        cout << left << setw(20) << "Default"
             << setw(20) << "-"
             << setw(20) << gatewayHop << endl;
    }

    string ipAddress;
    cin.ignore();
    cout << "\nWhat IP Address would you like to determine the Subnet for? Press Enter to QUIT: ";
    getline(cin, ipAddress);

    while (!ipAddress.empty()) {
        if (!isValid(ipAddress)) {
            cout << "Invalid IP format. Please try again.\n";
        } else {
            string binIP = ipToBinary(ipAddress);
            bool matched = false;

            for (int i = 0; i < numSubnets; i++) {
                string binMask = ipToBinary(subnetMasks[i]);
                string binSubnet = ipToBinary(subnetNumbers[i]);
                string ipMasked = "";

                for (int j = 0; j < 32; j++) {
                    if (binIP[j] == '1' && binMask[j] == '1') {
                        ipMasked += '1';
                    } else {
                        ipMasked += '0';
                    }
                }

                if (ipMasked == binSubnet) {
                    cout << "IP address " << ipAddress << " belongs to Subnet " << subnetNumbers[i] << " via " << nextHops[i] << ".\n";
                    matched = true;
                    break;
                }
            }

            if (!matched) {
                if (defGateway == 1) {
                    cout << "IP address " << ipAddress << " is forwarded to the default gateway via " << gatewayHop << ".\n";
                } else {
                    cout << "IP address " << ipAddress << " is lost. No matching subnet and no default gateway.\n";
                }
            }
        }

        cout << "\nWhat IP Address would you like to determine the Subnet for? Press Enter to QUIT: ";
        getline(cin, ipAddress);
    }
    cout << "\nThe program has been terminated successfully";
    return 0;
}
