#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

//Include all dependencies
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

//Array to hold round keys
string round_keys[16];
// String to hold the plain text
string pt;
// String to hold cipher text
string ct;
// Function to do a circular left shift by 1
string shift_left_once(string key_chunk);
// Function to do a circular left shift by 2
string shift_left_twice(string key_chunk);
// Function to generate key
void generate_keys(string key);
// Function to convert a number in decimal to binary
string convertDecimalToBinary(int decimal);
// Function to convert a number in binary to decimal
int convertBinaryToDecimal(string binary);
// Function to compute xor between two strings
string Xor(string a, string b);
// Function to implement DES encryption
string DES();


#endif