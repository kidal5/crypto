#include "BytesUtils.h"

#include "Bytes.h"

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