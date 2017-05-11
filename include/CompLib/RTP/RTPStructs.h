
#ifndef __FE_RTP_STRUCTS_H__
#define __FE_RTP_STRUCTS_H__

#include <string.h>
#include <vector>
#include "RTPProtocol.h"

namespace CompLib
{

    /** ��������� */
    typedef struct tagTransPara
    {
        char ip[16];              /**< ip��ַ */
        unsigned short rtpPort;   /**< RTP�˿� */
        unsigned short rtcpPort;  /**< RTCP�˿� */

        ///����
        tagTransPara(const char* ipAddr, unsigned short port)
        {
            strncpy(ip, ipAddr, sizeof(ip) - 1);
            ip[15] = 0;
            rtpPort = port;
            rtcpPort = port + 1;
        }
    }TransPara;

    /** RTP�������� */
    typedef struct tagRTPBasicConfig
    {
        size_t	       frameRate;       /**< ֡�� */
        size_t         pktSize;         /**< ÿһ�������С,������ͷ���� */
        PayloadTypes   payloadType;     /**< ��������PT */
        unsigned short startSeq;        /**< ��һ���غɰ������к� */
        unsigned long  ssrc;            /**< ͬ��Դ */
        unsigned long  startTs;         /**< ��ʼʱ��� */
        unsigned int   sample;          /**< ��Ӧý��ĳ���Ƶ�ʣ���λHz */
    }RTPBasicConfig;

    /** RTP��չ����,NPOD���� */
    typedef struct tagRTPExtConfig
    {
        bool                hasExtension;            /**< �Ƿ�����չ */
        unsigned short      extProfile;              /**< extension profile */
        std::vector<long>   vecExt;                  /**< ��չ���� */
    }RTPExtConfig;

    /** RTCP���� */
    typedef struct tagRTCPConfig
    {
        bool            enable;        /**< �Ƿ�����RTCP */
        unsigned int    sendInterval;  /**< ����SR��� */
    }RTCPConfig;

    /** �ش����� */
    typedef struct tagRTPRetransCfg
    {
        bool           enable;   /**< �Ƿ�֧���ش� */
        unsigned int   pktsNum;  /**< �ش����ٸ�֡��� */
    }RTPRetransCfg;

    /** ���ܡ��շ����Ʋ��� */
    typedef struct tagRTPProfileCfg
    {
        int      id;                    /**< �ỰID */
        size_t   maxSendNum;            /**< һ��������෢�Ͷ��ٸ�RTP���� */
        size_t   maxSendOctets;         /**< һ��������෢�Ͷ��ٸ��ֽ�����(�����) */
        size_t   maxSendOctetsThreshold;/**< �������Լ�����㣬������������ѻ����ֽ��� */
    }RTPProfileCfg;

    /** �Ự�������� */
    typedef struct tagRTPSessionConfig
    {
        RTPBasicConfig basicCfg;
        RTPExtConfig   extCfg;
        RTCPConfig     rtcpCfg;

        RTPRetransCfg  retranCfg;
        RTPProfileCfg  profileCfg;

        ///Ĭ��
        void SetDefault();

        ///Ĭ�Ϲ���
        tagRTPSessionConfig(){SetDefault();}
    }RTPSessConfig;

    /** RTCPͳ����Ϣ */
    typedef struct tagRTCPStats
    {
        unsigned long rec_fraction_lost;
        unsigned long rec_packet_lost;
        unsigned long snd_octet;
        unsigned long snd_packet;
        unsigned long hight_sequence;
        unsigned long lastreport_time;    /**< �ϴη���SR�����ʱ�� */
    }RTCPStats;

    /** RTP�Ự״̬��Ϣ */
    typedef struct tagRTPStatus
    {
        unsigned short lastSeq;         /**< ��һ�η��͵����к� */
        unsigned short fracsCnt;        /**< һ������֡�ڵ�֡Ƭ��,��������ʱ��� */
        unsigned long  lastTimestamp;   /**< ��һ��֡���ݵ�ʱ��� */
        unsigned int   lastUnsentNum;   /**< ��һ����������û�з�����ϵĴ�������ݵ�Ԫ���� */
    }RTPStatus;

    //////////////////////////////////////////////////////////////////////////////////

    /**
    * ���@c cfg �Ƿ���һ����Ч�����ýṹ����������Ϸ�������true
    * ���Լ�����£�
    *  RTPBasicConfig  1 <= frameRate <= 30
    *                  484 <= pktSize + rtp  hdrlen <= 1472
    *  RTCPConfig      if enable , 2 <= sendInterval <= 60
    *  RTPRetransCfg   if enable,  0 < pksNum < 300
    *  RTPProfileCfg   if enable,  0 < maxSendNum < 15; 20 000 < maxSendOctets < 128 000
    *                              maxSendOctets < maxSendOctetsThreshold < 256 000
    */
    bool CheckConfig(RTPSessConfig& cfg);
}

#endif
