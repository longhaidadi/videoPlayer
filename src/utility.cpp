#include "utility.h"
#include <iostream>
#include <sstream>
using namespace std;



inline char GetCharIndex(char c)
{	if((c >= 'A') && (c <= 'Z'))
	{	return c - 'A';
	}else if((c >= 'a') && (c <= 'z'))
	{	return c - 'a' + 26;
	}else if((c >= '0') && (c <= '9'))
	{	return c - '0' + 52;
	}else if(c == '+')
	{	return 62;
	}else if(c == '/')
	{	return 63;
	}else if(c == '=')
	{	return 0;
	}
	return 0;
}

int fnBase64Decode(const string& src_str,string& dst_str,int sLen){
	//cout << src_str << endl;
	
	static char lpCode[4];
	register int vLen = 0;
	if(sLen % 4)		
	{	
		return -1;
	}
	int src_idx = 0;
	int dst_idx = 0;
	while(sLen > 2)		
	{	
		//cout << sLen << endl;
		
		cout << src_str.substr(dst_idx,4) << endl;
		lpCode[0] = GetCharIndex(src_str[dst_idx+0]);
		lpCode[1] = GetCharIndex(src_str[dst_idx+1]);
		lpCode[2] = GetCharIndex(src_str[dst_idx+2]);
		lpCode[3] = GetCharIndex(src_str[dst_idx+3]);

		dst_str.push_back((lpCode[0] << 2) | (lpCode[1] >> 4));
		dst_str.push_back((lpCode[1] << 4) | (lpCode[2] >> 2));
		dst_str.push_back((lpCode[2] << 6) | (lpCode[3]));
		cout << dst_str[dst_str.size()-1] << endl;
		dst_idx += 4;
		sLen -= 4;
		vLen += 3;
	}
	return vLen;
}
string GetTagPart(const string& html_content,const string& tagname,size_t start){

	size_t start_pos;
	size_t end_pos;

	
	string content = html_content.substr(start);
	string tagstart = "<" + tagname;
	if( (start_pos =content.find(tagstart))==string::npos)
		return "";
	int cache = 1;
	end_pos = start_pos + tagstart.size();
	while(cache > 0){
		end_pos = content.find(tagname,end_pos);
		if(end_pos ==  string::npos)
			return "";

		if(content[end_pos-1] == '<')
			cache++;
		else if(content[end_pos-1] == '/')
			cache--;
		else;

		end_pos += 3;
	}

	return content.substr(start_pos,end_pos - start_pos+1);

}
string GetUrlValue(const string& info_parse,const string& key)
{
	string key_str = key;
	if(key_str[key_str.size()-1]!='=')
		key_str += '=';

	size_t start = info_parse.find(key_str);
	size_t end = info_parse.find("&",start+key_str.size());
	string value = "";
	if(start != string::npos){
		if(end!=string::npos)
			value = info_parse.substr(start+key_str.size(),end-start-key_str.size());
		else
			value = info_parse.substr(start+key_str.size());
	}
	return value;
}


void Split(const string& src_str,const string& pattern,vector<string>& result)
{
	std::string::size_type pos;
	string str =   src_str;
	str+=pattern;
	int size=str.size();
	for(int i=0; i<size; i++)
	{
		pos=str.find(pattern,i);
		if(pos<size)
		{
			std::string s=str.substr(i,pos-i);
			result.push_back(s);
			i=pos+pattern.size()-1;
		}
	}
}

string GetVidValue(const string &html_content,const string &vid,const string& seprator){

	size_t start,end;
	start = 0;
	string vidflag = vid;
	while((start = html_content.find(vidflag,start))!=string::npos){
		start = start+vidflag.size();
		end = html_content.find(":",start);
		if(end == string::npos)
			break;
		int k = end-start;
		bool ctest = true;
		for(int i = k-1 ; i >= 0; i--){
			if(html_content[start+i] != ' '){
				ctest = false;
				break;
			}
		}
		if(!ctest)
			continue;
		string content_tmp = html_content.substr(end+1);
		string test_vid = GetBetween(content_tmp,seprator,seprator);
		bool digtest = true;
		for(int i = 0; i < test_vid.size(); i++){
			if(isdigit(test_vid[i]) == false && test_vid[i] != '|')
				digtest = false;
		}
		if(digtest){
			return test_vid;
		}
	}
	return "";

}
bool GetBetweenList(const string &sData, size_t start ,const string& strBeg,const string& strEnd,vector<string>& list){
	
	list.clear();
	size_t end_pos = start;
	while(true){

		string::size_type stBeg = sData.find(strBeg,end_pos);
		if(stBeg == string::npos)
		{
			return true;
		}

		string::size_type stEnd = sData.find(strEnd,stBeg+strBeg.size());
		if(stEnd == string::npos)
		{
			return true;
		}
		string item =  sData.substr(stBeg+strBeg.size(),stEnd-stBeg-strBeg.size() );

		list.push_back(item);
		end_pos = stEnd+strEnd.size();
	}

}

