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

int videoaddressletv::GetDownloadAddr(unsigned int  return_num)
{
    string android_userAgent = "Mozilla/5.0 (Linux; Android 4.3; Nexus 10 Build/JSS15Q) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/29.0.1547.72 Safari/537.36"; 
    string sContent;
    bool bCode = GetWebPage(m_sUrl,sContent);
    if (bCode == false)
    {
        cout << "videoaddressletv::GetWebPage "  <<  m_sUrl << " failed!!!"  << endl;
        return -1;
    }
    string mmsid =Getmmsid(sContent);
    string mmsidurlm3u8="http://dynamic.app.m.letv.com/android/dynamic.php?playid=0&tss=ios&ctl=videofile&mod=minfo&pcode=010110263&act=index&mmsid="+mmsid+"&version=5.0";
    string mmsidurlmp4 ="http://app.m.letv.com/android/mindex.phtml?mod=minfo&ctl=videofile&act=index&mmsid="+mmsid+"&videoformat=&pcode=010110065&version";
    if(m_use_m3u8)
    {
        bCode = GetWebPage(mmsidurlm3u8,sContent,android_userAgent);
        if(bCode==false)
        {
            return -1;
        }
        string m3u8url="";
        bCode = Getm3u8url(sContent,m3u8url);
        if(bCode!=0 || m3u8url == "")
        {
            return -1;
        }
        m_sSeconds = "0";
        m_v_sHDUrls.push_back(m3u8url);
        m_iParts =  m_v_sHDUrls.size();
    }
    else 
    {
        string vid;
        if(GetVid(vid)!=0)
            return -1;
        time_t ts = time(NULL);
        srand(time(NULL));
        float rand_f = (rand()%1000000)/1000000.0;
        string rand_str,ts_str,ts_tmp_str;
        stringstream ss;
        ss.clear();
        ss << rand_f;
        ss >> rand_str;

        string time_url = "http://api.letv.com/time?tn=" + rand_str;

        if(GetWebPage(time_url,sContent) == false)
            return -1;
        string time_str = GetBetween(sContent,"stime\":","}");

        if(time_str.empty())
            return -1;
        unsigned int time_key;
        ss.clear();
        ss << time_str;
        ss >> time_key;
        cout<<"time_key"<<time_key<<endl;
        time_key=GetKey(time_key);
        ss.clear();
        ss << time_key;
        ss >> time_str; 
        string info_url = "http://api.letv.com/mms/out/video/play?id=" + vid + 
            "&platid=1&splatid=101&domain=http://www.letv.com&tkey=" + time_str;
        if(GetWebPage(info_url,sContent) == false)
            return -1;
        bool has_high;
        string video_key; 
        if(sContent.find("1000\":")!=string::npos){
            has_high = true;
            video_key = "1000\":[\"";
        }else if(sContent.find("350\":")!=string::npos){
            has_high = false;
            video_key = "350\":[\"";
        }else
            return -1;

        string link;

        size_t bpos,epos;
        epos=0;
        string low_data;
        while(true){
            low_data.clear();
            if((bpos =  sContent.find(video_key,epos))==string::npos)
                break;
            if((epos = sContent.find('"',bpos+video_key.size()+1))==string::npos)
                break;
            string sub_content = sContent.substr(bpos+video_key.size(),epos-bpos-video_key.size());
            for(unsigned int i = 0; i< sub_content.size();i++){
                if(sub_content[i]!='\\')
                    low_data +=sub_content[i];
            }
            //cout << low_data << endl; 
            if(memcmp(low_data.c_str(),"http",4)==0)
                break;
            epos++;
        }
        size_t tss_pos = low_data.find("tss=");
        if(tss_pos==string::npos)
        {   
            if(m_use_m3u8)
            {
                low_data = low_data+"tss=ios";   
            }
            else
            {

                low_data= low_data+"tss=no";
            }
        }
        else
        {
            size_t pos_back = low_data.find("&",tss_pos+1);
            if(pos_back!=string::npos)
            {   
                low_data = low_data.substr(0,tss_pos)+low_data.substr(pos_back);
            }
            else low_data = low_data.substr(0,tss_pos);

            if(m_use_m3u8)
            {
                low_data = low_data+"tss=ios";

            }
            else

            {
                low_data = low_data+"tss=no";
            }
        }
        m_sSeconds = "0";
        m_v_sHDUrls.push_back(low_data);
        m_iParts =  m_v_sHDUrls.size();

    }
    if(ParseNumOutput(return_num)==0)
        return 0;

}
int videoaddressletv::Getm3u8url(string htmlcontent,string & m3u8url)
{
    string content="";
    if(htmlcontent.find("mp4_1000")!=string::npos)
    {
        content = htmlcontent.substr(htmlcontent.find("mp4_1000"));
    }
    else if(htmlcontent.find("mp4_350")!=string::npos)
    {
        content = htmlcontent.substr(htmlcontent.find("mp4_350"));
    }else if(htmlcontent.find("mp4_1300")!=string::npos)
    {
        content = htmlcontent.substr(htmlcontent.find("mp4_1300"));
    }
    else return -1;


    string tempurl = GetBetween(content,"backUrl2\":\"","\"");
    tempurl=Formaturl(tempurl);
    if(tempurl=="")
    {
        return -1;
    }
    bool bcode = GetWebPage(tempurl,content);
    if(false == bcode)
    {
        return -1;
    }
    m3u8url = GetBetween(content,"location\": \"","\"");
    m3u8url = Formaturl(m3u8url);
    return 0;

}

