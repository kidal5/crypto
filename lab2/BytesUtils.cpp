#include "BytesUtils.h"
#include "Bytes.h"
#include "aes.hpp"

#include <assert.h>
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

Bytes BytesUtils::skip_n(const Bytes & bytes, int start, size_t skip)
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
	size_t elem1 = std::get<0>(iocs[0]);
	size_t elem2 = std::get<0>(iocs[1]);

	return gcd(elem1, elem2);
}

Bytes BytesUtils::encrypt_aes_block(const Bytes & data, const Bytes & key)
{
	return crypt_aes_block(data, key, AesOptions::ENCRYPT);
}

Bytes BytesUtils::decrypt_aes_block(const Bytes & data, const Bytes & key)
{
	return crypt_aes_block(data, key, AesOptions::DECRYPT);
}

Bytes BytesUtils::crypt_aes_block(const Bytes & data, const Bytes & key, AesOptions opt)
{
	if (key.size() != 16) throw std::exception("Key size is not 16 bytes");

	uint8_t dd[16];
	uint8_t kk[16];

	data.get_data(dd, 16);
	key.get_data(kk, 16);

	AES_ctx ctx;
	AES_init_ctx(&ctx, kk);
	if (opt == AesOptions::ENCRYPT) {
		AES_ECB_encrypt(&ctx, dd);
	}
	else {
		AES_ECB_decrypt(&ctx, dd);
	}

	return Bytes::from_uint8(dd, 16);
}

Bytes BytesUtils::pad(const Bytes & data)
{
	//pads data to be multiple of 16, aka PKCS#7 padding
	Bytes b = data;
	size_t to_pad = 16 - (data.size() % 16);
	for (size_t i = 0; i < to_pad; i++)
		b.push_back(to_pad);

	assert((b.size() % 16) == 0);
	return b;
}

Bytes BytesUtils::unpad(const Bytes & data)
{
	size_t padded = data[data.size() - 1];
	//first check if padding is correct
	for (size_t i = data.size() - 1; i > data.size() - 1 - padded; i--)
		assert(data[i] == padded);

	Bytes out = data;
	out.remove_last_n(padded);
	return out;
}

Bytes BytesUtils::cut_to_block(const Bytes & input, size_t index)
{
	Bytes b;
	for (size_t i = 0; i < 16; i++)
		b.push_back(input[index * 16 + i]);

	return b;
}

Bytes BytesUtils::swap_block(const Bytes & input, size_t block_a, size_t block_b)
{
	Bytes b = input;

	for (size_t i = 0; i < 16; i++)
	{
		int index_a = block_a * 16 + i;
		int index_b = block_b * 16 + i;

		b[index_a] = input[index_b];
		b[index_b] = input[index_a];
	}
	return b;
}

Bytes BytesUtils::encrypt_aes_ecb(const Bytes & data, const Bytes & key)
{
	Bytes padded = BytesUtils::pad(data);
	Bytes output;

	int num_blocks = padded.size() / 16;
	for (size_t i = 0; i < num_blocks; i++) {
		Bytes block = BytesUtils::cut_to_block(padded, i);
		output = output + BytesUtils::encrypt_aes_block(block, key);
	}
	return output;
}

Bytes BytesUtils::decrypt_aes_ecb(const Bytes & data, const Bytes & key)
{
	Bytes output;

	int num_blocks = data.size() / 16;
	for (size_t i = 0; i < num_blocks; i++) {
		Bytes block = BytesUtils::cut_to_block(data, i);
		output = output + BytesUtils::decrypt_aes_block(block, key);
	}

	return BytesUtils::unpad(output);
}

Bytes BytesUtils::welcome(std::string name)
{
	return welcome(Bytes::from_text_string(name));
}

Bytes BytesUtils::welcome(const Bytes & data)
{
	Bytes input = Bytes::from_text_string("Your name is ") + data + Bytes::from_text_string(" and you are a user");
	Bytes key = Bytes::from_text_string("RIDERSONTHESTORM");
	return encrypt_aes_ecb(input, key);
}

Bytes BytesUtils::hide_secret(const Bytes & in)
{
	Bytes secret = Bytes::from_text_string("this should stay secret");
	Bytes key = Bytes::from_text_string("COOL T MAGIC KEY");
	Bytes input = in + secret;

	return encrypt_aes_ecb(input, key);
}
