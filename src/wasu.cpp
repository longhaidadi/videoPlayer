#include "utility.h"
#include <string>
#include"videoaddress.h"
#include <iostream>
using namespace std;

#include "utility.h"
int videoaddresswasu::GetDownloadAddr(unsigned int  return_num){
    string htmlcontent;
    bool bcode = GetWebPage(m_sUrl,htmlcontent);
    if (false == bcode)
    {
        return -1;
    }

    string playurl = GetBetween(htmlcontent,"_playUrl = \'","\'");
    string seconds = GetBetween(htmlcontent,"_playDuration = \'","\'");
    if(playurl=="")
    {
        return -1;
    }
    if(seconds=="")
        m_sSeconds = "0";
    else m_sSeconds = seconds;
    string tmpurl = playurl;
    if(tmpurl.find("ifeng.com")!=string::npos)
    {
        if(-1==GetVideoFromifeng( tmpurl,return_num))
        {
            return -1;
        }
        return 0;
    }
    string encode_url= playurl;
    string url64;
    CBase64::Encode((const unsigned char * )encode_url.c_str(),encode_url.length(),url64);
    string temp_url = "http://www.wasu.cn/Api/getVideoPath/id/"+url64+"/datatype/xml";
    if(false ==GetWebPage(temp_url,htmlcontent))
    {
        return false;
    }
    string down_url = GetBetween(htmlcontent,"[CDATA[","]");
    m_v_sHDUrls.push_back(down_url);
    m_iParts =m_v_sHDUrls.size();

    //m_sSeconds="1";


    if(ParseNumOutput(return_num)==0)
        return 0;
    else 
        return -1;



};
int videoaddresswasu::GetVideoFromifeng( string ifengswfurl,unsigned int return_num){

    string htmlcontent;
    string guid = GetBetween(ifengswfurl,"guid=","&");
    if(guid.empty())
    {
        guid=ifengswfurl.substr(ifengswfurl.find("guid=")+5);
    }
    if(guid.length()!=36)
    {
        return -1;
    }
    string xmlurl = "http://v.ifeng.com/video_info_new/"+guid.substr(34,1)+"/"+guid.substr(34,2)+"/"+guid+".xml";
    if(GetWebPage(xmlurl,htmlcontent)==-1)
    {
        return -1;
    }
    vector<string> file_list;
    GetBetweenList(htmlcontent,0,"<parts","/>",file_list);
    if(file_list.size()==0)
    {
        return -1;
    }
    for(int i =0;i<file_list.size();i++)
    {
        video_part vpt;
        string play_url;
        string seconds;
        play_url=GetBetween(file_list[i],"playurl=\"","\"");
        seconds=GetBetween(file_list[i],"duration=\"","\"");
        if(seconds.find(".")!=string::npos)
        {
            seconds=seconds.substr(0,seconds.find("."));
        }
        if(play_url.empty()||seconds.empty())
        {
            continue;
        }
        vpt.url = play_url;
        vpt.size="0";
        vpt.seconds = seconds;
        m_v_Parts.push_back(vpt);

    }

    m_sSeconds = GetBetween(htmlcontent,"Duration=\"","\"");;
    m_iParts =  m_v_Parts.size();
    if(ParseNumOutput(return_num)==0)
     
        return 0;
    else
            return -1;
            
}

