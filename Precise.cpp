#include "Precise.h"
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

Precise::Precise(int length, int dot){
	this->length = length;
	this->dot = dot;
	this->sym = true;
	this->digits = new char[length];
	this->invalid = 0;
}

Precise::Precise(std::string str){
	if (str.find('.') == -1){
		str.append(1, '.');
	}
	this->sym = (str.at(0) != '-');
	this->length = str.length() - 1 - !this->sym;
	this->dot = str.length() - 1 - str.find('.');
	this->digits = new char[this->length];
	int index = this->length;
	for (unsigned i = 0; i < str.length(); i++){
		char c = str.at(i);
		if (c >= '0' && c <= '9'){
			index--;
			this->digits[index] = c - '0';
		}
	}
	for (int i = this->length - this->dot - 1; i >= -this->dot; i--){
		if ((*this)[i] == 0){
			this->length--;
		}
		else{
			break;
		}
	}
	this->invalid = 0;
}

Precise::Precise(std::string str, int dot){
	if (str.find('.') == -1){
		str.append(1, '.');
	}
	this->sym = (str.at(0) != '-');
	this->length = str.length() - 1 - !this->sym;
	int odot = str.length() - 1 - str.find('.');
	if (dot > odot){
		this->dot = dot;
		this->length += (dot - odot);
	}
	else{
		this->dot = odot;
	}
	this->digits = new char[this->length];
	int index = this->length;
	for (int i = 0; i < dot - odot; i++){
		this->digits[i] = 0;
	}
	for (unsigned i = 0; i < str.length(); i++){
		char c = str.at(i);
		if (c >= '0' && c <= '9'){
			index--;
			this->digits[index] = c - '0';
		}
	}
	for (int i = this->length - this->dot - 1; i >= -this->dot; i--){
		if ((*this)[i] == 0){
			this->length--;
		}
		else{
			break;
		}
	}
	this->invalid = 0;
}

Precise::Precise(Precise& another){
	this->length = another.length;
	this->dot = another.dot;
	this->sym = another.sym;
	this->digits = new char[this->length];
	int offset = another.invalid;
	for (int i = 0; i < this->length; i++){
		this->digits[i] = another.digits[i + offset];
	}
	this->invalid = 0;
}

Precise::Precise(Precise& another, int length){
	this->length = length;
	this->sym = another.sym;
	int diff = length - another.length;
	this->dot = another.dot + diff;
	this->digits = new char[this->length];
	int offset = another.invalid;

	if (diff >= 0){
		for (int i = 0; i < diff; i++){
			this->digits[i] = 0;
		}
		for (int i = diff; i < this->length; i++){
			this->digits[i] = another.digits[i - diff + offset];
		}
	}else{
		for (int i = 0; i < this->length; i++){
			this->digits[i] = another.digits[i - diff + offset];
		}
		if (another.digits[- diff - 1 + offset] >= 5){
			++(*this);
		}
	}
	this->invalid = 0;
}

Precise::~Precise(){
	delete[] this->digits;
}

std::string Precise::toString(){
	std::string str;
	if (!this->sym){
		str.append(1, '-');
	}
	if (this->dot >= 0){
		if (this->dot >= this->length){
			str.append("0.");
			str.append(this->dot - this->length, '0');
			for (int i = 0; i < this->length; i++){
				str.append(1, '0' + this->digits[this->length - 1 - i + this->invalid]);
			}
		}
		else{
			for (int i = 0; i < this->length - this->dot; i++){
				str.append(1, '0' + this->digits[this->length - 1 - i + this->invalid]);
			}
			str.append(1, '.');
			for (int i = this->length - this->dot; i < this->length; i++){
				str.append(1, '0' + this->digits[this->length - 1 - i + this->invalid]);
			}
		}
	}
	else{
		for (int i = 0; i < this->length; i++){
			str.append(1, '0' + this->digits[this->length - 1 - i + this->invalid]);
		}
		str.append(-this->dot, '0');
		str.append(1, '.');
	}
	return str;
}

