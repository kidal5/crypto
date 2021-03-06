#pragma once

#include <string>
#include <vector>

class Bytes{
public:
	static Bytes from_hex_string(std::string hex);
	static Bytes from_hex_file(std::string filename);
	static Bytes from_number(char number);
	static Bytes from_numbers(std::vector<char> numbers);
	static Bytes from_text_string(std::string text);

	char operator[](size_t index) const;
	friend bool operator== (const Bytes &left, const Bytes &right);

	void print_hex(std::string prepend = "", int column_size = -1);
	void print_text(std::string prepend = "");
	void print_text_first_line();

	size_t size() const;
	void push_back(char c);

	std::vector<char>::iterator begin();
	std::vector<char>::const_iterator begin() const;

	std::vector<char>::iterator end();
	std::vector<char>::const_iterator end() const;


private:
	std::vector<char> data;
};