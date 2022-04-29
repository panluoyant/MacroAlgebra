#pragma once
#include <complex>
#include "basic.hpp"

MANumCal

template <class _Ty>
class MAComplex :public std::complex<_Ty> {
public:
	MAComplex(_Ty& r, _Ty& i) noexcept :std::complex<_Ty>(r, i) {}
	virtual ~MAComplex() noexcept = default;
};

MANumCalEND