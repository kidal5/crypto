#include "Bytes.h"
#include "BytesUtils.h"

#include <string>
#include <iostream>
#include <assert.h>


void task_one() {
	auto test_text = Bytes::from_text_string("everything remains raw");
	auto test_key = Bytes::from_text_string("word up");
	auto test_output = Bytes::from_hex_string("121917165901181e01154452101d16061c1700071100");
	auto test_xor = BytesUtils::xor_together(test_text, test_key);
	assert(test_output == test_xor);

	auto task_text = Bytes::from_text_string("the world is yours");
	auto task_key = Bytes::from_text_string("illmatic");
	auto task_xor = BytesUtils::xor_together(test_text, test_key);

	task_xor.print_hex("Solution to task 1 is: ");
	std::cout << std::endl;
}

void task_two() {

	auto ciphertext = Bytes::from_hex_string("404b48484504404b48484504464d4848045d4b");
	auto key = Bytes::from_number('$');
	auto plaintext = BytesUtils::xor_together(ciphertext, key);

	plaintext.print_text("Solution to task 2 is: ");
	std::cout << std::endl;

	//explanation of patterns seen in hex text
	//there can be seen a lot of hex numbers in format 4x or 5x
	//the reason for this is simple, in ascii table all unsigned characters are in consectuive order spanning from hex 61 to 7A
	//that means hex string of valid text would contain lot of numbers in format 6x or 7x
	//and beacuse this simple cipher is just shifting, the consecutive order is kept and pattern is shown
}

void task_three() {
	auto ciphertext = Bytes::from_hex_file("text1.hex");
	auto key = Bytes::from_number('M');
	auto res = BytesUtils::xor_together(ciphertext, key);

	std::cout << "Solution to task 3 is: M" << std::endl;
	res.print_text_first_line();
	std::cout << std::endl;
}

void task_four() {
	auto ciphertext = Bytes::from_hex_file("text1.hex");

	for (int c = 0; c < 256; c++)
	{
		auto key = Bytes::from_number(c);
		auto res = BytesUtils::xor_together(ciphertext, key);

		int score = BytesUtils::score(res);
		if (score < 1000) {
			std::cout << "Solution to task 4 is: " << c << std::endl;
		}
	}
	std::cout << std::endl;
}

void task_five() {
	auto ciphertext = Bytes::from_hex_file("text2.hex");
	auto key = BytesUtils::find_key(ciphertext, 10);
	auto plain_text = BytesUtils::xor_together(ciphertext, key);
	//plain_text.print_text();
	
	key.print_hex("Solution to task 5 is: ");
	plain_text.print_text_first_line();
	std::cout << std::endl;
}

void task_six() {
	auto ciphertext = Bytes::from_hex_file("text3.hex");
	auto key_length = BytesUtils::compute_key_length(ciphertext);
	std::cout << key_length << std::endl;

	auto key = BytesUtils::find_key(ciphertext, key_length);
	auto plain_text = BytesUtils::xor_together(ciphertext, key);
	//plain_text.print_text();

	key.print_hex("Solution to task 6 is: ");
	plain_text.print_text_first_line();
	std::cout << std::endl;


}



int main()
{
	//task_one();
	//task_two();
	//task_three();
	//task_four();
	//task_five();
	task_six();
}
