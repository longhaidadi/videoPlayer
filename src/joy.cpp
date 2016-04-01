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

void videoaddressjoy::ParseSrcHtml(string& htmlcontent,string& xmlurl)
{
	if (htmlcontent.empty() )
	{
		xmlurl = "";
		return;
	}
	
	string programid,channelid, vidoid;
	programid = GetBetween(htmlcontent,"programId:\"","\"");
	vidoid = GetBetween(htmlcontent,"videoId:\"","\"");
	if (vidoid.empty() )
	{
		xmlurl = "";
		cout << "11" << endl;
		return;
	}
	if (programid.empty() )
	{
		xmlurl = "http://msx.app.joy.cn/service.php?action=msxv6&showad=true&videoid=" + vidoid + "&playertype=joyplayer&nocache=550";
	} 
	else
	{
		channelid = GetBetween(htmlcontent,"channelId:\"","\"");
		if (channelid.empty() )
		{
			xmlurl = "";
			cout << "222" << endl;
			return;
		}
		xmlurl = "http://msx.app.joy.cn/service.php?action=vodmsxv6&showad=true&channelid=" + channelid + "&programid="+programid + "&volumnid="
				+ vidoid + "&playertype=joyplayer&nocache=191";
	}
}

int videoaddressjoy::ParseXML(string &xmlcontent)
{
	if (xmlcontent.empty() )
	{
		return -11;
	}
	//m_sSeconds = GetBetween(xmlcontent,"length=\"","\"");
	//part num is 1 
	m_iParts = 1;

	//string::size_type urlbeg=0, urlend=0;
	int urlbeg=0,urlend=0;
	string urlbetween;
	//目前只有流畅和高清，流畅有可能重复。
	vector<string> v_urls;
	while((urlbeg=GetStrBetweenTwoStr(xmlcontent,"<Url","</Url>",urlbetween,urlend) ) != -1)
	{
		string surlpart = GetBetween(urlbetween,"<![CDATA[","]");
		if (!surlpart.empty() )
		{
			v_urls.push_back(surlpart);
		}
		urlend = urlbeg;
	}

	string::size_type sthostpath = xmlcontent.find("<HostPath");
	if (sthostpath == string::npos)
	{
		return -12;
	}
	string shostpath;
	GetStrBetweenTwoStr(xmlcontent,"http://","</HostPath>",shostpath,sthostpath);
	if (!v_urls.empty() )
	{
		if (v_urls.size() ==1 )
		{
			m_v_sFlvUrls.push_back("http://"+shostpath+v_urls[0]);
		}
		else
		{
			m_v_sFlvUrls.push_back("http://"+shostpath+v_urls[0]);
			m_v_sHDUrls.push_back("http://"+shostpath+v_urls.back() );
		}
	}
	return 0;
}

int videoaddressjoy::GetDownloadAddr(unsigned int  return_num)
{
	string htmlcontent;
	bool bcode = GetWebPage(m_sUrl,htmlcontent);
	if (false == bcode)
	{
		return -1;
	}
	//cout << htmlcontent << endl;
	string xmlurl;
	ParseSrcHtml(htmlcontent,xmlurl);
	//cout << xmlurl << endl;
	if (xmlurl.empty() )
	{
		return -2;
	}
	string xmlcontent;
	bcode = GetWebPage(xmlurl,xmlcontent);
	if (false == bcode)
	{
		return -1;
	}

	if(ParseXML(xmlcontent)!=0)
		return -1;

	if(ParseNumOutput(return_num)==0)
		return 0;
	else 
		return -1;
}
