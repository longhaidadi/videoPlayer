#ifndef VIDEOADDRESS_H_
#define VIDEOADDRESS_H_
#include <map>
#include <curl/curl.h>
#include <string>
#include <list>
#include <numeric>
#include <algorithm>
//added by Louis 2012-09-05
#include "videoaddressInterface.h"
#include <iostream>
using namespace videoaddressapi;
using namespace std;
#define DEFAULT_HTTP_USER_AGENT_FIREFOX	"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:17.0) Gecko/20100101 Firefox/17.0"
#define DEFAULT_HTTP_USER_AGENT_CHROME "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/22.0.1229.95 Safari/537.4"
#define DEFAULT_HTTP_USER_AGENT_VLC "vlc/1.1.11"
#define DEFAULT_HTTP_USER_AGENT_FIREFOX_LINUX "Mozilla/5.0 (X11; Linux x86_64; rv:17.0) Gecko/20130625 Firefox/17.0"
/*BASE*/
struct video_part{
    string url;
    string seconds;
    string size;
    string title;
};
class videoaddressbase
{
    public:
        videoaddressbase(){m_sSeconds="1";m_iParts=0;m_iHTTPTimeOut=50;m_use_m3u8=false;}
        ~videoaddressbase(){}
        bool GetWebPage(const string& sUrl,string &sContent,int type=0);
        bool GetWebPage(const string& sUrl,string &sContent,const string& agent );
        bool PostWebPage(const string& sUrl,const string& post,string &sContent,int type=0);
        virtual int GetDownloadAddr(unsigned int)=0;
        void PrintResultXML();

        //added by Louis 2012-09-05
        void PrintResultXML(string& svideoinfoxml);
        //added by Louis 2012-09-05
        void PrintResultXML(VideoInfo& videoinfo);
        int ParseNumOutput(unsigned int);
        void SetHttpTimeout(unsigned int);
        void SetM3U8(bool flag){m_use_m3u8=flag;}
        string GetRandomHash(int,int);
        bool GetWebPageReferer(const string& sUrl,const string referer,string &sContent,const string& agent);
    private:
        static int write_callback(void *ptr, size_t size, size_t nmemb, void *stream);
        static char errorBuffer[CURL_ERROR_SIZE];

        //string m_sUrl;
        //added
    protected:		
        string m_sSeconds;
        unsigned int m_iParts;
        vector<string> m_v_sFlvUrls;		
        vector<string> m_v_sMp4Urls;
        vector<string> m_v_sHDUrls;

        vector<video_part> m_v_Parts;
        unsigned int	m_iHTTPTimeOut;
        bool	m_use_m3u8;
        string	m_m3u8_content;
        //vector<string> m_sPartSec;
};




