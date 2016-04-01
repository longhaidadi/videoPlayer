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


/*youku*/


string videoaddressyouku::DigitStr(const string& content, unsigned int ipos)
{
	string sDigit;
	if(ipos >= content.size() )
	{
		return "";
	}
	int ibeg = ipos;
	while((int)content[ibeg]<0 || !isdigit(content[ibeg]) )
	{
		ibeg++;
	}
	while((int)content[ibeg]>0 && isdigit(content[ibeg]) )
	{
		sDigit.push_back(content[ibeg]);
		ibeg++;
	}
	return sDigit;
}

bool videoaddressyouku::GetTudouIID(const string& html_content)
{

	//http://v2.tudou.com/f?id=47782240&sid=11000&hd=3&sj=1
	//TODO
	//may be bug,has iid = condition
	//if(html_content.find("itemData") == string::npos)
	//	return "";
	string segs = GetBetween(html_content,",segs: '","'");
	Json::Reader reader;
    Json::Value root;
    if (!reader.parse(segs, root, false))
    	return false;
        try{
				string hd="";
				vector<int> kid_set;
				//const Json::Value arrayObj;
				for(int i = 6;i > 0 ;i--){
					int kid=0;
					int seconds = 0;
					int size = 0;
					stringstream ss;
					ss.clear();
					string hd_tmp;
					ss << i;
					ss >> hd_tmp;
					const Json::Value arrayObj = root[hd_tmp.c_str()];
					
					m_v_Parts.clear();
					if(arrayObj.size()!=0)
					{
						int psize = arrayObj.size();
						for(int j = 0; j<psize; j++)
						{
							video_part vp;
							kid = arrayObj[j]["k"].asInt();
							seconds = arrayObj[j]["seconds"].asInt();
							size = arrayObj[j]["size"].asInt();
							stringstream ss;
							ss.clear();
							ss << kid;
							ss >> vp.url;
							ss.clear();
							ss << (seconds/1000);
							ss >> vp.seconds;
							ss.clear();
							ss << size;
							ss >> vp.size;
							m_v_Parts.push_back(vp);
						}
						hd = hd_tmp;
						break;
					}
				}

				for(int i = 0; i<m_v_Parts.size();i++){
					//url = "http://v2.tudou.com/f?id=" +  m_v_Parts[i].url + "&hd=" + hd;
					m_v_Parts[i].url = "http://v2.tudou.com/f?id=" +  m_v_Parts[i].url + "&hd=" + hd;
					//cout << m_v_Parts[i].url << endl;
				}
				return true;
		}catch ( const std::exception &e ){
                printf( "Unhandled exception1:\n%s\n", e.what() );
				return false;
        }
	
/*	
	string::size_type stiid = html_content.find("iid: ");
	if(stiid == string::npos)
	{
		return "";
	}
	return DigitStr(html_content,stiid+4);*/
}

string videoaddressyouku::GetTudouAID(const string &html_content){
	string::size_type stiid = html_content.find("aid");
	if(stiid == string::npos)
	{
		return "";
	}
	return DigitStr(html_content,stiid+4);
}

string videoaddressyouku::GetTudouVcode(const string &html_content){

	string vflag = "vcode: ";
	string::size_type beg = html_content.find(vflag);
	if(beg == string::npos)
		return "";
	beg += vflag.length();
	string vcode="";
	char c ;
	do{
		c = html_content[beg];
	}while((c==' ' || c == '\'' || c== '\"' || c==':' || c=='=') &&++beg<html_content.size());

	while((c !=' '&& c != '\'' && c!='\"' &&c!='\r' && c!='\n')&& ++beg< html_content.size()){
		//printf("%c",c);
		vcode += c;
		c =html_content[beg];
	}

	return vcode;
}

