#ifndef _RTP_PROTOCOL_H_
#define _RTP_PROTOCOL_H_

namespace CompLib
{
    /** RTP�շ�Ӧ��ģʽ */
    enum Model
    {
        SENDER = 1,  /**< ��������ģʽ */
        RECEIVER     /**< ��������ģʽ */
    };

    /** RTP�����ģʽ */
    enum TransModel
    {
        RTPOverUDP = 0,    /**< RTP/UDP����ģʽ */
        RTPOverTCP = 1,    /**< RTP/TCPģʽ */
        RTPOverRTSP        /**< RTP/RTSPģʽ */
    };

    /** RTP�������� */
    enum PayloadTypes 
    {
        PCMU,         /**< G.711 u-Law */  
        FS1016,       /**< Federal Standard 1016 CELP */
        G721,         /**< ADPCM - Subsumed by G.726 */ 
        G726 = G721,
        GSM,          /**< GSM 06.10 */ 
        G7231,        /**< G.723.1 at 6.3kbps or 5.3 kbps */ 
        DVI4_8k,      /**< DVI4 at 8kHz sample rate */ 
        DVI4_16k,     /**< DVI4 at 16kHz sample rate */ 
        LPC,          /**< LPC-10 Linear Predictive CELP */ 
        PCMA,         /**< G.711 A-Law */ 
        G722,         /**< G.722 */ 
        L16_Stereo,   /**< 16 bit linear PCM stereo */ 
        L16_Mono,     /**< 16 bit linear PCM monotone*/ 
        G723,         /**< G.723 */ 
        CN,           /**< Confort Noise */ 
        MPA,          /**< MPEG1 or MPEG2 audio */ 
        G728,         /**< G.728 16kbps CELP */ 
        DVI4_11k,     /**< DVI4 at 11kHz sample rate */ 
        DVI4_22k,     /**< DVI4 at 22kHz sample rate */ 
        G729,         /**< G.729 8kbps */ 
        Cisco_CN,     /**< Cisco systems comfort noise (unofficial) */ 

        CelB = 25,    /**< Sun Systems Cell-B video */ 
        JPEG,         /**< Motion JPEG */ 
        H261 = 31,    /**< H.261 */ 
        MPV,          /**< MPEG1 or MPEG2 video */ 
        MP2T,         /**< MPEG2 transport system */
        H263,         /**< H.263 */ 

        ////Add official types here.
        LastKnownPayloadType,

        ////Dynamic types that aren't official
        DynamicBase = 96,
        DynamicVideo = DynamicBase,
        DynamicAudio = 97,
        MaxPayloadType = 127,
        IllegalPayloadType
    };

    /** RTCP��Ϣ���� */
    enum RTCPTYPE
    {
        SR		= 200,  /**< Sender Report */
        RR		= 201,  /**< Receiver Report*/
        SDES	= 202,  /**< Sender Description */
        BYE		= 203,  /**< Bye */
        APP		= 204,  /**< Application specification */
    };

    /** ý������֡���� */
	/** bit7 ~ bit2��ʾ�������� */
	/** 0:MPEG-4 */
	/** 1:H.264 */
    enum MediaFrameType
    {
        KeyFrame = 1,       /**< �ؼ�֡ */
        IFrame = KeyFrame,  /**< I֡ */
        PFrame = 2,         /**< P֡ */
        BFrame = 3,         /**< B֡ */
		H264Frame = 0x04,	/**< H.264֡ */
    };

    typedef struct
    {
        unsigned long ntpTimestampHigh; /**< NTP time high word*/
        unsigned long ntpTimestampLow;  /**< NTP time low word*/
        unsigned long rtpTimestamp;     /**< RTP timestamp */
        unsigned long packetCount;      /**< total packets sent till now */
        unsigned long octetCount;       /**< total octets sent*/
    } SSenderInfo;

    typedef struct
    {
        unsigned long fractionLost : 8;    //SR������������
        unsigned long packetsLost  : 24;   //�ܹ�������
    } SLost;

    typedef struct
    {
        SLost lost;
        unsigned long highestSequence;     //RR�����е����RTP���
        unsigned long jitter;              //���綶����
        unsigned long lastSR;              //��һ��SR�������ʱ���
        unsigned long delaySR;             //���������SR�����ʱ����
    } SReportBlock;

    typedef struct 
    {
        unsigned char type;               //type���� 
        unsigned char length;             //���ݳ���
        unsigned char data[64];           //��������
    }SCname;

    typedef struct 
    {
        unsigned long ssrc;   //ԴSSRC��ʾ
        SCname scname;        //Դ�˻�����ʾ
    }SChunk;

    typedef struct 
    {
        unsigned long name;        //Դ�˱�ʾ
        unsigned short seqence1;   //�ش�����ʼ���
        unsigned short seqence2;   //�ش����������
    }SAppResend;

}

#endif//_RTP_PROTOCOL_H_

