#pragma once
#include <deque>
#include <stdexcept>
//#include <stdexcept>
#include "basic.hpp"

MANumCal

template <class _Ty>
class Expression {
public:
	FASTCALL Expression() noexcept
		:type(0ui8),
		belongType(0ui8), 
		num(NULL) {}
	FASTCALL Expression(
		uint8_t t, 
		uint8_t bt = 0ui8,
		_Ty n = NULL) noexcept 
		:type(t), belongType(bt), num(n) {}
	virtual ~Expression() noexcept = default;
	virtual Expression<_Ty>* copy() const 
	{ 
		return new Expression<_Ty>(*this);
	}
	void VECTORCALL set(uint8_t t, uint8_t bt, _Ty n) noexcept {
		type = t;
		belongType = bt;
		num = n;
	}	
	bool VECTORCALL belongToType(uint8_t t) noexcept {
		return !type && belongType == t;
	}

public:
	//bool isComplex;
	uint8_t type; // 0_belong, 1_==, 2_!=, 3_<, 4_<=, 5_>, 6_>=,7_notbelong
	uint8_t belongType; // 0-∅, 1-N, 2-Z+, 3-Z-, 4-Z, 5-Q, 6-R+, 7-R, 8-I, 9-C
	_Ty num; // == num
};

struct ExpOperate {
	static uint16_t state;
};
template <class _Ty> Expression<_Ty> operator == (ExpOperate& e, _Ty n)
{
	return Expression<_Ty>(1ui8, 0ui8, n);
}
template <class _Ty> Expression<_Ty> operator != (ExpOperate& e, _Ty n)
{
	return Expression<_Ty>(2ui8, 0ui8, n);
}
template <class _Ty> Expression<_Ty> operator <  (ExpOperate& e, _Ty n)
{
	return Expression<_Ty>(3ui8, 0ui8, n);
}
template <class _Ty> Expression<_Ty> operator <= (ExpOperate& e, _Ty n)
{
	return Expression<_Ty>(4ui8, 0ui8, n);
}
template <class _Ty> Expression<_Ty> operator >  (ExpOperate& e, _Ty n)
{
	return Expression<_Ty>(5ui8, 0ui8, n);
}
template <class _Ty> Expression<_Ty> operator >= (ExpOperate& e, _Ty n)
{
	return Expression<_Ty>(6ui8, 0ui8, n);
}

/*****************
* Aggregate
******************/
template <class _Ty>
class Aggregate :public Expression<_Ty> {
public:
	FASTCALL Aggregate() noexcept
		:isExpression(true), parameter(0ui8) {}
	explicit FASTCALL Aggregate(uint8_t& p) noexcept
		:isExpression(true), parameter(p) {}
	FASTCALL Aggregate(Expression<_Ty> le) noexcept
		:Expression<_Ty>(le) { ExpBTSimp(); }
	FASTCALL Aggregate(std::deque<Aggregate<_Ty>>& le) noexcept
		:isExpression(false), parameter(0ui8), list_exp(le) {}
	virtual ~Aggregate() noexcept = default;

	virtual Expression<_Ty>* copy() const override {
		return new Aggregate<_Ty>(*this);
	}

	void VECTORCALL aggcopy(const Aggregate<_Ty>& agg) {
		this->type = agg.type;
		this->belongType = agg.belongType;
		this->num = agg.num;
		isExpression = agg.isExpression;
		parameter = agg.parameter;
		list_exp = agg.list_exp;
	}
	void FASTCALL setBelongType(const uint8_t t) noexcept {
		this->type = 0ui8;
		this->belongType = t;
		this->num = NULL;
		isExpression = true;
		list_exp.clear();
	}
	void Simplification() noexcept {



	}
	void set_empty() noexcept { //空集
		this->type = 0ui8;
		this->belongType = 0ui8;
		this->num = NULL;
		isExpression = true;
		list_exp.clear();
	}
	void check() noexcept { //只有一个表达式的集合转换为表达式
		for (Aggregate<_Ty>& lexp : list_exp)
			if (lexp.list_exp.size() == 1) {
				lexp = lexp.list_exp.front();
				lexp.list_exp.clear();
			}
			else if (lexp.list_exp.size() > 1) {
				lexp.check();
			}
	}
	void VECTORCALL swap(Aggregate<_Ty>& agg) noexcept { //交换
		uint8_t t_type = this->type;
		this->type = agg.type;
		agg.type = t_type;
		uint8_t t_belongType = this->belongType;
		this->belongType = agg.belongType;
		agg.belongType = t_belongType;
		_Ty t_num = this->num;
		this->num = agg.num;
		agg.num = t_num;
		bool t_isExpression = isExpression;
		isExpression = agg.isExpression;
		agg.isExpression = t_isExpression;
		uint8_t t_parameter = parameter;
		parameter = agg.parameter;
		agg.parameter = t_parameter;
		list_exp.swap(agg.list_exp);
	}

