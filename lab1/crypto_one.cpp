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
}

void task_two() {

	auto ciphertext = Bytes::from_hex_string("404b48484504404b48484504464d4848045d4b");
	auto key = Bytes::from_number('$');
	auto plaintext = BytesUtils::xor_together(ciphertext, key);

	plaintext.print_text("Solution to task 2 is: ");

	//explanation of patterns seen in hex text
	//there can be seen a lot of hex numbers in format 4x or 5x
	//the reason for this is simple, in ascii table all characters are in consectuive order spanning from hex 61 to 7A
	//that means hex string of valid text would contain lot of numbers in format 6x or 7x
	//and beacuse this simple cipher is just shifting, the consecutive order is kept and pattern is shown
}

void task_three() {
	auto ciphertext = Bytes::from_hex_file("text1.hex");
	auto key = Bytes::from_number('M');
	auto res = BytesUtils::xor_together(ciphertext, key);

	std::cout << "Solution to task 3 is: M" << std::endl;
	//res.print_text();
}

int main()
{
	task_one();
	task_two();
	task_three();
}
