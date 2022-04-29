#pragma once

#ifndef NA_API
#define NA_API __declspec(dllexport)
#endif

//#include <tbb/tbbmalloc_proxy.h>
#define VECTORCALL __vectorcall
#define FASTCALL __fastcall

#define MANumCal namespace MA::NumCal {
#define MANumCalEND }
#define UNMANumCal using namespace MA::NumCal;
#define NAMESPACEMANumCal MA::NumCal::
#define STD std::

MANumCal

template <typename _Ty> bool isInt(_Ty& num); //�Ƿ�������
template <typename _Ty> bool isComplexNum(_Ty& num);//�Ƿ��Ǹ���
template <typename _Ty> bool isRational(_Ty& num);//�Ƿ���������
template <typename _Ty> bool isVector(_Ty& num);//�Ƿ���ʸ��

MANumCalEND