	// Not
	void operator ~() noexcept {

	}
	// or
	Aggregate<_Ty> VECTORCALL operator |(Aggregate<_Ty>& a) noexcept {

	}
	void VECTORCALL operator |=(Aggregate<_Ty>& a) noexcept {

	}
	// and
	Aggregate<_Ty> VECTORCALL operator &(Aggregate<_Ty>& a) noexcept {

	}
	void VECTORCALL operator &=(Aggregate<_Ty>& a) noexcept {

	}
	// xor
	Aggregate<_Ty> VECTORCALL operator ^(Aggregate<_Ty>& a) noexcept {

	}
	void VECTORCALL operator ^=(Aggregate<_Ty>& a) noexcept {

	}
	// sub
	Aggregate<_Ty> VECTORCALL operator -(Aggregate<_Ty>& a) noexcept {

	}
	void VECTORCALL operator -=(Aggregate<_Ty>& a) noexcept {

	}
	// judge
	bool VECTORCALL operator ==(Aggregate<_Ty>& a) noexcept {

	}
	bool VECTORCALL operator !=(Aggregate<_Ty>& a) noexcept {

	}
	bool VECTORCALL belongTo(Aggregate<_Ty>& a) noexcept {

	}

public:
	bool isAllExp() const noexcept { //检查list_exp是否全为expression
		for (auto& le : list_exp) 
			if (!list_exp.empty())
				return false;
		return true;
	}
	void ExpSimp() { //Exp集合（可以对*this操作，内部无集合嵌套）化简
		if (!isAllExp()) 
			throw std::invalid_argument("Illegal element");
		//...
	}
	void ExpBTSimp() { // belong的基础化简
		if (!this->type) { //属于
			if (this->belongType == 6ui8) { //正实数 转换成 大于0
				this->type = 5ui8;
				this->belongType = 0ui8;
				this->num = 0;
			}
		}
		else if (this->type == 7ui8) { // 不属于
			if (!this->belongType) //空集
				throw std::invalid_argument("not belong to empty_set is not allow");
			else if (this->belongType == 6ui8) { //不属于正实数 转换成 小于等于0
				this->type = 4ui8;
				this->belongType = 0ui8;
				this->num = 0;
			}
			if (isComplexNum(this->num))//复数
				switch (this->belongType) {
				case 1ui8: //N -> Z-
					this->type = 0ui8;
					this->belongType = 3ui8;
					break;
				case 3ui8: //Z- -> N
					this->type = 0ui8;
					this->belongType = 1ui8;
					break;
				case 7ui8: //R -> I
					this->type = 0ui8;
					this->belongType = 8ui8;
					break;
				case 8ui8: //I -> R
					this->type = 0ui8;
					this->belongType = 7ui8;
					break;
				case 9ui8: //C -> ∅
					this->type = 0ui8;
					this->belongType = 0ui8;
					break;
				default:
					break;
				}
			else { //实数
				if (this->belongType > 7ui8)
					throw std::invalid_argument("invalid argument");
				switch (this->belongType) {
				case 1ui8: // N -> Z-
					this->type = 0ui8;
					this->belongType = 3ui8;
					break;
				case 3ui8: //Z- -> N
					this->type = 0ui8;
					this->belongType = 1ui8;
					break;
				case 7ui8: //R -> ∅
					this->type = 0ui8;
					this->belongType = 0ui8;
					break;
				default:
					break;
				}
			}
		}
		else if (isComplexNum(this->num))
			throw std::invalid_argument("invalid argument");
	}
	/***************
	以下未考虑复数!
	***************/
	void ExpExpComplement() { //取补集
		if (!isAllExp())
			throw std::invalid_argument("Illegal element");
		switch (this->type) {
		case 0ui8: //belong -> notbelong
			this->type = 7ui8;
			break;
		case 1ui8: //== -> !=
			this->type = 2ui8;
			break;
		case 2ui8: //!= -> ==
			this->type = 1ui8;
			break;
		case 3ui8: //< -> >=
			this->type = 6ui8;
			break;
		case 4ui8: //<= -> >
			this->type = 5ui8;
			break;
		case 5ui8: //> -> <=
			this->type = 4ui8;
			break;
		case 6ui8: //>= -> <
			this->type = 3ui8;
			break;
		case 7ui8: //notbelong -> belong
			this->type = 0ui8;
			break;
		default:
			throw std::runtime_error("Invalid switch parameter");
			break;
		}
		ExpBTSimp();
	}
	void VECTORCALL ExpExpIntersection(Aggregate<_Ty>& agg) { //取交集
		if (isComplexNum(this->num))
			throw std::invalid_argument("This operation cannot be completed on a complex number");//无法对复数操作
		else if (!isAllExp() || !agg.isAllExp())
			throw std::invalid_argument("Illegal element:Exp Exp");//只能对表达式操作
		ExpBTSimp();
		agg.ExpBTSimp();
		//...
		if (this->belongToType(0ui8) || agg.belongToType(0ui8)) { //有一个是空集
			set_empty();
			return; //空集
		}
		else if ((!this->type || this->type == 7ui8) && (!agg.type || agg.type == 7ui8)) { //两个个是bt
			if (this->type == agg.type) { //t类型相同
				if (this->belongType == agg.belongType) //完全相同
					return; //*this
				switch (this->belongType) {
				case 1ui8:// N
					switch (agg.belongType) {
					case 2ui8: //Z+
						aggcopy(agg); //Z+
						break;
					case 3ui8: //Z-
						set_empty(); //空集
						break;
					default: //*this N
						break;
					}
					return;
					break;
				case 2ui8: //Z+
					if (agg.belongType == 3)  //Z-
						set_empty();//空集
					return; //*this Z+
					break;
				case 3ui8: //Z-
					if (agg.belongType < 3ui8)  //N, Z+
						set_empty();//空集
					return; //*this Z-
					break;
				case 4ui8: //Z
					if (agg.belongType < 4ui8)  //N, Z+, Z-
						aggcopy(agg); //对应
					return; //*this Z
					break;
				case 5ui8: //Q
					if (agg.belongType < 5ui8) //N, Z+, Z-, Z
						aggcopy(agg); //对应
					return; //*this Q
					break;
					//6是R+已被转换成>0
				case 7ui8: //R
					aggcopy(agg); //对应
					return;
					break;
				default:
					throw std::runtime_error("Invalid switch parameter");
					break;
				}
			}
			else { //t类型不相同，一个是属于一个是不属于
				if (this->belongType == agg.belongType && this->type != agg.type) { //相反抵消
					set_empty();
					return; //空集
				}
				const uint8_t p_nbt = this->type ? this->belongType : agg.belongType; //type == 7 notbleong 的表达式
				if (p_nbt == 7ui8) { //不属于R
					set_empty();
					return; //空集
				}
				switch (!this->type ? this->belongType : agg.belongType) { //type == 0 belong的那个表达式
				case 1ui8: // N
					switch (p_nbt) {
					case 2ui8: //Z+
						this->set(1ui8, 0ui8, 0); // == 0
						break;
					case 3ui8: //Z-
						setBelongType(1ui8); //N
						break;
					default: //else
						set_empty(); //空集
						break;
					}
					return;
					break;
				case 2ui8: // Z+
					if (p_nbt != 3ui8)
						set_empty(); //空集
					else
						setBelongType(2ui8);//Z+
					return;
					break;
				case 3ui8: // Z-
					if (p_nbt < 3ui8)//N, Z+
						setBelongType(3ui8);//Z-
					else
						set_empty(); //空集
					return;
					break;
				case 4ui8: // Z
					if (p_nbt == 1ui8) //N
						setBelongType(3ui8);//Z-
					else if (p_nbt == 2ui8) { //Z+
						/* {Z-} U {0} */ //尽量缩小范围方便后续化简，所以不写成N/Z+
						set_empty();
						isExpression = false;
						Aggregate<_Ty> ta, tb;
						ta.setBelongType(3ui8);//Z-
						tb.set(1ui8, 0ui8, 0); // == 0;
						tb.parameter = 0ui8; //并
						list_exp.push_back(ta);
						list_exp.push_back(tb);
					}
					else if (p_nbt == 3ui8)//Z-
						setBelongType(1ui8); //N
					else
						set_empty(); //空集
					return;
					break;
				case 5ui8: { // Q
					set_empty();
					isExpression = false;
					Aggregate<_Ty> ta, tb;
					ta.setBelongType(5ui8);//Q
					tb.setBelongType(p_nbt);//*this
					tb.parameter = 3ui8; //minus
					list_exp.push_back(ta);
					list_exp.push_back(tb);
					return;//Q\*this
				} break;
				case 7ui8: {// R
					set_empty();
					isExpression = false;
					Aggregate<_Ty> ta, tb;
					ta.setBelongType(7ui8);//r
					tb.setBelongType(p_nbt);//*this
					tb.parameter = 3ui8; //minus
					list_exp.push_back(ta);
					list_exp.push_back(tb);
					return;//Q\*this
				} break;
				default:
					throw std::runtime_error("Invalid switch parameter");
					break;
				}
			}
		}
		else if ((!this->type || this->type == 7ui8) || (!agg.type || agg.type == 7ui8)) { //一个是bt 一个是t
			Aggregate<_Ty>* t;
			uint8_t bt;
			if (!this->type || this->type == 7ui8) {
				bt = this->belongType;
				t = &agg;
			}
			else {
				bt = agg.belongType;
				t = this;
			}
			switch (bt) {
			case 1ui8: //N
				switch (t->type) {
				case 1ui8:  //==
					if (isInt(t->num) && t->num >= _Ty(0)) //t->N
						aggcopy(*t); //N
					else
						set_empty();
					return;
					break;
				case 2ui8: //Z+
					if (isInt(t->num) && t->num > _Ty(0)) //t->Z+
						aggcopy(*t); //Z+
					else
						set_empty();
					return;
					break;
				case 3ui8: //Z-
					if (isInt(t->num) && t->num < _Ty(0)) //t->Z-
						aggcopy(*t); //Z-
					else
						set_empty();
					return;
					break;
				case 4ui8: //Z
					if (isInt(t->num)) //t->Z
						aggcopy(*t); //Z
					else
						set_empty();
					return;
					break;
				case 5ui8: //Q
					if (isRational(t->num)) //是有理数
						aggcopy(*t);
					else
						set_empty();
					return;
					break;
				case 7ui8: //R
					aggcopy(*t);
					return;
					break;
				default:
					break;
				}
				break;
			default:
				throw std::runtime_error("Invalid switch parameter");
				break;
			}
		}
		else { //集合表达式必须有序Xn <= Xn+1，如[5,6)|(6,7]不能写成[5,7]&!=6
		//...


		}
	}
	void VECTORCALL ExpExpUnion(Aggregate<_Ty>& agg) { //取并集
		if (!isAllExp() || !agg.isAllExp())
			throw std::invalid_argument("Illegal element");
		//...
	}
	void VECTORCALL ExpExpSub(Aggregate<_Ty>& agg) { //相减
		if (!isAllExp() || !agg.isAllExp())
			throw std::invalid_argument("Illegal element");
		//...
	}

