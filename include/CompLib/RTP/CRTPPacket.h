#ifndef __FE_RTP_PACKET_H__
#define __FE_RTP_PACKET_H__

#include "RTPProtocol.h"
#include <vector>

namespace CompLib
{
    /* 
    * RFC3550.txt
    * RTP: A Transport Protocol for Real-Time Applications
    *
    * The RTP header has the following format:
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |V=2|P|X|  CC   |M|     PT      |       sequence number         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           timestamp                           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |           synchronization source (SSRC) identifier            |
    +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    |            contributing source (CSRC) identifiers             |
    |                             ....                              |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    * version (V): 2 bits
    * padding (P): 1 bit
    * extension (X): 1 bit
    * CSRC count (CC): 4 bits
    * marker (M): 1 bit
    * payload type (PT): 7 bits
    * sequence number: 16 bits
    * timestamp: 32 bits
    * SSRC: 32 bits
    * CSRC list: 0 to 15 items, 32 bits each
    */
    class CRTPPacket
    {
    public:
		/*
		����: Ĭ�Ϲ���, ָ�������С
		����: size, �����С; _interleaved, RTSP over TCPģʽ�µ�ͨ����ʶ
		*/
        CRTPPacket(int size = 0, int _interleaved = 0);

		/*
		����: �����еİ���������
		����: packet, ���а�������
		*/
        CRTPPacket(const CRTPPacket& packet);

		/*
		����: ���ⲿָ���ʼ������Ĺ���, 
		      �������������������һ���Ϸ���RTP���ݰ�����Ϊ����ȷ
	    ����: data, ָ����������, len, ���ݳ���
		*/
        CRTPPacket(char* data, size_t len);

		/*
		����: ���ⲿָ���ʼ������Ĺ���, 
		      �������������������һ���Ϸ���RTP���ݰ�����Ϊ����ȷ
			  (���غ���, Ϊ�˺����˽ӿڱ���һ��)
        ����: data, ָ����������, len, ���ݳ���
		*/
		CRTPPacket(char* data, int len);
        ~CRTPPacket();

        /*
		����: �Ӹ����ı���������RTP��
		����: data, ָ��RTP��; len, RTP������
		����ֵ: ����汾�ͳ��Ȳ���, ����false; ����, ����true
		*/
        static bool ParseFromStream(const char*data, size_t len);

		/*
		����: �趨RTP����CSRC
		����: csrc, RTP����CSRCֵ
		*/
        void AddCSRC(int csrc);

		/*
		����: �õ�RTP���汾��
		����ֵ: ����RFC3550����, �汾��Ϊ2
		*/
        int  GetVersion() const;

		/*
		����: �趨RTP���汾��
		����: version, RTP�汾��
		*/
        void SetVersion(int version);

		/*
		����: ȷ��RTP���Ƿ������
		����ֵ: ���RTP�������, 
		*/
		bool GetPadding() const;

		/*
		����: �趨RTP���Ƿ������
		����: pad, true, �������; false, ���������
		*/
		void SetPadding(bool pad);

		/*
		����: ȷ��RTP���Ƿ�����չλ(Ŀǰ֧��)
		����ֵ: false, ���������λ; true, �������λ
		*/
        bool GetExtension()const;

		/*
		����: �趨RTP���Ƿ�����չ
		����: ext, true, ������չ; false, ��������չ
		*/
        void SetExtension(bool ext);

		/*
		����: ȷ��RTP���Ƿ���δ���ְ������Ƿְ�������һ��
		����ֵ: ���RTP��δ���ְ������Ƿְ�������һ��, ����true;
		        ���RTP�����Ƿְ�������1��, ����false
		*/
        bool GetMarker()const;

		/*
		����: �趨RTP����markerλ 
		����: mark, false, markerλΪ0, true, markerλΪ1
		*/
        void SetMarker(bool mark);

