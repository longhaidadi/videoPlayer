#include"videoaddress.h"
#include "json/value.h"
#include "json/json.h"
#include "urlcode.h"
#include "utility.h"
#include "stdio.h"
#include <iostream>
#include <cassert>
#include <cstring>
#include <sstream>
#include <fstream>

/************************************************************************/
/* kankan*/
/************************************************************************/


int videoaddresspptv::GetDownloadAddr(unsigned int  return_num)
{
	string tmp_url = m_sUrl;
	//title = Gettile(m_sUrl);
	string webcfg_start = "var webcfg = {\"id\":";
	string htmlcontent;
	bool bcode = GetWebPage(m_sUrl,htmlcontent);
	if (false == bcode)
	{
		return -1;
	}
	string id = GetBetween(htmlcontent,webcfg_start,",");
//	cout << id << endl;
	if(id.empty()){
		return -1;
	}

	string total_seconds = GetBetween(htmlcontent,"\"duration\":\"","\"");
	if(total_seconds.empty())
		return -1;
	

	string info_url  = "http://web-play.pptv.com/webplay3-0-" + id +".xml&type=web.fpp&version=4";
    cout<< info_url <<endl;
	if(!GetWebPage(info_url,htmlcontent))
		return -1;

	string file_info = GetTagPart(htmlcontent,"file",0);
	if(file_info.empty())
		return -1;
	vector<string> file_list;
	GetBetweenList(file_info,0,"<item","/>",file_list);
	if(file_list.size()==0)
		return -1;
    //cout<<"file_list"<<file_list.size()<<endl;
	int idx = min((int)file_list.size(),2) -1;

    //cout<<"idx"<<idx<<endl;

	string file_name = GetBetween(file_list[idx],"rid=\"","\"");
	if(file_name.empty())
		return -1;

	char dt_buf[100]={'\0'};
	sprintf(dt_buf,"<dt ft=\"%d\">",idx);
	string  dt_info =  GetBetween(htmlcontent,dt_buf,"</dt>");
	if(dt_info.empty()){
			return -1;
	}
	size_t drag_pos = htmlcontent.find(dt_buf) + strlen(dt_buf);


	string key_ex = GetBetween(dt_info,"<key expire=","key>");
	if(key_ex.empty())
		return -1;
	string key = GetBetween(key_ex,">","<");
	if(key.empty())
		return -1;
	string ip_addr = GetBetween(dt_info,"<sh>","</sh>");
	if(ip_addr.empty())
		return -1;	
	string drag_info = GetTagPart(htmlcontent,"dragdata",drag_pos);
	vector<string> part_list;
	GetBetweenList(drag_info,0,"<sgm","/>",part_list);	
	int part_size = part_list.size();
	if(part_size == 0)
		return -1;
	for(int i = 0; i< part_size ; i++){
		video_part vpt;
		string play_url;
		string seconds;
		string size;
		string part_num;
		stringstream streamtmp;
		streamtmp << i;
		streamtmp >> part_num;

		play_url = 	"http://" + ip_addr + "/" + part_num + "/" +file_name+"?k="+key+"&type=web.fpp";
		seconds = GetBetween(part_list[i],"dur=\"","\"");
		size = GetBetween(part_list[i],"fs=\"","\"");

		if(seconds.empty() || size.empty())
			continue;
		size_t pos;
		if( (pos=seconds.rfind('.'))!=string::npos){
			seconds = seconds.substr(0,pos);
		}
		vpt.url = play_url;
		vpt.seconds = seconds;
		vpt.size = size;	
		m_v_Parts.push_back(vpt);


	}

	m_sSeconds = total_seconds;
	m_iParts =	m_v_Parts.size();
	if(ParseNumOutput(return_num)==0)
		return 0;
	else 
		return -1;

}
