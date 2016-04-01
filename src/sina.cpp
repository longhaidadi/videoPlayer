#include"videoaddress.h"
#include "json/value.h"
#include "json/json.h"
#include "urlcode.h"
#include "utility.h"
#include <iostream>
#include <cassert>
#include <cstdlib>

#include <cstring>
#include <sstream>
#include <fstream>


/***********************************************/
/*sina*/
string videoaddresssina::GetVidValue(string html_content,string beg_str,string end_str)
{
    string strBeg = "video\']";
    string strEnd = "}";
    string videobetween = GetBetween(html_content,strBeg,strEnd);
    if(videobetween.empty() )
    {
        //may be bugs, strBeg not only.
        //TODO
        strBeg = "video:{";
        strEnd = "}";
        videobetween = GetBetween(html_content,strBeg,strEnd);
        if(videobetween.empty() )
        {
            return "";
        }
    }
    videobetween=GetBetween(videobetween,"vid:\'","\'");
    if(videobetween.empty())
        videobetween=GetBetween(videobetween,"vid:\"","\"");
    return videobetween;

}
void videoaddresssina::GetVidFromSingleVideoPage(string& html_content,string &sFlvVid, string& sHDVid)
{
	//struct 
/*
	string strBeg = "video\']";
	string strEnd = "}";
	string videobetween = GetBetween(html_content,strBeg,strEnd);	
	if(videobetween.empty() )
	{
		//may be bugs, strBeg not only.
		//TODO
		strBeg = "video:{";
		strEnd = "}";
		videobetween = GetBetween(html_content,strBeg,strEnd);
		if(videobetween.empty() )
		{
			return;
		}
	}*/
	//cout << "vibt2:\t" << videobetween << endl;
	// process baby.sina.com.cn/tv/pifujiankang/lihang7.html  vid [multi-space] :

//	string vvv  = GetVidValue(html_content,"vid ","\"");
//	cout << vvv << endl;

/*
	sHDVid = "";
	string video_info = GetBetween(html_content,"video:{","}");

	string vidbetween;
	if((vidbetween =GetVidValue(video_info,"hd_vid","\'"))!=""){
		sHDVid = vidbetween;
		return;
	}else if((vidbetween =GetVidValue(video_info,"hd_vid","\""))!=""){
		sHDVid = vidbetween;
		return;
	}
	if((vidbetween =GetVidValue(video_info,"vid","\'"))!=""){
		sHDVid = vidbetween;
		return;
	}else if((vidbetween =GetVidValue(video_info,"vid","\""))!=""){
		sHDVid = vidbetween;
		return;
	}


*/
	//cout << vidbetween << endl;
	string vidbetween = GetVidValue(html_content,"vid","\'");
	//cout << vidbetween << endl;
	if(vidbetween.empty() )
		return;
	vector<string> vidset;
	Split(vidbetween,"|",vidset);
	if(vidset.size()==0)
		return;
	if(vidset.size()==1)
		sHDVid	 = vidset[0];
	else{
		sHDVid = vidset[1];
		sFlvVid = vidset[0];
	}
	//cout << "vid:\t" << sFlvVid <<  endl;
}

int videoaddresssina::ParseInfo(const string& xml_content){
	//cout << xml_content << endl;
	string timelength = GetBetween(xml_content,"<timelength>","</timelength>");
	if(timelength.empty())
		return -1;
	m_sSeconds = timelength.substr(0,timelength.size()-3);

	vector<string> urls;
	vector<string> durations;
	string content = xml_content;
	string startflag = "http://";
	string endflag = "]";
	size_t pos ;
	while(true){
		string segment = GetBetween(content,startflag,endflag);
		if(segment.empty())
			break;
		urls.push_back("http://"+segment);
		if((pos = content.find(startflag))==string::npos)
			return -1;
		content = content.substr(content.find(endflag,pos)+endflag.size());
	}
	content = xml_content;
	startflag = "<length>";
	endflag = "</length>";
	while(true){
		string segment = GetBetween(content,startflag,endflag);
		if(segment.empty())
			break;
		durations.push_back(segment.substr(0,segment.size()-3));
		if((pos = content.find(startflag))==string::npos)
			return -1;
		content = content.substr(content.find(endflag,pos)+endflag.size());
	}

	if(urls.size()==durations.size()){
		for(unsigned int i =0;i<urls.size();i++){
			video_part vpt;
			vpt.url = urls[i];
			vpt.seconds = durations[i];
			vpt.size = "";
			m_v_Parts.push_back(vpt);
		}
		m_iParts = m_v_Parts.size();
		return 0;
	}else
		return -1;

}

