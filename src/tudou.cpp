#include"videoaddress.h"
#include <iostream>
#include "utility.h"
#include <cstring>
#include <time.h>
#define User_Agent "Tudou;3.4;iPhone OS;5.1.1;iPhone3,1"
using namespace std;
int videoaddresstudou::GetDownloadAddr(unsigned int  return_num){

    string  htmlcontent;
    string  id_name="id";
    bool bcode=false;
    string id_value="";
    bool finish = true;
    if(m_sUrl.find("albumplay")!=string::npos)//aibumplay
    {
        cout<<"albumplay:"<<m_sUrl<<endl;
        id_name="albumid";
        bcode= GetWebPage( m_sUrl,htmlcontent);
        cout<<htmlcontent<<endl;
        if(!bcode)return -1;
        cout<<htmlcontent<<endl;
        size_t aid_pos=htmlcontent.find(",aid=");
        if(aid_pos==string::npos)return -1;
        string mid_aid = htmlcontent.substr(aid_pos+5,13);
        size_t aid_pos_end = mid_aid.find(",");
        if(aid_pos_end==string::npos)return -1;
        else id_value=mid_aid.substr(0,aid_pos_end);
    //    cout<<id_value<<endl;
    }
    else  //listplay
    {
        size_t idPos_start = m_sUrl.find_last_of("/");
        size_t idPos_end = m_sUrl.find(".html");
        if(idPos_start==string::npos||idPos_end==string::npos||idPos_start>=idPos_end){
			bcode= GetWebPage( m_sUrl,htmlcontent);
			if(!bcode)return -1;
			size_t icode_pos=htmlcontent.find(",icode: ");
			if(icode_pos==string::npos)return -1;
			string mid_icode = htmlcontent.substr(icode_pos+8,15);
			size_t icode_pos_start = mid_icode.find_first_of("\"");
			size_t icode_pos_end = mid_icode.find_last_of("\"");
			if(icode_pos_start==string::npos||icode_pos_end==string::npos)
            {
                icode_pos_start = mid_icode.find_first_of("\'");
                icode_pos_end = mid_icode.find_last_of("\'");
                if(icode_pos_start==string::npos||icode_pos_end==string::npos)return -1;
            }
		     id_value=mid_icode.substr(icode_pos_start+1,icode_pos_end-icode_pos_start-1);
		} 
        else id_value=m_sUrl.substr(idPos_start+1,idPos_end-idPos_start-1);
    }

    cout<<id_value<<endl;

    string url_pattern="http://play.api.3g.tudou.com/v3_3/play?guid=1&pid=2&tudou_format=m3u8&youku_format=6&"+id_name+"=";
    
    string url= url_pattern+id_value;
   

    cout<<url<<endl;

    if( bcode = GetWebPage(url,htmlcontent,User_Agent)==false)return -1;



    cout<<htmlcontent<<endl;
    string content;
    size_t content_pos=htmlcontent.find("results");
    if(content_pos==string::npos)finish=false;
    else content=htmlcontent.substr(content_pos);
    string max_m3u8="";
    string max_size="";

    while(finish){

        string size="";
        string m3u8="";
        size_t size_pos_start=content.find("\"size\":");
        size_t size_pos_end=content.find("}]");
        if(size_pos_start==string::npos||size_pos_end==string::npos||size_pos_end<=size_pos_start)break;
        size = content.substr(size_pos_start+8,size_pos_end-size_pos_start-8);

        size_t m3u8_pos_start=content.find("\"url\":");
        size_t m3u8_pos_end=content.find("\", \"id\"");
        if(m3u8_pos_start==string::npos||m3u8_pos_end==string::npos||m3u8_pos_end<=m3u8_pos_start)break;
        m3u8=content.substr(m3u8_pos_start+8,m3u8_pos_end-m3u8_pos_start-8);

        if(max_size=="")
        {
            max_size=size;
            max_m3u8=m3u8;
        }
        if(max_size.length()<size.length()||(max_size.length()==size.length()&&max_size.compare(size)<0))
        {
            max_size=size;
            max_m3u8=m3u8;
        }
        size_t next_pos = content.find("}]");
        content=content.substr(next_pos+3);
    }

   
       if(!finish){
        
        if(GetWebPage( m_sUrl,htmlcontent)==false)return -1;
        cout<<htmlcontent<<endl;
        string vcode= GetBetween(htmlcontent,"vcode:","\n");
        //cout<<vcode<<endl;
        vcode = GetBetween(vcode,"\'","\'");
        cout<<vcode<<endl;
        if(vcode=="")return -1;

        
        long time_current = time(NULL);
        //cout<<time_current<<endl;
        char  cur_time_char[11] = {'\0'};
        sprintf(cur_time_char,"%ld",time_current);
        max_m3u8="http://v.youku.com/player/getM3U8/vid/"+vcode+"/type/flv/ts/"+cur_time_char+"/sc/2/useKeyframe/0/video.m3u8";
    
    }
    m_v_sHDUrls.push_back(max_m3u8);

    m_iParts =  m_v_sHDUrls.size();

    if(ParseNumOutput(return_num)==0)
        return 0;
    else
        return -1;

    return 0;

}