string GetBetween(const string &sData,string strBeg,string strEnd)
{
	string::size_type stBeg = sData.find(strBeg);
	if(stBeg == string::npos)
	{
		//cout << "find " << strBeg << " failed!!!" << endl;
		//PrintNoResutl();
		//exit(-1);
		return "";
	}
	string::size_type stEnd = sData.find(strEnd,stBeg+strBeg.size() );
	
	if(stEnd == string::npos)
	{
		//cout << "find " << strEnd << " failed!!!" << endl;
		//PrintNoResutl();
		//exit(-1);
		return "";
	}
	return sData.substr(stBeg+strBeg.size(),stEnd-stBeg-strBeg.size() );
}



void PrintNoResutl()
{
	cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
	cout << "<data>" << endl;
	cout << "<seconds>0</seconds>" << endl;
	cout << "<part>0</part>" << endl;
	cout << "<videoCacheUrls type=\"unknown\">" << endl;
	cout << "</videoCacheUrls>" << endl;
	cout << "</data>" << endl;
}

//added by Louis 2012-09-05
void PrintNoResutl(string& sResult)
{
	stringstream ssm;
	ssm << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
	ssm << "<data>" << endl;
	ssm << "<seconds>0</seconds>" << endl;
	ssm << "<part>0</part>" << endl;
	ssm << "<videoCacheUrls type=\"unknown\">" << endl;
	ssm << "</videoCacheUrls>" << endl;
	ssm << "</data>" << endl;
	sResult = ssm.str();
}


void ToAMPEntity(string &sUrl)
{
	string::size_type stBeg = 0,stEnd = 0;
	//modified by Louis 2012-09-27
	//todou has &amp; already
	if(sUrl.find("&amp;") != string::npos)
	{
		return;
	}
	
	while((stBeg=sUrl.find("&",stEnd)) != string::npos)
	{
		sUrl.replace(stBeg,1,"&amp;");
		stEnd = stBeg + 5;
	}
} 

//added by Louis 2012-10-10
//for tudou
void AMPEntityTo(string &sUrl)
{
	string::size_type stBeg = 0,stEnd = 0;

	while((stBeg=sUrl.find("&amp;",stEnd)) != string::npos)
	{
		sUrl.replace(stBeg,5,"&");
		stEnd = stBeg + 1;
	}
}


/*===========================================================================================	
	GetStrBetweenTwoStr..strSrc.pos.....strPre.strSuf........strContent.
	....
		-1...
		>=0.......strSuf.....
=============================================================================================*/
int GetStrBetweenTwoStr(const string& strSrc ,string strPre, string  strSuf,string& strContent,int iPos)
{
	if (strSrc.empty())
	{
		return -1;
	}
	string::size_type iPos1=strSrc.find(strPre,iPos);
	if (iPos1 != string::npos)
	{
		iPos1 += strPre.length();
		string::size_type iPos2 = strSrc.find(strSuf,iPos1);
		if (iPos2 != string::npos)
		{
			strContent = strSrc.substr(iPos1,iPos2 - iPos1);
			return (iPos2 + strSuf.length());
		}
	}

	return -1;
}


