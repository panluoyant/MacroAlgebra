#define SET_EXPRESSION_EXPO
#include "macomplex.hpp"
#include "expression.hpp"
#include "bigNum.hpp"
#include "parameter.hpp"
#include "mavector.hpp"

uint16_t NAMESPACEMANumCal ExpOperate::state = 0;
NAMESPACEMANumCal ExpOperate Expo;

//------------------------
//isInt
//------------------------

template <typename _Ty> bool NAMESPACEMANumCal isInt(_Ty&) {
	throw std::invalid_argument("error type");
}

template <> bool NAMESPACEMANumCal isInt<int8_t>(int8_t&) {
	return true;
}

template <> bool NAMESPACEMANumCal isInt<uint8_t>(uint8_t&) {
	return true;
}

template <> bool NAMESPACEMANumCal isInt<int16_t>(int16_t&) {
	return true;
}

template <> bool NAMESPACEMANumCal isInt<uint16_t>(uint16_t&) {
	return true;
}

template <> bool NAMESPACEMANumCal isInt<int32_t>(int32_t&) {
	return true;
}

template <> bool NAMESPACEMANumCal isInt<uint32_t>(uint32_t&) {
	return true;
}

template <> bool NAMESPACEMANumCal isInt<int64_t>(int64_t&) {
	return true;
}

template <> bool NAMESPACEMANumCal isInt<uint64_t>(uint64_t&) {
	return true;
}

template <> bool NAMESPACEMANumCal isInt<float_t>(float_t& num) {
	return num == round(num);
}

template <> bool NAMESPACEMANumCal isInt<double_t>(double_t& num) {
	return num == round(num);
}

template <> bool NAMESPACEMANumCal isInt<NAMESPACEMANumCal bigNum>(NAMESPACEMANumCal bigNum& num) {
	num.check();
	return num.mantissaPart.size() <= num.indexPart;
}

//------------------------
//isComplexNum
//------------------------

template <typename _Ty> bool NAMESPACEMANumCal isComplexNum(_Ty& num) {
	return false;
}

//template <> bool NAMESPACEMANumCal isComplexNum<std::complex>(std::complex& num) {
//	return true;
//}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<int8_t>>(std::complex<int8_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<uint8_t>>(std::complex<uint8_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<int16_t>>(std::complex<int16_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<uint16_t>>(std::complex<uint16_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<int32_t>>(std::complex<int32_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<uint32_t>>(std::complex<uint32_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<int64_t>>(std::complex<int64_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<uint64_t>>(std::complex<uint64_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<float_t>>(std::complex<float_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<double_t>>(std::complex<double_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<std::complex<NAMESPACEMANumCal bigNum>>(std::complex<NAMESPACEMANumCal bigNum>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<int8_t>>(NAMESPACEMANumCal MAComplex<int8_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<uint8_t>>(NAMESPACEMANumCal MAComplex<uint8_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<int16_t>>(NAMESPACEMANumCal MAComplex<int16_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<uint16_t>>(NAMESPACEMANumCal MAComplex<uint16_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<int32_t>>(NAMESPACEMANumCal MAComplex<int32_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<uint32_t>>(NAMESPACEMANumCal MAComplex<uint32_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<int64_t>>(NAMESPACEMANumCal MAComplex<int64_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<uint64_t>>(NAMESPACEMANumCal MAComplex<uint64_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<float_t>>(NAMESPACEMANumCal MAComplex<float_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<double_t>>(NAMESPACEMANumCal MAComplex<double_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isComplexNum<NAMESPACEMANumCal MAComplex<NAMESPACEMANumCal bigNum>>(NAMESPACEMANumCal MAComplex<NAMESPACEMANumCal bigNum>& num) {
	return true;
}

//----------------------------
//isRational
//----------------------------

template <typename _Ty> bool NAMESPACEMANumCal isRational(_Ty& num) {
	return true;
}

//complex

