#pragma once
#include <string>
#include <vector>
#include "basic.hpp"
#include "bigNum.hpp"
#include "expression.hpp"

MANumCal

template <class _Ty>
class Parameter {
public:
	Parameter() noexcept :type(0u) {}
	Parameter(unsigned t) noexcept :type(t) {}
	Parameter(const Parameter<_Ty>&) noexcept;
	virtual ~Parameter() noexcept = default;

	virtual Parameter<_Ty>* copy() const = 0;
	virtual _Ty FASTCALL getValue(uint64_t accuracy) const = 0;
	virtual bool isRational() {
		throw std::invalid_argument("error");
	}
	// aggregate

public:
	const unsigned type; //type 0
	Aggregate<_Ty> agg;
};

//-----------------------------
template <class _Ty>
class BasicNumber :public Parameter<_Ty> { 
public:
	BasicNumber() noexcept = default;
	// uint64_t..., bigNum
	BasicNumber(Parameter<_Ty>& n) noexcept; // :Parameter(1u), num(n) {}
	// double...
	BasicNumber(_Ty& n) noexcept;
	BasicNumber(const BasicNumber&) noexcept;
	virtual ~BasicNumber() noexcept = default;

	virtual Parameter<_Ty>* copy() const override;
	virtual _Ty FASTCALL getValue(uint64_t accuracy) const override;

public: // 123
	_Ty num;
}; //type 1

//-----------------------------
template <class _Ty>
class Symbal :public Parameter<_Ty> {
public:
	Symbal() = delete;
	Symbal(std::string str) noexcept 
		:Parameter(2u), name(str), value(nullptr) {}
	Symbal(std::string str, Parameter<_Ty>* n) noexcept
		:Parameter(2u), name(str), value(n) {}
	Symbal(Parameter<_Ty>* n); //:Parameter(2u)
	Symbal(const Symbal&) noexcept;
	virtual ~Symbal() noexcept;

	virtual Parameter<_Ty>* copy() const override;
	virtual _Ty FASTCALL getValue(uint64_t accuracy) const override;

public: // "x"
	std::string name;
	Parameter<_Ty>* value; //nullptr = null 
}; //type 2

//-----------------------------
template <class _Ty>
class Add :public Parameter<_Ty> {
public:
	Add() = delete;
	Add(std::vector<Parameter<_Ty>*> s) noexcept :series(s) {}
	Add(const Add&) noexcept;
	virtual ~Add() noexcept;

	virtual Parameter<_Ty>* copy() const override;
	virtual _Ty FASTCALL getValue(uint64_t accuracy) const override;

public: // [0] + [1] + ... + [n]
	std::vector<Parameter<_Ty>*> series;
};//type 3

template <class _Ty>
class Multiply :public Parameter<_Ty> {
public:
	Multiply() = delete;
	Multiply(std::vector<Parameter<_Ty>*> s) noexcept :series(s) {}
	Multiply(const Multiply&) noexcept;
	virtual ~Multiply() noexcept;

	virtual Parameter<_Ty>* copy() const override;
	virtual _Ty FASTCALL getValue(uint64_t accuracy) const override;

public: // [0] * [1] * ... * [n]
	std::vector<Parameter<_Ty>*> series;
};//type 4

template <class _Ty>
class Fraction :public Parameter<_Ty> {
public:
	Fraction() = delete;
	Fraction(Parameter<_Ty>* n, Parameter<_Ty>* d);
	Fraction(const Fraction&) noexcept;
	virtual ~Fraction() noexcept;

	virtual Parameter<_Ty>* copy() const override;
	virtual _Ty FASTCALL getValue(uint64_t accuracy) const override;

public: // numerator / denominator 
	Parameter<_Ty>* numerator;
	Parameter<_Ty>* denominator;
};//type 5

template <class _Ty>
class Power :public Parameter<_Ty> {
public:
	Power() = delete;
	Power(Parameter<_Ty>* m, Parameter<_Ty>* i);
	Power(const Power&) noexcept;
	virtual ~Power() noexcept;

	virtual Parameter<_Ty>* copy() const override;
	virtual _Ty FASTCALL getValue(uint64_t accuracy) const override;

public: // mantissa ^ index 
	Parameter<_Ty>* mantissa;
	Parameter<_Ty>* index;
};//type 6

//class Sqrt :public Parameter { //sqrt(x) == x^(1/x)
//public:
//	Sqrt() = delete;
//	Sqrt(Parameter* m, Parameter* i);
//	Sqrt(const Sqrt&) noexcept;
//	virtual ~Sqrt() noexcept;
//
//	virtual Parameter* copy() const override;
//	virtual bigNum FASTCALL getApproximate(uint64_t accuracy) const override;
//	virtual double getFloat() const override;
//
//public: // sqrt(num) 
//	Parameter* num;
//};//type 7
//...

MANumCalEND