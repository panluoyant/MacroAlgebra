#pragma once
#define UPPER_LIMIT 1000000000LL
//#define UPPER_LIMIT_INT 1000000000

#include <iostream>
#include <iomanip>
#include <string>
#include <strstream>
#include <mutex>
#include <ipp.h>
#include "basic.hpp"

#ifndef NA_API
#define NA_API __declspec(dllexport)
#endif

#define OPTIMIZATION_JUDGMENT_PROHIBITION  /*减少冗余判断*/
#define PROHIBIT_REDUNDANCY_CHECK  /*减少冗余检查*/

template<typename _Ty>
inline std::string VECTORCALL eleToStr(_Ty arg)
{
	std::strstream str;
	std::string res;
	str << arg;
	str >> res;
	return res;
}

inline void VECTORCALL numToStr(char* c, int64_t arg) //数字转换
{
//#pragma unroll(9)
	for (uint8_t i = 0ui8; i < 9ui8; i++) {
		c[8ui8 - i] = (arg % 10) + '0';
		arg /= 10;
	}
}

inline std::string VECTORCALL numToStr(int64_t arg) //数字转换
{
	std::string res("000000000");
	for (size_t i = 0; i < 9; i++) {
		res[8ull - i] = (arg % 10) + '0';
		arg /= 10;
	}
	return res;
}

namespace MA::NumCal {

	class NA_API bigNum {
	public:
		bigNum() noexcept;//构造
		FASTCALL bigNum(int64_t Integer) noexcept;
		bigNum(std::string& str);
		bigNum* copy();
		void check() noexcept;
		void VECTORCALL carry(bool alwaysCheck = true, size_t end = 0, size_t maxbit = LLONG_MAX, bool checked = true) noexcept;// alwaysCheck代表了是否处理遇到非进位就停止；
		void borrow() noexcept;
		void VECTORCALL retentionAccuracy(size_t length, bool isDecPart = true, bool Rounding = true) noexcept;
		int64_t VECTORCALL getEffectiveDigitalAccuracy(bool isIntPart = false) noexcept;//获取有效数位
		double toFloat() noexcept;
		int64_t toInt() noexcept;
		std::string toString();
		void swap(bigNum& arg);
		void setVal(std::string arg);
		void set_zero() noexcept;
		void VECTORCALL set_float(double f) noexcept;

	public:
		bool isPositive;
		std::vector<int64_t> mantissaPart;//0.[0],[1],[2]...
		int64_t indexPart;
	};

	//compare
	NA_API int8_t VECTORCALL compare(bigNum& arg1, bigNum& arg2, bool ignoreSymbol = false/*是否判断符号*/);//1代表大于,0代表等于,-1代表小于
	NA_API int8_t VECTORCALL compare(bigNum&& arg1, bigNum& arg2, bool ignoreSymbol = false);
	NA_API int8_t VECTORCALL compare(bigNum& arg1, bigNum&& arg2, bool ignoreSymbol = false);
	NA_API int8_t VECTORCALL compare(bigNum&& arg1, bigNum&& arg2, bool ignoreSymbol = false);