int videoaddressyouku::GetDownloadAddr(unsigned int  return_num)
{
	
	string sId;
	
	if(m_sUrl.find("tudou")!=string::npos){

		string html_content; 
		bool bcode = GetWebPage(m_sUrl,html_content);
		if(false == bcode)
		{
			printf("Get tudou web page error\n");
			return -1;
		}
		string vcode = GetTudouVcode(html_content);
		if(vcode.empty()&&m_use_m3u8 == false)
		{

			if( GetTudouIID(html_content) == false ||  m_v_Parts.size()==0)
			{
				printf("Parse tudou iid error\n");
				return -1;
			}
			for(int i = 0 ; i< m_v_Parts.size(); i++)
			{
				string xml_content;
				bcode = GetWebPage(m_v_Parts[i].url,xml_content);
				if(false == bcode)
				{
					printf("Get tudou vpage error\n");
					return -1;
				}
				string st_sUrl = GetBetween(xml_content,"http://","</f>");

				if(st_sUrl.empty())
				{
					printf("parse tudou vpage error\n");
					return -1;
				}
				m_v_Parts[i].url  = "http://"+ st_sUrl;
			}
			for(unsigned int i=0;i<m_v_Parts.size();++i)
				AMPEntityTo(m_v_Parts[i].url);

			m_iParts = m_v_Parts.size();
			if(ParseNumOutput(return_num)==0)
				return 0;
			else 
				return -1;
		}
		else if(vcode.empty()&& m_use_m3u8 == true){
			string id_name="id";
			string id_value="";
			string agent = "Tudou;3.4;iPhone OS;5.1.1;iPhone3,1";

			if(m_sUrl.find("albumplay")!=string::npos)//aibumplay
			{
				id_value = GetTudouAID(html_content);
				if(id_value.empty())
					return -1;
				id_name="albumid";
			}
			else  //listplay
			{
				size_t idPos_start = m_sUrl.find_last_of("/");
				size_t idPos_end = m_sUrl.find(".html");
				if(idPos_start==string::npos||idPos_end==string::npos||idPos_start>=idPos_end)
				{
					bcode= GetWebPage( m_sUrl,html_content);
					if(!bcode)return -1;
					size_t icode_pos=html_content.find(",icode: ");
					if(icode_pos==string::npos)return -1;
					string mid_icode = html_content.substr(icode_pos+8,15);
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


			string url_pattern="http://play.api.3g.tudou.com/v3_3/play?guid=1&pid=2&tudou_format=m3u8&youku_format=6&"+id_name+"=";
			string url= url_pattern+id_value;
            cout<<url<<endl;
			if(GetWebPage(url,html_content,agent)==false)return -1;
			string max_m3u8="";
			string max_size="";

			size_t content_pos=html_content.find("results");
			if(content_pos!=string::npos)
			{
				string content=html_content.substr(content_pos);
				while(1)
				{
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
			}
			if(!max_m3u8.empty())
			{
				m_v_sHDUrls.push_back(max_m3u8);

				m_iParts =  m_v_sHDUrls.size();

				if(ParseNumOutput(return_num)==0)
					return 0;
				else
					return -1;
			}else
				sId=vcode;
		}else
		{
			sId = vcode;
		}
	}
	else if(m_sUrl.find("youku")!=string::npos)
	{
		sId = ParseUrlId();
	}else
		return -1;


	if (sId.empty() )
		return -2;

//	if(m_use_m3u8 == true &&  m_sUrl.find("youku")!=string::npos){
	if(m_use_m3u8 == true ){
		if(m_sUrl.find("youku")!=string::npos){
			string m3u8_url = "http://v.youku.com/player/getRealM3U8/vid/" + sId + "/type/video.m3u8";
			m_v_sHDUrls.push_back(m3u8_url);
			m_iParts =	m_v_sHDUrls.size();

			if(ParseNumOutput(return_num)==0)
				return 0;
			else 
				return -1;
		}else if(m_sUrl.find("tudou")!=string::npos){
			time_t ts = time(NULL);
			stringstream ss;
			ss.clear();
			string ts_str;
			ss << ts;
			ss >> ts_str;
			//char tsbuf[100];
			//snprintf(tsbuf,100,"%ld",ts);
			string m3u8_url = "http://v.youku.com/player/getM3U8/vid/" + sId + "/type/flv/ts/" + string("") + ts_str + string("")+  "/sc/2/useKeyframe/0/video.m3u8";
			m_v_sHDUrls.push_back(m3u8_url);
			m_iParts =	m_v_sHDUrls.size();

			if(ParseNumOutput(return_num)==0)
				return 0;
			else 
				return -1;
		}
	}
	string sContent;
	string sUrl = "v.youku.com/player/getPlayList/VideoIDS/";
	sUrl += sId;

	if(m_sUrl.find("youku")!=string::npos)
		sUrl += "/timezone/+08/version/5/source/video?n=3&ran=4132&password=";
	else
		sUrl += "/timezone/+08/version/5/source/out/Sc/2?password=&n=3&ran=7411";
	cout << sUrl << endl;
	bool bCode = GetWebPage(sUrl,sContent);
	if (bCode == false)
	{
		cout << "videoaddressyouku::GetWebPage failed!!!"  << endl;
		return -1;
	}
	map<string,StreamInfo> streaminfo;
	//cout << sContent <<endl;
    int seed;
    if(ParseJson(sContent,streaminfo,seed) < 0)
		return -1;
	//cout << streaminfo.size() <<endl;
    GetStreamUrl(streaminfo,seed);

	m_iParts = m_v_Parts.size();

	if(ParseNumOutput(return_num)==0)
		return 0;
	else 
		return -1;

	return 0;
}


string videoaddressyouku::ParseUrlId()
{
	string sId="";
	string::size_type stBeg = m_sUrl.find("id_");
	if(stBeg == string::npos)
	{
		cout << m_sUrl << " ParseUrlId find id_ failed!!!" << endl;
		return "";
	}
	string::size_type stEnd = m_sUrl.find(".html");
	if(stEnd == string::npos)
	{
		
		cout << m_sUrl << " ParseUrlId find .html failed!!!" << endl;
		return "";
	}
	sId = m_sUrl.substr(stBeg+3,stEnd-stBeg-3);
	return sId;
}

int videoaddressyouku::ParseJson(string& buffer, map<string,StreamInfo>& streaminfo,int &seed)
{

		size_t sec_pos;
		bool sec_int = false;
		sec_pos = buffer.find("seconds\":\"");
		sec_pos = buffer.find("seconds\":\"",sec_pos+1);
		if(sec_pos ==string::npos){
			sec_int = true;
		}

		Json::Reader reader;
        Json::Value root;

        if (!reader.parse(buffer, root, false))
             return -1;
        StreamInfo flvinfo;
        StreamInfo mp4info;
        StreamInfo hd2info;

        try{
                const Json::Value arrayObj = root["data"];
                if( arrayObj.size() !=1)
                        return -1;
                for (unsigned int i = 0; i < arrayObj.size(); i++)
                {
                        seed = arrayObj[i]["seed"].asInt();
                        string seconds;
                        seconds = arrayObj[i]["seconds"].asString();
						m_sSeconds = seconds;
						if(arrayObj[i]["streamfileids"].empty())
							continue;
                        flvinfo.sid = arrayObj[i]["streamfileids"]["flv"].asString();
                        mp4info.sid = arrayObj[i]["streamfileids"]["mp4"].asString();
                        hd2info.sid = arrayObj[i]["streamfileids"]["hd2"].asString();
						if(arrayObj[i]["segs"].empty())
							continue;
                        const Json::Value segs_hd2Obj = arrayObj[i]["segs"]["hd2"];
                        const Json::Value segs_flvObj = arrayObj[i]["segs"]["flv"];
                        const Json::Value segs_mp4Obj = arrayObj[i]["segs"]["mp4"];
						if(segs_hd2Obj.size() > 0){
							for(unsigned int j=0;j< segs_hd2Obj.size();j++){
								string key;
							//	if(segs_hd2Obj[j]["k"].isConvertibleTo(Json::stringValue)){
								if(segs_hd2Obj[j]["k"].isString()){
									key = segs_hd2Obj[j]["k"].asString();
								}else{
									continue;
								}
								string sec;
								if(!sec_int)
									sec = segs_hd2Obj[j]["seconds"].asString();
								else{
									int isec = segs_hd2Obj[j]["seconds"].asInt();
									stringstream ss;
									ss << isec;
									ss >> sec;
								}
								string size = segs_hd2Obj[j]["size"].asString();
								if ("-1" == key){
									continue;
								}
								hd2info.keylist.push_back(key);
								hd2info.seclist.push_back(sec);
								hd2info.sizelist.push_back(size);
							}
							m_iParts = hd2info.keylist.size();
						}
						if(segs_flvObj.size()>0){
							for(unsigned int j=0;j< segs_flvObj.size();j++){
								string key;
								if(segs_flvObj[j]["k"].isString())
									key = segs_flvObj[j]["k"].asString();
								else
									continue;
								//string sec = segs_flvObj[j]["seconds"].asString();
								string size = segs_flvObj[j]["size"].asString();
								string sec;
								if(!sec_int)
									sec = segs_flvObj[j]["seconds"].asString();
								else{
									int isec = segs_flvObj[j]["seconds"].asInt();
									stringstream ss;
									ss << isec;
									ss >> sec;
								}

								if ("-1" == key)
									continue;
								flvinfo.keylist.push_back(key);
								flvinfo.seclist.push_back(sec);
								flvinfo.sizelist.push_back(size);
							}
							m_iParts = flvinfo.keylist.size();
						}
						if(segs_mp4Obj.size()>0){
							for(unsigned int j=0;j< segs_mp4Obj.size();j++){
								string key;
								if(segs_mp4Obj[j]["k"].isString())
									key = segs_mp4Obj[j]["k"].asString();
								else
									continue;
								//string sec = segs_mp4Obj[j]["seconds"].asString();
								string size = segs_mp4Obj[j]["size"].asString();
								string sec;
								if(!sec_int)
									sec = segs_mp4Obj[j]["seconds"].asString();
								else{
									int isec = segs_mp4Obj[j]["seconds"].asInt();
									stringstream ss;
									ss << isec;
									ss >> sec;
								}

								if ("-1" == key)
									continue;
								mp4info.keylist.push_back(key);
								mp4info.seclist.push_back(sec);
								mp4info.sizelist.push_back(size);
							}
							m_iParts = mp4info.keylist.size();
						}
                }
        }
        catch ( const std::exception &e ){
                printf( "Unhandled exception2:\n%s\n", e.what() );
				return -1;
        }
		if(hd2info.sid.size()!=0  &&hd2info.keylist.size()!=0){
                streaminfo["hd2"]=hd2info;
        }
        if(flvinfo.sid.size()!=0  &&flvinfo.keylist.size()!=0){
                streaminfo["flv"]=flvinfo;
        }
		if(mp4info.sid.size()!=0  &&mp4info.keylist.size()!=0){
                streaminfo["mp4"]=mp4info;
        }
		return 0;
}	

string videoaddressyouku::ParseStreamID(const string sid,int seed){
        string tran_sid;
        global_seed = seed;
        string abc ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/\\:._-1234567890";
        string cba;
        int k = abc.size();
        for(int i=0;i<k;i++){
               // cout<<rad()<<endl;
                int index = (rad()*abc.size()/65535);
                char ex_c = abc[index];
                abc.replace(index,1,"");
               // cout<<rad()<<"\t"<<index<<"\t"<<ex_c<<"\t"<<abc<<"\t"<<abc.size()<<endl;

         //       cout << abc.size() <<endl;
                cba+=ex_c;
        }
        cout<<cba<<endl;
        char* sidbuf = new char[sid.size()+1];
        memset(sidbuf,0,sid.size()+1);
        memcpy(sidbuf,sid.c_str(),sid.size());
//      sidbuf[sid.size()]=0;
        char* pch = strtok(sidbuf, "*");
        while (pch != NULL){
              //printf ("%s\n",pch);
                tran_sid += cba[atoi(pch)];
                pch = strtok (NULL, "*");
        }
        cout<<tran_sid<<endl;
        delete[] sidbuf;
        return tran_sid;
}


void videoaddressyouku::GetStreamUrl(map<string,StreamInfo>& streaminfo,int seed){

        map<string,StreamInfo>::const_iterator it = streaminfo.begin();
		vector<video_part> mp4Parts,flvParts,hd2Parts;
        for(;it!=streaminfo.end();it++){
                string config_id = "133576593670375839133";
                string sid = ParseStreamID(it->second.sid,seed);
				if(sid.size()<10)
					continue;
                char stream_url[1024];
                char num_buf[10];
                int type;

                if(it->first == "mp4"){
                        type=0;
                }
                else if(it->first == "flv"){
                        type=1;
                }
                else if(it->first == "hd2"){
                        type=1;
                }else{
                        cout <<it->first << "  type error" << endl;
                        continue;
                }
				
                string num_str;
                for(int i = 0;i< it->second.keylist.size();i++){
                        memset(stream_url,0,1024);
                        memset(num_buf,0,10);
                        sprintf(num_buf,"%02X",i);
                        cout<<num_buf<<endl;
						//cout << sid << endl;
                        sid.replace(8,2,num_buf);
                        if(type ==0 ){ //mp4
                                sprintf(stream_url,"http://f.youku.com/player/getFlvPath/sid/%s/st/%s/fileid/%s?K=%s",config_id.c_str(),"mp4",sid.c_str(),it->second.keylist[i].c_str());
								m_v_sMp4Urls.push_back(stream_url);
								video_part vp;
								vp.url = stream_url;
								vp.seconds = it->second.seclist[i];
								vp.size = it->second.sizelist[i];
								mp4Parts.push_back(vp);
                        }else if(type==1){//flv
                                sprintf(stream_url,"http://f.youku.com/player/getFlvPath/sid/%s/st/%s/fileid/%s?K=%s",config_id.c_str(),"flv",sid.c_str(),it->second.keylist[i].c_str());
								m_v_sFlvUrls.push_back(stream_url);
								video_part vp;
								vp.url = stream_url;
								vp.seconds = it->second.seclist[i];
								vp.size = it->second.sizelist[i];
								flvParts.push_back(vp);
                        }else if(type==2){//hd2
                                sprintf(stream_url,"http://f.youku.com/player/getFlvPath/sid/%s/st/%s/fileid/%s?K=%s",config_id.c_str(),"flv",sid.c_str(),it->second.keylist[i].c_str());
								m_v_sHDUrls.push_back(stream_url);
								video_part vp;
								vp.url = stream_url;
								vp.seconds = it->second.seclist[i];
								vp.size = it->second.sizelist[i];
								hd2Parts.push_back(vp);
						}
                }
        }

		if(mp4Parts.size()>0)
			m_v_Parts = mp4Parts;
		else if(flvParts.size()>0)
			m_v_Parts = flvParts;
		else if(hd2Parts.size()>0)
			m_v_Parts = hd2Parts;

}