unsigned int utf8_decode( char *s, unsigned int *pi )  
    {  
        unsigned int c;  
        int i = *pi;  
        /* one digit utf-8 */  
        if ((s[i] & 128)== 0 ) {  
            c = (unsigned int) s[i];  
            i += 1;  
        } else if ((s[i] & 224)== 192 ) { /* 110xxxxx & 111xxxxx == 110xxxxx */  
            c = (( (unsigned int) s[i] & 31 ) << 6) +  
                ( (unsigned int) s[i+1] & 63 );  
            i += 2;  
        } else if ((s[i] & 240)== 224 ) { /* 1110xxxx & 1111xxxx == 1110xxxx */  
            c = ( ( (unsigned int) s[i] & 15 ) << 12 ) +  
                ( ( (unsigned int) s[i+1] & 63 ) << 6 ) +  
                ( (unsigned int) s[i+2] & 63 );  
            i += 3;  
        } else if ((s[i] & 248)== 240 ) { /* 11110xxx & 11111xxx == 11110xxx */  
            c = ( ( (unsigned int) s[i] & 7 ) << 18 ) +  
                ( ( (unsigned int) s[i+1] & 63 ) << 12 ) +  
                ( ( (unsigned int) s[i+2] & 63 ) << 6 ) +  
                ( (unsigned int) s[i+3] & 63 );  
            i+= 4;  
        } else if ((s[i] & 252)== 248 ) { /* 111110xx & 111111xx == 111110xx */  
            c = ( ( (unsigned int) s[i] & 3 ) << 24 ) +  
                ( ( (unsigned int) s[i+1] & 63 ) << 18 ) +  
                ( ( (unsigned int) s[i+2] & 63 ) << 12 ) +  
                ( ( (unsigned int) s[i+3] & 63 ) << 6 ) +  
                ( (unsigned int) s[i+4] & 63 );  
            i += 5;  
        } else if ((s[i] & 254)== 252 ) { /* 1111110x & 1111111x == 1111110x */  
            c = ( ( (unsigned int) s[i] & 1 ) << 30 ) +  
                ( ( (unsigned int) s[i+1] & 63 ) << 24 ) +  
                ( ( (unsigned int) s[i+2] & 63 ) << 18 ) +  
                ( ( (unsigned int) s[i+3] & 63 ) << 12 ) +  
                ( ( (unsigned int) s[i+4] & 63 ) << 6 ) +  
                ( (unsigned int) s[i+5] & 63 );  
            i += 6;  
        } else {  
            c = '?';  
            i++;  
        }  
        *pi = i;  
        return c;  
    }  
      
    std::string UrlEncode(const std::string& src)  
    {  
        static    char hex[] = "0123456789ABCDEF";  
        std::string dst;  
      
        for (size_t i = 0; i < src.size(); i++)  
        {  
            unsigned char ch = src[i];  
            if (isalnum(ch))  
            {  
                dst += ch;  
            }  
            else  
                if (src[i] == ' ')  
                {  
                    dst += '+';  
                }  
                else  
                {  
                    unsigned char c = static_cast<unsigned char>(src[i]);  
                    dst += '%';  
                    dst += hex[c / 16];  
                    dst += hex[c % 16];  
                }  
        }  
        return dst;  
    }  
      
    std::string UrlDecode(const std::string& src)  
    {  
        std::string dst, dsturl;  
      
        int srclen = src.size();  
      
        for (size_t i = 0; i < srclen; i++)  
        {  
            if (src[i] == '%')  
            {  
                if(isxdigit(src[i + 1]) && isxdigit(src[i + 2]))  
                {  
                    char c1 = src[++i];  
                    char c2 = src[++i];  
                    c1 = c1 - 48 - ((c1 >= 'A') ? 7 : 0) - ((c1 >= 'a') ? 32 : 0);  
                    c2 = c2 - 48 - ((c2 >= 'A') ? 7 : 0) - ((c2 >= 'a') ? 32 : 0);  
                    dst += (unsigned char)(c1 * 16 + c2);  
                }  
            }  
            else  
                if (src[i] == '+')  
                {  
                    dst += ' ';  
                }  
                else  
                {  
                    dst += src[i];  
                }  
        }  
      
        int len = dst.size();  
      
        for(unsigned int pos = 0; pos < len;)  
        {  
            unsigned int nvalue = utf8_decode((char *)dst.c_str(), &pos);  
            dsturl += (unsigned char)nvalue;  
        }  
      
        return dsturl;  
    }  
      
    // ²âÊÔ³ÌÐò  
      
   /* int main(int argc, char* argv[])  
    { 
		string str1="fykan.home7.dk3.com(64.246.26.107";
        string str2=UrlEncode(str1);
        printf("%s\n", str1.c_str());
		printf("%s\n", str2.c_str());    // URL±àÂë  
        printf("%s\n", UrlDecode(str2).c_str());    // URL½âÂë  
      
        return 0;  
    } */ 


static const char *g_pCodes =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static const unsigned char g_pMap[256] =
{
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	 52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	  7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	 19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	 37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	 49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
};

CBase64::CBase64()
{
}

CBase64::~CBase64()
{
}

bool CBase64::Encode(const unsigned char *pIn, unsigned long uInLen, unsigned char *pOut, unsigned long *uOutLen)
{
	unsigned long i, len2, leven;
	unsigned char *p;

	if(pOut == NULL || *uOutLen == 0)
		return false;

	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL));

	len2 = ((uInLen + 2) / 3) << 2;
	if((*uOutLen) < (len2 + 1)) return false;

	p = pOut;
	leven = 3 * (uInLen / 3);
	for(i = 0; i < leven; i += 3)
	{
		*p++ = g_pCodes[pIn[0] >> 2];
		*p++ = g_pCodes[((pIn[0] & 3) << 4) + (pIn[1] >> 4)];
		*p++ = g_pCodes[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)];
		*p++ = g_pCodes[pIn[2] & 0x3f];
		pIn += 3;
	}

	if (i < uInLen)
	{
		unsigned char a = pIn[0];
		unsigned char b = ((i + 1) < uInLen) ? pIn[1] : 0;
		unsigned char c = 0;

		*p++ = g_pCodes[a >> 2];
		*p++ = g_pCodes[((a & 3) << 4) + (b >> 4)];
		*p++ = ((i + 1) < uInLen) ? g_pCodes[((b & 0xf) << 2) + (c >> 6)] : '=';
		*p++ = '=';
	}

	*p = 0; // Append NULL byte
	*uOutLen = p - pOut;
	return true;
}