	NA_API bool VECTORCALL operator ==(bigNum& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator ==(bigNum&& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator ==(bigNum& arg1, bigNum&& arg2);
	NA_API bool VECTORCALL operator ==(bigNum&& arg1, bigNum&& arg2);

	NA_API bool VECTORCALL operator !=(bigNum& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator !=(bigNum&& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator !=(bigNum& arg1, bigNum&& arg2);
	NA_API bool VECTORCALL operator !=(bigNum&& arg1, bigNum&& arg2);

	NA_API bool VECTORCALL operator >(bigNum& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator >(bigNum&& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator >(bigNum& arg1, bigNum&& arg2);
	NA_API bool VECTORCALL operator >(bigNum&& arg1, bigNum&& arg2);

	NA_API bool VECTORCALL operator >=(bigNum& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator >=(bigNum&& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator >=(bigNum& arg1, bigNum&& arg2);
	NA_API bool VECTORCALL operator >=(bigNum&& arg1, bigNum&& arg2);

	NA_API bool VECTORCALL operator <(bigNum& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator <(bigNum&& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator <(bigNum& arg1, bigNum&& arg2);
	NA_API bool VECTORCALL operator <(bigNum&& arg1, bigNum&& arg2);

	NA_API bool VECTORCALL operator <=(bigNum& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator <=(bigNum&& arg1, bigNum& arg2);
	NA_API bool VECTORCALL operator <=(bigNum& arg1, bigNum&& arg2);
	NA_API bool VECTORCALL operator <=(bigNum&& arg1, bigNum&& arg2);

	//adder
	NA_API void VECTORCALL adder(bigNum& result, bigNum& arg1, bigNum& arg2, bool isMinus = false/*是否为减*/, bool allowcarry = true);

	NA_API bigNum VECTORCALL operator +(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator +(bigNum&& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator +(bigNum& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator +(bigNum&& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator +=(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator +=(bigNum& arg1, bigNum&& arg2);

	NA_API bigNum VECTORCALL operator -(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator -(bigNum&& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator -(bigNum& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator -(bigNum&& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator -=(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator -=(bigNum& arg1, bigNum&& arg2);

	//multiply
	NA_API void VECTORCALL multiply(bigNum& result, bigNum& arg1, bigNum& arg2);
	NA_API void VECTORCALL multiply(bigNum& result, bigNum&& arg1, bigNum& arg2);
	NA_API void VECTORCALL multiply(bigNum& result, bigNum& arg1, bigNum&& arg2);
	NA_API void VECTORCALL multiply(bigNum& result, bigNum&& arg1, bigNum&& arg2);

	NA_API void VECTORCALL multiply_fft(bigNum& result, bigNum& arg1, bigNum& arg2);
	NA_API void VECTORCALL multiply_fft(bigNum& result, bigNum&& arg1, bigNum& arg2);
	NA_API void VECTORCALL multiply_fft(bigNum& result, bigNum& arg1, bigNum&& arg2);
	NA_API void VECTORCALL multiply_fft(bigNum& result, bigNum&& arg1, bigNum&& arg2);

	NA_API bigNum VECTORCALL operator *(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator *(bigNum&& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator *(bigNum& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator *(bigNum&& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator *=(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator *=(bigNum& arg1, bigNum&& arg2);

	//div
	NA_API void VECTORCALL divide(bigNum& result, bigNum& arg1, bigNum& arg2, uint64_t length = 0, bool Rounding = true);
	NA_API void VECTORCALL divide(bigNum& result, bigNum&& arg1, bigNum& arg2, uint64_t length = 0);
	NA_API void VECTORCALL divide(bigNum& result, bigNum& arg1, bigNum&& arg2, uint64_t length = 0);
	NA_API void VECTORCALL divide(bigNum& result, bigNum&& arg1, bigNum&& arg2, uint64_t length = 0);

	NA_API bigNum VECTORCALL operator /(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator /(bigNum&& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator /(bigNum& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator /(bigNum&& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator /=(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator /=(bigNum& arg1, bigNum&& arg2);

	//mod
	NA_API void VECTORCALL mod(bigNum& result, bigNum& arg1, bigNum& arg2, uint64_t length = 0);
	NA_API void VECTORCALL mod(bigNum& result, bigNum&& arg1, bigNum& arg2, uint64_t length = 0);
	NA_API void VECTORCALL mod(bigNum& result, bigNum& arg1, bigNum&& arg2, uint64_t length = 0);
	NA_API void VECTORCALL mod(bigNum& result, bigNum&& arg1, bigNum&& arg2, uint64_t length = 0);

	NA_API bigNum VECTORCALL operator %(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator %(bigNum&& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator %(bigNum& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator %(bigNum&& arg1, bigNum&& arg2);
	NA_API bigNum VECTORCALL operator %=(bigNum& arg1, bigNum& arg2);
	NA_API bigNum VECTORCALL operator %=(bigNum& arg1, bigNum&& arg2);

	//pow
	NA_API void VECTORCALL square(bigNum& result, bigNum& arg);//include n ,fft, toom
	NA_API void VECTORCALL pow(bigNum& result, bigNum& arg1, int64_t arg2, uint64_t length = 0);
	//NA_API void VECTORCALL pow(bigNum& result, bigNum& arg1, bigNum& arg2, uint64_t length = 0);
	NA_API void VECTORCALL pow10(bigNum& result, int64_t arg2);
	NA_API bigNum VECTORCALL operator ^(bigNum& arg1, int64_t arg2);
	NA_API bigNum VECTORCALL operator ^(bigNum&& arg1, int64_t arg2);
	NA_API bigNum VECTORCALL operator ^=(bigNum& arg1, int64_t arg2);

	NA_API void VECTORCALL extract_newton(bigNum& result, bigNum& arg1, int64_t arg2, uint64_t length = 0);
	//......

	//重载
	NA_API std::istream& VECTORCALL operator>>(std::istream& is, bigNum& arg);
	NA_API std::ostream& VECTORCALL operator<<(std::ostream& os, bigNum& arg);

	std::vector<Ipp64fc> VECTORCALL hnTodouble9fc(bigNum& hn);

	inline int64_t VECTORCALL divideFloor(const int64_t& temp1, const int64_t& temp2)//下取整
	{
		if (!temp2)
			throw std::invalid_argument("Divisor cannot be zero!");
		if (temp1 % temp2 == 0)
			return temp1 / temp2;
		if (temp1 / temp2 > 0)
			return temp1 / temp2;
		else
			return temp1 / temp2 - 1;
	}

	inline int64_t VECTORCALL divideCeil(const int64_t& temp1, const int64_t& temp2)//上取整
	{
		if (!temp2)
			throw std::invalid_argument("Divisor cannot be zero!");
		if (temp1 % temp2 == 0)
			return temp1 / temp2;
		if (temp1 / temp2 < 0)
			return temp1 / temp2;
		else
			return temp1 / temp2 + 1;
	}

	//inline bigNum gcd(bigNum a, bigNum b)
	//{
	//	bigNum ans = 1;
	//	while ((a % 2).mantissaPart.size() && (b % 2).mantissaPart.size()) {
	//		a /= 2;
	//		b /= 2;
	//		ans = ans * 2;
	//	}
	//	while (a != b) {
	//		if (a > b)
	//			a -= b;
	//		else
	//			b -= a;
	//	}
	//	return ans * a;
	//}

	inline int64_t hnpow(int64_t a, int64_t b) 
	{
		if (!b)
			return 1ll;
		int64_t result = 1;
		while (b) {
			if (b & 1)
				result *= a;
			b /= 2;
			if (b)
				a *= a;
		}
		return result;

	}
}


