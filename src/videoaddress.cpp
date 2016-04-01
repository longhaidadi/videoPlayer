#include"videoaddress.h"
#include "json/value.h"
#include "json/json.h"
#include "urlcode.h"
#include "utility.h"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
char videoaddressbase::errorBuffer[CURL_ERROR_SIZE];

int videoaddressbase::ParseNumOutput(unsigned int return_num){

	if(m_iParts<1 || m_iParts<return_num)
		return -1;
	
	if(return_num!=0){
		if(m_v_Parts.size()!=0){
			video_part vp;
			vp = m_v_Parts[return_num-1];
			m_v_Parts.clear();
			m_v_Parts.push_back(vp);
		}

		if(	m_v_sFlvUrls.size()!=0){
			string url = m_v_sFlvUrls[return_num-1];
			m_v_sFlvUrls.clear();
			m_v_sFlvUrls.push_back(url);
		}
		if(	m_v_sMp4Urls.size()!=0){
			string url = m_v_sMp4Urls[return_num-1];
			m_v_sMp4Urls.clear();
			m_v_sMp4Urls.push_back(url);
		}
		if(	m_v_sHDUrls.size()!=0){
			string url = m_v_sHDUrls[return_num-1];
			m_v_sHDUrls.clear();
			m_v_sHDUrls.push_back(url);
		}
	}
	size_t pos;
	if( (pos=m_sSeconds.rfind('.'))!=string::npos){
		m_sSeconds = m_sSeconds.substr(0,pos);
	}
	return 0;
}

bool videoaddressbase::GetWebPage(const string& sUrl,string &sContent,int type)
{
	
	if (sUrl.empty() )
	{
		sContent = "";
		return false;
	}
	string tmpurl;
	if (sUrl.find("http://") == string::npos)
	{
		tmpurl = "http://" + sUrl;
	}else 
		tmpurl = sUrl;
	CURL *curl;
	curl = curl_easy_init();
	if (!curl)
	{
		sContent = "";
		return false;
	}
	sContent = "";

   // cout<<"content:"<<endl;
	curl_easy_setopt(curl, CURLOPT_URL, tmpurl.c_str() );
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sContent);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_iHTTPTimeOut);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);                // 以▒~K3个为▒~G~M▒~Z▒~P~Q设置
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10);

	if(type==1){
		curl_easy_setopt(curl, CURLOPT_USERAGENT, get_player_agent().c_str());
		//cout << get_player_agent() << endl;
	}
	else
		curl_easy_setopt(curl, CURLOPT_USERAGENT, get_browser_agent().c_str());

	CURLcode res;
	res = curl_easy_perform(curl);
    // cout<<sContent<<endl;
	if (res != CURLE_OK)
	{
		sContent = "";
		curl_easy_cleanup(curl);
		return false;
	}
	curl_easy_cleanup(curl);
	return true;	
}
bool videoaddressbase::GetWebPage(const string& sUrl,string &sContent,const string& agent)
{
//cout<<"str*******************************"<<endl;
    
   // cout<<sUrl<<endl;
   // cout<<agent<<endl;
	if (sUrl.empty() )
	{
		sContent = "";
		return false;
	}
	string tmpurl;
	if (sUrl.find("http://") == string::npos)
	{
		tmpurl = "http://" + sUrl;
	}else 
		tmpurl = sUrl;
   // cout<<tmpurl<<endl;
	CURL *curl;
	curl = curl_easy_init();
	if (!curl)
	{
		sContent = "";
		return false;
	}
	sContent = "";

	curl_easy_setopt(curl, CURLOPT_URL, tmpurl.c_str() );
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sContent);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_iHTTPTimeOut);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);                // 以▒~K3个为▒~G~M▒~Z▒~P~Q设置
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, agent.c_str());

	CURLcode res;
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		sContent = "";
		curl_easy_cleanup(curl);
		return false;
	}
	curl_easy_cleanup(curl);
	return true;	
}
bool videoaddressbase::PostWebPage(const string& sUrl,const string& post,string &sContent,int type)
{
	if (sUrl.empty())
	{
		sContent = "";
		return false;
	}
	string urltmp ;
	if (sUrl.find("http://") == string::npos)
	{
		urltmp = "http://" + sUrl;
	}else
		urltmp = sUrl;
	CURL *curl;
	curl = curl_easy_init();
	if (!curl)
	{
		sContent = "";
		return false;
	}
	sContent = "";
	curl_easy_setopt(curl, CURLOPT_URL, urltmp.c_str() );
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,post.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sContent);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_iHTTPTimeOut);

	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
	//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);                // 以▒~K3个为▒~G~M▒~Z▒~P~Q设置
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10);

	if(type==1)
		curl_easy_setopt(curl, CURLOPT_USERAGENT, get_player_agent().c_str());
	else
		curl_easy_setopt(curl, CURLOPT_USERAGENT, get_browser_agent().c_str());

	CURLcode res;
	res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		//cout << sUrl << " gather error code: " << res << endl;
		//cout << "Error: [" << res<< "] - " << errorBuffer << endl;
		//PrintNoResutl();
		//exit(-1);

		sContent = "";
		curl_easy_cleanup(curl);
		return false;
	}
	curl_easy_cleanup(curl);
	return true;	
}

