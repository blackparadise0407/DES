#include"encrypt.h"
#include <vector>
#include <windows.h>
#include <bitset>

string shift_left_once(string key_chunk) {
	string shifted = "";
	for (int i = 1; i < 28; i++) {
		shifted += key_chunk[i];
	}
	shifted += key_chunk[0];
	return shifted;
};

string shift_left_twice(string key_chunk) {
	string shifted = "";
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j < 28; j++) {
			shifted += key_chunk[j];
		}
		shifted += key_chunk[0];
		key_chunk = shifted;
		shifted = "";
	}
	return key_chunk;
};

void generate_keys(string key) {
	// The PC1 table
	int pc1[56] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
	};
	// The PC2 table
	int pc2[48] = {
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
	};
	// 1. Compressing the key using the PC1 table
	string perm_key = "";
	for (int i = 0; i < 56; i++) {
		perm_key += key[pc1[i] - 1];
	}
	// 2. Dividing the key into two equal halves
	string left = perm_key.substr(0, 28);
	string right = perm_key.substr(28, 28);
	for (int i = 0; i < 16; i++) {
		// 3.1. For rounds 1, 2, 9, 16 the key_chunks
		// are shifted by one.
		if (i == 0 || i == 1 || i == 8 || i == 15) {
			left = shift_left_once(left);
			right = shift_left_once(right);
		}
		// 3.2. For other rounds, the key_chunks
		// are shifted by two
		else {
			left = shift_left_twice(left);
			right = shift_left_twice(right);
		}
		// Combining the two chunks
		string combined_key = left + right;
		string round_key = "";
		// Finally, using the PC2 table to transpose the key bits
		for (int i = 0; i < 48; i++) {
			round_key += combined_key[pc2[i] - 1];
		}
		round_keys[i] = round_key;
	}
	
	
}




string convertDecimalToBinary(int decimal)
{
	string binary;
	while (decimal != 0) {
		binary = (decimal % 2 == 0 ? "0" : "1") + binary;
		decimal = decimal / 2;
	}
	while (binary.length() < 4) {
		binary = "0" + binary;
	}
	return binary;
};

int convertBinaryToDecimal(string binary)
{
	int decimal = 0;
	int counter = 0;
	int size = binary.length();
	for (int i = size - 1; i >= 0; i--)
	{
		if (binary[i] == '1') {
			decimal += int(pow(2, counter));
		}
		counter++;
	}
	return decimal;
};

string Xor(string a, string b) {
	string result = "";
	int size = b.size();
	for (int i = 0; i < size; i++) {
		if (a[i] != b[i]) {
			result += "1";
		}
		else {
			result += "0";
		}
	}
	return result;
};


// Implementing the algorithm
string DES(string s) {
	// The initial permutation table 
	int initial_permutation[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
	};
	// The expansion table
	int expansion_table[48] = {
	32,1,2,3,4,5,4,5,
	6,7,8,9,8,9,10,11,
	12,13,12,13,14,15,16,17,
	16,17,18,19,20,21,20,21,
	22,23,24,25,24,25,26,27,
	28,29,28,29,30,31,32,1
	};
	// The substitution boxes. The should contain values
	// from 0 to 15 in any order.
	int substition_boxes[8][4][16] =
	{ {
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	},
	{
		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
	},
	{
		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
	},
	{
		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
	},
	{
		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
	},
	{
		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
	},
	{
		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
	},
	{
		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	} };
	// The permutation table
	int permutation_tab[32] = {
	16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25
	};
	// The inverse permutation table
	int inverse_permutation[64] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
	};
	//1. Applying the initial permutation
	string perm = "";
	for (int i = 0; i < 64; i++) {
		perm += s[initial_permutation[i] - 1];
	}
	// 2. Dividing the result into two equal halves 
	string left = perm.substr(0, 32);
	string right = perm.substr(32, 32);
	// The plain text is encrypted 16 times  
	for (int i = 0; i < 16; i++) {
		string right_expanded = "";
		// 3.1. The right half of the plain text is expanded
		for (int i = 0; i < 48; i++) {
			right_expanded += right[expansion_table[i] - 1];
		};  // 3.3. The result is xored with a key
		string xored = Xor(round_keys[i], right_expanded);
		string res = "";
		// 3.4. The result is divided into 8 equal parts and passed 
		// through 8 substitution boxes. After passing through a 
		// substituion box, each box is reduces from 6 to 4 bits.
		for (int i = 0; i < 8; i++) {
			// Finding row and column indices to lookup the
			// substituition box
			string row1 = xored.substr(i * 6, 1) + xored.substr(i * 6 + 5, 1);
			int row = convertBinaryToDecimal(row1);
			string col1 = xored.substr(i * 6 + 1, 1) + xored.substr(i * 6 + 2, 1) + xored.substr(i * 6 + 3, 1) + xored.substr(i * 6 + 4, 1);;
			int col = convertBinaryToDecimal(col1);
			int val = substition_boxes[i][row][col];
			res += convertDecimalToBinary(val);
		}
		// 3.5. Another permutation is applied
		string perm2 = "";
		for (int i = 0; i < 32; i++) {
			perm2 += res[permutation_tab[i] - 1];
		}
		// 3.6. The result is xored with the left half
		xored = Xor(perm2, left);
		// 3.7. The left and the right parts of the plain text are swapped 
		left = xored;
		if (i < 15) {
			string temp = right;
			right = xored;
			left = temp;
		}
	}
	// 4. The halves of the plain text are applied
	string combined_text = left + right;
	string ciphertext = "";
	// The inverse of the initial permuttaion is applied
	for (int i = 0; i < 64; i++) {
		ciphertext += combined_text[inverse_permutation[i] - 1];
	}
	//And we finally get the cipher text
	return ciphertext;
};

