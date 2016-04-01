#include "videoaddress.h"
#include <iostream>
#include <string>
#include <string.h>
using namespace  std;
#include "utility.h"
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <sstream>
int videoaddresspps::GetDownloadAddr(unsigned int return_num){

    string htmlcontent;
    bool bcode =false;
    bcode = GetWebPage(m_sUrl,htmlcontent);
    if(bcode==false)
    {
        return -1;
    }

    string url_key = Get_urlkey(htmlcontent);
    string tv_id = GetBetween(htmlcontent,"tv_id:\"","\"");


    if(tv_id!="")
    {   
        string tmp_url = "http://cache.m.iqiyi.com/jp/qmt/"+tv_id+"/";
        if(GetDownloadAddressfromiqiyi(tmp_url,return_num)==0)
        {
            return 0;
        }

    }
    if(!url_key.empty())
    {
        string UrlkryUrl="http://dp.ugc.pps.tv/get_play_url_cdn.php?sid="+url_key+"&flash_type=1";
        if(GetDownloadAddressfromppspc(UrlkryUrl, return_num)==0)
        {
            return 0;
        }
        if(GetDownloadAddressfromppsph(url_key, return_num)==0)
        {
            return 0;
        }
        else return -1;


    }


}
string  videoaddresspps::Get_urlkey(const string &htmlcontent)
{
    string url_key = "";
    url_key = GetBetween(htmlcontent,"url_key",",");
    url_key = GetBetween(url_key,"\"","\"");
    if(url_key==":")
    {
        url_key=GetBetween(htmlcontent,"\"url_key\":\"","\"");
    }
    if(url_key.empty())
    {
        url_key=GetBetween(m_sUrl,"play_",".html");
    }
    return url_key;
}
int videoaddresspps::GetDownloadAddressfromppsph(string urlkey,unsigned int return_num)
{


    bool bcode;
    string htmlcontent="";
    time_t timer = time(NULL);
    timer=timer*1000;
    stringstream ss;
    string timer_str = "";
    ss<<timer;
    ss>>timer_str;
    string json_url ="http://nc.ipd.pps.tv/web/public/aj_html5_player.php?jsonp_callback=jQuery172046692719403654337_"+timer_str+"&_="+timer_str+"&url_key="+urlkey; 
    string agent = "Mozilla/5.0 (Windows NT 6.1; rv:27.0) Gecko/20100101 Firefox/27.0";
    bcode = GetWebPageReferer(json_url,m_sUrl,htmlcontent,agent);
    if(bcode==false)
    {       
        return -1;
    }
    string path = GetBetween(htmlcontent,"play_url\":\"","\"");
    if(path=="")
    {
        return -1;
    }
    string tempurl = "";
    for(int i=0;i<path.length();i++)
    {
        if(path[i]!='\\')
            tempurl = tempurl+path[i];
    }
    bcode = GetWebPage(tempurl,htmlcontent);
    string playurl = "";
    if(bcode==false)
    {
        return -1;
    }
    else{
        playurl = GetBetween(htmlcontent,"l\":\"","\"");
    }
    m_v_sHDUrls.push_back(playurl);
    m_iParts =  m_v_sHDUrls.size();
    if(ParseNumOutput(return_num)==0)
        return 0;
    else
        return -1;

    return 0;


}
int videoaddresspps::GetDownloadAddressfromppspc(string midurl,unsigned int return_num)
{

    bool bcode;
    string htmlcontent="";
    bcode = GetWebPage(midurl,htmlcontent);
    if(bcode==false)
    {
        return -1;
    }
    string path=GetBetween(htmlcontent,"http","?");
    if(path.find(".pfv")!=string::npos)
    {
        path = "http"+path;
        m_sSeconds="0";
        m_v_sHDUrls.push_back(path);
        m_iParts =  m_v_sHDUrls.size();

        if(ParseNumOutput(return_num)==0)
            return 0;
        else
            return -1;

        return 0;
    }
    else {return -1;}
}
int videoaddresspps::GetDownloadAddressfromiqiyi(string midurl,unsigned int return_num){
    video_part  playurl_info; 
    bool bcode;
    string htmlcontent;
    string tmp_url = midurl;
    bcode = GetWebPage(tmp_url,htmlcontent);
    if(false == bcode)
    {
        return -1;
    }
    video_part  urlinfo;
    if(m_use_m3u8)
    {
        if(-1==Get_playurlByvd(htmlcontent,urlinfo,96,0)&&-1==Get_playurlByvd(htmlcontent,urlinfo,1,0))
        {
            return -1;
        }
    }
    else
    {
        if(-1==Get_playurlByvd(htmlcontent,urlinfo,2,1)&&-1==Get_playurlByvd(htmlcontent,urlinfo,1,1))
        {
            return -1;
        }

    }
    string tmp_playurl = urlinfo.url;
    if(!m_use_m3u8)
    {
        if(false==GetWebPage(tmp_playurl,htmlcontent))
        {
            return -1;
        }
        tmp_playurl =  GetBetween(htmlcontent,"l\":\"","\"");
        if(tmp_playurl.empty()){

            return -1;
        }
    }
    playurl_info.url=tmp_playurl;
    playurl_info.size = urlinfo.size;
    playurl_info.seconds=urlinfo.seconds;

    m_sSeconds = playurl_info.seconds;
    m_v_sHDUrls.push_back(playurl_info.url);
    m_iParts =  m_v_sHDUrls.size();
    if(ParseNumOutput(return_num)==0)
        return 0;
    else
        return -1;
}
int videoaddresspps::Get_playurlByvd( string htmlcontent,video_part & urlinfo,int vd,int type){

    string playtype ;

    if(type==0){
        playtype="\"m3u\":";
    }else
        playtype="\"m4u\":";
    char temp [10]={0};
    string tem_vd;
    sprintf(temp,"\"vd\":%d",vd);
    tem_vd=temp;
    string content;
    for(;;){
        content = GetBetween(htmlcontent,playtype,"}");
        if(content.empty())
        {
            break;
        }
        if(content.find(tem_vd)!=string::npos){
            break;
        }else
        {
            htmlcontent=htmlcontent.substr(htmlcontent.find(content)+content.length());
            content = "";
            continue;
        }
    }
    if(content.empty()||content.find("http")==string::npos)
    {
        return -1;
    }
    content=content+",";
    urlinfo.url ="http"+ GetBetween(content,"http","\"");
    urlinfo.seconds=GetBetween(content,"plt\":",",");
    urlinfo.size=GetBetween(content,"\"fle\":","\"");

    return 0;

}
