#ifndef UTILITY_H_
#define UTILITY_H_

#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
using namespace std;

void PrintNoResutl();
//added by Louis 2012-09-05
void PrintNoResutl(string& sResult);
string GetBetween(const string &sData,string strBeg,string strEnd);
string GetTagPart(const string& html_content,const string& tagname,size_t start);
bool GetBetweenList(const string &sData, size_t start ,const string& strBeg,const string& strEnd,vector<string>& list);
void ToAMPEntity(string &sUrl);
int GetStrBetweenTwoStr(const string& strSrc ,string strPre, string  strSuf,string& strContent,int iPos);

void Split(const string& str,const string& pattern,vector<string>& result);
string GetUrlValue(const string& info_parse,const string& key);
string GetVidValue(const string &html_content,const string &vid,const string& seprator);
//added by Louis 2012-10-10
//for tudou
void AMPEntityTo(string &sUrl);
unsigned int utf8_decode( char *s, unsigned int *pi )  ;
std::string UrlEncode(const std::string& src)  ;
std::string UrlDecode(const std::string& src)  ;
inline char GetCharIndex(char c);
int fnBase64Decode(const string& src_str,string& dst_str,int sLen);

string replace_all_distinct(string &str,const string &old_value,const string & new_value);
class CBase64
{
public:
	CBase64();
	~CBase64();

	/*********************************************************
	* 函数说明：将输入数据进行base64编码
	* 参数说明：[in]pIn		需要进行编码的数据
				[in]uInLen  输入参数的字节数
				[out]strOut 输出的进行base64编码之后的字符串
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, string& strOut);

	/*********************************************************
	* 函数说明：将输入数据进行base64编码
	* 参数说明：[in]pIn			需要进行编码的数据
				[in]uInLen		输入参数的字节数
				[out]pOut		输出的进行base64编码之后的字符串
				[out]uOutLen	输出的进行base64编码之后的字符串长度
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen);
	
	/*********************************************************
	* 函数说明：将输入数据进行base64解码
	* 参数说明：[in]strIn		需要进行解码的数据
				[out]pOut		输出解码之后的节数数据
				[out]uOutLen	输出的解码之后的字节数长度
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Decode(const string& strIn, unsigned char *pOut, unsigned long *uOutLen) ;
	bool static Decode(const string& strIn, string& dstOut) ;
	/*********************************************************
	* 函数说明：将输入数据进行base64解码
	* 参数说明：[in]strIn		需要进行解码的数据
				[out]pOut		输出解码之后的节数数据
				[out]uOutLen	输出的解码之后的字节数长度
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	//bool static Decode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen) ;
};
//MD5.h 
#ifndef BZF_MD5_H
#define BZF_MD5_H

#include <string>
#include <iostream>


// a small class for calculating MD5 hashes of strings or byte arrays
// it is not meant to be fast or secure
//
// usage: 1) feed it blocks of uchars with update()
//      2) finalize()
//      3) get hexdigest() string
//      or
//      MD5(std::string).hexdigest()
//
// assumes that char is 8 bit and int is 32 bit
class MD5
{
public:
	typedef unsigned int size_type; // must be 32bit

	MD5();
	MD5(const std::string& text);
	void update(const unsigned char *buf, size_type length);
	void update(const char *buf, size_type length);
	MD5& finalize();
	std::string hexdigest() const;
	friend std::ostream& operator<<(std::ostream&, MD5 md5);

private:
	void init();
	typedef unsigned char uint1; //  8bit
	typedef unsigned int uint4;  // 32bit
	enum {blocksize = 64}; // VC6 won't eat a const static int here

	void transform(const uint1 block[blocksize]);
	static void decode(uint4 output[], const uint1 input[], size_type len);
	static void encode(uint1 output[], const uint4 input[], size_type len);

	bool finalized;
	uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
	uint4 count[2];   // 64bit counter for number of bits (lo, hi)
	uint4 state[4];   // digest so far
	uint1 digest[16]; // the result

	// low level logic operations
	static inline uint4 F(uint4 x, uint4 y, uint4 z);
	static inline uint4 G(uint4 x, uint4 y, uint4 z);
	static inline uint4 H(uint4 x, uint4 y, uint4 z);
	static inline uint4 I(uint4 x, uint4 y, uint4 z);
	static inline uint4 rotate_left(uint4 x, int n);
	static inline void FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
};

std::string md5(const std::string str);

#endif



#define AES_KEY_ROW_NUMBER 4
#define AES_KEY_COLUMN_NUMBER 4
#define AES_ROUND_COUNT 10
typedef   unsigned char BYTE;
class AES
{
public:
	AES(void);
	AES(BYTE* key);
	virtual ~AES(void);

	void Encrypt(BYTE *, BYTE *,size_t);
	void Decrypt(BYTE *, BYTE *,size_t);

private:

	BYTE swapbox[11][4][4];

	BYTE* Cipher(BYTE* input);
	BYTE* InvCipher(BYTE* input);

	BYTE* Cipher(void * input, size_t length);
	BYTE* InvCipher(void * input, size_t length);	

	void KeyExpansion(BYTE* key, BYTE w[][4][AES_KEY_COLUMN_NUMBER]);
	BYTE FFmul(BYTE a, BYTE b);

	void SubBytes(BYTE state[][AES_KEY_COLUMN_NUMBER]);
	void ShiftRows(BYTE state[][AES_KEY_COLUMN_NUMBER]);
	void MixColumns(BYTE state[][AES_KEY_COLUMN_NUMBER]);
	void AddRoundKey(BYTE state[][AES_KEY_COLUMN_NUMBER], BYTE k[][AES_KEY_COLUMN_NUMBER]);

	void InvSubBytes(BYTE state[][AES_KEY_COLUMN_NUMBER]);
	void InvShiftRows(BYTE state[][AES_KEY_COLUMN_NUMBER]);
	void InvMixColumns(BYTE state[][AES_KEY_COLUMN_NUMBER]);


};

void bin2str(const unsigned char* hash, unsigned char* pszHash);

bool str2bin(const unsigned  char* pszHash, unsigned char* hash,int len);
#endif