bool CBase64::Encode(const unsigned char *pIn, unsigned long uInLen, string& strOut) 
{
	unsigned long i, len2, leven;
	strOut = "";

	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL));

	len2 = ((uInLen + 2) / 3) << 2;
	//if((*uOutLen) < (len2 + 1)) return false;

	//p = pOut;
	leven = 3 * (uInLen / 3);
	for(i = 0; i < leven; i += 3)
	{
		strOut += g_pCodes[pIn[0] >> 2];
		strOut += g_pCodes[((pIn[0] & 3) << 4) + (pIn[1] >> 4)];
		strOut += g_pCodes[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)];
		strOut += g_pCodes[pIn[2] & 0x3f];
		pIn += 3;
	}

	if (i < uInLen)
	{
		unsigned char a = pIn[0];
		unsigned char b = ((i + 1) < uInLen) ? pIn[1] : 0;
		unsigned char c = 0;

		strOut += g_pCodes[a >> 2];
		strOut += g_pCodes[((a & 3) << 4) + (b >> 4)];
		strOut += ((i + 1) < uInLen) ? g_pCodes[((b & 0xf) << 2) + (c >> 6)] : '=';
		strOut += '=';
	}

	//*p = 0; // Append NULL byte
	//*uOutLen = p - pOut;
	return true;
}

bool CBase64::Decode(const string& strIn, unsigned char *pOut, unsigned long *uOutLen) 
{
	unsigned long t, x, y, z;
	unsigned char c;
	unsigned long g = 3;

	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL));

	for(x = y = z = t = 0; x < strIn.length(); x++)
	{
		c = g_pMap[strIn[x]];
		if(c == 255) continue;
		if(c == 254) { c = 0; g--; }

		t = (t << 6) | c;

		if(++y == 4)
		{
			if((z + g) > *uOutLen) { return false; } // Buffer overflow
			pOut[z++] = (unsigned char)((t>>16)&255);
			if(g > 1) pOut[z++] = (unsigned char)((t>>8)&255);
			if(g > 2) pOut[z++] = (unsigned char)(t&255);
			y = t = 0;
		}
	}

	*uOutLen = z;
	return true;
}



bool  CBase64::Decode(const string& strIn, string& dstOut){
	unsigned long t, x, y, z;
	unsigned char c;
	unsigned long g = 3;

	for(x = y = z = t = 0; x < strIn.length(); x++)
	{
		c = g_pMap[strIn[x]];
		if(c == 255) continue;
		if(c == 254) { c = 0; g--; }

		t = (t << 6) | c;

		if(++y == 4)
		{
			//if((z + g) > *uOutLen) { return false; } // Buffer overflow
			unsigned char  dstc=  (unsigned char)((t>>16)&255);
			dstOut.push_back(dstc);
		
			if(g > 1) {dstc=(unsigned char)((t>>8)&255);dstOut.push_back(dstc);}
			if(g > 2) {dstc=(unsigned char)(t&255);dstOut.push_back(dstc);}
			y = t = 0;
		}
	}
	return true;
}
string replace_all_distinct(string &str,const string &old_value,const string & new_value){

    for (string::size_type pos(0);pos!=string::npos; pos+=new_value.length() )
    {
        if((pos=str.find(old_value,pos))!=string::npos)str.replace(pos,old_value.length(),new_value);
        else break;
    }
    return str;
}

//MD5.cpp
/* MD5
converted to C++ class by Frank Thilo (thilo@unix-ag.org)
for bzflag (http://www.bzflag.org)

based on:

md5.h and md5.c
reference implemantion of RFC 1321

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

/* interface header */

/* system implementation headers */


// Constants for MD5Transform routine.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

///////////////////////////////////////////////

// F, G, H and I are basic MD5 functions.
inline MD5::uint4 MD5::F(uint4 x, uint4 y, uint4 z) {
	return x&y | ~x&z;
}

inline MD5::uint4 MD5::G(uint4 x, uint4 y, uint4 z) {
	return x&z | y&~z;
}

inline MD5::uint4 MD5::H(uint4 x, uint4 y, uint4 z) {
	return x^y^z;
}

