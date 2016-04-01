#include "videoaddress.h"
#include <cstring>
#include <string>
#include <iostream>
#include "json/value.h"
#include "json/json.h"
#include <vector>
#include "utility.h"
#include <stdlib.h>
using namespace std;


int videoaddresssohu::GetDownloadAddr(unsigned int return_num){
	string htmlcontent;

	string vid = GetVidFromUrlcontent();
	if(vid.empty())return -1;

	string jsurl = "http://m.tv.sohu.com/mb/dist/js/basePlayPage.min.20140610.js?t=2";
	if(GetWebPage(jsurl,htmlcontent)==-1)
		return -1;
	string key = GetBetween(htmlcontent,"API_KEY=\"","\"");

	string tmpurl = "http://api.tv.sohu.com/v4/video/info/"+vid+".json?site=1&api_key="+key+"&plat=17&callback=jsonp6&sver=4.0&partner=78";
	if(GetWebPage(tmpurl,htmlcontent)==-1)
	  tmpurl = "http://api.tv.sohu.com/v4/video/info/"+vid+".json?site=2&api_key="+key+"&plat=17&callback=jsonp6&sver=4.0&partner=78";
	if(GetWebPage(tmpurl,htmlcontent)==-1)
		return -1;
	string urlcontent = GetBetween(htmlcontent,"\"url_super_mp4\":\"","\"");
	string timecontent = GetBetween(htmlcontent,"clips_duration_super\":\"","\"");
	vector<string>url;vector<string>seconds;
	Split(urlcontent,",",url);
	Split(timecontent,",",seconds);

	for(int i =0 ; i<url.size()&&i<seconds.size();i++)
	{


		video_part vpt;
		string play_url;
		string second;
		string size;

		play_url=url[i];
		second = seconds[i];
		cout<<i<<"\t"<<play_url<<"\t"<<second<<endl;
		vpt.url = play_url;
		vpt.seconds = second;
		m_v_Parts.push_back(vpt);
	}


	return 0;


}

string videoaddresssohu::GetVidFromUrlcontent(){
	string htmlcontent;
	if(GetWebPage(m_sUrl,htmlcontent)==false)
		return "";
	else {
		string vid = GetBetween(htmlcontent,"var vid=\"","\"");
		return vid;
	}
}
