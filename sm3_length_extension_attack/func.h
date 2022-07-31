#pragma once
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

//二进制字串转换为十六进制
//四位二进制对应转换成一位十六进制
string Bin2Hex(string in) {
	string out = "";
	int temp = 0;
	while (in.size() % 4 != 0) {
		in = "0" + in;
	}
	for (int i = 0; i < in.size(); i += 4) {
		temp = (in[i] - '0') * 8 + (in[i + 1] - '0') * 4 + (in[i + 2] - '0') * 2 + (in[i + 3] - '0');
		if (temp < 10) {
			out += to_string(temp);
		}
		else {
			out += 'A' + (temp - 10);
		}
	}
	return out;
}

//十六进制字串转换为二进制
string Hex2Bin(string in) {
	string out = "";
	string table[16] = { "0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111" };
	for (int i = 0; i < in.size(); i++) {
		if (in[i] >= '0' && in[i] <= '9') {
			out += table[in[i] - '0'];
		}
		else if (in[i] >= 'A' && in[i] <= 'F') {
			out += table[in[i] - 'A' + 10];
		}
	}
	return out;
}

//十进制转换为十六进制
string Dec2Hex(int in) {
	string out = "";
	int temp = 0;
	while (in >= 1) {
		temp = in % 16;
		if (temp < 10 && temp >= 0) {
			out = to_string(temp) + out;
		}
		else {
			out += ('A' + (temp - 10));
		}
		in = in / 16;
	}
	return out;
}

//十进制转换为二进制的函数实现
string Dec2Bin(int in) {
	string out = "";
	while (in >= 1) {
		out = to_string(in % 2) + out;
		in = in / 2;
	}
	return out;
}

//循环左移n位
string shift(string str, int n) {
	string out = Hex2Bin(str);
	out = out.substr(n) + out.substr(0, n);
	return Bin2Hex(out);
}

//异或
string XOR(string str1, string str2) {
	string res1 = Hex2Bin(str1);
	string res2 = Hex2Bin(str2);
	string out = "";
	for (int i = 0; i < min(res1.size(),res2.size()); i++) {
		if (res1[i] == res2[i]) {
			out += "0";
		}
		else {
			out += "1";
		}
	}
	return Bin2Hex(out);
}

//与
string AND(string str1, string str2) {
	string res1 = Hex2Bin(str1);
	string res2 = Hex2Bin(str2);
	string out = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == '1' && res2[i] == '1') {
			out += "1";
		}
		else {
			out += "0";
		}
	}
	return Bin2Hex(out);
}

//或
string OR(string str1, string str2) {
	string res1 = Hex2Bin(str1);
	string res2 = Hex2Bin(str2);
	string out = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == '0' && res2[i] == '0') {
			out += "0";
		}
		else {
			out += "1";
		}
	}
	return Bin2Hex(out);
}

//非
string NOT(string str) {
	string res1 = Hex2Bin(str);
	string out = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == '0') {
			out += "1";
		}
		else {
			out += "0";
		}
	}
	return Bin2Hex(out);
}

//实现单比特的异或操作
char binXor(char str1, char str2) {
	return str1 == str2 ? '0' : '1';
}

//实现单比特的与操作
char binAnd(char str1, char str2) {
	return (str1 == '1' && str2 == '1') ? '1' : '0';
}