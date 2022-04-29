#include "bigNum.hpp"

namespace MA::NumCal {

	void VECTORCALL adder(bigNum& result, bigNum& arg1, bigNum& arg2, bool isMinus, bool allowcarry)//bugbugbugbugbugbugbug
	{
		bool oneortwo;//f=1,t=2
		result.set_zero();
#ifndef OPTIMIZATION_JUDGMENT_PROHIBITION
		if (!arg1.mantissa_part().size())
			return arg2;
		else if (!arg2.mantissa_part().size())
			return arg1;
#endif // !OPTIMIZATION_JUDGMENT_PROHIBITION
		if (compare(arg1, arg2, true) != -1) {
			result = arg1;
			oneortwo = true;
		}
		else {
			result = arg2;
			oneortwo = false;
		}
		if (isMinus && !oneortwo)
			result.isPositive = !result.isPositive;
		if ((arg1.isPositive != arg2.isPositive) ^ isMinus) {
			if (result.indexPart - (oneortwo ? arg2 : arg1).indexPart + (oneortwo ? arg2 : arg1).mantissaPart.size() > result.mantissaPart.size())
				result.mantissaPart.resize(result.indexPart - (oneortwo ? arg2 : arg1).indexPart + (oneortwo ? arg2 : arg1).mantissaPart.size(), 0);
			size_t i = result.indexPart - (oneortwo ? arg2 : arg1).indexPart;
#pragma omp simd
			for (size_t t = 0; t < (oneortwo ? arg2 : arg1).mantissaPart.size(); t++) {
				result.mantissaPart[i] -= (oneortwo ? arg2 : arg1).mantissaPart[t];
				i++;
			}
			result.borrow();
		}
		else {
			if (result.indexPart - (oneortwo ? arg2 : arg1).indexPart + (oneortwo ? arg2 : arg1).mantissaPart.size() > result.mantissaPart.size())
				result.mantissaPart.resize(result.indexPart - (oneortwo ? arg2 : arg1).indexPart + (oneortwo ? arg2 : arg1).mantissaPart.size(), 0);
			size_t i = result.indexPart - (oneortwo ? arg2 : arg1).indexPart;
#pragma omp simd
			for (size_t t = 0; t < (oneortwo ? arg2 : arg1).mantissaPart.size(); t++) {
				result.mantissaPart[i] += (oneortwo ? arg2 : arg1).mantissaPart[t];
				i++;
			}
			if (allowcarry)
				result.carry();
		}
	}

	bigNum VECTORCALL operator +(bigNum& arg1, bigNum& arg2)
	{
		bigNum result;
		adder(result, arg1, arg2, false);
		return result;
	}

	bigNum VECTORCALL operator +(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 + arg2;
	}

	bigNum VECTORCALL operator +(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 + arg2;
	}

	bigNum VECTORCALL operator +(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 + arg2;
	}

	bigNum VECTORCALL operator +=(bigNum& arg1, bigNum& arg2)
	{
		return arg1 = arg1 + arg2;
	}

	bigNum VECTORCALL operator +=(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 = arg1 + arg2;
	}

	bigNum VECTORCALL operator -(bigNum& arg1, bigNum& arg2)
	{
		bigNum result;
		adder(result, arg1, arg2, true);
		return result;
	}

	bigNum VECTORCALL operator -(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 - arg2;
	}

	bigNum VECTORCALL operator -(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 - arg2;
	}

	bigNum VECTORCALL operator -(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 - arg2;
	}

	bigNum VECTORCALL operator -=(bigNum& arg1, bigNum& arg2)
	{
		return arg1 - arg2;
	}

	bigNum VECTORCALL operator -=(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 - arg2;
	}

}