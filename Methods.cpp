#include "Methods.h"
#include <iostream>

Precise Methods::getSqrt(Precise& x, Precise& guess, int ndigit){
	Precise half_x(x, ndigit + 1);
	half_x = half_x / 2;
	Precise even(guess, ndigit + 1);
	Precise odd(0, 0);
	while (true){
		odd = half_x / even + even / 2;
		if ((odd - even).getLength() <= 1){
			return odd;
		}
		even = half_x / odd + odd / 2;
		if ((odd - even).getLength() <= 1){
			return even;
		}
	}
}

Precise Methods::getLn10(int ndigit){
	if (ndigit < 1000){
		Precise r(static_ln10().substr(0, ndigit + 1));
		return r;
	}
	Precise ten("10"), guess("3.16227766");
	Precise x = getSqrt(ten, guess, ndigit + 1);
	Precise one("1", ndigit + 1);
	Precise y = (x - one) / (x + one);
	Precise y2 = y * y;
	Precise yn = one;
	Precise err = one >> ndigit;
	Precise r = one;
	int k = 1;
	while (yn > err){
		k += 2;
		yn = yn * y2;
		r = r + yn / k;
	}
	r = r * y * 4;
	return r;
}

Precise Methods::taylor(Precise& x, int ndigit){
	Precise one("1", ndigit + 2);
	if (x == one){
		Precise zero("0");
		return zero;
	}
	int level = x.getLength() - x.getDot();
	Precise x1 = x >> level;
	Precise thr("0.316");
	if (x1 < thr){
		x1 = x1 << 1;
		level--;
	}
	Precise upper(x1 - one, ndigit + 2);
	Precise y = upper / (x1 + one);
	Precise y2 = y * y;
	Precise yn = one;
	Precise err = one >> ndigit;
	Precise r = one;
	int k = 1;
	while (yn > err){
		k += 2;
		yn = yn * y2;
		r = r + yn / k;
	}
	r = r * y * 2;
	Precise s(r, ndigit + 2);
	if (level != 0){
		s = s + getLn10(ndigit + 2) * level;
	}
	Precise result(s, ndigit);
	return result;
}

Precise Methods::integral_short(Precise& x, int ndigit){
	Precise one("1", ndigit + 2);
	if (x == one){
		Precise zero("0");
		return zero;
	}
	Precise h(x - one, ndigit + 2);
	bool symflag = true;
	if (!h.getSym()) {
		h = -h;
		symflag = false;
	}
	Precise* initial = new Precise((one + one / x) * (h / 2));
	Precise* mid = new Precise((one + x) / 2, ndigit + 2);
	Precise** list = new Precise*[1];
	Precise** prev = NULL;
	list[0] = initial;
	Precise** samples = new Precise*[1];
	Precise** samples_prev = NULL;
	
	samples[0] = mid;
	int sample_len = 1;
	int final_iter;
	Precise result(0, 0);
	
	for (int ii = 1; true; ii++) {
		prev = list;
		int i = (ii > 13) ? 13 : ii;
		list = new Precise*[i + 1];
		list[0] = new Precise(*prev[0]);
		for (int j = 0; j < sample_len; j++) {
			*list[0] = *list[0] + h / (*samples[j]);
		}
		*list[0] = *list[0] / 2;
		
		int coef = 1;
		Precise ref(*prev[i - 1]);
		for (int j = 0; j < i; j++) {
			coef *= 4;
			list[j + 1] = new Precise(((*list[j])*coef - (*prev[j])) / (coef - 1));
			delete prev[j];
		}
		delete[] prev;
		
		if ((*list[i]).getLength() - (*list[i] - ref).getLength() >= ndigit){
			result = *list[i];
			final_iter = i + 1;
			break;
		}

		samples_prev = samples;
		h = h / 2;
		Precise half = h / 2;
		sample_len *= 2;
		samples = new Precise*[sample_len];
		for (int j = 0; j < sample_len / 2; j++) {
			samples[2 * j] = new Precise(*samples_prev[j] - half);
			samples[2 * j + 1] = new Precise(*samples_prev[j] + half);
			delete samples_prev[j];
		}
		delete[] samples_prev;
	}
	for (int i = 0; i < final_iter; i++) {
		delete list[i];
	}
	delete[] list;
	for (int i = 0; i < sample_len; i++) {
		delete samples[i];
	}
	delete[] samples;

	if (!symflag) {
		result = -result;
	}
	return result;
}

