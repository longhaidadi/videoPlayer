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
int videoaddressku6::GetDownloadAddr(unsigned int  return_num){

    bool bcode  = false ;
    string htmlcontent = "" ;
    /*
     * vid
     */
    string vid="";
    size_t n_pos_start = m_sUrl.rfind("/");
    size_t n_pos_end = m_sUrl.find(".html");
    if(n_pos_start!=string::npos&&n_pos_end!=string::npos){
        vid = m_sUrl.substr(n_pos_start+1,n_pos_end-n_pos_start-1);
    }
    cout<<vid<<endl;
    if(vid.empty()||vid.find("index")!=string::npos||vid.find("show")!=string::npos)
    {
        bcode= GetWebPage(m_sUrl,htmlcontent);
        if (false == bcode)
        {
            return -1;
        }
        vid = GetBetween(htmlcontent,"data-vid=\"","\"");
        if(vid.empty()){
            vid = GetBetween(htmlcontent,"vid : \'","\'");
        }
        if(vid.empty()){
            vid = GetBetween(htmlcontent,"vid:\"","\"");
        }
        if(vid.empty()){
            vid = GetBetween(htmlcontent,"vid:",",");
        }
        size_t n_space_pos = vid.find_first_not_of("\r\t\n ");
        if(n_space_pos!=string::npos){
            vid.erase(0,n_space_pos);
        }
        n_space_pos = vid.find_last_not_of("\r\t\n ");
        if(n_space_pos!=string::npos){        
            vid.erase(n_space_pos+1);
        }
        size_t n_potpos = vid.find(".");
        if(n_potpos!=string::npos){
            vid=vid.substr(0,n_potpos);
        }

    }
    if(vid.empty())
        return -1;

    string json_url = "http://v.ku6.com/fetchVideo4Player/"+vid+".html";
    bcode= GetWebPage(json_url,htmlcontent);
    if (false == bcode)
    {
        return -1;
    }
    if(!bcode||htmlcontent.find("\"status\":404")!=string::npos)
    {
        return -1;
    }

    replace_all_distinct(htmlcontent,"\\u003a",":");
    replace_all_distinct(htmlcontent,"\\u002e",".");
    replace_all_distinct(htmlcontent,"\\u002d","-");
    replace_all_distinct(htmlcontent,"\\u002c",",");

    int VideoApartNum = 0;
    string Vtime = GetBetween(htmlcontent,"\"vtime\":","\"vtimems\":");
    if(Vtime.find("\"")!=string::npos)
        VideoApartNum=1;

    string VideoUrl =  GetBetween(htmlcontent,"\"f\":","}");
    VideoUrl = GetBetween(VideoUrl,"\"","\"");

    video_part Vpt;
    string PlayUrl;
    string Seconds;
    string TotalSeconds;

    if(VideoApartNum==0){
        Vpt.seconds = Vtime.substr(0,Vtime.length()-1);;
        Vpt.url = VideoUrl;
        Vpt.size = "0";
        TotalSeconds = Vpt.seconds;
        m_v_Parts.push_back(Vpt);       
    }

    else {
        int  n_Seconds_pos_start =-1;
        int  n_Seconds_pos_end =0;
        int  n_Url_pos_start =-1;
        int  n_Url_pos_end = 0;
        bool b_total = true;
        Vtime.erase(Vtime.find("\""),1);
        Vtime.erase(Vtime.rfind("\""),1);
        while(1){

            n_Seconds_pos_end = Vtime.find(",",n_Seconds_pos_start+1);
            n_Url_pos_end = VideoUrl.find(",",n_Url_pos_start+1);
            Vpt.seconds = Vtime.substr(n_Seconds_pos_start+1,n_Seconds_pos_end-n_Seconds_pos_start-1);
            Vpt.url = VideoUrl.substr(n_Url_pos_start+1,n_Url_pos_end-n_Url_pos_start-1);
            Vpt.size="0";

            if(Vpt.seconds=="0")
            {
                continue;
            }

            if(Vpt.url==""||Vpt.seconds=="")
            {
                break;
            }
            if(b_total){
                TotalSeconds=Vpt.seconds;
                b_total=false;
                n_Seconds_pos_start=n_Seconds_pos_end;
                continue;
            }
            n_Seconds_pos_start=n_Seconds_pos_end;
            n_Url_pos_start=n_Url_pos_end;

            m_v_Parts.push_back(Vpt);
            if(n_Seconds_pos_start>=Vtime.length()-1||n_Url_pos_start>=VideoUrl.length()-1)
            {
                break;
            }
        }
    }

    m_sSeconds = TotalSeconds;
    m_iParts =  m_v_Parts.size();
    if(ParseNumOutput(return_num)==0)
        return 0;
    else
        return -1;
}

string videoaddressku6::replace_all_distinct(string &str,const string &old_value,const string & new_value){

    for (string::size_type pos(0);pos!=string::npos; pos+=new_value.length() )
    {
        if((pos=str.find(old_value,pos))!=string::npos)str.replace(pos,old_value.length(),new_value);
        else break;
    }
    return str;
}