		/*
		����: �õ�RTP�����غ�����
		����ֵ: ����ֵ��������
		enum PayloadTypes 
		{
			PCMU,         // G.711 u-Law *  
			FS1016,       // Federal Standard 1016 CELP 
			G721,         // ADPCM - Subsumed by G.726  
			G726 = G721,
			GSM,          // GSM 06.10  
			G7231,        // G.723.1 at 6.3kbps or 5.3 kbps  
			DVI4_8k,      // DVI4 at 8kHz sample rate 
			DVI4_16k,     // DVI4 at 16kHz sample rate  
			LPC,          // LPC-10 Linear Predictive CELP  
			PCMA,         // G.711 A-Law  
			G722,         // G.722  
			L16_Stereo,   // 16 bit linear PCM stereo  
			L16_Mono,     // 16 bit linear PCM monotone 
			G723,         // G.723  
			CN,           // Confort Noise  
			MPA,          // MPEG1 or MPEG2 audio  
			G728,         // G.728 16kbps CELP  
			DVI4_11k,     // DVI4 at 11kHz sample rate  
			DVI4_22k,     // DVI4 at 22kHz sample rate  
			G729,         // G.729 8kbps  
			Cisco_CN,     // Cisco systems comfort noise (unofficial)  
			CelB = 25,    // Sun Systems Cell-B video  
			JPEG,         // Motion JPEG 
			H261 = 31,    // H.261  
			MPV,          // MPEG1 or MPEG2 video  
			MP2T,         // MPEG2 transport system 
			H263,         // H.263  
			////Add official types here.
			LastKnownPayloadType,
			////Dynamic types that aren't official
			DynamicBase = 96,
			DynamicVideo = DynamicBase,
			DynamicAudio = 97,
			MaxPayloadType = 127,
			IllegalPayloadType
		};
		DVR����ý������������ΪDynamicBase
		*/
        PayloadTypes GetPayloadType()const;

		/*
		����: �趨RTP�����غ�����
		����: t �غ�����
		enum PayloadTypes 
		{
		PCMU,         // G.711 u-Law *  
		FS1016,       // Federal Standard 1016 CELP 
		G721,         // ADPCM - Subsumed by G.726  
		G726 = G721,
		GSM,          // GSM 06.10  
		G7231,        // G.723.1 at 6.3kbps or 5.3 kbps  
		DVI4_8k,      // DVI4 at 8kHz sample rate 
		DVI4_16k,     // DVI4 at 16kHz sample rate  
		LPC,          // LPC-10 Linear Predictive CELP  
		PCMA,         // G.711 A-Law  
		G722,         // G.722  
		L16_Stereo,   // 16 bit linear PCM stereo  
		L16_Mono,     // 16 bit linear PCM monotone 
		G723,         // G.723  
		CN,           // Confort Noise  
		MPA,          // MPEG1 or MPEG2 audio  
		G728,         // G.728 16kbps CELP  
		DVI4_11k,     // DVI4 at 11kHz sample rate  
		DVI4_22k,     // DVI4 at 22kHz sample rate  
		G729,         // G.729 8kbps  
		Cisco_CN,     // Cisco systems comfort noise (unofficial)  
		CelB = 25,    // Sun Systems Cell-B video  
		JPEG,         // Motion JPEG 
		H261 = 31,    // H.261  
		MPV,          // MPEG1 or MPEG2 video  
		MP2T,         // MPEG2 transport system 
		H263,         // H.263  
		////Add official types here.
		LastKnownPayloadType,
		////Dynamic types that aren't official
		DynamicBase = 96,
		DynamicVideo = DynamicBase,
		DynamicAudio = 97,
		MaxPayloadType = 127,
		IllegalPayloadType
		};
		DVR����ý������������ΪDynamicBase
		*/
        void SetPayloadType(PayloadTypes t);

		/*
		����: �õ�RTP�����
		����ֵ: RTP�����
		*/
        unsigned short GetSequenceNumber()const;

		/*
		����: �趨RTP�����
		����: n, RTP�����
		*/
        void SetSequenceNumber(unsigned short n);