int Precise::abscmp(Precise& a, Precise& b){
	int head = MAX(a.length - a.dot, b.length - b.dot) - 1;
	int tail = -MAX(a.dot, b.dot);
	for (int i = head; i >= tail; i--){
		int diff = a.at(i) - b.at(i);
		if (diff > 0){
			return 1;
		}
		else if (diff < 0){
			return -1;
		}
	}
	return 0;
}

Precise& Precise::operator++ (){
	bool flag = false;
	for (int i = -dot; i < length - dot; i++){
		if ((*this)[i] < 9){
			(*this)[i]++;
			flag = true;
			break;
		}
		else{
			(*this)[i] = 0;
		}
	}
	if (!flag){
		dot--;
		(*this)[length - dot - 1] = 1;
	}
	return *this;
}

Precise Precise::operator= (Precise& another){
	this->length = another.length;
	this->dot = another.dot;
	this->sym = another.sym;
	if (this->digits != NULL){
		delete[] this->digits;
	}
	this->digits = new char[this->length];
	int offset = another.invalid;
	for (int i = 0; i < this->length; i++){
		this->digits[i] = another.digits[i + offset];
	}
	this->invalid = 0;
	return *this;
}

Precise Precise::add_sub(Precise& q, bool sub){
	Precise& p = *this;
	int int_part = MAX(p.length - p.dot, q.length - q.dot) + 1;
	int dec_part = MIN(p.dot, q.dot);
	Precise r(int_part + dec_part, dec_part);

	if (p.sym ^ q.sym ^ sub){
		Precise *top, *btm;
		if (abscmp(p, q) > 0){
			top = &p;
			btm = &q;
		}
		else{
			top = &q;
			btm = &p;
		}
		if (sub){
			r.sym = p >= q;
		}
		else{
			r.sym = top->sym;
		}
		int carry = (top->at(-dec_part - 1) - btm->at(-dec_part - 1) >= 5) ? 1 : (
			(top->at(-dec_part - 1) - btm->at(-dec_part - 1) <= -5) ? -1 : 0);
		for (int i = -dec_part; i < int_part; i++){
			r[i] = top->at(i) - btm->at(i) + carry;
			if (r[i] < 0){
				r[i] += 10;
				carry = -1;
			}
			else{
				carry = 0;
			}
		}
	}
	else{
		r.sym = p.sym;
		int carry = (p.at(-dec_part - 1) + q.at(-dec_part - 1) >= 5);
		for (int i = -dec_part; i < int_part; i++){
			r[i] = p.at(i) + q.at(i) + carry;
			if (r[i] >= 10){
				r[i] -= 10;
				carry = 1;
			}
			else{
				carry = 0;
			}
		}
	}

	for (int i = int_part - 1; i >= -dec_part; i--){
		if (r[i] == 0){
			r.length--;
		}
		else{
			break;
		}
	}
	return r;
}

Precise Precise::operator- (){
	Precise r(*this);
	r.sym = !r.sym;
	return r;
}

Precise Precise::operator* (Precise& q){
	Precise& p = *this;
	Precise r(p.length + q.length, p.dot + q.dot);
	r.sym = !(p.sym ^ q.sym);
	for (int i = -r.dot; i < r.length - r.dot; i++){
		r[i] = 0;
	}
	char temp;
	for (int i = -p.dot; i < p.length - p.dot; i++){
		for (int j = -q.dot; j < q.length - q.dot; j++){
			temp = p[i] * q[j] + r[i + j];
			r[i + j] = temp % 10;
			r[i + j + 1] += temp / 10;
		}
	}
	int shift = MAX(p.length, q.length);
	for (int i = r.length - r.dot - 1; r[i] == 0; i--){
		if (shift > 0) shift--;
	}
	bool plus1 = r.at(-r.dot + shift - 1) >= 5;
	r.invalid = shift;
	r.dot -= shift;
	r.length -= shift;
	if (plus1){
		++r;
	}
	for (int i = -r.dot + r.length - 1; i >= -r.dot; i--){
		if (r[i] == 0){
			r.length--;
		}
		else{
			break;
		}
	}
	return r;
}

