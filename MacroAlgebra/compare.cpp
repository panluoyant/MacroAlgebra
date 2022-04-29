#include "bigNum.hpp"

namespace MA::NumCal {

	int8_t VECTORCALL compare(bigNum& arg1, bigNum& arg2, bool gnoreSymbol)//1代表大于,0代表等于,-1代表小于
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		arg1.check();
		arg2.check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		if ((arg1.isPositive != arg2.isPositive) && !gnoreSymbol)
			return (arg1.isPositive ? 1 : -1);
		else
		{
			if (arg1.indexPart > arg2.indexPart)
				return 1;
			else if (arg1.indexPart < arg2.indexPart)
				return -1;
			else
			{
				for (size_t i = 0; i < (arg1.mantissaPart.size() < arg2.mantissaPart.size() ? arg1.mantissaPart.size() : arg2.mantissaPart.size()); i++)
					if (arg1.mantissaPart[i] > arg2.mantissaPart[i])
						return 1;
					else if (arg1.mantissaPart[i] < arg2.mantissaPart[i])
						return -1;
				if (arg1.mantissaPart.size() == arg2.mantissaPart.size())//比较完后都相等？
					return 0;
				else
					return (arg1.mantissaPart.size() > arg2.mantissaPart.size() ? 1 : -1);
			}
		}
	}

	int8_t VECTORCALL compare(bigNum&& arg1, bigNum& arg2, bool gnoreSymbol)
	{
		return compare(arg1, arg2, gnoreSymbol);
	}

	int8_t VECTORCALL compare(bigNum& arg1, bigNum&& arg2, bool gnoreSymbol)
	{
		return compare(arg1, arg2, gnoreSymbol);
	}

	int8_t VECTORCALL compare(bigNum&& arg1, bigNum&& arg2, bool gnoreSymbol)
	{
		return compare(arg1, arg2, gnoreSymbol);
	}

	bool VECTORCALL operator ==(bigNum& arg1, bigNum& arg2)
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		arg1.check();
		arg2.check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		return  arg1.isPositive == arg2.isPositive && arg1.indexPart == arg2.indexPart && arg1.mantissaPart == arg2.mantissaPart;
	}

	bool VECTORCALL operator ==(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 == arg2;
	}

	bool VECTORCALL operator ==(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 == arg2;
	}

	bool VECTORCALL operator ==(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 == arg2;
	}

	bool VECTORCALL operator !=(bigNum& arg1, bigNum& arg2)
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		arg1.check();
		arg2.check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		return arg1.isPositive != arg2.isPositive || arg1.indexPart != arg2.indexPart || arg1.mantissaPart != arg2.mantissaPart;
	}

	bool VECTORCALL operator !=(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 != arg2;
	}

	bool VECTORCALL operator !=(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 != arg2;
	}

	bool VECTORCALL operator !=(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 != arg2;
	}

	bool VECTORCALL operator >(bigNum& arg1, bigNum& arg2)
	{
		return compare(arg1, arg2) == 1;
	}

	bool VECTORCALL operator >(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 > arg2;
	}

	bool VECTORCALL operator >(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 > arg2;
	}

	bool VECTORCALL operator >(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 > arg2;
	}

	bool VECTORCALL operator >=(bigNum& arg1, bigNum& arg2)
	{
		return compare(arg1, arg2) >= 0;
	}

	bool VECTORCALL operator >=(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 >= arg2;
	}

	bool VECTORCALL operator >=(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 >= arg2;
	}

	bool VECTORCALL operator >=(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 >= arg2;
	}

	bool VECTORCALL operator <(bigNum& arg1, bigNum& arg2)
	{
		return compare(arg1, arg2) == -1;
	}

	bool VECTORCALL operator <(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 < arg2;
	}

	bool VECTORCALL operator <(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 < arg2;
	}

	bool VECTORCALL operator <(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 < arg2;
	}

	bool VECTORCALL operator <=(bigNum& arg1, bigNum& arg2)
	{
		return compare(arg1, arg2) <= 0;
	}

	bool VECTORCALL operator <=(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 <= arg2;
	}

	bool VECTORCALL operator <=(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 <= arg2;
	}

	bool VECTORCALL operator <=(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 <= arg2;
	}

}
