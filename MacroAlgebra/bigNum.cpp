#include "bigNum.hpp"

#include <omp.h>

namespace MA::NumCal {

	bigNum one(1);
	bigNum mone(-1);
	bigNum two(2);

	bigNum::bigNum() noexcept
		:isPositive(true), indexPart(0ll)
	{
		mantissaPart.reserve(8);
	}

	FASTCALL bigNum::bigNum(int64_t Integer) noexcept
		:isPositive(Integer >= 0), indexPart(2ll)
	{
		mantissaPart.reserve(8);
		if (Integer < 0)
			Integer = -Integer;
		mantissaPart.push_back(Integer / UPPER_LIMIT);
		mantissaPart.push_back(Integer % UPPER_LIMIT);
		carry();
	}

	bigNum::bigNum(std::string& arg) 
	{
		try {
			setVal(arg);
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			mantissaPart.clear();//剩下的交给check
		}
		carry();
	}

	bigNum* bigNum::copy()
	{
		return new bigNum(*this);
	}

	std::string bigNum::toString()
	{
		check();
		char tempchar[10];
		tempchar[9] = 0;
		std::string temp;
		if (!isPositive)//正负号
			temp += '-';
		if (mantissaPart.size() == 0)
			temp = "0";
		else if (indexPart <= 0) {
			temp += "0.";
			for (size_t i = 0; i < -indexPart; i++)
				temp += "000000000";
#pragma  block_loop
			for (size_t i = 0; i < mantissaPart.size(); i++) {
				if (i != (mantissaPart.size() - 1)) {
					::numToStr(tempchar, mantissaPart[i]);
					temp += tempchar;
				}
				else {
					std::string c = ::numToStr(mantissaPart[i]);
					while (c.back() == '0')//删除末尾为0
						c.pop_back();
					temp += c;
				}
			}
		}
		else {
			temp += std::to_string(mantissaPart[0]);
			int64_t i;
			for (i = 1; i < indexPart; i++) {
				if (i < mantissaPart.size()){
					::numToStr(tempchar, mantissaPart[i]);
					temp += tempchar;
				}
				else 
					temp += "000000000";
			}
			if (i < mantissaPart.size()) {
				temp += ".";
#pragma  block_loop
				for (; i < mantissaPart.size(); i++)
					if (i != (mantissaPart.size() - 1)) {
						::numToStr(tempchar, mantissaPart[i]);
						temp += tempchar;
					}
					else {
						std::string c = ::numToStr(mantissaPart[i]);
						while (c.back() == '0')//删除末尾为0
							c.pop_back();
						temp += c;
					}
			}
		}
		return temp;
	}

	void bigNum::swap(bigNum& arg)
	{
		bool temp = arg.isPositive;
		arg.isPositive = isPositive;
		isPositive = temp;
		int64_t tempi = arg.indexPart;
		arg.indexPart = indexPart;
		indexPart = tempi;
		mantissaPart.swap(arg.mantissaPart);
	}

	void bigNum::check() noexcept//处理无效精度位
	{
		if (mantissaPart.size()) {
			while (mantissaPart.size() && mantissaPart.back() == 0ll)
				mantissaPart.pop_back();
			size_t sizeTemp = 0;
			while (mantissaPart.size() && mantissaPart[sizeTemp] == 0ll)
				sizeTemp++;
			if (sizeTemp > 0) {
				mantissaPart.erase(mantissaPart.begin(), mantissaPart.begin() + sizeTemp);
				indexPart -= sizeTemp;
			}
		}
		if (!mantissaPart.size()) {
			indexPart = 0;
			isPositive = true;
		}
	}