string strToBinary(string s)
{
	string bin = "";
	for (int i = 0; i <= s.length(); i++)
	{
		// convert each char to 
		// ASCII value 
		int val = int(s[i]);

		// Convert ASCII value to binary 
		while (val > 0)
		{
			(val % 2) ? bin.push_back('1') :
				bin.push_back('0');
			val /= 2;
		}
	}
	reverse(bin.begin(), bin.end());
	return bin;
}

bool checkValid(string str) {
	int count = 0;
	bool isBin = true;
	for (int i = 0; i < str.size(); i++) {
		if (str.at(i) != '0' && str.at(i) != '1') {
			isBin = false;
		}
			
	}
	return isBin;
}



int main() {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

	MoveWindow(console, r.left, r.top, 900, 620, TRUE); // 900 width, 620 height
	cout << "\n\n\t\t=========================================================================================" << endl;
	cout << "\t\t|\t\t\t\t\t\t\t\t\t\t\t|" << endl;
	cout << "\t\t|\t\t\t\t\t\t\t\t\t\t\t|" << endl;
	cout << "\t\t|\t\t\t    WELLCOME TO DES ENCRYPT & DECYRPT       \t\t\t|" << endl;
	cout << "\t\t|\t\t\t\t\t\t\t\t\t\t\t|" << endl;
	cout << "\t\t|\tMode of operation: ECB     No Initialization Vector	  No Padding Scheme \t|" << endl;
	cout << "\t\t|\t\t\t\t\t\t\t\t\t\t\t|" << endl;
	cout << "\t\t|\t\t\t   Still working on this project ...   \t\t\t\t|" << endl;
	cout << "\t\t|\t\t\t\t\t\t\t\t\t@2020 - 1712534\t|" << endl;
	cout << "\t\t=========================================================================================\n\n\n" << endl;
	cout << "\nThis project did not have padding scheme so plain text or cipher text with a length greater than 64 bits will cause this program to crash!!!\n" << endl;
	vector<string> sub_str_e;
	vector<string> sub_str_d;
	/*string input_text;
	string encodedText;
	cout << "Input plain text: ";
	getline(cin, input_text);
	encodedText = strToBinary(input_text);
	cout << encodedText << endl;*/
	
	//string key = "1010101010111011000010010001100000100111001101101100110011011110";
	string key;
	string input;
	do {
		cout << "Input your key (in binary & 64 bits): ";
		cin >> key;
		if (key.length() != 64 || checkValid(key) != 1)
			cout << "Wrong input format!" << endl;
	} while (key.length() != 64 || checkValid(key) != 1);
	generate_keys(key);
		do {
			cout << "\nChoose your method Encrypt[E] || Decrypt[D]: ";
			cin >> input;
			if (input == "E" || input == "e") {
				do {
					cout << "Input your plain text (in binary & 64 bits): ";
					cin >> pt;
					
					int remain = pt.size() % 64;
					int block = int(pt.size() / 64);
					cout << "Block: " << block << " remain: " << remain << endl;
					for (int i = 0; i < block; i++) {
						sub_str_e.push_back(pt.substr(i * 64, 64));
					}
					if (remain != 0) {
						sub_str_e.push_back(pt.substr(block * 64, remain));
					}
					
					cout << "Vector size: " << sub_str_e.size() << endl;
					cout << "Cipher text: " << endl;

					for (size_t i = 0; i < sub_str_e.size(); i++) {
						//cout << sub_str_e[i] << " ";
						cout << DES(sub_str_e[i]) << " ";
					};
				} while (checkValid(pt) != 1);
				cout << "\n======================================================================";
			}
			if (input == "D" || input == "d") {
				cout << "Input your cipher text (in binary & 64 bits): ";
				cin >> ct;
				int remain = ct.size() % 64;
				int block = int(ct.size() / 64);
				cout << "Block: " << block << " remain: " << remain << endl;
				for (int i = 0; i < block; i++) {
					sub_str_d.push_back(ct.substr(i * 64, 64));
				}
				cout << "Vector size: " << sub_str_d.size() << endl;
				cout << "Decrypted text: " << endl;
				// Reversing the round_keys array for decryption
				int i = 15;
				int j = 0;
				while (i > j)
				{
					string temp = round_keys[i];
					round_keys[i] = round_keys[j];
					round_keys[j] = temp;
					i--;
					j++;
				}
				for (size_t k = 0; k < sub_str_d.size(); k++) {
					cout << DES(sub_str_d[k]) << " ";
				};
				cout << "\n======================================================================";
			}

		} while (input != "E" && input != "D");
	
	
	//do {
	//	cout << "Input your plain text (in binary & 64 bits): ";
	//	cin >> pt;
	//	int remain = pt.size() % 64;
	//	int block = int(pt.size() / 64);
	//	cout << "Block: " << block << " remain: " << remain << endl;
	//	for (int i = 0; i < block; i++) {
	//		sub_str.push_back(pt.substr(i * 64, 64));
	//	}
	//	cout << "Plain text vector: " << endl;
	//	for (string pt_bin : sub_str) {
	//		cout << pt_bin << endl;
	//	}
	//	cout << "Vector size: " << sub_str.size() << endl;
	//	cout << "Cipher text vector: " << endl;

	//	for (size_t i = 0; i < sub_str.size(); i++) {
	//		/*string temp = "";
	//		temp = sub_str[i];
	//		temp = DES(sub_str[i]);
	//		cout << "CT_" << sub_str[i] << endl;
	//		cout << "TEMP_" << temp << endl;*/
	//		cout << "CT_" << DES(sub_str[i]) << endl;
	//	};
	//} while (pt.length() != 64 || checkValid(pt) != 1);
	

	//string sub_str[MAX_SUB_STR];
	
	//while (1) {
	//	// A 64 bit key
	//	string key = "1010101010111011000010010001100000100111001101101100110011011101";
	//	// A block of plain text of 64 bits
	//	do {
	//		cout << "Input your plain text (in binary): ";
	//		cin >> pt;
	//		if (pt.length() != 64 || checkValid(pt) != 1)
	//			cout << "Wrong input format!" << endl;
	//	} while (pt.length() != 64 || checkValid(pt) != 1);

	//	string apt = pt;
	//	// Calling the function to generate 16 keys
	//	generate_keys(key);
	//	cout << "Plain text: " << pt << endl;
	//	// Applying the algo
	//	string ct = DES();
	//	cout << "Ciphertext: " << ct << endl;
	//	// Reversing the round_keys array for decryption
	//	int i = 15;
	//	int j = 0;
	//	while (i > j)
	//	{
	//		string temp = round_keys[i];
	//		round_keys[i] = round_keys[j];
	//		round_keys[j] = temp;
	//		i--;
	//		j++;
	//	}
	//	pt = ct;
	//	string decrypted = DES();
	//	cout << "Decrypted text:" << decrypted << endl;
	//	// Comapring the initial plain text with the decrypted text
	//	if (decrypted == apt) {
	//		cout << "Plain text encrypted and decrypted successfully." << endl;
	//	}
	//}
	
};