#include "Bytes.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <streambuf>


Bytes Bytes::from_hex_string(std::string hex)
{
	auto convert_to_char = [](std::string conv) {
		char num = std::stoi(conv, nullptr, 16);
		return num;
	};

	Bytes output;
	for (size_t i = 0; i < hex.size(); i += 2) {
		if (hex[i] == '\n') i++;
		char one_hex[3] = { hex[i], hex[i + 1], NULL };
		auto converted = convert_to_char(one_hex);
		output.push_back(converted);
	}
	return output;
}

Bytes Bytes::from_hex_file(std::string filename)
{
	std::ifstream t(filename);
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	return Bytes::from_hex_string(str);
}

Bytes Bytes::from_number(char number)
{
	Bytes b;
	b.push_back(number);
	return b;
}

Bytes Bytes::from_numbers(std::vector<char> numbers)
{
	Bytes b;
	b.data = numbers;
	return b;
}

Bytes Bytes::from_text_string(std::string text)
{
	Bytes output;
	for (const auto & c : text)
		output.push_back(c);
	return output;
}

char Bytes::operator[](size_t index) const
{
	return data[index];
}

void Bytes::print_hex(std::string prepend, int column_size)
{
	std::cout << prepend;

	auto newline = [column_size](size_t index) {
		if (index == 0) return false;
		if (column_size == -1) return false;
		return ((index + 1) % column_size) == 0;
	};

	for (size_t i = 0; i < size(); i++)
	{
		std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)data[i];
		if (newline(i)) std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Bytes::print_text(std::string prepend)
{
	std::cout << prepend;

	for (const auto & c : data) std::cout << c;
	std::cout << std::endl;
}

void Bytes::print_text_first_line()
{
	for (const auto & c : data) {
		std::cout << c;
		if (c == '\n') return;
	}
}

size_t Bytes::size() const
{
	return data.size();
}

void Bytes::push_back(char c)
{
	data.push_back(c);
}

std::vector<char>::iterator Bytes::begin()
{
	return data.begin();
}

std::vector<char>::const_iterator Bytes::begin() const
{
	return data.cbegin();
}

std::vector<char>::iterator Bytes::end()
{
	return data.end();
}

std::vector<char>::const_iterator Bytes::end() const
{
	return data.cend();
}

bool operator==(const Bytes & a, const Bytes & b)
{
	if (a.size() != b.size()) return false;
	
	for (size_t i = 0; i < a.size(); i++)
		if (a[i] != b[i]) return false;
	return true;
}
