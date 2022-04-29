#pragma once
#include <vector>
#include <ippbase.h>
#include "basic.hpp"
#include "macomplex.hpp"
#include "bigNum.hpp"

MANumCal

template <class _Ty> //普通类型
class MAVector :public std::vector<_Ty> {

public:
	MAVector() noexcept = default;
	virtual ~MAVector() noexcept = default;

public:


};

//对complex类型实例化
template <class _Ty> 
class _basic_complex_vector :public std::vector<_Ty> {

};

template <> class MAVector<MAComplex<float_t>> :public _basic_complex_vector<Ipp32fc> {

};

template <> class MAVector<MAComplex<double_t>> :public _basic_complex_vector<Ipp64fc> {
	
};

//......

//矢量运算...
//模板实例化
MANumCalEND