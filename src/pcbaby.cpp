#include"videoaddress.h"
#include <iostream>

#include <cstring>
using namespace std;
int videoaddresspcbaby::GetDownloadAddr(unsigned int  return_num){

    string  htmlcontent;
    bool bcode=false;
    bcode= GetWebPage( m_sUrl,htmlcontent);
    if(!bcode)return -1;
    size_t download_end = htmlcontent.rfind("&thumb=");
    size_t download_start = htmlcontent.rfind("&flv=");
    if(download_end==string::npos||download_start==string::npos)return -1;
    string download_url = htmlcontent.substr(download_start+5,download_end-download_start-5);
    if(download_url.find("http")==string::npos)download_url="http://flv.pcvideo.com.cn/"+download_url;

    m_v_sHDUrls.push_back(download_url);

    m_iParts =  m_v_sHDUrls.size();

    if(ParseNumOutput(return_num)==0)
        return 0;
    else
        return -1;

}

