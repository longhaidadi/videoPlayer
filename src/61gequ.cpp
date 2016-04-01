#include"videoaddress.h"
#include "json/value.h"
#include "json/json.h"
#include "urlcode.h"
#include "utility.h"
#include <iostream>
#include <cassert>

#include <cstring>
#include <sstream>
#include <fstream>

/************************************************************************/
/* 61gequ*/
/************************************************************************/
int videoaddress61gequ::ParseSrcHtml(string& htmlcontent,string& playurl)
{
	string url = GetBetween(htmlcontent,"mp4url:\"","\"");
	if(url.empty())
		return -1;
	if(url.find(".mp4")!=string::npos){
		playurl = "www.61gequ.com" +url;
	}else{
		string base64_url;
		CBase64::Decode(url,base64_url);
		//if(fnBase64Decode(url,base64_url,url.size())<0)
		//	return -1;
		playurl = base64_url;
	}
	return 0;
}
int videoaddress61gequ::GetDownloadAddr(unsigned int  return_num)
{
	string htmlcontent;
	bool bcode = GetWebPage(m_sUrl,htmlcontent);
	if (false == bcode)
	{
		return -1;
	}
	string playurl;
	if(ParseSrcHtml(htmlcontent,playurl)<0)
		return -1;

	m_sSeconds = "1";
	m_v_sMp4Urls.push_back(playurl);
	if (m_v_sMp4Urls.empty() )
	{
		return -3;
	}
	m_iParts = m_v_sMp4Urls.size();

	if(ParseNumOutput(return_num)==0)
		return 0;
	else 
		return -1;

	return 0;
}
