#pragma once

class Bytes;

class BytesUtils {
public:
	static Bytes xor_together(const Bytes & text, const Bytes & key);
	static Bytes find_key(const Bytes & text, size_t key_length);
	
	static int score(const Bytes & bytes);
	static Bytes skip_n(const Bytes & bytes, int start, int skip);
	static size_t compute_key_length(const Bytes & bytes, int max_key_length = 50);
};
