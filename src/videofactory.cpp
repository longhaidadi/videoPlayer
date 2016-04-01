#include "videofactory.h"
#include <iostream>
using namespace std;

videoaddressbase * videofactory::factory()
{
	//added by Louis 2012-09-27
	if(m_sUrl.find("https://") != string::npos)
		m_sUrl = m_sUrl.substr(8);
	if(m_sUrl.find("http://") == string::npos)
	{
		m_sUrl = "http://" + m_sUrl;
	}

	if(m_sUrl.find("funshion.com") != string::npos||m_sUrl.find("smgbb.cn") != string::npos)
	{
		return new videoaddressfunshion(m_sUrl);
	}else	if(m_sUrl.find("pptv.com") != string::npos)
	{
		return new videoaddresspptv(m_sUrl);
	}
	else   if(m_sUrl.find("fengyunzhibo.com") != string::npos)
	{
		return new videoaddressmfengyun(m_sUrl);
	}
	else   if(m_sUrl.find("iqiyi.com") != string::npos)
	{
		return new videoaddressiqiyi(m_sUrl);
	}
	else   if(m_sUrl.find("tudou.com") != string::npos)
	{
		return new videoaddressyouku(m_sUrl);
	}

	else   if(m_sUrl.find("pcbaby.com") != string::npos)
	{
		return new videoaddresspcbaby(m_sUrl);
	}
	else if (m_sUrl.find("babycenter.com") != string::npos)
	{
		return new videoaddressbabycenter(m_sUrl);

	}

	else if (m_sUrl.find("wasu.cn") != string::npos)
	{
		return new videoaddresswasu(m_sUrl);

	}
	else if (m_sUrl.find("ku6.com") != string::npos)
	{
		return new videoaddressku6(m_sUrl);

	}
	else if (m_sUrl.find("kankan.com") != string::npos)
	{
		return new videoaddresskankan(m_sUrl);


	}
	else if (m_sUrl.find("pps.tv") != string::npos)
	{
		return new videoaddressiqiyi(m_sUrl);


	}
	else if (m_sUrl.find("cntv.cn") != string::npos)
	{
		return new videoaddresscntv(m_sUrl);


	}
	else if(m_sUrl.find("letv.com") != string::npos)
	{
		return new videoaddressletv(m_sUrl);
	}
	else if(m_sUrl.find("youku.com") != string::npos)
	{
		return new videoaddressyouku(m_sUrl);
	}
	else if(m_sUrl.find("sina.com") !=string::npos)
	{
		return new videoaddresssina(m_sUrl);
	}
	else if(m_sUrl.find("education-portal.com") !=string::npos)
	{
		return new videoaddresseducation_portal(m_sUrl);
	}
	else if(m_sUrl.find("61ertong.com") !=string::npos)
	{
		return new videoaddress61gequ(m_sUrl);
	}
	else if(m_sUrl.find("sohu.com") !=string::npos)
	{	
		return new videoaddresssohu(m_sUrl);
	}

	else 

	{
		cout << "unknown video site type!!!" << endl;	
		return NULL;
	}
}