		/*
		����: �õ�RTP��ʱ���
		����ֵ: RTP��ʱ���
		*/
        unsigned long GetTimestamp()const;

		/*
		����: �趨RTP��ʱ���
		����: time, RTP��ʱ���
		*/
        void SetTimestamp(unsigned long time);

		/*
		����: �õ�RTP��SSRCԴ
		����ֵ: RTP��SSRCԴ
		*/
        unsigned long GetSyncSource()const;

		/*
		����: �趨RTP��SSRCԴ
		����: ssrc, RTP��SSRCԴ
		*/
        void SetSyncSource(unsigned long ssrc);

		/*
		����: �õ���ѡͬ��Դ��ʶ�ĸ���
		����ֵ: ��ѡͬ��Դ��ʶ�ĸ���
		*/
        unsigned char  GetContribSrcCount()const;

		/*
		����: �趨��ѡͬ��Դ��ʶ�ĸ���
		����: cc, ��ѡͬ��Դ��ʶ�ĸ���
		*/
		void SetContribSrcCount(unsigned char cc);

		/*
		����: �õ�RTP����indexԴ��CSRC
		����: index, ��indexԴ
		����ֵ: RTP����indexԴ��CSRC
		*/
        unsigned long GetContribSource(unsigned char index)const;
        
		/*
		����: �趨RTP����indexԴ��CSRC
		����: index, ��indexԴ; csrc, RTP����indexԴ��CSRC
		*/
        void SetContribSource(int index, unsigned long csrc) ;

		/*
		����: �õ�RTP��ͷ��С
		����ֵ: RTP��ͷ��С
		*/
        int GetHeaderSize()const;

		/*
		����: �õ�RTP������չ����
		����ֵ: RTP����չ����
		*/
        unsigned short GetExtensionType()const;

		/*
		����: �趨RTP������չ����
		����: type, RTP������չ����
		*/
        void SetExtensionType(unsigned short type);

		/*
		����: �õ�RTP��ͷ��չ��С
		����ֵ: RTP��ͷ��չ��С
		*/
        unsigned short GetExtensionSize()const;

		/*
		����: �趨RTP��ͷ��չ��С
		����: len, RTP��ͷ��չ��С
		*/
        void SetExtensionLen(unsigned short len);

		/*
		����: �õ�RTP��ͷ��չָ��
		����: ָ��RTP��ͷ��չ
		*/
        const char* GetExtensionPtr()const;

		/*
		����: �趨RTP��ͷ��չ
		����: vecExt, RTP��ͷ��չ
		*/
        void SetExtensionContent(const std::vector<long>& vecExt);

		/*
		����: �趨RTP��ͷ��չ
		����: type, ��չ����; pBuf, ��չ����; len, ��չ���峤��
		����ֵ: <0, ʧ��; >=0, �ɹ�
		*/
		int SetExtensionBuf(unsigned short type, void *pbuf, int len);

        /* ���崦�� */
		/*
		����: �õ�RTP�����س���
		����ֵ: RTP�����ش�С
		*/
        int GetPayloadSize()const;
		/*
		����:�õ�RTP��ʵ�ʸ��س��ȣ�����������ֽ�
		����ֵ:����������ֽڵ�RTP�����ش�С
		*/
		int GetPayloadNoPaddingSize()const;

		/*
		����: ��Ӹ�������, �����ָ�����ݱ��������,
		      Ч��Ҫ��һ��, ������ȫ
	    ����: data, ָ���������; len, �������ݳ���
		����ֵ: <0, ʧ��; >=0, �ɹ�
		*/
        int AddPayload(const char* data, int len);

		/*
		����: ��Ӹ�������, �����ָ�����ݱ��������,
		      Ч��Ҫ��һ��, ������ȫ(���ذ汾)
			  (���˽ӿ�)
		����: data, ָ���������; len, �������ݳ���
		����ֵ: <0, ʧ��; >=0, �ɹ�
		*/
		int AddPayload(char* data, int len);

