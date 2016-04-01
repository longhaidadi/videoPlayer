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

/************************************************************************/
/* kankan*/
/************************************************************************/

string upper(string s);
int count(string source,char p){
    int i = 0;
    int j =0;
    while(source[j])
        if(source[j++]==p)i++;
    return i;
}
int videoaddresskankan::GetDownloadAddr(unsigned int  return_num)
{

    bool sucess =false;
    string htmlcontent;
    string agent = "Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5376e Safari/8536.25";
    size_t subid_pos = m_sUrl.find("subid");
    string subid="";
    string tmpurl;
    bool bcode=false;

    if(subid_pos!=string::npos)
    {
        subid =  GetBetween(m_sUrl,"subid=",".&");
        string vid ="";
        if(subid.empty())
        {
            subid=m_sUrl.substr(subid_pos+6);
            vid  = GetBetween(m_sUrl,"kankan.com/v/",".");
        }
        tmpurl = "http://api.movie.kankan.com/vodjs/subdata/"+vid+"/"+subid+".js";

    }
    else {
        string urlresult = GetBetween(m_sUrl,"kankan.com/v/",".");
        int pathcount=count(urlresult,'/');
        if(pathcount==1){
           bcode =  GetWebPage(m_sUrl,htmlcontent);
           if(false==bcode)return -1;
           string urlresult = GetBetween(m_sUrl,"kankan.com/v/",".");

           string submovieid = GetBetween(htmlcontent,"submovieid:",",");
           tmpurl = "http://api.movie.kankan.com/vodjs/subdata/"+urlresult+"/"+submovieid+".js";

        }
    }

    cout<<tmpurl<<endl;
    bcode=  GetWebPage(tmpurl,htmlcontent);



    string msurl_mid = GetBetween(htmlcontent,"msurls:[","]");
    string msurl = GetBetween(msurl_mid,"/pubnet.sandai.net:8080/6/","/");


    string gid = upper(msurl);



    string source_url = "http://mp4.cl.kankan.com/getCdnresource_flv?gcid=" + gid;

    cout<<gid<<endl;
    cout<<source_url<<endl;
    gid="";
    if ("" == gid)
    {


        //   string gid;
        size_t http_pos = m_sUrl.find("http://");
        if(http_pos==string::npos)m_sUrl="http://"+m_sUrl;
        m_sUrl = "http://m."+m_sUrl.substr(11);

        cout<<m_sUrl<<endl;
        bcode = GetWebPage(m_sUrl,htmlcontent,agent);
        if (false == bcode)
        {
            return -1;
        }

        tmpurl = GetUrl(htmlcontent,1);

        cout<<tmpurl<<endl;
        if(tmpurl.empty())
            return -1;
        gid = GetGID(tmpurl);

        if(gid.empty())
            return -1;
        gid = upper(gid);
        source_url = "http://mp4.cl.kankan.com/getCdnresource_flv?gcid=" + gid;

        cout<<source_url<<endl;
        bcode = GetWebPageReferer(source_url,m_sUrl,htmlcontent,agent);
        if (false == bcode)return -1;
        

    }
    else {

        bcode=GetWebPageReferer(source_url,m_sUrl,htmlcontent,agent);
        if (false == bcode)
        {
            return -1;
        }

    }

    string request_ip = GetBetween(htmlcontent,"ip:\"","\"");
    string request_path = GetBetween(htmlcontent,"path:\"","\"");
    string request_key1;
    if(htmlcontent.find("param2:")!=string::npos)
        request_key1 = GetBetween(htmlcontent,"param2:","}");
    else if(htmlcontent.find("param1:")!=string::npos)
        request_key1 = GetBetween(htmlcontent,"param1:",",");
    else 
        return -1;
    if(request_ip.empty() || request_path.empty() || request_key1.empty())
        return -1;
    /*string restcontent = htmlcontent.substr(htmlcontent.find("path:")+5);

      string request_ip_bak = GetBetween(restcontent,"ip:\"","\"");
      string request_path_bak = GetBetween(restcontent,"path:\"","\"");*/

    string key = GetRandomHash(40,0);
    string playurl = "http://" + request_ip +"/"+ request_path + "?key=" + key + "&key1=" + request_key1;
    //string playurl_bak = "http://" + request_ip_bak + request_path_bak + "?key=" + key + "&key1=" + request_key1;
    //cout << playurl << endl;
    //cout << playurl_bak << endl;
    m_sSeconds = "";
    m_v_sHDUrls.push_back(playurl);

    m_iParts =	m_v_sHDUrls.size();

    if(ParseNumOutput(return_num)==0)
        return 0;
    else 
        return -1;

}
string videoaddresskankan::GetUrl(const string& content,int index){

    string urls = GetBetween(content,"surls:[","]");
    string url = "";
    if(!urls.empty()){
        size_t bpos = 0;
        size_t epos;

        for( int i=0;i <= index; i++){
            bpos = urls.find("http",bpos+1);
            if(bpos == string::npos)
                break;
            epos = urls.find("\'",bpos+1);
            if(epos == string::npos)
                break;
            url = urls.substr(bpos,epos-bpos-1);
        }
    }else{
        url = GetBetween(content,"lurl:\'","\'");
    }
    //	cout << url << endl;
    return url;

}
string videoaddresskankan::GetGID(const string& url){

    size_t bpos =0;
    size_t epos;
    int idx=0;
    for(int i =0 ;i < 4; i++){
        bpos = url.find('/',bpos+1);
        if(bpos ==string::npos)
            return "";
    }
    epos = url.find('/',bpos+1);
    if(epos==string::npos || (epos - bpos-1)!=40)
        return "";
    return url.substr(bpos+1,epos-bpos-1);


}

string upper(string s) //返回字符串s的大写形式
{ 
    string temp(s);
    for (int i=0;i<(int)s.size();i++)
    {    
        temp[i]=toupper(s[i]);   
    }
    return temp;
}