inline MD5::uint4 MD5::I(uint4 x, uint4 y, uint4 z) {
	return y ^ (x | ~z);
}

// rotate_left rotates x left n bits.
inline MD5::uint4 MD5::rotate_left(uint4 x, int n) {
	return (x << n) | (x >> (32-n));
}

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
inline void MD5::FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a+ F(b,c,d) + x + ac, s) + b;
}

inline void MD5::GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + G(b,c,d) + x + ac, s) + b;
}

inline void MD5::HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + H(b,c,d) + x + ac, s) + b;
}

inline void MD5::II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
	a = rotate_left(a + I(b,c,d) + x + ac, s) + b;
}

//////////////////////////////////////////////

// default ctor, just initailize
MD5::MD5()
{
	init();
}

//////////////////////////////////////////////

// nifty shortcut ctor, compute MD5 for string and finalize it right away
MD5::MD5(const std::string &text)
{
	init();
	update(text.c_str(), text.length());
	finalize();
}

//////////////////////////////

void MD5::init()
{
	finalized=false;

	count[0] = 0;
	count[1] = 0;

	// load magic initialization constants.
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
}

//////////////////////////////

// decodes input (unsigned char) into output (uint4). Assumes len is a multiple of 4.
void MD5::decode(uint4 output[], const uint1 input[], size_type len)
{
	for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((uint4)input[j]) | (((uint4)input[j+1]) << 8) |
		(((uint4)input[j+2]) << 16) | (((uint4)input[j+3]) << 24);
}

//////////////////////////////

// encodes input (uint4) into output (unsigned char). Assumes len is
// a multiple of 4.
void MD5::encode(uint1 output[], const uint4 input[], size_type len)
{
	for (size_type i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = input[i] & 0xff;
		output[j+1] = (input[i] >> 8) & 0xff;
		output[j+2] = (input[i] >> 16) & 0xff;
		output[j+3] = (input[i] >> 24) & 0xff;
	}
}

//////////////////////////////

// apply MD5 algo on a block
void MD5::transform(const uint1 block[blocksize])
{
	uint4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	decode (x, block, blocksize);

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	// Zeroize sensitive information.
	memset(x, 0, sizeof x);
}

//////////////////////////////

// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block
void MD5::update(const unsigned char input[], size_type length)
{
	// compute number of bytes mod 64
	size_type index = count[0] / 8 % blocksize;

	// Update number of bits
	if ((count[0] += (length << 3)) < (length << 3))
		count[1]++;
	count[1] += (length >> 29);

	// number of bytes we need to fill in buffer
	size_type firstpart = 64 - index;

	size_type i;

	// transform as many times as possible.
	if (length >= firstpart)
	{
		// fill buffer first, transform
		memcpy(&buffer[index], input, firstpart);
		transform(buffer);

		// transform chunks of blocksize (64 bytes)
		for (i = firstpart; i + blocksize <= length; i += blocksize)
			transform(&input[i]);

		index = 0;
	}
	else
		i = 0;

	// buffer remaining input
	memcpy(&buffer[index], &input[i], length-i);
}

//////////////////////////////

// for convenience provide a verson with signed char
void MD5::update(const char input[], size_type length)
{
	update((const unsigned char*)input, length);
}

//////////////////////////////

// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.
MD5& MD5::finalize()
{
	static unsigned char padding[64] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	if (!finalized) {
		// Save number of bits
		unsigned char bits[8];
		encode(bits, count, 8);

		// pad out to 56 mod 64.
		size_type index = count[0] / 8 % 64;
		size_type padLen = (index < 56) ? (56 - index) : (120 - index);
		update(padding, padLen);

		// Append length (before padding)
		update(bits, 8);

		// Store state in digest
		encode(digest, state, 16);

		// Zeroize sensitive information.
		memset(buffer, 0, sizeof buffer);
		memset(count, 0, sizeof count);

		finalized=true;
	}

	return *this;
}

//////////////////////////////

// return hex representation of digest as string
std::string MD5::hexdigest() const
{
	if (!finalized)
		return "";

	char buf[33];
	for (int i=0; i<16; i++)
		sprintf(buf+i*2, "%02x", digest[i]);
	buf[32]=0;

	return std::string(buf);
}

//////////////////////////////

std::ostream& operator<<(std::ostream& out, MD5 md5)
{
	return out << md5.hexdigest();
}

//////////////////////////////