	void AggComplement() { //取补集

	}
	void VECTORCALL AggExpIntersection(Aggregate<_Ty>& agg) { //取交集

	}
	void VECTORCALL AggExpUnion(Aggregate<_Ty>& agg) { //取并集

	}
	void VECTORCALL AggExpSub(Aggregate<_Ty>& agg) { //相减

	}

	void VECTORCALL AggAggIntersection(Aggregate<_Ty>& agg) { //取交集

	}
	void VECTORCALL AggAggUnion(Aggregate<_Ty>& agg) { //取并集

	}
	void VECTORCALL AggAggSub(Aggregate<_Ty>& agg) { //相减

	}
	/*集合方程
	* 0 slove (A, A = B)
	* 1 slove (A, A is a subset of B)
	* 2 slove (A, A is the proper subset of B)
	* 3 judge (A, A = B)
	* 4 judge (A, A is a subset of B)
	* 5 judge (A, A is the proper subset of B)
	*/
	void VECTORCALL AggEqualAgg(Aggregate<_Ty>& agg, uint16_t& state, uint16_t t = 0ui16) {

	}

public:
	bool isExpression;

public:
	uint8_t parameter;// 0_or并, 1_and交, 2_not补, 3_minus减 //第一个无效
	std::deque<Aggregate<_Ty>> list_exp;
};

MANumCalEND

#ifndef SET_EXPRESSION_EXPO
extern NAMESPACEMANumCal ExpOperate Expo;
#endif // !SET_EXPRESSION_EXPO

