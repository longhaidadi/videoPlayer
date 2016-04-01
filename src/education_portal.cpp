#include "videoaddress.h"
#include <iostream>
#include <string>
#include <string.h>
using namespace  std;
#include "utility.h"
#include <stddef.h>
#include <stdlib.h>
#include <sstream>


int videoaddresseducation_portal::GetDownloadAddr(unsigned int return_num)
{
    string htmlcontent;
    bool icode = GetWebPage(m_sUrl,htmlcontent);
    if(icode == false)
    {
        cout<<"GetWebPage error: "<<m_sUrl<<endl;
        return -1;
    }
    string vid = "";
    if(-1==GetVid(htmlcontent,vid))
        return -1;

    string jsonurl = "http://fast.wistia.com/embed/medias/"+vid+".json?callback=json1";
    string agent = "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/22.0.1229.95 Safari/537.4";
    icode= GetWebPageReferer(jsonurl,m_sUrl,htmlcontent,agent);
    if(icode==false)
        return -1;
    string playurl = GetBetween(htmlcontent,"url\":\"","\"");
    string size = GetBetween(htmlcontent,"size\":",",");
    string seconds = GetBetween(htmlcontent,"duration\":",",");
    if(playurl.empty())
        return -1;
    m_sSeconds = seconds;
    m_v_sHDUrls.push_back(playurl);
    m_iParts =  m_v_sHDUrls.size();

    if(ParseNumOutput(return_num)==0)
        return 0;
    else
        return -1;
    return 0;

}

int videoaddresseducation_portal::GetVid(const string & htmlcontent,string & vid)
{
    vid = GetBetween(htmlcontent,"<div id=\"wistia_","\"");
    if(vid.empty())
    {
        vid = GetBetween(htmlcontent,"data-wid=\"","\"");
        if(vid.empty()) 
            return -1;
    }
    return 0;
}

