
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

void task_eight() {
	//question 1:
	//there are lot of lines, that has the same encryption / 3130c68457ca3d0783e5a5beec8965b1ce2b204963fd41209775362f9db531ef
	//i would suppose, that this will be text of chorus when decrypted

	//question 2:
	//that last 'alone' block is padding
	//its plaintext should be 16 coppies of byte 16 / aka 10101010101010101010101010101010 in hex
	auto input = Bytes::from_hex_file("text1.hex");
	input = BytesUtils::swap_block(input, 0, 4);
	input = BytesUtils::swap_block(input, 1, 5);
	auto key = Bytes::from_text_string("TLKNGBTMYGNRTION");

	auto plaintext = BytesUtils::decrypt_aes_ecb(input, key);
	plaintext.print_text_first_line("Solution to excersise 8 is: ");
}

void task_nine() {
	auto jim = BytesUtils::welcome("Jim");
	jim.print_hex("Solution to excersise 9 is: Jim: ", 16);

	//to obtain block with 16x16 i need to write following input
	//13 bytes + 3 random bytes of input + 16 bytes of target input;
	//result will be the second block

	auto sixteen = BytesUtils::cut_to_block(BytesUtils::welcome(Bytes::from_hex_string("aaaaaa10101010101010101010101010101010")), 1);

	//you are an admin - length 16
	auto admin = BytesUtils::cut_to_block(BytesUtils::welcome(Bytes::from_text_string("OOOyou are an admin")), 1);

	//13 bytes + "vladislav trnka" (15) + 4 padding bytes;
	auto name = BytesUtils::welcome(Bytes::from_text_string("Vladislav      and "));
	name.remove_last_n(32);

	auto crafted = name + admin + sixteen;
	crafted.print_hex("Solution to excersise 9 is: Crafted exploit: ", 16);

	//decrypt using 'known' key
	auto uncrafted = BytesUtils::decrypt_aes_ecb(crafted, Bytes::from_text_string("RIDERSONTHESTORM"));
	uncrafted.print_text("Solution to excersise 9 is: Crafted exploit decrypted: ");

	//question 7:
	//This current example can be problem in any computer based system
	//User would get admin privilegies even though he is just normal user.
	//nevertheless, this type of attack can be performed on many more examples
	//in banking - altering amount of money, that has been send
	//in university - changing grade, that has professor given to student
}

void task_ten() {

}

int main()
{
	//task_one();
	//task_two();
	//task_three();
	//task_four();
	//task_five();
	//task_six();
	//task_seven();
	//task_eight();
	//task_nine();
	task_ten();
}