	void VECTORCALL bigNum::carry(bool alwaysCheck, size_t end, size_t maxbit, bool checked) noexcept//严格依赖串行，多线程算法可能会提高时间复杂度
	{
#pragma block_loop
		for (int64_t i = (mantissaPart.size() - 1 - end > 0 ? mantissaPart.size() - 1 - end : 0); i >= 0 && maxbit > 0; i--) {
			if (mantissaPart[i] >= UPPER_LIMIT) {
				if (i != 0) {
					mantissaPart[i - 1] += mantissaPart[i] / UPPER_LIMIT;
					mantissaPart[i] %= UPPER_LIMIT;
				}
				else {
					while (mantissaPart[0] >= UPPER_LIMIT) {
						mantissaPart.insert(mantissaPart.begin(), mantissaPart[0] / UPPER_LIMIT);
						mantissaPart[1] %= UPPER_LIMIT;
						indexPart++;
					}
				}
			}
			else if (!alwaysCheck)
				return;
			maxbit--;
		}
		if (checked)
			check();
	}

	void bigNum::borrow() noexcept//注意，只处理非0！//32 bits
	{
#pragma block_loop
		for (int64_t i = mantissaPart.size() - 1; i > 0; i--)
			if (mantissaPart[i] < 0) {
				mantissaPart[i - 1] -= 1;
				mantissaPart[i] += UPPER_LIMIT;
			}
		check();
	}

