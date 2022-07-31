#include <iostream>
#include <string>
#include <cmath>
#include"func.h"
using namespace std;

//返回Tj常量值
string T(int j) {
	if (0 <= j && j <= 15) {
		return "79CC4519";
	}
	else {
		return "7A879D8A";
	}
}

//布尔函数FF
string FF(string str1, string str2, string str3, int j) {
	if (0 <= j && j <= 15) {
		return XOR(XOR(str1, str2), str3);
	}
	else {
		return OR(OR(AND(str1, str2), AND(str1, str3)), AND(str2, str3));
	}
}

//布尔函数GG
string GG(string str1, string str2, string str3, int j) {
	if (0 <= j && j <= 15) {
		return XOR(XOR(str1, str2), str3);
	}
	else {
		return OR(AND(str1, str2), AND(NOT(str1), str3));
	}
}

//置换函数P0
string P0(string str) {
	return XOR(XOR(str, shift(str, 9)), shift(str, 17));
}

//置换函数P1
string P1(string str) {
	return XOR(XOR(str, shift(str, 15)), shift(str, 23));
}

//mod 2^32运算的函数实现
string ModAdd(string str1, string str2) {
	string res1 = Hex2Bin(str1);
	string res2 = Hex2Bin(str2);
	char temp = '0';
	string res = "";
	for (int i = res1.size() - 1; i >= 0; i--) {
		res = binXor(binXor(res1[i], res2[i]), temp) + res;
		if (binAnd(res1[i], res2[i]) == '1') {
			temp = '1';
		}
		else {
			if (binXor(res1[i], res2[i]) == '1') {
				temp = binAnd('1', temp);
			}
			else {
				temp = '0';
			}
		}
	}
	return Bin2Hex(res);
}

//消息填充，输入str为十进制，输出为十六进制字串
string padding(string str) {
	string res = "";
	for (int i = 0; i < str.size(); i++) {
		res += Dec2Hex((int)str[i]);
	}
	int res_length = res.size() * 4;
	res += "8";
	while (res.size() % 128 != 112) {
		res += "0";
	}
	string res_len = Dec2Hex(res_length);
	while (res_len.size() != 16) {
		res_len = "0" + res_len;
	}
	res += res_len;
	return res;
}


//消息扩展
string extension(string str) {
	string res = str;
	for (int i = 16; i < 68; i++) {
		res += XOR(XOR(P1(XOR(XOR(res.substr((i - 16) * 8, 8), res.substr((i - 9) * 8, 8)), shift(res.substr((i - 3) * 8, 8), 15))), shift(res.substr((i - 13) * 8, 8), 7)), res.substr((i - 6) * 8, 8));
	}
	for (int i = 0; i < 64; i++) {
		res += XOR(res.substr(i * 8, 8), res.substr((i + 4) * 8, 8));
	}
	return res;
}

//消息压缩
string compress(string str1, string str2) {
	string IV = str2;
	string A = IV.substr(0, 8), B = IV.substr(8, 8), C = IV.substr(16, 8), D = IV.substr(24, 8), E = IV.substr(32, 8), F = IV.substr(40, 8), G = IV.substr(48, 8), H = IV.substr(56, 8);
	string SS1 = "", SS2 = "", TT1 = "", TT2 = "";
	for (int j = 0; j < 64; j++) {
		SS1 = shift(ModAdd(ModAdd(shift(A, 12), E), shift(T(j), (j % 32))), 7);
		SS2 = XOR(SS1, shift(A, 12));
		TT1 = ModAdd(ModAdd(ModAdd(FF(A, B, C, j), D), SS2), str1.substr((j + 68) * 8, 8));
		TT2 = ModAdd(ModAdd(ModAdd(GG(E, F, G, j), H), SS1), str1.substr(j * 8, 8));
		D = C;
		C = shift(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = shift(F, 19);
		F = E;
		E = P0(TT2);
	}
	string res = (A + B + C + D + E + F + G + H);
	return res;
}

//迭代压缩函数
string iteration(string str) {
	int num = str.size() / 128;
	string V = "7380166F4914B2B9172442D7DA8A0600A96F30BC163138AAE38DEE4DB0FB0E4E";	//初始IV
	string B = "", extensionB = "", compressB = "";
	for (int i = 0; i < num; i++) {
		B = str.substr(i * 128, 128);
		extensionB = extension(B);
		compressB = compress(extensionB, V);
		V = XOR(V, compressB);
	}
	return V;
}

string lenth_attack_iteration(string str, string iv) {
	int num = str.size() / 128;
	string V = iv;
	string B = "", extensionB = "", compressB = "";
	for (int i = 0; i < num; i++) {
		B = str.substr(i * 128, 128);
		extensionB = extension(B);
		compressB = compress(extensionB, V);
		V = XOR(V, compressB);
	}
	return V;
}

void lenth_attack(string m1, string m2) {
	string V = "7380166F4914B2B9172442D7DA8A0600A96F30BC163138AAE38DEE4DB0FB0E4E";	//初始IV
	string pad = padding(m1);
	string x1 = lenth_attack_iteration(pad, V);
	string x2 = lenth_attack_iteration(pad + m2, V);

	cout << "原哈希结果：" << endl;
	for (int i = 0; i < x2.size() / 64; i++) {
		for (int j = 0; j < 8; j++) {
			cout << x2.substr(i * 64 + j * 8, 8) << "  ";
		}
		cout << endl;
	}

	string x3 = lenth_attack_iteration(m2, x1);

	cout << "长度扩展后哈希结果：" << endl;
	for (int i = 0; i < x3.size() / 64; i++) {
		for (int j = 0; j < 8; j++) {
			cout << x3.substr(i * 64 + j * 8, 8) << "  ";
		}
		cout << endl;
	}
	if (x2 == x3) {
		cout << "攻击成功！" << endl;
	}
}



//int main() {//主函数
//	string str_test = "abc";
//	cout << "输入消息为字符串: " + str_test << endl;
//	cout << endl;
//	string paddingValue = padding(str_test);
//	cout << "填充后的消息为：" << endl;
//	for (int i = 0; i < paddingValue.size() / 64; i++) {
//		for (int j = 0; j < 8; j++) {
//			cout << paddingValue.substr(i * 64 + j * 8, 8) << "  ";
//		}
//		cout << endl;
//	}
//	string result = iteration(paddingValue);
//	cout << "最终的杂凑值为：" << endl;
//	for (int i = 0; i < 8; i++) {
//		cout << result.substr(i * 8, 8) << "  ";
//	}
//	cout << endl;
//	return 0;
//}

int main() {
	string r1 = "abcd";
	string r2 = "hutao";
	lenth_attack(r1, r2);
	return 0;
}