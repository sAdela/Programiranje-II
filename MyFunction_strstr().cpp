#include<iostream>
using namespace std;


const char* strstr1(const char* string, const char* substring) {	//I created function that does what strstr() does.												
	const char *Pointer = nullptr;							//	(Find substring in string, and return pointer on place where substring in string starts)
	int lenghtOf_Substring = strlen(substring);
	int lenghtOf_String = strlen(string);
	int counter = 0, counterOf_Correct = 0;


	for (int i = 0; i < lenghtOf_String; i++) {

		if (string[i] == substring[counter++]) {

			counterOf_Correct++;

			if (counter == lenghtOf_Substring) {

				counter = 0;

			}
			if (counterOf_Correct == lenghtOf_Substring) {

				Pointer = &string[i - lenghtOf_Substring + 1];

				break;
			}
		}
		else {
			counter = 0;
			counterOf_Correct = 0;
		}

	}
	return Pointer;
}

void main() {

	//Test it yourselves ;)
	//It would return nullptr if string doesn't contain substring

	//if (strstr1("Insert", "Insert") != nullptr) { cout << strstr1("Insert", "Insert"); }

	system("pause>0");
}