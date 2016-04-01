#include <time.h>
#include"videoaddress.h"
#include "json/value.h"
#include "json/json.h"
#include "urlcode.h"
#include "utility.h"
#include "stdio.h"
#include <iostream>
#include <cassert>

#include <cstring>
#include <sstream>
#include <fstream>
#include <stdlib.h>

int videoaddressmfengyun::GetDownloadAddr(unsigned int  return_num){

    string htmlcontent;
   
    bool bcode = GetWebPage(m_sUrl,htmlcontent);
    if (false == bcode)
    {
        return -1;
    }
  
    size_t m3u8_pos_start = htmlcontent.find("iosPlayURL");
    if(m3u8_pos_start==string::npos)return -1;
    string mid_m3u8_url = htmlcontent.substr(m3u8_pos_start,175);
    size_t m3u8_pos_end  = mid_m3u8_url.find("',");
    if(m3u8_pos_end==string::npos)return -1;
    string m3u8_url_content = mid_m3u8_url.substr(13,m3u8_pos_end-13);

    m_v_sHDUrls.push_back(m3u8_url_content);

    m_iParts =  m_v_sHDUrls.size();

    if(ParseNumOutput(return_num)==0)
        return 0;
    else 
        return -1;

    return 0;
}
