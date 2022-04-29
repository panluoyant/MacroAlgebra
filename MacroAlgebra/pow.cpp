#include "bigNum.hpp"

namespace MA::NumCal {

	extern bigNum one;
	extern bigNum mone;

	void VECTORCALL square(bigNum& result, bigNum& arg)
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		arg.check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		if (arg.mantissaPart.size() < 64) {
			multiply(result, arg, arg);
			return;
		}
		result.mantissaPart.clear();
		result.isPositive = true;
		const int lengthsize = log2(9 * arg.mantissaPart.size()) + 2;
		const size_t lengthlog = hnpow(2, lengthsize);
		std::vector<Ipp64fc> arg_fc = hnTodouble9fc(arg);
		arg_fc.resize(lengthlog, { 0. ,0. });
		Ipp64fc* p_arg = &arg_fc[0];
		IppsFFTSpec_C_64fc* pSpec = 0;
		Ipp8u* pMemSpec = 0, * pMemInit = 0, * pMemBuffer = 0;
		int sizeSpec = 0, sizeInit = 0, sizeBuffer = 0;
		ippsFFTGetSize_C_64fc(lengthsize, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer);
		pMemSpec = reinterpret_cast<Ipp8u*>(ippMalloc(sizeSpec));
		if (sizeInit > 0)
			pMemInit = reinterpret_cast<Ipp8u*>(ippMalloc(sizeInit));
		if (sizeBuffer > 0)
			pMemBuffer = reinterpret_cast<Ipp8u*>(ippMalloc(sizeBuffer));
		ippsFFTInit_C_64fc(&pSpec, lengthsize, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone, pMemSpec, pMemInit);
		if (sizeInit > 0)
			ippFree(pMemInit);
		ippsFFTFwd_CToC_64fc_I(p_arg, pSpec, pMemBuffer);
		ippsSqr_64fc_I(p_arg, lengthlog);
		ippsFFTInv_CToC_64fc_I(p_arg, pSpec, pMemBuffer);
		if (sizeBuffer > 0)
			ippFree(pMemBuffer);
		ippFree(pMemSpec);
		result.mantissaPart.resize(lengthlog / 9 + 1, 0);
		size_t arg1_size = arg_fc.size();
		size_t t = 0, r = 0;
		for (int64_t i = lengthlog / 9; i >= 0; i--) {
			result.mantissaPart[i] += (t < arg1_size ? int64_t(arg_fc[t].re + 0.5) % UPPER_LIMIT : 0);
			result.mantissaPart[i] += (t < arg1_size - 1 ? int64_t(arg_fc[t + 1].re + 0.5) % UPPER_LIMIT * 10 : 0);
			result.mantissaPart[i] += (t < arg1_size - 2 ? int64_t(arg_fc[t + 2].re + 0.5) % UPPER_LIMIT * 100 : 0);
			result.mantissaPart[i] += (t < arg1_size - 3 ? int64_t(arg_fc[t + 3].re + 0.5) % UPPER_LIMIT * 1000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 4 ? int64_t(arg_fc[t + 4].re + 0.5) % UPPER_LIMIT * 10000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 5 ? int64_t(arg_fc[t + 5].re + 0.5) % UPPER_LIMIT * 100000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 6 ? int64_t(arg_fc[t + 6].re + 0.5) % UPPER_LIMIT * 1000000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 7 ? int64_t(arg_fc[t + 7].re + 0.5) % UPPER_LIMIT * 10000000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 8 ? int64_t(arg_fc[t + 8].re + 0.5) % UPPER_LIMIT * 100000000 : 0);
			t += 9;
		}
		result.carry(true, 0, LLONG_MAX, false);
		while (result.mantissaPart[r] == 0)
			r++;
		result.mantissaPart.erase(result.mantissaPart.begin(), result.mantissaPart.begin() + r);
		result.indexPart = result.mantissaPart.size() - 2 * (arg.mantissaPart.size() - arg.indexPart > 0 ? arg.mantissaPart.size() - arg.indexPart : 0);
		result.carry();
	}

	void VECTORCALL pow(bigNum& result, bigNum& arg1, int64_t arg2, uint64_t length)//this!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		arg1.check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		if (arg1.indexPart * 9ll * arg2 > int64_t(INT_MAX))//超过限制
			throw std::invalid_argument("pow: The result is too large!");
		else if (arg1 == one) {  //1^n
			result = one;
			return;
		}
		else if (arg1 == mone) { //-1^n
			result = arg2 % 2 ? mone : one;
			return;
		}
		else if (!arg1.mantissaPart.size())//arg1==0, 0^n ,except 0^0
			if (!arg2)//arg2==0
				throw std::invalid_argument("Maths Error: 0 ^ 0 isn't allowed!");
			else {
				result.set_zero();
				return;
			}
		else if (arg2 == one) { //n^1
			result = arg1;
			return;
		}
		else if (!arg2) {//arg2==0, 0^0 was thrown before!
			result = one;
			return;
		}
		else if (arg2 == mone) {
			divide(result, one, arg1, length);
			return;
		} //arg1==0 ,was thrown before!
		else {
			result = one;
			bool arg2pos = (arg2 >= 0);
			if (!arg2pos)
				arg2 = -arg2;
			bigNum arg1_result(arg1);
			bigNum temp;
			while (arg2) {
				if (arg2 & 1)
					result *= arg1_result;
				arg2 >>= 1;  //bot/=2
				if (arg2) {
					square(temp, arg1_result);
					arg1_result.swap(temp);
				}
			}
			arg1_result.set_zero();//优化空间
			if (!arg2pos) {
				divide(temp, one, result, length);
				result.swap(temp);
			}
			else
				result.retentionAccuracy(length);
		}
	}

	//void VECTORCALL pow(bigNum& result, bigNum& arg1, bigNum& arg2, uint64_t length)
	//{
	//	pow(result, arg1, arg2.toInt(), length);
	//}

	void VECTORCALL pow10(bigNum& result, int64_t arg2)
	{
		result.set_zero();
		result.indexPart = arg2 / 9;
		if (arg2 < 0)
			arg2 = -arg2;
		result.mantissaPart.push_back(hnpow(10ll, arg2 % 9));
	}

	bigNum VECTORCALL operator ^(bigNum& arg1, int64_t arg2)
	{
		bigNum temp;
		try {
			pow(temp, arg1, arg2, 9);
			return temp;
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
			return 0ll;
		}
	}

	bigNum VECTORCALL operator ^(bigNum&& arg1, int64_t arg2)
	{
		return arg1 ^ arg2;
	}

	bigNum VECTORCALL operator ^=(bigNum& arg1, int64_t arg2)
	{
		return arg1 = arg1 ^ arg2;
	}
}