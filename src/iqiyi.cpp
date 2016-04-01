#include "videoaddress.h"
#include <cstring>
#include <string>
#include <iostream>
#include "json/value.h"
#include "json/json.h"
#include <vector>
#include "utility.h"
#include <stdlib.h>
#include<ctime>
using namespace std;

string videoaddressiqiyi::Getpath(string path)
{
	size_t pos_start = path.rfind("/");
	size_t pos_end = path.find(".f4v");
	if(pos_end!=string::npos && pos_start != string::npos && pos_end>pos_start)
		return path.substr(pos_start+1,pos_end-1-pos_start);
	return "";
}
int videoaddressiqiyi::GetDownloadAddr(unsigned int return_num){
	string tvid = "";
	string videoid = "";
	string video_id="";
	if(m_sUrl.find("pps")!=string::npos)
		Getidfrompps(tvid,videoid,video_id);
	else 
		Getidfromiqiyi(tvid,videoid);
	string tmpurl = "http://cache.video.qiyi.com/vp/"+tvid+"/"+videoid+"/";
	cout<<tmpurl<<endl;
	string htmlcontent="";
	if(tvid.empty() || videoid.empty())
	{
		string id = GetBetween(m_sUrl,"play_",".html");
		string url = "http://dp.ppstream.com/get_play_url_cdn.php?sid="+id+"&flash_type=1";
		bool icode = GetWebPage(url,htmlcontent);
		if(icode==false)
			return -1;
		if(htmlcontent.find("pfv")!=string::npos){
			string mp4url = GetBetween(htmlcontent,"http","pfv");
			if(mp4url.empty()) return -1;

			mp4url = "http"+mp4url+"pfv";
			video_part vpt;
			vpt.url=mp4url;
			m_v_Parts.push_back(vpt);
		}else if(video_id.empty())
			return -1;
	}

	/*	string m3u8url = "http://cache.m.iqiyi.com/jp/tmts/"+tvid+"/"+videoid+"/?platForm=h5&type=mp4&rate=1&__sigC=f5e837de28d7aa33bda0bf4df2d311b5";


		bool icode = GetWebPage(m3u8url,htmlcontent);
		if(icode==false)
		return -1;

		string mp4url = GetBetween(htmlcontent,"\"m3u\":\"","\"");
		if(mp4url.empty()) return -1;
		video_part vpt;
		vpt.url=mp4url;
		m_v_Parts.push_back(vpt);
	 */
	int retcode=parajson(tmpurl);
	if(retcode!=0)
	{
		string temurl = "http://api.ipd.pps.tv/iqiyi/getIqyPlayUrl/"+video_id+"/";
		retcode = parajson(temurl);
		if(retcode!=0)
		{
			string m3u8url = "http://cache.m.iqiyi.com/jp/tmts/"+tvid+"/"+videoid+"/?platForm=h5&type=mp4&rate=1&__sigC=f5e837de28d7aa33bda0bf4df2d311b5";


			bool icode = GetWebPage(m3u8url,htmlcontent);
			if(icode==false)
				return -1;

			string mp4url = GetBetween(htmlcontent,"\"m3u\":\"","\"");
			if(mp4url.empty()) return -1;
			video_part vpt;
			vpt.url=mp4url;
			m_v_Parts.push_back(vpt);
		}
	}
	return 0;
}

int videoaddressiqiyi::Getidfromiqiyi(string & tvid,string & videoid){

	string htmlcontent;
	if(GetWebPage(m_sUrl,htmlcontent)==false)
		return -1;
	else {
		size_t pos_s = htmlcontent.find("播放区");
		if(string::npos != pos_s) htmlcontent = htmlcontent.substr(pos_s); 
		videoid = GetBetween(htmlcontent,"data-player-videoid=\"","\"");
		tvid = GetBetween(htmlcontent,"tvid=\"","\"");
	//	title = Gettitle(m_sUrl);

		return 0;
	}
	return 0;
}

int videoaddressiqiyi::Getidfrompps(string & tvid,string & videoid,string & video_id){

	string htmlcontent;
	if(m_sUrl.find("//"))return -1;
	if(GetWebPage(m_sUrl,htmlcontent)==false)
		return -1;
	else {
		tvid = GetBetween(htmlcontent,"tv_id:\"","\"");
		videoid  = GetBetween(htmlcontent,"vid:\"","\"");
		video_id = GetBetween(htmlcontent,"\"video_id\":\"","\"");
	//	title = Gettitle(m_sUrl);
		return 0;
	}
	return 0;
}

int videoaddressiqiyi::parajson(string jsonurl)
{	
	string htmlcontent;
	bool icode = GetWebPage(jsonurl,htmlcontent);
	if(false == icode )return -1;
	Json::Reader reader;
	Json::Value root;
	if(!reader.parse(htmlcontent,root,false))
		return -1;
	Json::Value videoinfovs;
	cout<<root["aid"]<<endl;
	int num =0;
	if(root["tkl"].size()==0)return -1;
	videoinfovs =root["tkl"][num]["vs"];
	bool sucess = false;
	for(int j = 0;j<videoinfovs.size();j++)
	{
		const Json::Value videoinfo = videoinfovs[j]["fs"];
		for(int i =0 ;i<videoinfo.size();i++)
		{
			string path = videoinfo[i]["l"].asString();
			string id  = Getpath(path);
			if(id == "") break;
			time_t now_time;
			now_time = time(NULL);
			long int time = (now_time/600);
			char time_str[15]={0};
			sprintf(time_str,"%ld",time);
			string md5_f = "";
			md5_f=md5_f+time_str+")(*&^flash@#$%a"+id;
			string md5_key =  md5(md5_f.c_str());
			string md5url = "http://data.video.qiyi.com/"+md5_key+"/videos"+path+"?retry=1&z=zibo_cnc&bt=&ct=&tn=60608";
			cout<<md5url<<endl;
			if(GetWebPage(md5url,htmlcontent)==false)
				return -1;
			string playurl = GetBetween(htmlcontent,"\"l\":\"","\"");
			if(playurl.empty()) return -1;
			video_part vpt;
			vpt.url = playurl;
			m_v_Parts.push_back(vpt);
			sucess = true;

		}
		if(sucess==true) break;
	}
	return 0;
}