Precise Methods::integral(Precise& x, int ndigit){
	Precise one("1", ndigit + 2);
	if (x == one){
		Precise zero("0");
		return zero;
	}
	int level = x.getLength() - x.getDot();
	Precise x1 = x >> level;
	Precise thr("0.316");
	if (x1 < thr){
		x1 = x1 << 1;
		level--;
	}
	Precise r = integral_short(x1, ndigit + 1);
	if (level != 0){
		if (r.getLength() > 0){
			r = r + getLn10(ndigit+2) * level;
		}
		else{
			r = getLn10(ndigit+2) * level;
		}
	}
	Precise cut_result(r, ndigit);
	return cut_result;
}

Precise Methods::get10thRoot(Precise& x_minus_1, Precise& x0_minus_1, int ndigit){
	Precise prev(x0_minus_1, ndigit + 2);
	Precise now(0, 0);
	Precise one("1", ndigit + 2);
	Precise k(x_minus_1, ndigit + 2);
	int coef[] = { 1, 9, 36, 84, 126, 126, 84, 36, 9, 1 };
	while (true){
		Precise xn = prev;
		Precise item = xn * 9;
		Precise nano = item;
		Precise base = item;
		for (int i = 2; true; i++){
			if (item < xn >> ndigit + 2) break;
			if (i > 2){
				base = base + item;
			}
			if (i == 10) break;
			xn = xn * prev;
			item = xn * coef[i];
		}
		now = (nano + (k - base) / (one + base)) >> 1;
		if ((now-prev).getLength() <= 1){
			return now;
		}
		prev = now;
	}
}

Precise Methods::guess_init(Precise& x){
	std::string thr[] = { "3.00", "2.00", "1.000", "0.500", "0.100", "0.010" };
	std::string guess[] = { "0.149", "0.116", "0.072", "0.041", "0.0095", "0.0010" };
	Precise btm(thr[5]);
	Precise res(0, 0);
	if (x <= btm){
		res = x >> 1;
		return res;
	}
	for (int i = 0; true; i++){
		Precise upper(thr[4 - i]);
		Precise bigger(guess[4 - i]);
		if (x == upper){
			res = bigger;
			return bigger;
		}
		else if (x < upper){
			Precise lower(thr[5 - i]);
			Precise smaller(guess[5 - i]);
			res = bigger * (x - lower) + smaller * (upper - x);
			res = res / (upper - lower);
			return res;
		}
	}
}

Precise Methods::rooting(Precise& x, int ndigit){
	int nndigit = ndigit;
	if (ndigit < 3){
		ndigit = 3;
	}
	bool minus = false;
	Precise one("1", ndigit + 2);
	if (x == one){
		Precise zero("0");
		return zero;
	}
	int level = x.getLength() - x.getDot();
	Precise x1 = x >> level;
	Precise res(0, 0);
	Precise thr("0.316");
	if (x1 < thr){
		x1 = x1 << 1;
		level--;
	}
	if (x1 != one){
		if (x1 < one){
			x1 = one / x1;
			minus = true;
		}
		Precise prev(x1 - one, ndigit + 2);
		Precise now(0, 0);
		int iter = 0;
		while (true){
			Precise guess = guess_init(prev);
			now = get10thRoot(prev, guess, ndigit + 2);
			iter++;
			res = now << iter;
			Precise err = res - (prev << (iter - 1));
			if (res.getLength() - err.getLength() >= ndigit + 1){
				break;
			}
			prev = now;
		}
	}
	if (minus){
		res = -res;
	}
	if (level != 0){
		if (res.getLength() > 0){
			res = res + getLn10(ndigit + 2) * level;
		}
		else{
			res = getLn10(ndigit + 2) * level;
		}
	}
	Precise result(res, nndigit);
	return result;
}