string videoaddressletv::GetFlagContent(const string &html_content,const string & flag){

    string vflag = flag;
    string::size_type beg = html_content.find(vflag);
    if(beg == string::npos)
        return "";
    beg += vflag.length();
    string vcode="";
    char c ;
    do{
        c = html_content[beg];
    }while((c==' ' || c == '\'' || c== '\"' || c==':' || c=='=') &&++beg<html_content.size());

    while((c !=' '&& c != '\'' && c!='\"' &&c!='\r' && c!='\n')&& ++beg< html_content.size()){
        vcode += c;
        c =html_content[beg];
    }

    return vcode;
}

string videoaddressletv::Getmmsid(const string & htmlcontent){
    string mmsid = "";
    mmsid = GetFlagContent(htmlcontent,"mmsid:");
    if(mmsid.find(",")!=string::npos)
    {
        mmsid = mmsid.substr(0,mmsid.find(","));
    }
    if(mmsid=="")
    {
        mmsid = GetBetween(htmlcontent,"mmsid:",",");   
    }
    return mmsid;
}

string videoaddressletv::videoaddressletv::Formaturl(const string &url)
{
    string return_url = "";
    for(int i =0 ;i<url.length();i++)
    {
        if(url[i]!='\\')
            return_url = return_url + url[i];
    }
    return return_url;
}

unsigned int  videoaddressletv::GetKey(unsigned int time_key){

    unsigned int e;
    for(int i = 0 ; i< 8 ; i++){
        e = 1 & time_key;
        time_key >>=1;
        e <<=31;
        time_key += e;
    }
    cout<<time_key<<"\t"<<(time_key^185025305)<<endl;
    return time_key^185025305;

}
int videoaddressletv::GetVid(string & vid)
{
    string sContent;
    bool bCode = GetWebPage(m_sUrl,sContent);
    if (bCode == false)
    {
        cout << "videoaddressletv::GetWebPage "  <<  m_sUrl << " failed!!!"  << endl;
        return -1;
    }
    vid = GetBetween(sContent,"vid:",",");
    if(vid.empty()){
        vid = GetBetween(sContent,"vid :",",");
        if(vid.empty()){
            cout << "videoaddressletv::Getvid error " << endl;
            return -1;
        }

    }
    string rvid;
    for(int i=0;i< vid.size();i++){
        if(isdigit(vid[i]))
            rvid+=vid[i];
    }
    vid = rvid;

    return 0;
}
