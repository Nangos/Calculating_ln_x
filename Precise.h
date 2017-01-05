#ifndef PRECISE_H
#define PRECISE_H
#include <string>

class Precise{
private:
	char* digits; //存放位的数组
	int length; //总（有效）位数
	int dot; //小数位数
	bool sym; //符号位
	int invalid; //无效位数
	char at(int k){ //内部函数，取得第k位（个位k=0），只读，越界返回0
		int index = k + (dot + invalid);
		return (index < invalid || index >= length+invalid) ? 0 : digits[index];
	}
	char& operator[] (int k){ //内部函数，取得第k位（个位k=0），可写，不可以越界
		int index = k + (dot + invalid);
		return digits[index];
	}
	static int abscmp(Precise& a, Precise& b); //内部函数，比较两数绝对值大小
	Precise& operator++ (); //内部函数，四舍五入时用来入
	bool isZero(){ //内部函数，判断某数是否为零
		for (int i = -dot; i < length - dot; i++){
			if ((*this)[i] > 0) return false;
		}
		return true;
	}
	Precise add_sub(Precise& q, bool sub); //内部函数，统一加减

public:
	Precise(int length, int dot); //构造有效位数为length，小数位数为dot的数
	Precise(std::string str); //用字符串构造
	Precise(std::string str, int dot); //用字符串以给定小数位数构造
	Precise(Precise& another); //拷贝构造
	Precise(Precise& another, int length); //给定有效位数的拷贝构造
	~Precise(); //析构

	std::string toString(); //转换为字符串
	int getLength(){
		return length;
	}
	int getDot(){
		return dot;
	}
	bool getSym(){
		return sym;
	}

	Precise operator+ (Precise& q){
		return add_sub(q, false);
	}
	Precise operator- (Precise& q){
		return add_sub(q, true);
	}
	Precise operator- ();
	Precise operator* (Precise& q);
	Precise operator* (int n);
	Precise operator/ (Precise& q);
	Precise operator/ (int n);

	Precise operator<< (int n){ //左移一位表示乘10
		Precise r(*this);
		r.dot -= n;
		return r;
	}
	Precise operator>> (int n){
		return (*this) << (-n);
	}

	Precise operator= (Precise& another);

	bool operator== (Precise& o){
		return sym ^ o.sym ? ((*this).isZero() && o.isZero()) : abscmp(*this, o) == 0;
	}
	bool operator> (Precise& o){
		return sym ? (o.sym ? abscmp(*this, o) > 0 : !((*this).isZero() && o.isZero())): 
			(o.sym ? false : abscmp(o, *this) > 0);
	}
	bool operator< (Precise& o){
		return o > *this;
	}
	bool operator>= (Precise& o){
		return !(o > *this);
	}
	bool operator<= (Precise& o){
		return !(*this > o);
	}
	bool operator!= (Precise& o){
		return !(*this == o);
	}
};

#endif