	void bigNum::retentionAccuracy(size_t length, bool ismantissaPart, bool Rounding) noexcept
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		if (!mantissaPart.size())
			return;
		if (ismantissaPart) {
			if (length / 9 + indexPart + 1 <= mantissaPart.size()) {
				mantissaPart.erase(mantissaPart.begin() + (length / 9 + indexPart + 1 > 0 ? length / 9 + indexPart + 1 : 0), mantissaPart.end());
				if (mantissaPart.size() - (indexPart > 0 ? indexPart : 0) > 0 && indexPart < mantissaPart.size()) {
					mantissaPart.back() -= mantissaPart.back() % hnpow(10, 9 - length % 9);
					if (Rounding) {
						int64_t carryTemp = mantissaPart.back() / hnpow(10, 8 - length % 9) % 10;
						carryTemp = (carryTemp > 4 ? hnpow(10, 9 - length % 9) : 0);
						mantissaPart.back() += carryTemp;
					}
					carry(false);
				}
			}
		}
		else if (divideCeil(length, 9) <= mantissaPart.size()) {
			int64_t temp = 0, i = mantissaPart[0];
			while (i / 100000000 == 0) {
				temp++;
				i *= 10;
			}
			mantissaPart.erase(mantissaPart.begin() + divideCeil(length, 9), mantissaPart.end());
			if (mantissaPart.size()) {
				if (mantissaPart.size() == 1)
					mantissaPart[0] = mantissaPart[0] / hnpow(10, (9 - temp - length > 0 ? 9 - temp - length : 0)) * hnpow(10, (9 - temp - length > 0 ? 9 - temp - length : 0));
				else
					mantissaPart.back() = mantissaPart.back() / hnpow(10, 9 - (temp + length) % 9) * hnpow(10, 9 - (temp + length) % 9);
			}
			carry(false);
		}
		check();
	}

	int64_t VECTORCALL bigNum::getEffectiveDigitalAccuracy(bool isIntPart) noexcept
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		if (!mantissaPart.size())
			return 0;
		int64_t length = 9 * (isIntPart ? (mantissaPart.size() <= indexPart ? mantissaPart.size() : indexPart) : mantissaPart.size()), i;
		i = mantissaPart[0];
		while (i / 100000000 == 0) {
			length--;
			i *= 10;
		}
		i = 10;
		if (!isIntPart || (isIntPart && mantissaPart.size()) <= indexPart)
			while (mantissaPart.back() % i == 0) {
				length--;
				i *= 10;
			}
		return length;
	}

	double bigNum::toFloat() noexcept
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		if (indexPart > 4096)
			return HUGE_VAL;
		double temp = 0;
		for (size_t i = 0; i < (mantissaPart.size() < 3 ? mantissaPart.size() : 3); i++)
			temp = temp * UPPER_LIMIT + mantissaPart[i];
		temp /= ::pow(UPPER_LIMIT, (mantissaPart.size() < 30 ? mantissaPart.size() : 30) - indexPart);
		return temp;
	}

	int64_t bigNum::toInt() noexcept
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		if (indexPart > 2)
			return 0ll;
		int64_t temp = 0;
		if (indexPart == 2) {
			if (mantissaPart.size() == 1)
				temp = mantissaPart[0];
			else
				temp += (mantissaPart[0] * UPPER_LIMIT + mantissaPart[1]);
		}
		else if (indexPart == 1) 
			temp = mantissaPart[0];
		if (!isPositive)
			temp = -temp;
		return temp;
	}

	void bigNum::setVal(std::string arg)//对特殊符号的处理未做  bug
	{
		isPositive = (arg[0] != '-');
		if (arg.find('-', 1) != std::string::npos || arg.find('+', 1) != std::string::npos) {
			throw std::invalid_argument("Incorrect input");
			mantissaPart.clear();
			check();
			return;
		}
		mantissaPart.clear();
		mantissaPart.reserve(arg.size() / 9 + 9);
		size_t start = 0, end = arg.size() - 1;
		arg += '.';
		size_t point = arg.find('.');
		arg.pop_back();
		if (!isPositive || arg[0] == '+')
			start = 1;//有效数字计算位数
		if (point < start)
			point = start + 1;
		while (arg[start] == 0)
			start++;
		indexPart = divideCeil(point - start, 9);
		mantissaPart.push_back(stoll(arg.substr(start, ((point - start) % 9 ? (point - start) % 9 : 9))));
		start += ((point - start) % 9 ? (point - start) % 9 : 9);
		while (start < point) {
			mantissaPart.push_back(stoll(arg.substr(start, 9)));
			start += 9;
		}
		if (start >= point) {
			start = point + 1;
			arg += "000000000";
			while (start <= end) {
				mantissaPart.push_back(stoll(arg.substr(start, 9)));
				start += 9;
			}
		}
		check();
	}

	inline void bigNum::set_zero() noexcept
	{
		isPositive = true;
		mantissaPart.clear();
		indexPart = 0;
	}

	inline void VECTORCALL bigNum::set_float(double f) noexcept
	{
		set_zero();
		if (f == 0) {
			indexPart = 0;
			return;
		}
		isPositive = (f >= 0);
		f = (f >= 0 ? f : -f);
		int index = log10(f);
		f /= ::pow(10, ++index);//0.?????*10^index
		if (index % 9 == 0)
			indexPart = index / 9;
		else if (index % 9 > 0) {
			indexPart = static_cast<int64_t>(index / 9) + 1;//上取整
			f /= ::pow(10, 9 * indexPart - index);
		}
		else {
			indexPart = index / 9;
			f *= ::pow(10, index % 9);
		}
		if (indexPart < -100000000) {
			mantissaPart.clear();
			indexPart = 0;
			//std::clog << "warning: Infinitesimal, rounded to 0" << std::endl;
			return;
		}
		for (int i = 0; i < 4; i++) {
			f *= 1000000000;
			mantissaPart.push_back(int64_t(f));
			f -= int64_t(f);
		}
		check();
	}
	//func

	inline bigNum __fastcall fibonacciIteratively(uint64_t n)
	{
		bigNum result = 0, nextItem = 1, tmp;
		for (size_t i = 0; i < n; i++) {
			tmp = nextItem;
			nextItem += result;
			result = tmp;
		}
		return result;
	}

	std::ostream& VECTORCALL operator<<(std::ostream& os, bigNum& arg)
	{
		os << arg.toString();
		return os;
	}

	std::istream& VECTORCALL operator>>(std::istream& is, bigNum& arg)
	{
		std::string s;
		is >> s;
		try {
			arg.setVal(s);
		}
		catch (const std::exception&) {
			std::cerr << "Incorrect input" << std::endl;
			arg.mantissaPart.clear();
			arg.check();
		}
		return is;
	}

}