Precise Precise::operator* (int n){
	Precise q(10, 0);
	if (n < 0){
		q.sym = false;
		n = -n;
	}
	else{
		q.sym = true;
	}
	int digcnt;
	for (digcnt = 0; digcnt < 10; digcnt++){
		if (n == 0) break;
		q[digcnt] = n % 10;
		n /= 10;
	}
	q.length = digcnt;

	Precise& p = *this;
	Precise r(p.length + q.length, p.dot + q.dot);
	r.sym = !(p.sym ^ q.sym);
	for (int i = -r.dot; i < r.length - r.dot; i++){
		r[i] = 0;
	}
	char temp;
	for (int i = -p.dot; i < p.length - p.dot; i++){
		for (int j = -q.dot; j < q.length - q.dot; j++){
			temp = p[i] * q[j] + r[i + j];
			r[i + j] = temp % 10;
			r[i + j + 1] += temp / 10;
		}
	}
	int shift = q.length;
	for (int i = r.length - r.dot - 1; r[i] == 0; i--){
		if (shift > 0) shift--;
	}
	bool plus1 = r.at(-r.dot + shift - 1) >= 5;
	r.invalid = shift;
	r.dot -= shift;
	r.length -= shift;
	if (plus1){
		++r;
	}
	for (int i = -r.dot + r.length - 1; i >= -r.dot; i--){
		if (r[i] == 0){
			r.length--;
		}
		else{
			break;
		}
	}
	return r;
}

Precise Precise::operator/ (Precise& q){
	Precise& p = *this;
	if (p.isZero()){
		Precise r(0, 0);
		return r;
	}
	for (int i = q.length - q.dot - 1; q[i] == 0; i--){
		q.length--;
	}
	Precise r(MIN(p.length, q.length), 0);
	r.sym = !(p.sym ^ q.sym);
	int shift = 0;
	int curs_p = p.length - p.dot - 1;
	while (true){
		for (int i = 0; i < q.length; i++){
			int diff = p.at(curs_p - i) - q.at(q.length - q.dot - 1 - i + shift);
			if (diff > 0){
				goto break_;
			}
			else if (diff < 0){
				goto continue_;
			}
		}
	break_:
		break;
	continue_:
		shift++;
		continue;
	}
	curs_p = curs_p - shift - q.length + 1;
	Precise t(q.length + 1, 0);
	t.sym = true;
	Precise s(q);
	s.sym = true;
	for (int i = t.length - 1; i >= 0; i--){
		t[i] = p.at(curs_p + i);
	}
	s.dot = 0;
	for (int i = r.length - 1; i >= 0; i--){
		r[i] = 0;
		while (t >= s){
			t = t - s;
			r[i]++;
		}
		Precise temp(q.length + 1, 0);
		temp.sym = true;
		for (int j = temp.length - 1; j >= 1; j--){
			temp[j] = t.at(j - 1);
		}
		curs_p--;
		temp[0] = p.at(curs_p);
		t = temp;
	}
	r.dot = r.length - (p.length - p.dot - q.length + 1 - shift) - q.dot;
	if (t >= s + s + s + s + s){
		++r;
	}
	return r;
}

Precise Precise::operator/ (int n){
	Precise& p = *this;
	if (p.isZero()){
		Precise r(0, 0);
		return r;
	}
	Precise r(p.length, 0);
	if (n < 0){
		n = -n;
		r.sym = !p.sym;
	}
	else{
		r.sym = p.sym;
	}
	int shift = -1;
	int pool = 0;
	int curs = p.length - p.dot;
	while (pool < n){
		curs--;
		pool *= 10;
		pool += p.at(curs);
		shift++;
	}
	for (int i = r.length - 1; i >= 0; i--){
		r[i] = 0;
		while (pool >= n){
			pool -= n;
			r[i]++;
		}
		curs--;
		pool *= 10;
		pool += p.at(curs);
	}
	r.dot = p.dot + shift;
	if (pool >= n * 5){
		++r;
	}
	return r;
}