std::string md5(const std::string str)
{
	MD5 md5 = MD5(str);

	return md5.hexdigest();
}
static unsigned char permutebox[] =
{ /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f */
	0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76, /*0*/
	0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0, /*1*/
	0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15, /*2*/
	0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75, /*3*/
	0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84, /*4*/
	0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf, /*5*/
	0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8, /*6*/
	0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2, /*7*/
	0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73, /*8*/
	0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb, /*9*/
	0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79, /*a*/
	0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08, /*b*/
	0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a, /*c*/
	0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e, /*d*/
	0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf, /*e*/
	0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16  /*f*/
};
//inversepermutationbox
static	unsigned char inversepermutationbox[]= 
{ /*  0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f  */
	0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb, /*0*/
	0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb, /*1*/
	0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e, /*2*/
	0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25, /*3*/
	0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92, /*4*/
	0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84, /*5*/
	0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06, /*6*/
	0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b, /*7*/
	0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73, /*8*/
	0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e, /*9*/
	0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b, /*a*/
	0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4, /*b*/
	0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f, /*c*/
	0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef, /*d*/
	0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61, /*e*/
	0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d  /*f*/
}; 



AES::AES(void)
{
}
AES::AES(unsigned char* key) 
{
	KeyExpansion(key, swapbox);
}

AES::~AES(void)
{
}

/************************************************************************/
/* create a Encrypt method                                              */
/* param : data     encrypt data                                        */
/* param :encryptArrayencryptArray                                   */
/* param :len encrypt data length                                      */
/* return : void                                                        */
/************************************************************************/

void AES::Encrypt(unsigned char* data ,unsigned char * encryptArray,size_t len)
{
	memcpy(encryptArray,data,len);
	Cipher((void *)encryptArray,len);

}

/************************************************************************/
/* create a Decrypt method                                              */
/* param : data     decrypt data                                        */
/* param :decryptArraydecryptArray                                   */
/* param :len decrypt data length                                      */
/* return : void                                                        */
/************************************************************************/

void AES::Decrypt(unsigned char * data,unsigned char * decryptArray,size_t len)
{	
	memcpy(decryptArray,data,len);
	InvCipher((void *)decryptArray,len);
}

/************************************************************************/
/* create a Cipher  method  only one time  Encrypt                      */
/* param : input     input encrypt data                                 */
/* return : unsigned char *                                             */
/************************************************************************/

