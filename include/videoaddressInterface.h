/*!
 * \file   videoaddressInterface.h 
 * \brief  获取视频下载地址接口
 * \date   2012-09-05 
 * \author nanjunxiao
*/ 
#ifndef VIDEOADDRESS_INTERFACE_H_
#define VIDEOADDRESS_INTERFACE_H_
#include <string>
#include <vector>
using std::string;
using std::vector;

//typedef void VIDEOAPI;
namespace videoaddressapi
{
	string get_player_agent();
	string get_browser_agent();
	
	struct VideoInfo
	{
		string m_sSeconds;//视频时长
		unsigned int m_iParts;//分片数
		vector<string> m_v_sFlvUrls;//Flv格式urls		
		vector<string> m_v_sMp4Urls;//Mp4格式urls
		vector<string> m_v_sHDUrls;//HD格式urls
		VideoInfo()
		{
			m_sSeconds = "";
			m_iParts = 0;
		}
	};
	/*!
	 * \fn int GetDownloadAddress(const string& sVideoAddr, string& svideoinfoxml)
	 * \brief  获取视频下载地址接口一（目前支持优酷、搜狐、搜狐播客、腾讯、新浪、土豆，电视剧、电影下载地址解析）
	 * \param  [in]视频在线url
	 * \param  [out]视频下载地址等信息的xml格式串
	 * \return 自定义错误码，定位错误位置：==0 正确，<0 解析失败
	 * \date   2012-09-05 
	 * \author nanjunxiao
	 */
	int GetDownloadAddress(const string& sVideoAddr, string& svideoinfoxml,unsigned int num=0,unsigned int timeout=20,bool use_m3u8=false);

	/*!
	 * \fn int GetDownloadAddress(const string& sVideoAddr, VideoInfo& videoinfo)
	 * \brief  获取视频下载地址接口二（目前支持优酷、搜狐、搜狐播客、腾讯、新浪、土豆，电视剧、电影下载地址解析）
	 * \param  [in]视频在线url
	 * \param  [out]视频信息的结构体，方便自定义获取
	 * \return 自定义错误码，定位错误位置：==0 正确，<0 解析失败
	 * \date   2012-09-05 
	 * \author nanjunxiao
	 */
	//int GetDownloadAddress(const string& sVideoAddr, VideoInfo& videoinfo,unsigned int num=0,unsigned int timeout=20);

}//namespace
#endif //VIDEOADDRESS_INTERFACE_H_
