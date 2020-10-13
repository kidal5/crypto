#include "Bytes.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <streambuf>
#include <algorithm>


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

Bytes Bytes::from_uint8(uint8_t * buffer, size_t buflen)
{
	Bytes out;
	out.data.reserve(buflen);
	for (size_t i = 0; i < buflen; i++)
		out.push_back(buffer[i]);
	return out;
}

char Bytes::operator[](size_t index) const
{
	return data[index];
}

char & Bytes::operator[](size_t index)
{
	return data[index];
}

void Bytes::print_hex(std::string prepend, int column_size)
{
	std::cout << prepend;
	if (column_size != -1 && prepend.size() > 0)
		std::cout << std::endl;


	auto newline = [column_size](size_t index) {
		if (index == 0) return false;
		if (column_size == -1) return false;
		return ((index + 1) % column_size) == 0;
	};

	for (size_t i = 0; i < size(); i++)
	{
		std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)(uint8_t)data[i];
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

void Bytes::print_text_first_line(std::string prepend)
{
	std::cout << prepend;
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

void Bytes::push_front(char c)
{
	data.insert(data.begin(), c);
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

void Bytes::get_data(uint8_t * buffer, size_t buflen) const
{
	size_t to_copy = std::min(buflen, size());
	for (size_t i = 0; i < to_copy; i++)
		buffer[i] = data[i];
}

void Bytes::remove_last_n(size_t size)
{
	data.resize(data.size() - size);
}

void Bytes::remove_first_n(size_t size)
{
	data.erase(data.begin());
}

bool operator==(const Bytes & a, const Bytes & b)
{
	if (a.size() != b.size()) return false;
	
	for (size_t i = 0; i < a.size(); i++)
		if (a[i] != b[i]) return false;
	return true;
}

Bytes operator+(const Bytes & left, const Bytes & right)
{
	using vec = std::vector<char>;

	auto concatenate = [](vec & AB, const vec & A, const vec & B) {
		AB.reserve(A.size() + B.size()); // preallocate memory
		AB.insert(AB.end(), A.begin(), A.end());
		AB.insert(AB.end(), B.begin(), B.end());
	};

	Bytes b;
	concatenate(b.data, left.data, right.data);
	return b;
}

Bytes operator*(const Bytes & left, int number)
{
	Bytes b;
	for (size_t i = 0; i < number; i++)
		for (const auto val : left)
			b.push_back(val);
	return b;
}


