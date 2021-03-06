#include "BytesUtils.h"
#include "Bytes.h"

#include <functional>
#include <algorithm>
#include <iostream>
#include <tuple>

Bytes BytesUtils::xor_together(const Bytes & text, const Bytes & key)
{
	Bytes output;
	for (size_t i = 0; i < text.size(); i++)
	{
		char c = text[i];
		char k = key[i % key.size()];
		output.push_back(c^k);
	}
	return output;
}

Bytes BytesUtils::find_key(const Bytes & text, size_t key_length)
{
	std::vector<char> final_key_temp;

	for (int key_index = 0; key_index < key_length; key_index++)
	{
		bool already_put = false;
		auto ciphertext_inner = BytesUtils::skip_n(text, key_index, key_length);
		for (int c = 0; c < 256; c++)
		{
			auto key = Bytes::from_number(c);
			auto res = BytesUtils::xor_together(ciphertext_inner, key);

			int score = BytesUtils::score(res);

			if (score < 1000) {
				if (already_put) throw std::exception("More keys possible");
				final_key_temp.push_back((char)c);
				already_put = true;
			}
		}
		if (!already_put)
			throw std::exception("Could not find key with given length");

	}

	return Bytes::from_numbers(final_key_temp);
}

int BytesUtils::score(const Bytes & bytes)
{
	//first check if all bytes are in valid english range...
	auto is_char_in_range = [](char c) {
		if (c == '\n') return true;
		if (c >= 128) return false;
		if (c < 32) return false;
		return true;
	};

	for (const auto & byte : bytes) {
		if (!is_char_in_range(byte)) return 1000;
	}

	return 0;
}

Bytes BytesUtils::skip_n(const Bytes & bytes, int start, int skip)
{
	Bytes b;
	for (size_t i = start; i < bytes.size(); i += skip)
		b.push_back(bytes[i]);

	return b;
}

size_t BytesUtils::compute_key_length(const Bytes & bytes, int max_key_length)
{
	std::function<size_t(size_t, size_t)> gcd;
	gcd = [&gcd](size_t a, size_t b) {
		if (b == 0)
			return a;
		return gcd(b, a % b);
	};

	using element = std::tuple<size_t, float>;
	std::vector<element> iocs;

	for (size_t step = 1; step < max_key_length; step++) {
		int match = 0;
		int total = 0;

		for (size_t i = 0; i < bytes.size(); i++)
		{
			for (size_t j = i + step; j < bytes.size(); j += step)
			{
				total += 1;
				if (bytes[i] == bytes[j]) match += 1;
			}

		}
		float ioc = float(match) / float(total);
		iocs.push_back(std::make_tuple(step, ioc));
		//std::cout << step << "\t" << ioc * 100 << "\t" << std::endl;
	}

	std::sort(iocs.begin(), iocs.end(), [](const element & a, const element & b) {
		return std::get<1>(a) > std::get<1>(b);
	});

	//take two topmost element and find gcd.. this should be the keylenth
	int elem1 = std::get<0>(iocs[0]);
	int elem2 = std::get<0>(iocs[1]);

	return gcd(elem1, elem2);
}
