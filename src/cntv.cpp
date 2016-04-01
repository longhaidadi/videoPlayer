#include "videoaddress.h"
#include <string>
#include <iostream>
using namespace std;
#include "utility.h"
#include <math.h>
#include <stdio.h> 
#include <stdlib.h> 

int videoaddresscntv::GetDownloadAddr(unsigned int return_num){

    string htmlcontent;
    if(GetWebPage(m_sUrl,htmlcontent)==false)
        return -1;
    
    string videoCenterId = GetBetween(htmlcontent,"videoCenterId:",",");
    if(!videoCenterId.empty())
        videoCenterId = GetBetween(videoCenterId,"\'","\'");
    else 
     {
         videoCenterId = GetBetween(htmlcontent,"\"videoCenterId\"",")");
         videoCenterId = GetBetween(videoCenterId,"\"","\"");
     }

    if(videoCenterId.empty())return -1;
    string mid_url="http://vdn.apps.cntv.cn/api/getHttpVideoInfo.do?pid="+videoCenterId;
    
    if(GetWebPage(mid_url,htmlcontent)==false)
        return -1;


    string content = GetBetween(htmlcontent,"chapters2","]");
    if(content.empty())
        content = GetBetween(htmlcontent,"is_p2p_use","]");
    if (content.empty())
        content = GetBetween(htmlcontent,"lowChapters","]");
    string totalLength = GetBetween(htmlcontent,"totalLength\":\"","\"");
    content = content +"]";

    float  total_length=0;
    content = GetBetween(content,"[","]");
    vector<string> part_list;
    GetBetweenList(content,0,"{","}",part_list);
    for(int i = 0; i< part_list.size() ; i++){
        video_part vpt;
        string play_url;
        string seconds;
        string size;

        seconds = GetBetween(part_list[i],"duration\":\"","\"");
        size = "1";
        play_url=GetBetween(part_list[i],"url\":\"","\"");

        if(seconds.empty()||play_url.empty())continue;
        total_length += atof(seconds.c_str());
        vpt.url = play_url;
        vpt.seconds = seconds;
        vpt.size = size;
        m_v_Parts.push_back(vpt);
    }

    if(totalLength.empty())
        m_sSeconds = totalLength;
    else {
        char  total_time[11] = {'\0'};
        sprintf(total_time,"%f",total_length);
        m_sSeconds=total_time;

    }
    m_iParts =  part_list.size();
    if(ParseNumOutput(return_num)==0)
        return 0;
    else
        return -1;



}
