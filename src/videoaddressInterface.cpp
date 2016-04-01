#include "videoaddressInterface.h"
#include"videofactory.h"
#include "videoaddress.h"
#include "utility.h"
#include <sstream>
#include <fstream>
 string download_url;
//#include "global.h"
namespace videoaddressapi
{

    string catch_download_url;
	static string player_agent ;
	static string browser_agent;

	string get_player_agent(){return player_agent;}
	string get_browser_agent(){return browser_agent;}

	int GetDownloadAddress(const string& sVideoAddr, string& svideoinfoxml,unsigned int num,unsigned int timeout,bool m3u8)
	{
		if (sVideoAddr.empty())
		{
			//printf("input videoaddress is empty!\n");
			PrintNoResutl(svideoinfoxml);
			return -1000;
		}

		player_agent =  "vlc/1.1.11";
		browser_agent = "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/22.0.1229.95 Safari/537.4";

		ifstream inf;
        inf.open("agent.cfg");
		if(inf){
			string strline;
			string key;
			//stringstream ss;
			while(getline(inf,strline)){
				string::size_type pos = strline.find('=');
				if(pos != string::npos)
					key = strline.substr(0,pos-0);
				else
					continue;

				if(key.find("player_agent")!=string::npos){
					player_agent = strline.substr(pos+1);
					//cout << player_agent << endl;
				}else if(key.find("browser_agent")!=string::npos){
					browser_agent = strline.substr(pos+1);
					//cout << browser_agent << endl;
				}else
					continue;
					
			 }
		}

		videofactory * pfactory = new videofactory(sVideoAddr);
		videoaddressbase *pvideo = pfactory->factory();
       // download=download_url;
       // cout<<"where is wrong";
		if (pvideo == NULL)
		{
			PrintNoResutl(svideoinfoxml);
			delete pfactory;
			return -1001;
		}
		pvideo->SetHttpTimeout(timeout);
        pvideo->SetM3U8(m3u8);
		int iCode = pvideo->GetDownloadAddr(num);
		if (iCode < 0)
		{
			//cout << "Error code: " << iCode << endl;
		//	cout<<iCode;
		//	cout<<"***********";	
			PrintNoResutl(svideoinfoxml);
		}
		else
			pvideo->PrintResultXML(svideoinfoxml);
 //       cout<<"download: "<<download_url<<endl;
		delete pvideo;
		delete pfactory;
		return iCode;
	}

	/*int GetDownloadAddress(const string& sVideoAddr, VideoInfo& videoinfo,unsigned int num,unsigned int timeout)
	{
		if (sVideoAddr.empty() )
		{
			printf("input videoaddress is empty!\n");
			videoinfo = VideoInfo();
			return -1000;
		}
		player_agent =  "vlc/1.1.11";
		browser_agent = "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.4 (KHTML, like Gecko) Chrome/22.0.1229.95 Safari/537.4";

		videofactory * pfactory = new videofactory(sVideoAddr);
		videoaddressbase *pvideo = pfactory->factory();
		if (pvideo == NULL)
		{
			videoinfo = VideoInfo();
			delete pfactory;
			return -1001;
		}
		pvideo->SetHttpTimeout(timeout);
		int iCode = pvideo->GetDownloadAddr(num);
		if (iCode < 0)
		{
			cout << "Error code: " << iCode << endl;	
			videoinfo = VideoInfo();
		}
		else
			pvideo->PrintResultXML(videoinfo);
		delete pvideo;
		delete pfactory;
		return iCode;
	}*/
}
