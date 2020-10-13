
#include "Bytes.h"
#include "BytesUtils.h"

#include <iostream>



void task_one() {
	auto text1 = "I, Vladislav Trnka, understand that cryptography is easy to mess up, and that I will not carelessly combine pieces of cryptographic ciphers to encrypt my ";
	auto text2 = "users' data. I will not write crypto code myself, but defer to high - level libaries written by experts who took the right decisions for me, like NaCL.";

	std::cout << text1 << text2 << std::endl;
}

void task_two() {
	auto text = Bytes::from_text_string("90 miles an hour");
	auto key = Bytes::from_text_string("CROSSTOWNTRAFFIC");

	auto plain_text = BytesUtils::encrypt_aes_block(text, key);
	plain_text.print_hex("Solution to excersise 2 is: ");
}

void task_three() {
	auto text = Bytes::from_hex_string("fad2b9a02d4f9c850f3828751e8d1565");
	auto key = Bytes::from_text_string("VALLEYSOFNEPTUNE");
	
	auto plain_text = BytesUtils::decrypt_aes_block(text, key);
	plain_text.print_text("Solution to excersise 3 is: ");
}

void task_four() {
	auto input = Bytes::from_text_string("hello");
	auto padded = BytesUtils::pad(input);
	padded.print_hex("Solution to excersise 4 is: ");
}

void task_five() {
	auto input = Bytes::from_text_string("hello") + Bytes::from_hex_string("0b0b0b0b0b0b0b0b0b0b0b");
	auto unpadded = BytesUtils::unpad(input);
	unpadded.print_text("Solution to excersise 5 is: ");
}

void task_six() {
	auto text = Bytes::from_text_string("Well, I stand up next to a mountain and I chop it down with the edge of my hand");
	auto key = Bytes::from_text_string("vdchldslghtrturn");

	auto output = BytesUtils::encrypt_aes_ecb(text, key);
	output.print_hex("Solution to excersise 6 is: ", 16);
}

void task_seven() {
	auto text = Bytes::from_hex_string("792c2e2ec4e18e9d3a82f6724cf53848abb28d529a85790923c94b5c5abc34f50929a03550e678949542035cd669d4c66da25e59a5519689b3b4e11a870e7cea");
	auto key = Bytes::from_text_string("If the mountains");

	auto output = BytesUtils::decrypt_aes_ecb(text, key);
	output.print_text("Solution to excersise 7 is: ");
}

int main()
{
	//task_one();
	//task_two();
	//task_three();
	//task_four();
	//task_five();
	//task_six();
	task_seven();
}
