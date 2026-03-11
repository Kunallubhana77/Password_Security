#include <fstream>
#include <iostream>
#include <string>
#include <limits>
using namespace std;

string encrypt(string pass) {
  for (int i = 0; i < pass.length(); i++) {
    pass[i] = pass[i] + 3;
  }
  return pass;
}

string decrypt(string pass) {
  for (int i = 0; i < pass.length(); i++) {
    pass[i] = pass[i] - 3;
  }
  return pass;
}

void savePassword() {
  ofstream file("vault.txt", ios::app);
  if (!file) {
    cout << "Error opening vault for writing.\n";
    return;
  }

  string site, username, password;

  cout << "Enter Site: ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, site);

  cout << "Enter Username: ";
  getline(cin, username);

  cout << "Enter Password: ";
  getline(cin, password);

  password = encrypt(password);
  file << site << "|" << username << "|" << password << endl; // Using | as delimiter to handle spaces
  file.close();

  cout << "Password Saved Successfully\n";
}

void retrievePassword() {
  ifstream file("vault.txt");
  if (!file) {
    cout << "No passwords saved yet. (Vault file not found)\n";
    return;
  }

  string line, site, username, password, search;

  cout << "Enter site to search: ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, search);

  bool found = false;
  while (getline(file, line)) {
    size_t first_pipe = line.find('|');
    size_t last_pipe = line.find_last_of('|');
    
    if (first_pipe != string::npos && last_pipe != string::npos && first_pipe != last_pipe) {
        site = line.substr(0, first_pipe);
        username = line.substr(first_pipe + 1, last_pipe - (first_pipe + 1));
        password = line.substr(last_pipe + 1);

        if (site == search) {
          cout << "Username: " << username << endl;
          cout << "Password: " << decrypt(password) << endl;
          found = true;
          break;
        }
    }
  }

  if (!found) cout << "Record not found\n";
  file.close();
}

void deleteRecord() {
  ifstream file("vault.txt");
  if (!file) {
    cout << "No passwords saved yet. (Vault file not found)\n";
    return;
  }

  ofstream temp("temp.txt");
  string line, site, del;
  bool found = false;

  cout << "Enter site to delete: ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  getline(cin, del);

  while (getline(file, line)) {
    size_t first_pipe = line.find('|');
    if (first_pipe != string::npos) {
        site = line.substr(0, first_pipe);
        if (site != del) {
          temp << line << endl;
        } else {
          found = true;
        }
    } else {
        temp << line << endl; // Keep invalid lines just in case
    }
  }

  file.close();
  temp.close();

  remove("vault.txt");
  rename("temp.txt", "vault.txt");

  if (found)
    cout << "Record deleted successfully\n";
  else
    cout << "Record not found\n";
}

int main() {
  int choice;
  do {
    cout << "\n--- Password Manager ---\n";
    cout << "1 Save Password\n";
    cout << "2 Retrieve Password\n";
    cout << "3 Delete Record\n";
    cout << "4 Exit\n";
    cout << "Enter Choice: ";
    
    if (!(cin >> choice)) {
        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continue;
    }

    switch (choice) {
    case 1:
      savePassword();
      break;
    case 2:
      retrievePassword();
      break;
    case 3:
      deleteRecord();
      break;
    case 4:
      cout << "Exiting...\n";
      break;
    default:
      cout << "Invalid choice.\n";
    }
  } while (choice != 4);
  return 0;
}