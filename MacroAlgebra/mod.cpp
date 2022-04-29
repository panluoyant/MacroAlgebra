#include "bigNum.hpp"

namespace MA::NumCal {

	void VECTORCALL mod(bigNum& result, bigNum& arg1, bigNum& arg2, uint64_t length)
	{
		if (arg2.mantissaPart.size() > 1) {
			bigNum temp;
			divide(result, arg1, arg2, length, false);
			if ((arg1.mantissaPart.size() < 5 || arg2.mantissaPart.size() < 5)
				|| (arg1.mantissaPart.size() < 32 && arg2.mantissaPart.size() < 32))
				multiply(temp, result, arg2);
			else
				multiply_fft(temp, result, arg2);
			adder(result, arg1, temp, true);
		}
		else {
			if (!arg2.mantissaPart.size())//arg2==0
				throw std::invalid_argument("Divisor cannot be zero!");
			int64_t UPPL = UPPER_LIMIT % arg2.mantissaPart[0];
			const int64_t UPPLM = UPPL;
			int64_t REI64 = (arg1.indexPart - arg2.indexPart >= arg1.mantissaPart.size() ? 0 : arg1.mantissaPart[arg1.indexPart - arg2.indexPart])% arg2.mantissaPart[0];
			for (int64_t i = arg1.indexPart - arg2.indexPart - 1; i >= 0; i--) {
				REI64 += (i >= arg1.mantissaPart.size() ? 0 : arg1.mantissaPart[i]) % arg2.mantissaPart[0] * UPPL;
				UPPL = UPPL * UPPLM % arg2.mantissaPart[0];
				REI64 %= arg2.mantissaPart[0];
			}
			result.mantissaPart.clear();
			result.mantissaPart.push_back(REI64);
			result.indexPart = arg2.indexPart;
			for (int64_t i = arg1.indexPart - arg2.indexPart + 1; i < arg1.mantissaPart.size(); i++)
				result.mantissaPart.push_back(arg1.mantissaPart[i]);
			result.isPositive = arg1.isPositive;
			result.retentionAccuracy(length);
		}
	}

	void VECTORCALL mod(bigNum& result, bigNum&& arg1, bigNum& arg2, uint64_t length)
	{
		mod(result, arg1, arg2, length);
	}

	void VECTORCALL mod(bigNum& result, bigNum& arg1, bigNum&& arg2, uint64_t length)
	{
		mod(result, arg1, arg2, length);
	}

	void VECTORCALL mod(bigNum& result, bigNum&& arg1, bigNum&& arg2, uint64_t length)
	{
		mod(result, arg1, arg2, length);
	}

	bigNum VECTORCALL operator %(bigNum& arg1, bigNum& arg2)
	{
		bigNum temp;
		mod(temp, arg1, arg2, 0);
		return temp;
	}

	bigNum VECTORCALL operator %(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 % arg2;
	}

	bigNum VECTORCALL operator %(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 % arg2;
	}

	bigNum VECTORCALL operator %(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 % arg2;
	}

	bigNum VECTORCALL operator %=(bigNum& arg1, bigNum& arg2)
	{
		return arg1 = arg1 % arg2;
	}

	bigNum VECTORCALL operator %=(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 = arg1 % arg2;
	}

}