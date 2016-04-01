#include "videoaddressInterface.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
using namespace videoaddressapi;

void PrintVideoinfoToXML(VideoInfo& videoinfo)
{
	cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
	cout << "<data>" << endl;
	cout << "<seconds>" << videoinfo.m_sSeconds << "</seconds>" << endl;;
	cout << "<part>" << videoinfo.m_iParts << "</part>" << endl;;

	if(!videoinfo.m_v_sFlvUrls.empty() )
	{
		cout << "<videoCacheUrls type=\"flv\">" << endl;
		for(int i=0;i<videoinfo.m_v_sFlvUrls.size();++i)
		{
			cout << "<videoCacheUrl>" <<  videoinfo.m_v_sFlvUrls[i]<< "</videoCacheUrl>" ;
		}
		cout << endl;
		cout << "</videoCacheUrls>" << endl;
	}

	if(!videoinfo.m_v_sMp4Urls.empty() )
	{
		cout << "<videoCacheUrls type=\"mp4\">" << endl;
		for(int i=0;i<videoinfo.m_v_sMp4Urls.size();++i)
		{
			cout << "<videoCacheUrl>" << videoinfo.m_v_sMp4Urls[i] << "</videoCacheUrl>" ;
		}
		cout << endl;
		cout << "</videoCacheUrls>" << endl;
	}

	if(!videoinfo.m_v_sHDUrls.empty() )
	{
		cout << "<videoCacheUrls type=\"hd\">" << endl;
		for(int i=0;i<videoinfo.m_v_sHDUrls.size();++i)
		{
			cout << "<videoCacheUrl>" <<  videoinfo.m_v_sHDUrls[i]<< "</videoCacheUrl>" ;
		}
		cout << endl;
		cout << "</videoCacheUrls>" << endl;
	}
	cout << "</data>" << endl;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		cout << "Usage: [exe] [videoaddr]" << endl;
		return 1;
	}
    FILE* fp = fopen("abc.xml","w");
	string svideoinfoxml;
	int icode = GetDownloadAddress(string(argv[1]),svideoinfoxml,0,40,false);
   
	if (icode < 0)
	{
		cout << "GetDownloadAddress Failed!" << endl;
	} 
	else
	{
		cout << svideoinfoxml << endl << endl;
    }
	fwrite(svideoinfoxml.c_str(),sizeof(char),svideoinfoxml.size(),fp);
    fclose(fp);
	//=============================================================
	/*Sleep(1000*5); //sleep 5 seconds;
	VideoInfo videoinfo;
	icode = GetDownloadAddress(string(argv[1]),videoinfo);
	if (icode < 0)
	{
		cout << "GetDownloadAddress Failed!" << endl;
	} 
	else
	{
		PrintVideoinfoToXML(videoinfo);
	}*/
	return 0;
}
