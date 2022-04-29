#include "bigNum.hpp"

namespace MA::NumCal {

	extern bigNum one;
	extern bigNum mone;

	void VECTORCALL multiply(bigNum& result, bigNum& arg1, bigNum& arg2) //O(N*M)
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		arg1.check();
		arg2.check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
#ifndef OPTIMIZATION_JUDGMENT_PROHIBITION
		if (!arg1.mantissa_part().size() || !arg2.mantissa_part().size()) {
			result.set_zero();
			return;
		}
		else if (arg1 == one) {
			result = arg2;
			return;
		}
		else if (arg1 == mone) {
			result = arg2;
			result.positive() = !arg2.positive();
			return;
		}
		else if (arg2 == one) {
			result = arg1;
			return;
	}
		else if (arg2 == mone) {
			result = arg1;
			result.positive() = !arg1.positive();
			return;
		}
#endif // !OPTIMIZATION_JUDGMENT_PROHIBITION
		result.mantissaPart.clear();
		result.isPositive = (arg1.isPositive == arg2.isPositive);
		result.indexPart = arg1.indexPart + arg2.indexPart;
		result.mantissaPart.assign(arg1.mantissaPart.size() + arg2.mantissaPart.size(), 0);
		int64_t checktresult = 0;
		for (int64_t i = arg1.mantissaPart.size() - 1; i >= 0; i--) {
			for (int64_t j = arg2.mantissaPart.size() - 1; j >= 0; j--)
				result.mantissaPart[i + j + 1] += arg1.mantissaPart[i] * arg2.mantissaPart[j];
			result.carry(true, checktresult++, arg2.mantissaPart.size(), false);
		}
		result.check();
	}

	void VECTORCALL multiply(bigNum& result, bigNum&& arg1, bigNum& arg2)
	{
		multiply(result, arg1, arg2);
	}

	void VECTORCALL multiply(bigNum& result, bigNum& arg1, bigNum&& arg2)
	{
		multiply(result, arg1, arg2);

	}

	void VECTORCALL multiply(bigNum& result, bigNum&& arg1, bigNum&& arg2)
	{
		multiply(result, arg1, arg2);
	}

	void VECTORCALL multiply_fft(bigNum& result, bigNum& arg1, bigNum& arg2) //O(2^(ceil(log(max(N,M)))+1)*log(2^(ceil(log(max(N,M)))+1))) = O(max(N,M)*log(max(N,M)))
	{
#ifndef PROHIBIT_REDUNDANCY_CHECK
		arg1.check();
		arg2.check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
#ifndef OPTIMIZATION_JUDGMENT_PROHIBITION
		if (!arg1.mantissa_part().size() || !arg2.mantissa_part().size()) {
			result.set_zero();
			return;
		}
		else if (arg1 == one) {
			result = arg2;
			return;
		}
		else if (arg1 == mone) {
			result = arg2;
			result.positive() = !arg2.positive();
			return;
		}
		else if (arg2 == one) {
			result = arg1;
			return;
		}
		else if (arg2 == mone) {
			result = arg1;
			result.positive() = !arg1.positive();
			return;
		}
#endif // !OPTIMIZATION_JUDGMENT_PROHIBITION
		result.mantissaPart.clear();
		result.isPositive = arg1.isPositive == arg2.isPositive;
		int lengthsize = log2(9 * (arg1.mantissaPart.size() > arg2.mantissaPart.size() ? arg1.mantissaPart.size() : arg2.mantissaPart.size())) + 2;
		size_t lengthlog = ::pow(2, lengthsize);
		std::vector<Ipp64fc> arg1_fc = hnTodouble9fc(arg1), arg2_npr = hnTodouble9fc(arg2);
		arg1_fc.resize(lengthlog, { 0. ,0. });
		arg2_npr.resize(lengthlog, { 0. ,0. });
		Ipp64fc* p_arg1 = &arg1_fc[0], * p_arg2 = &arg2_npr[0];
		IppsFFTSpec_C_64fc* pSpec = 0;
		Ipp8u* pMemSpec = 0, * pMemInit = 0, * pMemBuffer = 0;
		int sizeSpec = 0, sizeInit = 0, sizeBuffer = 0;
		ippsFFTGetSize_C_64fc(lengthsize, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer);
		pMemSpec = (Ipp8u*)ippMalloc(sizeSpec);
		if (sizeInit > 0)
			pMemInit = (Ipp8u*)ippMalloc(sizeInit);
		if (sizeBuffer > 0)
			pMemBuffer = (Ipp8u*)ippMalloc(sizeBuffer);
		ippsFFTInit_C_64fc(&pSpec, lengthsize, IPP_FFT_DIV_INV_BY_N, ippAlgHintNone, pMemSpec, pMemInit);
		if (sizeInit > 0)
			ippFree(pMemInit);
		ippsFFTFwd_CToC_64fc_I(p_arg1, pSpec, pMemBuffer);
		ippsFFTFwd_CToC_64fc_I(p_arg2, pSpec, pMemBuffer);
		ippsMul_64fc_I(p_arg2, p_arg1, lengthlog);
		ippsFFTInv_CToC_64fc_I(p_arg1, pSpec, pMemBuffer);
		if (sizeBuffer > 0)
			ippFree(pMemBuffer);
		ippFree(pMemSpec);
		result.mantissaPart.resize(lengthlog / 9 + 1, 0);
		size_t arg1_size = arg1_fc.size();
		size_t t = 0, r = 0;
		for (int64_t i = lengthlog / 9; i >= 0; i--) {
			result.mantissaPart[i] += (t < arg1_size ? int64_t(arg1_fc[t].re + 0.5) % UPPER_LIMIT : 0);
			result.mantissaPart[i] += (t < arg1_size - 1 ? int64_t(arg1_fc[t + 1].re + 0.5) % UPPER_LIMIT * 10 : 0);
			result.mantissaPart[i] += (t < arg1_size - 2 ? int64_t(arg1_fc[t + 2].re + 0.5) % UPPER_LIMIT * 100 : 0);
			result.mantissaPart[i] += (t < arg1_size - 3 ? int64_t(arg1_fc[t + 3].re + 0.5) % UPPER_LIMIT * 1000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 4 ? int64_t(arg1_fc[t + 4].re + 0.5) % UPPER_LIMIT * 10000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 5 ? int64_t(arg1_fc[t + 5].re + 0.5) % UPPER_LIMIT * 100000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 6 ? int64_t(arg1_fc[t + 6].re + 0.5) % UPPER_LIMIT * 1000000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 7 ? int64_t(arg1_fc[t + 7].re + 0.5) % UPPER_LIMIT * 10000000 : 0);
			result.mantissaPart[i] += (t < arg1_size - 8 ? int64_t(arg1_fc[t + 8].re + 0.5) % UPPER_LIMIT * 100000000 : 0);
			t += 9;
		}
		result.carry(true, 0, LLONG_MAX, false);
		while (result.mantissaPart[r] == 0)
			r++;
		result.mantissaPart.erase(result.mantissaPart.begin(), result.mantissaPart.begin() + r);
		result.indexPart = result.mantissaPart.size()
			- (arg1.mantissaPart.size() - arg1.indexPart > 0 ? arg1.mantissaPart.size() - arg1.indexPart : 0)
			- (arg2.mantissaPart.size() - arg2.indexPart > 0 ? arg2.mantissaPart.size() - arg2.indexPart : 0);
		result.carry();
	}

	void VECTORCALL multiply_fft(bigNum& result, bigNum&& arg1, bigNum& arg2)
	{
		multiply_fft(result, arg1, arg2);
	}

	void VECTORCALL multiply_fft(bigNum& result, bigNum& arg1, bigNum&& arg2)
	{
		multiply_fft(result, arg1, arg2);
	}

	void VECTORCALL multiply_fft(bigNum& result, bigNum&& arg1, bigNum&& arg2)
	{
		multiply_fft(result, arg1, arg2);
	}

	bigNum VECTORCALL operator *(bigNum& arg1, bigNum& arg2)
	{
		bigNum temp;
		if (arg1.mantissaPart.size() > 2147483648 || arg2.mantissaPart.size() > 2147483648) {
			/*throw std::invalid_argument("multiply: too large");*/
			//std::cerr << "multiply: too large" << std::endl;
			return 0ll;
		}
		else if ((arg1.mantissaPart.size() < 5 || arg2.mantissaPart.size() < 5)
			|| (arg1.mantissaPart.size() < 64 && arg2.mantissaPart.size() < 64))
			multiply(temp, arg1, arg2);
		else
			multiply_fft(temp, arg1, arg2);
		return temp;
	}

	bigNum VECTORCALL operator *(bigNum&& arg1, bigNum& arg2)
	{
		return arg1 * arg2;
	}

	bigNum VECTORCALL operator *(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 * arg2;
	}

	bigNum VECTORCALL operator *(bigNum&& arg1, bigNum&& arg2)
	{
		return arg1 * arg2;
	}

	bigNum VECTORCALL operator *=(bigNum& arg1, bigNum& arg2)
	{
		return arg1 = arg1 * arg2;
	}

	bigNum VECTORCALL operator *=(bigNum& arg1, bigNum&& arg2)
	{
		return arg1 = arg1 * arg2;
	}

	inline std::vector<Ipp64fc> VECTORCALL hnTodouble9fc(bigNum& hn)
	{
		std::vector<Ipp64fc> temp;
#ifndef PROHIBIT_REDUNDANCY_CHECK
		hn.check();
#endif // !PROHIBIT_REDUNDANCY_CHECK
		temp.reserve(9 * hn.mantissaPart.size() + 9);
		for (int64_t i = hn.mantissaPart.size() - 1; i >= 0; i--) {
			temp.push_back({ Ipp64f(hn.mantissaPart[i] % 10), 0. });
			temp.push_back({ Ipp64f(hn.mantissaPart[i] % 100 / 10), 0. });
			temp.push_back({ Ipp64f(hn.mantissaPart[i] % 1000 / 100), 0. });
			temp.push_back({ Ipp64f(hn.mantissaPart[i] % 10000 / 1000), 0. });
			temp.push_back({ Ipp64f(hn.mantissaPart[i] % 100000 / 10000), 0. });
			temp.push_back({ Ipp64f(hn.mantissaPart[i] % 1000000 / 100000), 0. });
			temp.push_back({ Ipp64f(hn.mantissaPart[i] % 10000000 / 1000000), 0. });
			temp.push_back({ Ipp64f(hn.mantissaPart[i] % 100000000 / 10000000), 0. });
			temp.push_back({ Ipp64f(hn.mantissaPart[i] / 100000000), 0. });
		}
		return temp;
	}

	inline void hnSwap(double& x, double& y)//inline warning?
	{
		double t = x;
		x = y;
		y = t;
	}

	inline int64_t hnRev(int64_t x, int64_t len)//inline warning?
	{
		int64_t ans = 0, i;
		for (i = 0; i < len; i++) {
			ans <<= 1;
			ans |= (x & 1);
			x >>= 1;
		}
		return ans;
	}

	inline void hnFFT(std::vector<double>& reA, std::vector<double>& inA, bool flag)//fft
	{
		const double hnpi = 3.14159265358979323846;
		int64_t s, i;
		const int64_t n = reA.size();
		const int64_t lgn = log(n) / log(2);
		for (i = 0; i < n; i++) {
			int64_t j = hnRev(i, lgn);
			if (j > i) {
				hnSwap(reA[i], reA[j]);
				hnSwap(inA[i], inA[j]);
			}
		}
		for (s = 1; s <= lgn; s++) {
			int64_t m = (1ll << s);
			double reWm = cos(2 * hnpi / m), inWm = sin(2 * hnpi / m);
			if (flag)
				inWm = -inWm;
			for (int64_t k = 0; k < n; k += m) {
				double reW = 1.0, inW = 0.0;
				for (int64_t j = 0; j < m / 2; j++) {
					int64_t tag = k + j + m / 2;
					double reT = reW * reA[tag] - inW * inA[tag];
					double inT = reW * inA[tag] + inW * reA[tag];
					double reU = reA[k + j], inU = inA[k + j];
					reA[k + j] = reU + reT;
					inA[k + j] = inU + inT;
					reA[tag] = reU - reT;
					inA[tag] = inU - inT;
					double rew_t = reW * reWm - inW * inWm;
					double inw_t = reW * inWm + inW * reWm;
					reW = rew_t;
					inW = inw_t;
				}
			}
		}
		if (flag)
			for (i = 0; i < n; i++) {
				reA[i] /= n;
				inA[i] /= n;
			}
	}
}
