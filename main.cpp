#include <iostream>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include "Precise.h"
#include "Methods.h"
using namespace std;

//�ж������Ƿ�Ϸ���
bool isValidNumber(string s){
	bool dot = false;
	for (int i = 0; i < s.length(); i++){
		char c = s.at(i);
		if (c == '.'){
			if (dot) return false;
			dot = true;
		}
		else{
			if (c < '0' || c > '9') return false;
		}
	}
	return true;
}

//ת����ʱ����ʾ��
string to_time(LONGLONG interval, LONGLONG freq){
	LONGLONG us = interval * 1000000 / freq;
	string str;
	stringstream iss;
	if (us > 10000000){
		iss << (us / 1000000) << "s";
	}
	else if (us > 10000){
		iss << (us / 1000) << "ms";
	}
	else{
		iss << us << "us";
	}
	iss >> str;
	return str;
}

//�����ٶȲ�log
void test_speed(Precise& p, int max_seconds, string filename){
	ofstream ofs(filename);
	LARGE_INTEGER tic, toc, freq;
	ofs << "p = " << p.toString() << "Testing speed" << endl;

	ofs << "\nTaylor:" << endl;
	cout << "\nTaylor:" << endl;
	for (int t = 10; true; t += t / 10){
		for (int k = 0; k < 3; k++){
			Precise q(p, t + 2);
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&tic);
			Precise r = Methods::taylor(q, t);
			QueryPerformanceCounter(&toc);
			ofs << t << " " << (toc.QuadPart - tic.QuadPart) << " " << freq.QuadPart << endl;
			cout << t << " " << (toc.QuadPart - tic.QuadPart) << " " << freq.QuadPart << endl;
			if (((toc.QuadPart - tic.QuadPart) / freq.QuadPart) >= max_seconds){
				goto break1_;
			}
		}
	}
	break1_:
	ofs << "\nIntegral:" << endl;
	cout << "\nIntegral:" << endl;
	for (int t = 10; true; t += t / 10){
		for (int k = 0; k < 3; k++){
			Precise q(p, t + 2);
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&tic);
			Precise r = Methods::integral(q, t);
			QueryPerformanceCounter(&toc);
			ofs << t << " " << (toc.QuadPart - tic.QuadPart) << " " << freq.QuadPart << endl;
			cout << t << " " << (toc.QuadPart - tic.QuadPart) << " " << freq.QuadPart << endl;
			if (((toc.QuadPart - tic.QuadPart) / freq.QuadPart) >= max_seconds){
				goto break2_;
			}
		}
	}
	break2_:
	ofs << "\nRooting:" << endl;
	cout << "\nRooting:" << endl;
	for (int t = 10; true; t += t / 10){
		for (int k = 0; k < 3; k++){
			Precise q(p, t + 2);
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&tic);
			Precise r = Methods::rooting(q, t);
			QueryPerformanceCounter(&toc);
			ofs << t << " " << (toc.QuadPart - tic.QuadPart) << " " << freq.QuadPart << endl;
			cout << t << " " << (toc.QuadPart - tic.QuadPart) << " " << freq.QuadPart << endl;
			if (((toc.QuadPart - tic.QuadPart) / freq.QuadPart) >= max_seconds){
				goto break3_;
			}
		}
	}
	break3_:
	ofs << "\nDone." << endl;
	cout << "\nDone." << endl;
}

int main(){
	while (true){
		cin.clear();
		cin.sync();
		string line;
		stringstream iss;
		string s;
		int t;
		LARGE_INTEGER tic, toc, freq;

		//���룺
		std::cout << "����ln(x)��������x��ֵ����Ч����Ĭ��Ϊ21λ��������x�����룬�Կո�ָ���\n����'e'�˳���\n";
		getline(std::cin, line);
		iss.str(line);
		iss >> s;
		if (!(iss >> t)){
			t = 21;
		}
		if (s.length() == 0){
			continue;
		}
		if (s.at(0) == 'e'){
			break;
		}
		if (!isValidNumber(s)){
			std::cout << "x = " << s << " ����һ��������\n\n" << endl;
			continue;
		}
		Precise q(s);
		if (q.getLength() == 0){
			std::cout << "x = " << s << " ����һ��������\n\n" << endl;
			continue;
		}
		if (t <= 0){
			std::cout << "��������Чλ�� " << t << " ����һ��������\n\n" << endl;
			continue;
		}
		Precise p(q, t + 2);

		//��ʼ����ͼ�ʱ��
		QueryPerformanceFrequency(&freq);
		std::cout << "���ڼ��� ln(" << s << ") ...��Ч����Ϊ" << t << "λ��\n";
		Precise result(0, 0);

		QueryPerformanceCounter(&tic);
		result = Methods::taylor(p, t);
		QueryPerformanceCounter(&toc);
		std::cout << "̩��չ��:  " << result.toString();
		std::cout << "  ��ʱ: " << to_time((toc.QuadPart - tic.QuadPart), freq.QuadPart) << "\n";

		QueryPerformanceCounter(&tic);
		result = Methods::integral(p, t);
		QueryPerformanceCounter(&toc);
		std::cout << "������:  " << result.toString();
		std::cout << "  ��ʱ: " << to_time((toc.QuadPart - tic.QuadPart), freq.QuadPart) << "\n";

		QueryPerformanceCounter(&tic);
		result = Methods::rooting(p, t);
		QueryPerformanceCounter(&toc);
		std::cout << "��������:  " << result.toString();
		std::cout << "  ��ʱ: " << to_time((toc.QuadPart - tic.QuadPart), freq.QuadPart) << "\n";
		std::cout << "\n\n";
	}
	/*��������Ϊʱ������á�
	while (true){
		string s;
		cin >> s;
		Precise q(s);
		test_speed(q, 10, "G:/test_ln3.txt");
	}
	*/
	return 0;
}

