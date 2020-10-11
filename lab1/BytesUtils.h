#pragma once

class Bytes;

class BytesUtils {
public:
	static Bytes xor_together(const Bytes & text, const Bytes & key);
};