unsigned char* AES::Cipher(unsigned char* input)
{

	unsigned char state[AES_KEY_ROW_NUMBER][AES_KEY_COLUMN_NUMBER];
	int i,r,c;

	for(r=0; r<AES_KEY_ROW_NUMBER; r++)
	{
		for(c=0; c<AES_KEY_COLUMN_NUMBER ;c++)
		{
			state[r][c] = input[c*AES_KEY_COLUMN_NUMBER+r];
		}
	}

	AddRoundKey(state,swapbox[0]);

	for(i=1; i<=AES_ROUND_COUNT; i++)
	{
		SubBytes(state);
		ShiftRows(state);
		if(i!=AES_ROUND_COUNT)MixColumns(state);
		AddRoundKey(state,swapbox[i]);
	}

	for(r=0; r<AES_KEY_ROW_NUMBER; r++)
	{
		for(c=0; c<AES_KEY_COLUMN_NUMBER ;c++)
		{
			input[c*AES_KEY_COLUMN_NUMBER+r] = state[r][c];
		}
	}
	return input;
}
/************************************************************************/
/* create a InvCipher  method  only one time  decrypt                   */
/* param : input     input decrypt data                                 */
/* return : unsigned char *                                             */
/************************************************************************/
unsigned char* AES::InvCipher(unsigned char* input)
{
	unsigned char state[AES_KEY_ROW_NUMBER][AES_KEY_COLUMN_NUMBER];
	int i,r,c;

	for(r=0; r<AES_KEY_ROW_NUMBER; r++)
	{
		for(c=0; c<AES_KEY_COLUMN_NUMBER ;c++)
		{
			state[r][c] = input[c*AES_KEY_COLUMN_NUMBER+r];
		}
	}

	AddRoundKey(state, swapbox[10]);
	for(i=9; i>=0; i--)
	{
		InvShiftRows(state);
		InvSubBytes(state);
		AddRoundKey(state, swapbox[i]);
		if(i)
		{
			InvMixColumns(state);
		}
	}	
	for(r=0; r<AES_KEY_ROW_NUMBER; r++)
	{
		for(c=0; c<AES_KEY_COLUMN_NUMBER ;c++)
		{
			input[c*AES_KEY_COLUMN_NUMBER+r] = state[r][c];
		}
	}
	return input;
}
/************************************************************************/
/* Create a specified length of data encryption method                  */
/* param : input     input data encryption                              */
/* param : length    Input the length of the encrypted data             */
/* return : unsigned char *                                             */
/************************************************************************/
unsigned char* AES::Cipher(void * input, size_t length)
{
	unsigned char* in = (unsigned char*) input;
	size_t i;
	if(!length)
	{
		while(*(in+length++));
		in = (unsigned char*) input;
	}
	for(i=0; i<length; i+=16)
	{
		Cipher(in+i);
	}
	return (unsigned char*)input;
}
/************************************************************************/
/* Create a specified length of InvCipher method                        */
/* param : input     input data InvCipher                               */
/* param : length    Input the length of the InvCipher data             */
/* return : unsigned char *                                             */
/************************************************************************/
unsigned char* AES::InvCipher(void * input, size_t length)
{
	unsigned char* in = (unsigned char*) input;
	size_t i;
	for(i=0; i<length; i+=16)
	{
		InvCipher(in+i);
	}
	return (unsigned char*)input;
}
/************************************************************************/
/*Create key method                                                     */
/* param : key      input data encryption key                           */
/* param :swapbox  Conversion of key array                             */
/* return : void                                                        */
/************************************************************************/
void AES::KeyExpansion(unsigned char* key, unsigned char swapbox[][4][4])
{
	int i,j,r,c;
	unsigned char rc[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
	for(r=0; r<AES_KEY_ROW_NUMBER; r++)
	{
		for(c=0; c<AES_KEY_COLUMN_NUMBER; c++)
		{
			swapbox[0][r][c] = key[r+c*AES_KEY_COLUMN_NUMBER];
		}
	}
	for(i=1; i<=10; i++)
	{
		for(j=0; j<AES_KEY_COLUMN_NUMBER; j++)
		{
			unsigned char t[AES_KEY_ROW_NUMBER];
			for(r=0; r<AES_KEY_ROW_NUMBER; r++)
			{
				t[r] = j ? swapbox[i][r][j-1] : swapbox[i-1][r][3];
			}
			if(j == 0)
			{
				unsigned char temp = t[0];
				for(r=0; r<AES_KEY_ROW_NUMBER-1; r++)
				{
					t[r] = permutebox[t[(r+1)%AES_KEY_ROW_NUMBER]];
				}
				t[3] = permutebox[temp];
				t[0] ^= rc[i-1];
			}
			for(r=0; r<AES_KEY_ROW_NUMBER; r++)
			{
				swapbox[i][r][j] = swapbox[i-1][r][j] ^ t[r];
			}
		}
	}
}

/************************************************************************/
/*Create mixed operation method ranks                                   */
/* param : a      row  char                                             */
/* param : b      column  char                                          */
/* return : unsigned char                                               */
/************************************************************************/

unsigned char AES::FFmul(unsigned char a, unsigned char b)
{
	unsigned char bw[AES_KEY_ROW_NUMBER];
	unsigned char res=0;
	int i;
	bw[0] = b;
	for(i=1; i<AES_KEY_ROW_NUMBER; i++)
	{
		bw[i] = bw[i-1]<<1;
		if(bw[i-1]&0x80)
		{
			bw[i]^=0x1b;
		}
	}
	for(i=0; i<AES_KEY_ROW_NUMBER; i++)
	{
		if((a>>i)&0x01)
		{
			res ^= bw[i];
		}
	}
	return res;
}
/************************************************************************/
/* Create bytes alternatives                                            */
/* param :  state[][]  Byte array alternative                           */
/* return : void                                                        */
/************************************************************************/
void AES::SubBytes(unsigned char state[][AES_KEY_COLUMN_NUMBER])
{
	int r,c;
	for(r=0; r<AES_KEY_ROW_NUMBER; r++)
	{
		for(c=0; c<AES_KEY_COLUMN_NUMBER; c++)
		{
			state[r][c] = permutebox[state[r][c]];
		}
	}
}
/************************************************************************/
/* Create rows transform method                                         */
/* param :  state[][]  line array alternative                           */
/* return : void                                                        */
/************************************************************************/
void AES::ShiftRows(unsigned char state[][AES_KEY_COLUMN_NUMBER])
{
	unsigned char t[AES_KEY_COLUMN_NUMBER];
	int r,c;
	for(r=1; r<AES_KEY_ROW_NUMBER; r++)
	{
		for(c=0; c<AES_KEY_COLUMN_NUMBER; c++)
		{
			t[c] = state[r][(c+r)%AES_KEY_COLUMN_NUMBER];
		}
		for(c=0; c<AES_KEY_COLUMN_NUMBER; c++)
		{
			state[r][c] = t[c];
		}
	}
}
/************************************************************************/
/* Create columns transform method                                      */
/* param :  state[][]  columns array alternative                        */
/* return : void                                                        */
/************************************************************************/
void AES::MixColumns(unsigned char state[][AES_KEY_COLUMN_NUMBER])
{
	unsigned char t[AES_KEY_ROW_NUMBER];
	int r,c;
	for(c=0; c< AES_KEY_COLUMN_NUMBER; c++)
	{
		for(r=0; r<AES_KEY_ROW_NUMBER; r++)
		{
			t[r] = state[r][c];
		}
		for(r=0; r<AES_KEY_ROW_NUMBER; r++)
		{
			state[r][c] = FFmul(0x02, t[r])
				^ FFmul(0x03, t[(r+1)%AES_KEY_COLUMN_NUMBER])
				^ FFmul(0x01, t[(r+2)%AES_KEY_COLUMN_NUMBER])
				^ FFmul(0x01, t[(r+3)%AES_KEY_COLUMN_NUMBER]);
		}
	}
}

/************************************************************************/
/*Create round keys plus transform method                               */
/* param :  state[][]  keys plus array alternative                      */
/* param :  k[][]  temp array alternative                               */
/* return : void                                                        */
/************************************************************************/
void AES::AddRoundKey(unsigned char state[][AES_KEY_COLUMN_NUMBER], unsigned char k[][AES_KEY_COLUMN_NUMBER])
{
	int r,c;
	for(c=0; c<AES_KEY_COLUMN_NUMBER; c++)
	{
		for(r=0; r<AES_KEY_ROW_NUMBER; r++)
		{
			state[r][c] ^= k[r][c];
		}
	}
}
/************************************************************************/
/* CreateInvSubBytes alternatives                                      */
/* param :  state[][]  InvSubBytes array alternative                    */
/* return : void                                                        */
/************************************************************************/
void AES::InvSubBytes(unsigned char state[][AES_KEY_COLUMN_NUMBER])
{
	int r,c;
	for(r=0; r<AES_KEY_ROW_NUMBER; r++)
	{
		for(c=0; c<AES_KEY_COLUMN_NUMBER; c++)
		{
			state[r][c] = inversepermutationbox[state[r][c]];
		}
	}
}
/************************************************************************/
/* CreateInvShiftRows transform method                                 */
/* param :  state[][]  InvShiftRows array alternative                   */
/* return : void                                                        */
/************************************************************************/
void AES::InvShiftRows(unsigned char state[][AES_KEY_COLUMN_NUMBER])
{
	unsigned char t[AES_KEY_COLUMN_NUMBER];
	int r,c;
	for(r=1; r<AES_KEY_ROW_NUMBER; r++)
	{
		for(c=0; c<AES_KEY_COLUMN_NUMBER; c++)
		{
			t[c] = state[r][(c-r+AES_KEY_COLUMN_NUMBER)%AES_KEY_COLUMN_NUMBER];
		}
		for(c=0; c<AES_KEY_COLUMN_NUMBER; c++)
		{
			state[r][c] = t[c];
		}
	}
}
/************************************************************************/
/* CreateInvMixColumns transform method                                */
/* param :  state[][]  InvMixColumns array alternative                  */
/* return : void                                                        */
/************************************************************************/
void AES::InvMixColumns(unsigned char state[][AES_KEY_COLUMN_NUMBER])
{
	unsigned char t[AES_KEY_ROW_NUMBER];
	int r,c;
	for(c=0; c< AES_KEY_COLUMN_NUMBER; c++)
	{
		for(r=0; r<AES_KEY_ROW_NUMBER; r++)
		{
			t[r] = state[r][c];
		}
		for(r=0; r<AES_KEY_ROW_NUMBER; r++)
		{
			state[r][c] = FFmul(0x0e, t[r])
				^ FFmul(0x0b, t[(r+1)%AES_KEY_ROW_NUMBER])
				^ FFmul(0x0d, t[(r+2)%AES_KEY_ROW_NUMBER])
				^ FFmul(0x09, t[(r+3)%AES_KEY_ROW_NUMBER]);
		}
	}
}
void bin2str(const unsigned char* hash, unsigned char* pszHash)
{
    char _acHexDigits[] = "0123456789ABCDEF";
    for (int i = 0; i < 16; i++){
  		*pszHash++ = _acHexDigits[hash[i] >> 4];
  		*pszHash++ = _acHexDigits[hash[i] & 0xf];
 	}
 	*pszHash = '\0';
}

bool str2bin(const unsigned  char* pszHash, unsigned char* hash,int len)
{
 memset(hash, 0, len);
 for (int i = 0; i < len; i++)
 {
  char byte[3];
  byte[0] = pszHash[i*2+0];
  byte[1] = pszHash[i*2+1];
  byte[2] = '\0';
  int b;
  if (sscanf(byte, "%x", &b) != 1)
   return false;
  hash[i] = (unsigned char)b;
 }
 return true;
}

