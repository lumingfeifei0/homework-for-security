#include<iostream>
#include"va.h"

using namespace std;

u32 rk[32];


//循环左移，输入为1字
u32 shift(u32 in, int num)
{
	u32 out;
	out = (in << num) | (in >> (32 - num));
	return out;
}

//S盒置换，输入为1字节
u8 find_Sbox(u8 in)
{
	return Sbox[in >> 4][in & 0x0f];
}

//1个字分4个字节找S盒置换，输入为1字
u32 Tran_Sbox(u32 in)
{
	u8 temp[4];
	u32 out;
	temp[0] = find_Sbox((u8)(in >> 24) & 0xff);
	temp[1] = find_Sbox((u8)(in >> 16) & 0xff);
	temp[2] = find_Sbox((u8)(in >> 8) & 0xff);
	temp[3] = find_Sbox((u8)(in) & 0xff);
	out = ((u32)temp[0] << 24) + ((u32)temp[1] << 16) + ((u32)temp[2] << 8) + ((u32)temp[3]);
	return out;
}

//T'变换
u32 Tfunc_(u32 in)
{
	u32 out;
	u32 temp;
	temp = Tran_Sbox(in);
	out = temp ^ shift(temp, 13) ^ shift(temp, 23);
	return out;
}

//T变换
u32 Tfunc(u32 in)
{
	u32 out;
	u32 temp;
	temp = Tran_Sbox(in);
	out = temp ^ shift(temp, 2) ^ shift(temp, 10) ^ shift(temp, 18) ^ shift(temp, 24);
	return out;
}

//密钥扩展算法，生成全局变量轮密钥
void get_rk(u32 MK[4])
{
	u32 K[4];
	
	for (int i = 0; i < 4; i++)
	{
		K[i] = MK[i] ^ FK[i];
	}

	rk[0] = K[0] ^ Tfunc_(K[1] ^ K[2] ^ K[3] ^ CK[0]);
	rk[1] = K[1] ^ Tfunc_(K[2] ^ K[3] ^ rk[0] ^ CK[1]);
	rk[2] = K[2] ^ Tfunc_(K[3] ^ rk[0] ^ rk[1] ^ CK[2]);
	rk[3] = K[3] ^ Tfunc_(rk[0] ^ rk[1] ^ rk[2] ^ CK[3]);

	for (int i = 4; i < 32; i++)
	{
		rk[i] = rk[i - 4] ^ Tfunc_(rk[i - 3] ^ rk[i - 2] ^ rk[i - 1] ^ CK[i]);
	}
}

//轮函数
u32 F(u32 x0, u32 x1, u32 x2, u32 x3, u32 rk)
{
	return x0 ^ Tfunc(x1 ^ x2 ^ x3 ^ rk);
}

//加密算法
void encrypt(u32 plain[])
{
	u32 temp;
	for (int i = 0; i < 32; i++)
	{
		temp = F(plain[0], plain[1], plain[2], plain[3], rk[i]);
		plain[0] = plain[1];
		plain[1] = plain[2];
		plain[2] = plain[3];
		plain[3] = temp;
	}

	//最后4个调序
	temp = plain[0];
	plain[0] = plain[3];
	plain[3] = temp;
	temp = plain[1];
	plain[1] = plain[2];
	plain[2] = temp;

}

//将16个字节的输入拼接成4字的结果
void hecheng(u8 in[],u32 out[])
{
	for (int i = 0; i < 4; i++)
	{
		out[i] = ((u32)in[i * 4] << 24) + ((u32)in[i * 4 + 1] << 16) + ((u32)in[i * 4 + 2] << 8) + ((u32)in[i * 4 + 3]);
	}
}

//将4字的输入拆分成16个字节
void fenli(u32 in[], u8 out[])
{
	for (int i = 0; i < 4; i++)
	{
		out[i * 4] = (u8)(in[i] >> 24) & 0xff;
		out[i * 4 + 1] = (u8)(in[i] >> 16) & 0xff;
		out[i * 4 + 2] = (u8)(in[i] >> 8) & 0xff;
		out[i * 4 + 3] = (u8)in[i] & 0xff;
	}
}

//最终的加密算法
void final_encrypt(u8 plain[], u8 key[], u8 out[])
{
	u32 MK[4];
	hecheng(key, MK);
	get_rk(MK);

	u32 X[4];
	hecheng(plain, X);
	encrypt(X);	//X为最后的加密结果，是4个字

	fenli(X, out);
}

//解密算法
void decrypt(u8 cipher[],u8 out[])
{
	u32 dekey[32];
	for (int i = 0; i < 32; i++)
		dekey[i] = rk[32 - i - 1];
	for (int i = 0; i < 32; i++)
		rk[i] = dekey[i];

	u32 X[4];
	hecheng(cipher, X);
	encrypt(X);	//X为最后的加密结果，是4个字

	fenli(X, out);
}

int main()
{
	u8 key[16] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };

	u8 plaintext[16] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };

	u8 output[16] = { 0 };

	final_encrypt(plaintext, key, output);

	cout << "加密结果:";
	for (int i = 0; i < 16; i++)
	{
		cout << hex << ((u32)output[i]) << " ";
	}
	cout << endl;

	u8 plain[16];
	decrypt(output, plain);
	cout << "解密结果:";
	for (int i = 0; i < 16; i++)
	{
		cout << hex << ((u32)plain[i]) << " ";
	}
	
	return 0;
}