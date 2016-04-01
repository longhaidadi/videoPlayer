#include"videoaddress.h"
#include "json/value.h"
#include "json/json.h"
#include "urlcode.h"
#include "utility.h"
#include <iostream>
#include <cstring>
#include <stdlib.h> 
int videoaddressfunshion::getId(string & id,string & cat, string &  playNum)
{
    string htmlcontent="";
    bool icode = GetWebPage(m_sUrl,htmlcontent);
    if(icode == false) return -1;
    id = GetBetween(htmlcontent,"\"mediaid\":",",");
    cat = GetBetween(htmlcontent,"displaytype\":\"","\"");
    playNum = GetBetween(htmlcontent,"minfo.playNumber = \'","\'");
    if(id=="" or cat== "" or playNum == "")return -1;
    return 0;

}

int videoaddressfunshion::GetDownloadAddr(unsigned int  return_num)
{
    string id,cat,playNum="";
    if(getId(id,cat,playNum)==-1) return -1;
    string mp4URL = "http://jsonfe.funshion.com/media/?cli=aphone&ver=0.0.0.1&ta=0&mid="+id;
    string m3u8URL="http://jsonfe.funshion.com/media/?cli=ipad&ver=1.1.2.6&jk=0&mid="+id;
    if(m_use_m3u8)
    {
        string htmlcontent = "";
        bool icode = GetWebPage(m3u8URL,htmlcontent);
        if(icode==false)return -1;
        getURLtable(htmlcontent,cat,playNum);
    }
    else 
    {
        //       cout<<mp4URL<<endl;
        string htmlcontent = "";
        bool icode = GetWebPage(mp4URL,htmlcontent);
        if(icode==false)return -1;
        //    if(-1==getURLtable(htmlcontent,cat,playNum))
        //  {
        getUrlFormPC(id,cat,playNum);
        //    }
    }

    m_iParts =  m_v_sHDUrls.size();

    if(ParseNumOutput(return_num)==0)
        return 0;
    else
        return -1;


}

int videoaddressfunshion::getURLtable(string fmtjson,string category,string index)
{
    Json::Reader reader;
    Json::Value root;
    if(!reader.parse(fmtjson,root,false)) return -1;
    Json::Value mpurls;
    if(root["return"].asString()=="error")
    {
        return -1;
    }
    if(category == "movie")
        mpurls = root["data"]["pinfos"]["mpurls"];
    else if(category=="tv" )
    {	
        int num = atoi(index.c_str());
        mpurls = root["data"]["pinfos"]["content"]["gylang"];
        if(mpurls.size()==0)
            mpurls=root["data"]["pinfos"]["content"]["yslang"];
        if(mpurls.size()==0)return -1;
        mpurls = mpurls["fsps"][num-1]["mpurls"];
        cout<<mpurls<<endl;

    }else if(category == "cartoon")
    {
        int num = atoi(index.c_str());
        mpurls = root["data"]["pinfos"]["content"]["yslang"];
        if(mpurls.size()==0)
            mpurls = root["data"]["pinfos"]["content"]["gylang"];
        if(mpurls.size()==0)return -1;
        mpurls = mpurls["fsps"][num-1]["mpurls"];
    }
    else if(category == "variety")
    {
        mpurls = root["data"]["pinfos"]["fsps"];
        for(int i =0 ;i<mpurls.size();i++)
        {
            if(index == mpurls[i]["number"].asString())
            {
                mpurls =  mpurls[i]["mpurls"];
                break;
            }
        }
    }
    if(mpurls.size()==0)return -1;
    string tmpUrl = mpurls["dvd"]["url"].asString();
    if(tmpUrl=="")tmpUrl = mpurls["highdvd"]["url"].asString();
    if(tmpUrl=="")tmpUrl =  mpurls["tv"]["url"].asString(); 
    if(tmpUrl=="")return -1;

    if(m_use_m3u8)
    {
        m_v_sHDUrls.push_back(tmpUrl);
        m_sSeconds = "0";
        return 0;

    }
    string htmlcontent = "";
    bool icode  = GetWebPage(tmpUrl,htmlcontent);
    if(icode==false)return -1;
    string size = GetBetween(htmlcontent,"size\":\"","\"");
    string playurl = GetBetween(htmlcontent,"http","\"");
    string url = "http";
    for(int i =0 ;i<playurl.size();i++)
        if(playurl[i]!='\\')
            url = url + playurl[i];
    if(url=="http")return -1;
    m_v_sHDUrls.push_back(url);
    m_sSeconds = "0";
    return 0;
}


int videoaddressfunshion::getUrlFormPC(string id,string cat,string index)
{
    string url = "http://api.funshion.com/ajax/vod_panel/"+id+"/w-1?isajax=1";
    string htmlcontent = "";
    cout<<url<<endl<<cat<<endl<<index<<endl;
    bool icode  = GetWebPage(url,htmlcontent);
    if(icode==false)return -1;

    Json::Reader reader;
    Json::Value root;
    if(!reader.parse(htmlcontent,root,false)) return -1;
    Json::Value mpurls;
    if(root["status"].asInt()!=200)
    {
        cout<<"status failure"<<endl;
        return -1;
    }
    if(cat == "tv" || cat == "movie")
    {
        int num = atoi(index.c_str());
        cout<< root["data"]["fsps"]["mult"][1]<<endl;
        mpurls = root["data"]["fsps"]["mult"][num-1]["hashid"];
    }
    else if (cat =="variety"||cat == "cartoon")
    {
        mpurls = root["data"]["fsps"]["mult"];
        for(int i =0 ;i<mpurls.size();i++)
        {
            cout<< mpurls[i]["number"].asInt()<<endl;
            if(atoi(index.c_str()) == mpurls[i]["number"].asInt())
            {
                mpurls =  mpurls[i]["hashid"];
                break;
            }
        }
    }

    string hashid = mpurls.asString();
    string jsonurl = "http://jobsfe.funshion.com/query/v1/mp4/"+hashid+".json";
    cout<<jsonurl<<endl;
    icode  = GetWebPage(jsonurl,htmlcontent);
    if(icode==false)return -1;
    string size = GetBetween(htmlcontent,"size\":\"","\"");
    string playurl = GetBetween(htmlcontent,"http","\"");
    url = "http";
    for(int i =0 ;i<playurl.size();i++)
        if(playurl[i]!='\\')
            url = url + playurl[i];
    if(url=="http")return -1;
    m_v_sHDUrls.push_back(url);
    m_sSeconds = "0";
    return 0; 



}
