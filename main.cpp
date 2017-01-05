#include <iostream>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include "Precise.h"
#include "Methods.h"
using namespace std;

//判断输入是否合法：
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

//转换成时间显示：
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

//测试速度并log
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

		//输入：
		std::cout << "计算ln(x)，请输入x的值。有效数字默认为21位，可以在x后输入，以空格分隔。\n输入'e'退出。\n";
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
			std::cout << "x = " << s << " 不是一个正数！\n\n" << endl;
			continue;
		}
		Precise q(s);
		if (q.getLength() == 0){
			std::cout << "x = " << s << " 不是一个正数！\n\n" << endl;
			continue;
		}
		if (t <= 0){
			std::cout << "给定的有效位数 " << t << " 不是一个正数！\n\n" << endl;
			continue;
		}
		Precise p(q, t + 2);

		//开始计算和计时：
		QueryPerformanceFrequency(&freq);
		std::cout << "正在计算 ln(" << s << ") ...有效数字为" << t << "位。\n";
		Precise result(0, 0);

		QueryPerformanceCounter(&tic);
		result = Methods::taylor(p, t);
		QueryPerformanceCounter(&toc);
		std::cout << "泰勒展开:  " << result.toString();
		std::cout << "  用时: " << to_time((toc.QuadPart - tic.QuadPart), freq.QuadPart) << "\n";

		QueryPerformanceCounter(&tic);
		result = Methods::integral(p, t);
		QueryPerformanceCounter(&toc);
		std::cout << "龙贝格法:  " << result.toString();
		std::cout << "  用时: " << to_time((toc.QuadPart - tic.QuadPart), freq.QuadPart) << "\n";

		QueryPerformanceCounter(&tic);
		result = Methods::rooting(p, t);
		QueryPerformanceCounter(&toc);
		std::cout << "开根迭代:  " << result.toString();
		std::cout << "  用时: " << to_time((toc.QuadPart - tic.QuadPart), freq.QuadPart) << "\n";
		std::cout << "\n\n";
	}
	/*下面代码段为时间测试用。
	while (true){
		string s;
		cin >> s;
		Precise q(s);
		test_speed(q, 10, "G:/test_ln3.txt");
	}
	*/
	return 0;
}