		/*
		����: ��ȡ��������, ������RTPͷ����
		����: len, ��������, �������ݳ���
		����ֵ: ָ��������
		*/
        const char* ReadPayload(int& len)const;

		/*
		����: ��ȡ��������, ������RTPͷ����,����������ֽ�
		����: len, ��������, �������ݳ���(����������ֽ�)
		����ֵ: ָ��������
		*/
        const char* ReadPayloadNoPadding(int& len)const;

		/*
		����: ���ø��ػ��壬���������ڴ�,
		      �����Ҫ������绯��������ָ��������ڱ���ȸ������������ڳ���������ȡ�����ڴ�
              �����Ҫ��ȡ���绯����������Ը��صĿ����ز����٣���Ϊ���봮��ͷ�͸����γɱ�����
	    ����: data, ָ��������, len, ���س���
		����ֵ: <0, ʧ��; >=0, �ɹ�
		*/
        int SetPayloadPtr(const char* data, size_t len);

		/*
		����: ���ø��ػ��壬���������ڴ�,
		�����Ҫ������绯��������ָ��������ڱ���ȸ������������ڳ���������ȡ�����ڴ�
		�����Ҫ��ȡ���绯����������Ը��صĿ����ز����٣���Ϊ���봮��ͷ�͸����γɱ�����
		(���ذ汾, ���˽ӿ�)
		����: data, ָ��������, len, ���س���
		����ֵ: <0, ʧ��; >=0, �ɹ�
		*/
		int SetPayloadPtr(char* data, int len);

        /*
		����: �õ����л�������
		����ֵ: ָ�����л�������
		*/
        const char* GetStream(void)const;

		/*
		����: �õ����л�������(���ذ汾, ���˽ӿ�)
		����ֵ: ָ�����л�������
		*/
		char* GetStream();

		/*
		����: �õ����л�����������
		����ֵ: ���л�����������
		*/
        int GetStreamLen(void)const;

		/*
		����: �趨RTSP OVER TCP��ʽ��, RTP��ͨ����ʶ
		����: chn, RTP��ͨ����ʶ
		*/
		int AddRtspChn(char chn);

		/*
		����: �ͷŶ�������
		����ֵ: ���--_ref����0, delete this, ����0;
		���򷵻�--_ref
		*/
        int Release(void);

		/*
		����: ���Ӷ�������
		����ֵ: ++_ref
		*/
        int AddRef(void);

    public:
        static const int MIN_HEAD_LENGTH = 12;
        static const int PROTOCOL_VERSION = 2;
        static const int RTP_BUFFER_SIZE = 8*1024;

    protected:
        int	  _length;	/**< ���س��� */
        char* _buffer;	/**< ������ */
        int   _wholeLen;/**< ���ܳ��ȣ�����RTPͷ+RTP��չ+RTP���� */
        int   _ref;     /**< ���ü��� */

        bool  _bCopy;           /**< ��Ӧ���ڴ�ռ��Ƿ����ⲿ�ڴ� */
        bool  _bExtPayload;     /**< �Ƿ�ʹ���ⲿPayloadָ�� */
        const char* _extPayload;/**< �ⲿPayload */
		int _interleaved;

    };

    class IRTPListener
    {
    public:
        virtual ~IRTPListener(){};
		/*
		����: �˴���packetָ����ڴ�Ϊ���÷�����ʱ�ڴ棬�ϲ������Ҫ�첽�洢�����ݣ�����Ҫ��������ȿ���
		      �������ͨ��CRTPPacket�Ŀ�����������ɣ�����ֱ�ӿ����仺�����ع���; 
			  һ������£��������غ�RTPPacketָ����ڴ�ռ�Ͳ�����Ч
	    ����: session, �Ự��ʶ; connId, ͨ��ID, packet, ָ��Ԥ�����RTP��
		����ֵ: <0, �ϲ㴦��������; >=0, �ϲ㴦�����, ���쳣
		*/
        virtual int onRTPPacket(int session, int connId, CRTPPacket* packet) = 0;
    };

}

#endif