template <> bool NAMESPACEMANumCal isRational<std::complex<int8_t>>(std::complex<int8_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<uint8_t>>(std::complex<uint8_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<int16_t>>(std::complex<int16_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<uint16_t>>(std::complex<uint16_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<int32_t>>(std::complex<int32_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<uint32_t>>(std::complex<uint32_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<int64_t>>(std::complex<int64_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<uint64_t>>(std::complex<uint64_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<float_t>>(std::complex<float_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<double_t>>(std::complex<double_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<std::complex<NAMESPACEMANumCal bigNum>>(std::complex<NAMESPACEMANumCal bigNum>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<int8_t>>(NAMESPACEMANumCal MAComplex<int8_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<uint8_t>>(NAMESPACEMANumCal MAComplex<uint8_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<int16_t>>(NAMESPACEMANumCal MAComplex<int16_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<uint16_t>>(NAMESPACEMANumCal MAComplex<uint16_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<int32_t>>(NAMESPACEMANumCal MAComplex<int32_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<uint32_t>>(NAMESPACEMANumCal MAComplex<uint32_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<int64_t>>(NAMESPACEMANumCal MAComplex<int64_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<uint64_t>>(NAMESPACEMANumCal MAComplex<uint64_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<float_t>>(NAMESPACEMANumCal MAComplex<float_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<double_t>>(NAMESPACEMANumCal MAComplex<double_t>& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal MAComplex<NAMESPACEMANumCal bigNum>>(NAMESPACEMANumCal MAComplex<NAMESPACEMANumCal bigNum>& num) {
	return false;
}

//Parameter
template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<int8_t>>(NAMESPACEMANumCal Parameter<int8_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<uint8_t>>(NAMESPACEMANumCal Parameter<uint8_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<int16_t>>(NAMESPACEMANumCal Parameter<int16_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<uint16_t>>(NAMESPACEMANumCal Parameter<uint16_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<int32_t>>(NAMESPACEMANumCal Parameter<int32_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<uint32_t>>(NAMESPACEMANumCal Parameter<uint32_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<int64_t>>(NAMESPACEMANumCal Parameter<int64_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<uint64_t>>(NAMESPACEMANumCal Parameter<uint64_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<float_t>>(NAMESPACEMANumCal Parameter<float_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<double_t>>(NAMESPACEMANumCal Parameter<double_t>& num) {
	return num.isRational();
}

template <> bool NAMESPACEMANumCal isRational<NAMESPACEMANumCal Parameter<NAMESPACEMANumCal bigNum>>(NAMESPACEMANumCal Parameter<NAMESPACEMANumCal bigNum>& num) {
	return num.isRational();
}

//------------------
//isVector
//------------------

template <typename _Ty> bool NAMESPACEMANumCal isVector(_Ty& num) {
	return false;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<int8_t>>(std::vector<int8_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<uint8_t>>(std::vector<uint8_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<int16_t>>(std::vector<int16_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<uint16_t>>(std::vector<uint16_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<int32_t>>(std::vector<int32_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<uint32_t>>(std::vector<uint32_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<int64_t>>(std::vector<int64_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<uint64_t>>(std::vector<uint64_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<float_t>>(std::vector<float_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<double_t>>(std::vector<double_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<std::vector<NAMESPACEMANumCal bigNum>>(std::vector<NAMESPACEMANumCal bigNum>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<int8_t>>(NAMESPACEMANumCal MAVector<int8_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<uint8_t>>(NAMESPACEMANumCal MAVector<uint8_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<int16_t>>(NAMESPACEMANumCal MAVector<int16_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<uint16_t>>(NAMESPACEMANumCal MAVector<uint16_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<int32_t>>(NAMESPACEMANumCal MAVector<int32_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<uint32_t>>(NAMESPACEMANumCal MAVector<uint32_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<int64_t>>(NAMESPACEMANumCal MAVector<int64_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<uint64_t>>(NAMESPACEMANumCal MAVector<uint64_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<float_t>>(NAMESPACEMANumCal MAVector<float_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<double_t>>(NAMESPACEMANumCal MAVector<double_t>& num) {
	return true;
}

template <> bool NAMESPACEMANumCal isVector<NAMESPACEMANumCal MAVector<NAMESPACEMANumCal bigNum>>(NAMESPACEMANumCal MAVector<NAMESPACEMANumCal bigNum>& num) {
	return true;
}