class videoaddressfunshion:public videoaddressbase
{
    public:
        videoaddressfunshion(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddressfunshion(){}
        int GetDownloadAddr(unsigned int return_num=0);
        int getId(string & id,string & cat, string & playNum);
        int getURLtable(string fmtjson,string category,string index);
        int getUrlFormPC(string id,string cat,string playNum);
    private:
        string m_sUrl;
    private:
};

class videoaddresspptv:public videoaddressbase
{
    public:
        videoaddresspptv(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddresspptv(){}
        int GetDownloadAddr(unsigned int return_num=0);
    private:
        string m_sUrl;
    private:
};

/*class videoaddressyoutube:public videoaddressbase
  {
  public:
  videoaddressyoutube(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
  virtual ~videoaddressyoutube(){}
  int GetDownloadAddr(unsigned int return_num=0);

  private:
  string m_sUrl;
  private:
  list<string> split(string str,string pattern);
  int contain(list<string>list_canshu,string target,int &fir_pos,int &count,list<string> & list_value);
  string merge(list<string>list_canshu);
  string &replace_all_distinct(string &str,const string &old_value,const string & new_value);
  void delete_repeat_canshu(list<string>&list_canshu);
  void add_canshu(list<string>&list_canshu,string target);
  void delete_canshu(list<string>&list_canshu,string target);
  void Result(list<string>list_canshu,string head ,list<string>&urls);
  void output(list<string>urls);
  int itag_divide(list<string>&list_canshu,string head,list<string>&list);
  int get_url_type(list<string> urls , string & result,string type,string &clen,string & dur);
  int get_info(string source,string target, string & result);

  };*/
class videoaddressmfengyun:public videoaddressbase
{
    public:
        videoaddressmfengyun(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddressmfengyun(){}
        int GetDownloadAddr(unsigned int return_num=0);


    private:
        string m_sUrl;
};


class videoaddresspps:public videoaddressbase
{
    public:
        videoaddresspps(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddresspps(){}
        string Get_urlkey(const string &htmlcontent);
        int GetDownloadAddr(unsigned int return_num=0);
        int Get_playurlByvd( string htmlcontent,video_part & ,int vd,int type);
        int GetDownloadAddressfromiqiyi(string midurl,unsigned int return_num);
        int GetDownloadAddressfromppspc(string midurl,unsigned int return_num);
        int GetDownloadAddressfromppsph(string midurl,unsigned int return_num);

    private:
        string m_sUrl;
    private:
};
class videoaddresstudou:public videoaddressbase
{
    public:
        videoaddresstudou(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddresstudou(){}
        int GetDownloadAddr(unsigned int return_num=0);


    private:
        string m_sUrl;
};
class videoaddresspcbaby:public videoaddressbase
{
    public:
        videoaddresspcbaby(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddresspcbaby(){}
        int GetDownloadAddr(unsigned int return_num=0);

    private:
        string m_sUrl;

};
class videoaddressbabycenter:public videoaddressbase
{
    public:
        videoaddressbabycenter(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddressbabycenter(){}
        int GetDownloadAddr(unsigned int return_num=0);

        string replace_all_distinct(string &str,const string &old_value,const string & new_value);

    private:
        string m_sUrl;

};

class videoaddresswasu:public videoaddressbase
{
    public:
        videoaddresswasu(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddresswasu(){}
        int GetDownloadAddr(unsigned int return_num=0);
        int GetVideoFromifeng( string ifengswfurl,unsigned int  return_num=0);


    private:
        string m_sUrl;

};
class videoaddressku6:public videoaddressbase
{
    public:
        videoaddressku6(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddressku6(){}
        int GetDownloadAddr(unsigned int return_num=0);

        string  replace_all_distinct(string &str,const string &old_value,const string & new_value);
    private:
        string m_sUrl;

};


class videoaddresskankan:public videoaddressbase
{
    public:
        videoaddresskankan(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddresskankan(){}
        int GetDownloadAddr(unsigned int return_num=0); 
    private:
        string m_sUrl;
    private:
        string GetGID(const string& url);
        string GetUrl(const string& content,int index);
};
class videoaddressiqiyi:public videoaddressbase
{
    public:
        videoaddressiqiyi(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddressiqiyi(){}
        int GetDownloadAddr(unsigned int return_num =0);
	int Getidfromiqiyi(string & tvid,string & videoid);
	int Getidfrompps(string & tvid,string & videoid,string & video_id);
	string Gettitle();
	string Getpath(string path);
	int parajson(string jsonurl);
    private:
        string m_sUrl;
};
class videoaddresscntv:public videoaddressbase
{
    public:
        videoaddresscntv(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddresscntv(){}
        int GetDownloadAddr(unsigned int return_num=0);
    private:
        string m_sUrl;
};

typedef struct SohuInfo{
    string clipsUrl;
    int clipsBytes;
    int clipsDuration;
    string su;
}SohuInfo;

class videoaddresssohu:public videoaddressbase
{
    public:
        videoaddresssohu(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddresssohu(){}
        int GetDownloadAddr(unsigned int return_num =0);
        string GetVidFromUrlcontent();
        int parsesohujson(const string htmlcontent,vector<SohuInfo>&m_vectors);
    private:
        string m_sUrl;
};
class videoaddressletv:public videoaddressbase
{
    public:
        int GetVid(string & vid);
        unsigned int GetKey(unsigned int time_key);
        videoaddressletv(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddressletv(){}
        int GetDownloadAddr(unsigned int return_num=0);
        //   int get_vid(string & vid);
        //  string get_v(const string & content);
        string Getmmsid(const string & htmlcontent);
        string GetFlagContent(const string &html_content,const string & flag);
        int Getm3u8url(string htmlcontent,string & m3u8url);
        string Formaturl(const string &url);
    private:
        string m_sUrl;
};
class videoaddressyouku:public videoaddressbase
{
    public:
        videoaddressyouku(string sUrl):m_sUrl(sUrl),videoaddressbase() {}
        ~videoaddressyouku()
        {}
        int GetDownloadAddr(unsigned int return_num=0);  
    public:
        string m_sUrl;
        struct StreamInfo{
            string sid;
            vector<string> keylist;
            vector<string> seclist;
            vector<string> sizelist;};

