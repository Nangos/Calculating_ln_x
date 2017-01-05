#ifndef PRECISE_H
#define PRECISE_H
#include <string>

class Precise{
private:
	char* digits; //���λ������
	int length; //�ܣ���Ч��λ��
	int dot; //С��λ��
	bool sym; //����λ
	int invalid; //��Чλ��
	char at(int k){ //�ڲ�������ȡ�õ�kλ����λk=0����ֻ����Խ�緵��0
		int index = k + (dot + invalid);
		return (index < invalid || index >= length+invalid) ? 0 : digits[index];
	}
	char& operator[] (int k){ //�ڲ�������ȡ�õ�kλ����λk=0������д��������Խ��
		int index = k + (dot + invalid);
		return digits[index];
	}
	static int abscmp(Precise& a, Precise& b); //�ڲ��������Ƚ���������ֵ��С
	Precise& operator++ (); //�ڲ���������������ʱ������
	bool isZero(){ //�ڲ��������ж�ĳ���Ƿ�Ϊ��
		for (int i = -dot; i < length - dot; i++){
			if ((*this)[i] > 0) return false;
		}
		return true;
	}
	Precise add_sub(Precise& q, bool sub); //�ڲ�������ͳһ�Ӽ�

public:
	Precise(int length, int dot); //������Чλ��Ϊlength��С��λ��Ϊdot����
	Precise(std::string str); //���ַ�������
	Precise(std::string str, int dot); //���ַ����Ը���С��λ������
	Precise(Precise& another); //��������
	Precise(Precise& another, int length); //������Чλ���Ŀ�������
	~Precise(); //����

	std::string toString(); //ת��Ϊ�ַ���
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

	Precise operator<< (int n){ //����һλ��ʾ��10
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