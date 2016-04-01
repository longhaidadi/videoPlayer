#include"videoaddress.h"
#include <iostream>
#include <cstring>
#include "json/value.h"
#include "json/json.h"
#include "utility.h"
using namespace std;
int videoaddressbabycenter::GetDownloadAddr(unsigned int  return_num){
    string  htmlcontent;
    bool bcode=false;
    if(GetWebPage( m_sUrl,htmlcontent)==false){
        return -1; 
    }
    string datavid = GetBetween(htmlcontent,"data-vid=\"","\"");
    if(datavid.empty()){
        datavid = GetBetween(htmlcontent,"vid : \'","\'");
    }
    if(datavid.empty()){
        datavid = GetBetween(htmlcontent,"vid:\"","\"");
    }

    if(datavid.empty()){
        return -1; 
    }

    size_t n_potpos = datavid.find(".");
    if(n_potpos!=string::npos){
        datavid=datavid.substr(0,n_potpos); 
    }
    string mid_url="http://v.ku6.com/fetchVideo4Player/"+datavid+".html";
    bcode= GetWebPage( mid_url,htmlcontent);
    cout<<mid_url<<endl;
    if(!bcode||htmlcontent.find("\"status\":404")!=string::npos)return -1;



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
        int  n_Seconds_pos_start = -1;
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
string videoaddressbabycenter::replace_all_distinct(string &str,const string &old_value,const string & new_value){

    for (string::size_type pos(0);pos!=string::npos; pos+=new_value.length() )
    {
        if((pos=str.find(old_value,pos))!=string::npos)str.replace(pos,old_value.length(),new_value);
        else break;
    }
    return str;
}

