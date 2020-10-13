#pragma once

#include <string>

class Bytes;

enum class AesOptions {
	ENCRYPT,
	DECRYPT,
};

class BytesUtils {
public:
	static Bytes xor_together(const Bytes & text, const Bytes & key);
	static Bytes find_key(const Bytes & text, size_t key_length);
	
	static int score(const Bytes & bytes);
	static Bytes skip_n(const Bytes & bytes, int start, size_t skip);
	static size_t compute_key_length(const Bytes & bytes, int max_key_length = 50);
	
	static Bytes encrypt_aes_block(const Bytes & data, const Bytes & key);
	static Bytes decrypt_aes_block(const Bytes & data, const Bytes & key);
	static Bytes crypt_aes_block(const Bytes & data, const Bytes & key, AesOptions opt);
	static Bytes pad(const Bytes & data);
	static Bytes unpad(const Bytes & data);

	static Bytes cut_to_block(const Bytes & input, size_t index);
	static Bytes swap_block(const Bytes & input, size_t block_a, size_t block_b);

	static Bytes encrypt_aes_ecb(const Bytes & data, const Bytes & key);
	static Bytes decrypt_aes_ecb(const Bytes & data, const Bytes & key);

	static Bytes welcome(std::string name);
	static Bytes welcome(const Bytes & input);

	static Bytes hide_secret(const Bytes & input);
};
