#include "bigNum.hpp"

namespace MA::NumCal {

	extern bigNum one;
	extern bigNum mone;
	extern bigNum two;

	void VECTORCALL divide(bigNum& result, bigNum& arg1, bigNum& arg2, uint64_t length, bool Rounding)
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		arg1.check();
		arg2.check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		bool positiveres = arg1.isPositive == arg2.isPositive;
		if (!arg2.mantissaPart.size())//arg2==0
			throw std::invalid_argument("Divisor cannot be zero!");
#ifndef OPTIMIZATION_JUDGMENT_PROHIBITION
		if (!arg1.mantissaPart.size()) { //arg1==0
			result.set_zero();
			return;
		}
		else if (arg2 == one) {
			resutl = arg1;
			return;
		}
		else if (arg2 == mone) {
			result = arg1;
			result.isPositive = !arg1.isPositive;
			return;
		}
#endif // !OPTIMIZATION_JUDGMENT_PROHIBITION
		result.mantissaPart.clear();
		if (arg2.mantissaPart.size() == 1) {
			result = arg1;
			result.indexPart = arg1.indexPart - arg2.indexPart + 1;
			int64_t recresult = 0, argf = arg2.mantissaPart.back();
			if (arg1.mantissaPart.size() < arg1.indexPart + length + 1)
				result.mantissaPart.resize(arg1.indexPart + length + 1, 0);
			for (int64_t i = 0; i < arg1.indexPart + length + 1; i++) {
				result.mantissaPart[i] += recresult * UPPER_LIMIT;
				recresult = result.mantissaPart[i] % argf;
				result.mantissaPart[i] /= argf;
			}
		}
		else if (arg1.mantissaPart == arg2.mantissaPart) {
			result.mantissaPart.push_back(1);
			result.indexPart = arg1.indexPart - arg2.indexPart + 1;
		}
		else {
			result.indexPart = 1 - arg2.indexPart;
			result.mantissaPart.push_back(UPPER_LIMIT / ((arg2.mantissaPart[0] + 1) == 1 ? 2 : (arg2.mantissaPart[0] + 1)));
			//改成for循环
			bigNum resultold = 0;
			while (resultold != result) {
				resultold = result;
				result *= (arg2.indexPart ? (two - result * arg2) : (two + result * arg2));
				result.retentionAccuracy(length + 27 + 9 * arg1.indexPart);
			}//方便优化
			if (arg1 != one)
				result *= arg1;
		}
		result.isPositive = positiveres;
		result.retentionAccuracy(length, true, Rounding);
	}

	void VECTORCALL divide(bigNum& result, bigNum&& arg1, bigNum& arg2, uint64_t length)
	{
		divide(result, arg1, arg2, length);
	}

	void VECTORCALL divide(bigNum& result, bigNum& arg1, bigNum&& arg2, uint64_t length)
	{
		divide(result, arg1, arg2, length);
	}

	void VECTORCALL divide(bigNum& result, bigNum&& arg1, bigNum&& arg2, uint64_t length)
	{
		divide(result, arg1, arg2, length);
	}

	bigNum VECTORCALL operator /(bigNum& arg1, bigNum& arg2)
	{
		bigNum temp;
		try {
			divide(temp, arg1, arg2);
			return temp;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			return 0ll;
		}
	}

	bigNum VECTORCALL operator /(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 / arg2;
	}

	bigNum VECTORCALL operator /(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 / arg2;
	}

	bigNum VECTORCALL operator /(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 / arg2;
	}

	bigNum VECTORCALL operator /=(bigNum& arg1, bigNum& arg2)
	{
		return arg1 = arg1 / arg2;
	}

	bigNum VECTORCALL operator /=(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 = arg1 / arg2;
	}
}