videoaddresssina::sinaVideoInfo videoaddresssina::ParseUrlsFromXML(string& xml_content)
{
	sinaVideoInfo infotmp;
	infotmp.m_sTimeLen = GetBetween(xml_content,"<timelength>","</timelength>");

	infotmp.m_iPartNum = 0;
	int stbegin = 0;
	string soneurl;
	while(stbegin != -1)
	{
		stbegin = GetStrBetweenTwoStr(xml_content,"http://","]",soneurl,stbegin);
		if(stbegin != -1)
		{
			infotmp.m_v_Urls.push_back("http://"+soneurl);
			//cout << soneurl << endl;
			infotmp.m_iPartNum++;
		}	
	}		
	return infotmp;
}
string videoaddresssina::GetKeyUrl(const string& vid){

	time_t ts = time(NULL);
	srand(time(NULL));
	float rand_f = (rand()%1000000)/1000000.0;
	string rand_str,ts_str,ts_tmp_str;
	stringstream ss;
	ss.clear();
	ss << rand_f;
	ss >> rand_str;
	//cout << rand_str << " " << ts << endl;
//	ss.clear();
//	ss << ts;
//	ss >> ts_str;

	unsigned int  ts_tmp = ts;
	ts_tmp = ts_tmp >> 6;
	ss.clear();
	ss << ts_tmp;
	ss >> ts_tmp_str;

	string magic = "Z6prk18aWxP278cVAH";
	string compute_str = vid + magic + ts_tmp_str + rand_str;
	string md5_key = md5(compute_str.c_str());
	md5_key = md5_key.substr(0,16);
	string real_key = md5_key + ts_tmp_str;
	string url = "http://v.iask.com/v_play.php?vid=" + vid + "&ran=" + rand_str + "&p=i&k=" + real_key;
	cout << url << endl;
	return url;
}

int videoaddresssina::GetDownloadAddr(unsigned int  return_num)
{
	string sFlvVid,sHDVid;
	bool bcode;
	string url;
/*
	if(m_sUrl.find(".htm") == string::npos&&m_sUrl.find('#') != string::npos){

		size_t vidpos = m_sUrl.find('#');
		sHDVid = m_sUrl.substr(vidpos+1);
		url = "http://v.iask.com/v_play_ipad.php?vid=" + sHDVid;
	}else{
		string content;
		string iphone_agent = "Mozilla/5.0 (iPhone; CPU iPhone OS 6_1_2 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10B146 Safari/8536.25";
		cout << m_sUrl << endl;
		bcode =  GetWebPage(m_sUrl,content,iphone_agent);
		if(bcode == false);
		return -1;
		cout << "111" << endl;
		size_t pos_start = content.find("http://v.iask.com/v_play_ipad");
		if(pos_start == string::npos)
			return -1;
		size_t pos_end = content.find('\"',pos_start);
		if(pos_end == string::npos)
			return -1;
		url = content.substr(pos_start,pos_end-pos_start);
	}
	m_v_sHDUrls.push_back(url);

	m_iParts =  m_v_sHDUrls.size();

	if(ParseNumOutput(return_num)==0)
		return 0;
	else 
		return -1;

	return 0;
*/



//	if(m_sUrl.find(".htm") == string::npos&&m_sUrl.find('#') != string::npos)
	if(m_sUrl.find('#') != string::npos)
	{
		size_t vidpos = m_sUrl.find('#');
		sHDVid = m_sUrl.substr(vidpos+1);
	}else{
		string shtml_content;
		bcode =  GetWebPage(m_sUrl,shtml_content);	
		if(false == bcode)
		{
//			cout << "gather failed!!!" << endl;
			return -1;
		}
		GetVidFromSingleVideoPage(shtml_content,sFlvVid,sHDVid);
	}

	if(sHDVid.empty())
	{
		return -10;
	}
/*	url = "http://v.iask.com/v_play_ipad.php?vid=" + sHDVid;
	m_v_sHDUrls.push_back(url);

	m_iParts =  m_v_sHDUrls.size();

	if(ParseNumOutput(return_num)==0)
		return 0;
	else 
		return -1;

	return 0;
*/


	if(!sHDVid.empty() )
	{
		string sxmlurl = GetKeyUrl(sHDVid);
		//string sxmlurl = "http://v.iask.com/v_play.php?vid=" + sHDVid +  "&uid=0&referrer=http://video.sina.com.cn&ran=0.23728&r=video.sina.com.c";
		//cout << sxmlurl <<endl;
		string sxml_content;
		bcode = GetWebPage(sxmlurl,sxml_content);
		if(false == bcode)
		{
			//PrintNoResutl();
			//exit(-10);
			return -10;
		}
		if(ParseInfo(sxml_content)<0)
			return -1;
/*		sinaVideoInfo sinavinfo = ParseUrlsFromXML(sxml_content);
		m_sSeconds = sinavinfo.m_sTimeLen;
		m_iParts = sinavinfo.m_iPartNum;
		m_v_sFlvUrls = sinavinfo.m_v_Urls;*/	
	}
	else if(!sFlvVid.empty() )	
	{

		cout << "http://v.iask.com/v_play.php?vid=113148043&uid=0&pid=6&tid=408&plid=4001&prid=ja_7_2099518768&referrer=&ran=0.28173780953511596&r=video.sina.com.cn&v=4.1.42.26&p=i&k=bce1ed2e7f3fce5721525672" <<endl;
		string sxmlurl = "http://v.iask.com/v_play.php?vid=" + sFlvVid +  "&uid=0&referrer=http://video.sina.com.cn&ran=0.23728&r=video.sina.com.c";
		cout << sxmlurl <<endl;
		string sxml_content;
		bcode = GetWebPage(sxmlurl,sxml_content);
		if(false == bcode)
		{
			//PrintNoResutl();
			//exit(-10);
			return -10;
		}
		if(ParseInfo(sxml_content)<0)
			return -1;
		//sinaVideoInfo sinavinfo = ParseUrlsFromXML(sxml_content);
		//m_sSeconds = sinavinfo.m_sTimeLen;
		//m_iParts = sinavinfo.m_iPartNum;
		//m_v_sHDUrls = sinavinfo.m_v_Urls;	
	}
	//PrintResultXML();


	if(ParseNumOutput(return_num)==0)
		return 0;
	else 
		return -1;


	return 0;
}