int videoaddressbase::write_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	static_cast<string *>(stream)->append(static_cast<const char *>(ptr), size * nmemb);
	return size * nmemb;
}

void videoaddressbase::PrintResultXML()
{
	cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
	cout << "<data>" << endl;
	cout << "<seconds>" << m_sSeconds << "</seconds>" << endl;;
	cout << "<part>" << m_iParts << "</part>" << endl;;

	if(!m_v_sHDUrls.empty() )
	{
		cout << "<videoCacheUrls type=\"hd\">" << endl;
		for(int i=0;i<m_v_sHDUrls.size();++i)
		{
			//ToAMPEntity(m_v_sHDUrls[i]);
			cout << "<videoCacheUrl>" <<  m_v_sHDUrls[i]<< "</videoCacheUrl>" ;
		}
		cout << endl;
		cout << "</videoCacheUrls>" << endl;
	}else if(!m_v_sFlvUrls.empty() ){
		cout << "<videoCacheUrls type=\"flv\">" << endl;
		for(int i=0;i<m_v_sFlvUrls.size();++i)
		{
			//ToAMPEntity(m_v_sFlvUrls[i]);
			cout << "<videoCacheUrl>" <<  m_v_sFlvUrls[i]<< "</videoCacheUrl>" ;
		}
		cout << endl;
		cout << "</videoCacheUrls>" << endl;
	}else if(!m_v_sMp4Urls.empty() ){
		cout << "<videoCacheUrls type=\"mp4\">" << endl;
		for(int i=0;i<m_v_sMp4Urls.size();++i)
		{
			//ToAMPEntity(m_v_sMp4Urls[i]);
			cout << "<videoCacheUrl>" << m_v_sMp4Urls[i] << "</videoCacheUrl>" ;
		}
		cout << endl;
		cout << "</videoCacheUrls>" << endl;
	}


	cout << "</data>" << endl;
}

