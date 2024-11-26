#include <iostream>      
#include <fstream>      
#include <vector>       
#include <string>       
#include <algorithm>    
#include <random>       
#include <cstdlib>      
#include <experimental/random>


using namespace std;

vector<int> randkey() {
    vector<int> key = {0, 1, 2, 3, 4};
    random_device rd;
    mt19937 g(rd());

    // Shuffle the vector
    shuffle(key.begin(), key.end(), g);
    fstream key_file;
    key_file.open("key.txt", fstream::out);
    if(!key_file.is_open()) {
        cout << "\nError: file doesn't exist\nQuitting...\n";
        exit(-1);
    }
    for(int i = 0; i < 5; i++) {
        key_file << key[i];
    }
    cout << "Key saved to key.txt\n";
    return key;
}

void encrypt_char_shift(string file_name) {
    vector<int> key = randkey();
    char c;
    string file_text;
    vector<vector<char>> transpose_vector;
    fstream file_to_encrypt;
    fstream encrypted_file;

    file_to_encrypt.open(file_name, fstream::in);
    encrypted_file.open("encrypted_file.txt", fstream::out);
    if (!file_to_encrypt.is_open() || !encrypted_file.is_open()){
        cout << "\nError: file doesn't exist\nQuitting...\n";
        exit(-1);
    }
    int row_end = 0;
    while(file_to_encrypt  >> noskipws >> c) {
        
        file_text += c;
        row_end++;
    }
    row_end = (row_end + 4) / 5;
    transpose_vector.resize(row_end);
    int i = 0;
    for(int row = 0; row < row_end; row++) {
        for(int col = 0; col < 5; col++) {
            transpose_vector[row].resize(5);
            if(file_text.size() >= (i + 1)) {
                transpose_vector[row][col] = file_text[i];
                //cout << transpose_vector[row][col];
            } else {
                transpose_vector[row][col] += '.';
                //cout << transpose_vector[row][col];
            }
            i++;
            
        }
    }
    for(int col = 0; col < 5; col++) {
        for(int row = 0; row < row_end; row++) {
            //cout << key[col];
            encrypted_file << transpose_vector[row][key[col]];
        }
    }
    file_to_encrypt.close();
    encrypted_file.close();
}

void decrypt_char_shift(string file_name) {
    int row_end = 0;
    string file_text;
    char c;
    int k;
    int i = 0;
    vector<int> key;
    vector<vector<char>> transpose_vector;
    fstream file_to_decrypt;
    fstream decrypted_file;
    fstream key_file;
    
    file_to_decrypt.open(file_name, fstream::in);
    decrypted_file.open("decrypted_file.txt", fstream::out);
    key_file.open("key.txt", fstream::in);
    if (!file_to_decrypt.is_open() || !decrypted_file.is_open() || !key_file.is_open()){
        cout << "\nError: file doesn't exist\nQuitting...\n";
        exit(-1);
    }
    while (file_to_decrypt >> noskipws >> c) {
        //cout << c;
        file_text += c;
        row_end++;
    }
    row_end = (row_end + 4) / 5;
    
    key.resize(5);

    while (key_file >> noskipws >> c) {
        k = c - 48;
        key[i] = k;
        i++;
    }
    i = 0;
    transpose_vector.resize(row_end);
    for(int col = 0; col < 5; col++) {
        for(int row = 0; row < row_end; row++) {
            transpose_vector[row].resize(5);
            transpose_vector[row][key.at(col)] = file_text[i];
            //cout << transpose_vector[row][col];
            i++;
        }
        //cout << endl;
    }
    for(int row = 0; row < row_end; row++) {
        for(int col = 0; col < 5; col++) {
            
            //cout << "column: " + to_string(key.at(col));
            //cout << transpose_vector[row][col];
            if(transpose_vector[row][col] == '.') {
                break;
            }
            decrypted_file << transpose_vector[row][col];
        }
    }
    key_file.close();
    file_to_decrypt.close();
    decrypted_file.close();
}

int main() {
    string choice;
    string input_file;
    int key;
    cout << "Choose encrypt, decrypt, or quit\n";
    cin >> choice;

    if(choice == "encrypt") {
        cout << "You chose encrypt\nEnter file to encrypt: ";
        cin >> input_file;
        encrypt_char_shift(input_file);
        cout << "Done. Quitting...\n";
        return 0;
    }
    if(choice == "decrypt") {
        cout << "You chose decrypt\nEnter file to decrypt: ";
        cin >> input_file;
        decrypt_char_shift(input_file);
        cout << "Done. Quitting...\n";
        return 0;
    }
    if(choice == "quit") {
        cout << "Done. Quitting...\n";
        return 0;
    }
    cout << "Invalid choice. Quitting...\n";

    return 0;
}