        int global_seed;

        int ParseJson(string& buffer, map<string,StreamInfo>& streaminfo,int &seed);
        int rad(){
            global_seed = (global_seed*211+30031)%65536;
            return global_seed;
        }

        string  ParseStreamID(const string sid,int seed);
        void GetStreamUrl(map<string,StreamInfo>& streaminfo,int seed);
        string ParseUrlId();

        bool GetTudouIID(const string& html_content);
        string GetTudouAID(const string& html_content);
        string DigitStr(const string& content, unsigned int ipos);
        string GetTudouVcode(const string &html_content);
};
class videoaddresssina:public videoaddressbase
{
    public:
        videoaddresssina(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        ~videoaddresssina(){}
        string GetVidValue(string,string,string);    
        int GetDownloadAddr(unsigned int return_num=0);  
    public:
        struct sinaVideoInfo
        {
            string m_sTimeLen;
            unsigned int m_iPartNum;
            vector<string> m_v_Urls;
            vector<string> m_v_Times;
        };
    private:
        string m_sUrl;
    private:
        /*SingleVideo, not support videolist now*/
        void GetVidFromSingleVideoPage(string& html_content,string &sFlvVid, string& sHDVid);
        //flv or HD urls
        sinaVideoInfo ParseUrlsFromXML(string& xml_content);
        int ParseInfo(const string& xml_content);
        string GetKeyUrl(const string&);

};
class videoaddressjoy:public videoaddressbase
{
    public:
        videoaddressjoy(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddressjoy(){}

        int GetDownloadAddr(unsigned int return_num=0);  
    private:
        string m_sUrl;
    private:
        //two case. has programid or not?
        void ParseSrcHtml(string& htmlcontent,string& xmlurl);
        int ParseXML(string &xmlcontent);
};
/*class videoaddress56:public videoaddressbase
{
    public:
        videoaddress56(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddress56(){}   
        int GetDownloadAddr(unsigned int return_num=0);  
    private:
        string m_sUrl;
    private:
        string GetVid(string &sUrl);
        void ParseJson(string& jsoncontent);
};*/
class videoaddresseducation_portal:public videoaddressbase
{
    public:
        videoaddresseducation_portal(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddresseducation_portal(){}
        int GetDownloadAddr(unsigned int return_num=0);
    private:
        string m_sUrl;
    private:
        int GetVid(const string&, string&);
};
class videoaddress61gequ:public videoaddressbase
{
    public:
        videoaddress61gequ(string sUrl):m_sUrl(sUrl),videoaddressbase(){}
        virtual ~videoaddress61gequ(){}
        int GetDownloadAddr(unsigned int return_num=0);  
    private:
        string m_sUrl;
    private:
        int ParseSrcHtml(string& htmlcontent,string& playurl);

};

#endif