//added by Louis 2012-09-05
void videoaddressbase::PrintResultXML(string& svideoinfoxml)
{
	stringstream ssm;
	ssm << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
	ssm<< "<data>" << endl;
	ssm << "<seconds>" << m_sSeconds << "</seconds>" << endl;;
	ssm << "<part>" << m_iParts << "</part>" << endl;;
	if(!m_v_Parts.empty()){
		ssm << "<videoCacheUrls type=\"hd\">" << endl;
		for(int i=0;i<m_v_Parts.size();++i)
		{
			ssm<< "<videoCacheUrl sec=\"" << m_v_Parts[i].seconds << "\" size=\""<< m_v_Parts[i].size <<"\">" << "<![CDATA[" <<   m_v_Parts[i].url<< "]]>" << "</videoCacheUrl>" ;
		}
		ssm<< endl;
		ssm << "</videoCacheUrls>" << endl;
	}else if(!m_v_sHDUrls.empty() ){
		ssm<< "<videoCacheUrls type=\"hd\">" << endl;
		for(int i=0;i<m_v_sHDUrls.size();++i)
		{
			//ToAMPEntity(m_v_sHDUrls[i]);
			//AMPEntityTo(m_v_sHDUrls[i]);
			ssm << "<videoCacheUrl>" << "<![CDATA[" <<  m_v_sHDUrls[i]<< "]]>" << "</videoCacheUrl>" ;
		}
		ssm<< endl;
		ssm<< "</videoCacheUrls>" << endl;
	}else if(!m_v_sFlvUrls.empty() ){
		ssm<< "<videoCacheUrls type=\"flv\">" << endl;
		for(int i=0;i<m_v_sFlvUrls.size();++i)
		{
			//ToAMPEntity(m_v_sFlvUrls[i]);
			//AMPEntityTo(m_v_sFlvUrls[i]);
			ssm << "<videoCacheUrl>" << "<![CDATA[" << m_v_sFlvUrls[i] << "]]>" << "</videoCacheUrl>" ;
		}
		ssm<< endl;
		ssm << "</videoCacheUrls>" << endl;
	}else if(!m_v_sMp4Urls.empty() ){
		ssm<< "<videoCacheUrls type=\"mp4\">" << endl;
		for(int i=0;i<m_v_sMp4Urls.size();++i)
		{
			//ToAMPEntity(m_v_sMp4Urls[i]);
			//AMPEntityTo(m_v_sMp4Urls[i]);
	    ssm << "<videoCacheUrl>" << "<![CDATA[" << m_v_sMp4Urls[i] << "]]>" << "</videoCacheUrl>" ;
		}
		ssm << endl;
		ssm << "</videoCacheUrls>" << endl;
	}


	ssm << "</data>" << endl;
    ssm<<"";
	svideoinfoxml = ssm.str();
}
//added by Louis 2012-09-05
void videoaddressbase::PrintResultXML(VideoInfo& videoinfo)
{
	videoinfo.m_sSeconds = m_sSeconds;
	videoinfo.m_iParts = m_iParts;
	videoinfo.m_v_sFlvUrls = m_v_sFlvUrls;
	videoinfo.m_v_sMp4Urls = m_v_sMp4Urls;
	videoinfo.m_v_sHDUrls = m_v_sHDUrls;
}

void videoaddressbase::SetHttpTimeout(unsigned int t){
	m_iHTTPTimeOut = t;
}


string videoaddressbase::GetRandomHash(int number,int case_flag){

	string key;
	srand(time(NULL));
	char key_rand;
	for(int i =0 ;i < number;i++){
		if(rand()%2==1){
			if(case_flag==0)
				key_rand = 'a' + rand()%6;
			else
				key_rand = 'A' + rand()%6;
		}
		else
			key_rand = '0' + rand()%10;
		key += key_rand;
	}
	return key;
}









bool videoaddressbase::GetWebPageReferer(const string& sUrl,const string referer,string &sContent,const string& agent)
{
//cout<<"str*******************************"<<endl;

   // cout<<sUrl<<endl;
   // cout<<agent<<endl;
    if (sUrl.empty() )
    {
        sContent = "";
        return false;
    }
    string tmpurl;
    if (sUrl.find("http://") == string::npos)
    {
        tmpurl = "http://" + sUrl;
    }else
        tmpurl = sUrl;
   // cout<<tmpurl<<endl;
    CURL *curl;
    curl = curl_easy_init();
    if (!curl)
    {
        sContent = "";
        return false;
    }
    sContent = "";

    curl_easy_setopt(curl, CURLOPT_URL, tmpurl.c_str() );
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sContent);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, m_iHTTPTimeOut);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
    //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);                // 以▒~K3个为▒~G~M▒~Z▒~P~Q设置
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, agent.c_str());
    curl_easy_setopt(curl, CURLOPT_REFERER, referer.c_str());
    CURLcode res;
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        sContent = "";
        curl_easy_cleanup(curl);
        return false;
    }
    curl_easy_cleanup(curl);
    return true;
}

