
#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>	/* inet(3) functions */
#endif

#include <fcntl.h>		/* for nonblocking */
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include "CRTPPacket.h"

namespace CompLib
{
	//Ĭ�Ϲ���, ָ�������С
	CRTPPacket::CRTPPacket(int size, int interleaved)
	{
		if (size <= 0)
		{
			_buffer = new char[RTP_BUFFER_SIZE];
			memset(_buffer, 0, RTP_BUFFER_SIZE);
			_length = RTP_BUFFER_SIZE;
			_wholeLen = RTP_BUFFER_SIZE;
		}
		else
		{
			_buffer = new char[size];
			memset(_buffer, 0, size);
			_length = 0;
			_wholeLen = size;
		}
		_interleaved = interleaved;
		/*
		*	yehao 2007-08-22:
		*	��ʼ����RTP VERΪ10
		*/
		_buffer[0 + _interleaved] |= 0x80;

		_bCopy = 0;
		_ref = 0;
		_extPayload = 0;
		_bExtPayload = false;
	}

	//�Ӹ����ı���������RTP��
    bool CRTPPacket::ParseFromStream(const char*data, size_t len)
    {
        ///�ȼ��汾������汾����ֱ�ӷ��ش���
        unsigned char byte = (unsigned char)(*data);
        if (!(byte & 0x80))
        {
            printf("====RTP Parse error, bad RTP version!====\n");
            return false;
        }

        ///���鳤��
        if (len < MIN_HEAD_LENGTH)
        {
            printf("===RTP parase error, len < 12!====\n");
        }

        ///TODO:������չ�ֶ�,len>=12
        return true;
    }

	//���ⲿָ���ʼ������Ĺ���, �������������������һ���Ϸ���RTP���ݰ�����Ϊ����ȷ
    CRTPPacket::CRTPPacket(char* data, size_t len)
    {
        if (!ParseFromStream(data, len))
        {
            assert(!"====Bad RTP Stream!===\n");
        }
        _buffer = data;
        _wholeLen = len;
        _length = len - GetHeaderSize();
        assert(_length > 0 && _length < len);

        /// ������������ɾ����ֻ���ڴ�
        _bCopy = true;
        _ref = 0;

        _extPayload = 0;
        _bExtPayload = false;
		_interleaved = 0;
    }

	//���ⲿָ���ʼ������Ĺ���
	CRTPPacket::CRTPPacket(char* data, int len)
	{
		_buffer = data;
		_interleaved = 0;
		_length = len - GetHeaderSize() - _interleaved;

		_wholeLen = len;
		_bCopy = 1;

		_ref = 0;
	}

    CRTPPacket::CRTPPacket(const CRTPPacket& packet)
    {
		_interleaved = 0;
        _length = packet._length;
        _buffer = new char[packet._wholeLen];
        _wholeLen = packet._wholeLen;
        memcpy(_buffer, packet._buffer, packet._wholeLen);
        _bCopy = false;

        _extPayload = 0;
        _bExtPayload = false;

        _ref = 0;
    }

    int CRTPPacket::Release(void)
    {
        if(--_ref == 0)
        {
            delete this;
            return 0;
        }
        else
        {
            return _ref;
        }
    }

    int CRTPPacket::AddRef(void)
    {
        return ++_ref;
    }

    CRTPPacket::~CRTPPacket()
    {
        ///������Ҫɾ���Լ�������ڴ�
        if (!_bCopy)
        {
            delete []_buffer;
        }
    }

	//��ȡRTP���汾��
    int CRTPPacket::GetVersion()const
    {
        return (_buffer[0 + _interleaved] >> 6) & 3;
    }

	//�趨RTP���汾��
    void CRTPPacket::SetVersion(int version)
    {
        _buffer[0 + _interleaved] |= ((version<<6) & 0xc0);
    }

	//ȷ��RTP���Ƿ������
	bool CRTPPacket::GetPadding()const
	{
		return (_buffer[0] & 0x20) ? 1 : 0;
	}

	//�趨RTP���Ƿ������
	void CRTPPacket::SetPadding(bool pad)
	{
		pad ? (_buffer[0] |= 0x20) : (_buffer[0] &= (~0x20));
	}

    void CRTPPacket::AddCSRC(int csrc)
    {

    }

	//ȷ��RTP���Ƿ�����չλ(Ŀǰ֧��)
    bool CRTPPacket::GetExtension()const
    {
        return (_buffer[0 + _interleaved] & 0x10) != 0;
    }

	//�趨RTP���Ƿ�����չ
    void CRTPPacket::SetExtension(bool ext)
    {
        ext ? _buffer[0 + _interleaved] |= 0x10 : _buffer[0 + _interleaved] &= 0xef;
    }

	//ȷ��RTP���Ƿ���δ���ְ������Ƿְ�������һ��
    bool CRTPPacket::GetMarker()const
    {
        return (_buffer[1 + _interleaved] & 0x80) != 0;
    }

	//�趨RTP����markerλ
    void CRTPPacket::SetMarker(bool mark)
    {
        mark ? (_buffer[1 + _interleaved] |= 0x80) : (_buffer[1 + _interleaved] &= 0x7f);
    }

	//��ȡRTP�����غ�����
    PayloadTypes CRTPPacket::GetPayloadType()const
    {
        return (PayloadTypes)(_buffer[1 + _interleaved] & 0x7f);
    }

	//�趨RTP�����غ�����
    void CRTPPacket::SetPayloadType(PayloadTypes t)
    {
        _buffer[1 + _interleaved] = _buffer[1 + _interleaved] & 0x80 | t;
    }

	//��ȡRTP�����
    unsigned short CRTPPacket::GetSequenceNumber()const
    {
        unsigned short value = *(unsigned short *)&_buffer[2 + _interleaved];
        return ntohs(value);
    }

	//�趨RTP�����
    void CRTPPacket::SetSequenceNumber(unsigned short n)
    {
        *(unsigned short *)&_buffer[2 + _interleaved] = htons(n);
    }

	//��ȡRTP���¼���
    unsigned long CRTPPacket::GetTimestamp()const
    {
        return ntohl(*(unsigned long *)&_buffer[4 + _interleaved]);
    }

	//�趨RTP��ʱ���
    void CRTPPacket::SetTimestamp(unsigned long time)
    {
        *(unsigned int *)&_buffer[4 + _interleaved] = htonl(time);
    }

	//��ȡRTP��SSRCԴ
    unsigned long CRTPPacket::GetSyncSource()const
    {
        return ntohl(*(unsigned long *)&_buffer[8 + _interleaved]);
    }

	//�趨RTP��SSRCԴ
    void CRTPPacket::SetSyncSource(unsigned long ssrc)
    {
        *(unsigned int *)&_buffer[8 + _interleaved] = htonl(ssrc);
    }

	//�趨��ѡͬ��Դ��ʶ�ĸ���
	void CRTPPacket::SetContribSrcCount(unsigned char cc)
	{
		if(cc > 15)
		{
			printf("Set ContribSrcCount error,cc = 0x%X\n");
		}
		else
		{
			_buffer[0 + _interleaved] |= (cc & 0x0F);
		}
	}

	//��ȡ��ѡͬ��Դ��ʶ�ĸ���
    unsigned char CRTPPacket::GetContribSrcCount()const
    {
        return _buffer[0 + _interleaved] & 0xf;
    }

	//��ȡRTP����indexԴ��CSRC
    unsigned long CRTPPacket::GetContribSource(unsigned char index)const
    {
        ///������Ҫȷ����ַ��Ч
        unsigned char cnt = GetContribSrcCount();
        if (index >= cnt)
        {
            printf("===CRTPPacket===, GetCSRC index error!\n");
            return 0;
        }
        assert(_wholeLen > MIN_HEAD_LENGTH + cnt * 4);

        return ntohl(((unsigned long *)&_buffer[MIN_HEAD_LENGTH + _interleaved])[index]);
    }

	//�趨RTP����indexԴ��CSRC
    void CRTPPacket::SetContribSource(int index, unsigned long csrc)
    {
        ///ȷ��ǰ���ֶ���ȷ����ַ��ȷ����ֹ��ȡ�����ڴ�
        unsigned char cnt = GetContribSrcCount();
        if (index >= cnt)
        {
            printf("===CRTPPacket===, SetCSRC index error!\n");
            return ;
        }
        assert(_wholeLen > MIN_HEAD_LENGTH + cnt * 4);
        ((unsigned int *)&_buffer[MIN_HEAD_LENGTH])[index + _interleaved] = htonl(csrc);
    }

	//��ȡRTP��ͷ��С
    int CRTPPacket::GetHeaderSize()const
    {
        return MIN_HEAD_LENGTH + 4 * GetContribSrcCount() + GetExtensionSize();
    }

	//��ȡRTP������չ����
    unsigned short CRTPPacket::GetExtensionType()const
    {
        return GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH + _interleaved + 4 * GetContribSrcCount()] : (-1);
        //return GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH] : (-1);
    }

	//�趨RTP������չ����
    void CRTPPacket::SetExtensionType(unsigned short type)
    {
        GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH + _interleaved + 4 * GetContribSrcCount()] = type : 0;
        //GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH] = htons(type) : 0;
    }

	//�趨RTP��ͷ��չ��С
    void CRTPPacket::SetExtensionLen(unsigned short len)
    {
        GetExtension() ? (*(unsigned short *)&_buffer[MIN_HEAD_LENGTH + _interleaved + 2]) = htons(len) : 0 ;
    }

	//��ȡRTP��ͷ��չ��С
    unsigned short CRTPPacket::GetExtensionSize()const
    {
        //return GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH + 4 * GetContribSrcCount() + 2] : 0;
        if (GetExtension())
        {
            int extCnt = ntohs(*(unsigned short *)&_buffer[MIN_HEAD_LENGTH  +_interleaved + 4 * GetContribSrcCount() + 2]);
            return extCnt * 4 + 4;
        }
        else
        {
            return 0;
        }
    }

	//��ȡRTP��ͷ��չָ��
    const char* CRTPPacket::GetExtensionPtr()const
    {
        return GetExtension() ? &_buffer[MIN_HEAD_LENGTH + _interleaved + 4 * GetContribSrcCount() + 4] : (char *)0;
        //return GetExtension() ? &_buffer[MIN_HEAD_LENGTH + 4] : (char *)0;
    }

	//�趨RTP��ͷ��չ
    void CRTPPacket::SetExtensionContent(const std::vector<long>& vecExt)
    {
        if (GetExtension())
        {
            long* pItem = (long*)GetExtensionPtr();
            for (std::vector<long>::const_iterator it = vecExt.begin();
                    it != vecExt.end(); ++it)
            {
                *pItem = htonl(*it);
                pItem += 4;
            }
        }
    }

	//�趨RTP��ͷ��չ
	int CRTPPacket::SetExtensionBuf(unsigned short type, void *pbuf, int len)
	{
		SetExtension(true);
		SetExtensionType(type);
		unsigned short *ptrlen = (unsigned short *)(_buffer + _interleaved + MIN_HEAD_LENGTH+4*GetContribSrcCount()+2);
		unsigned short tmplen = len%4;
		if (len/4)
		{
			tmplen += 1;
		}
		*ptrlen = htons(tmplen);
		char *ptrExt = (char*)GetExtensionPtr();
		if (ptrExt)
		{
			memcpy(ptrExt, pbuf, len);
		}
		else
		{
			return -1;
		}
		return 0;
	}

	//��ȡRTP�����س���
    int CRTPPacket::GetPayloadSize()const
    {
        return _length;
    }

	//��ȡRTP��ʵ�����ݸ��س��ȣ�����������ֽ�
    int CRTPPacket::GetPayloadNoPaddingSize()const
    {
    	int PayloadSize = _length;
    	if(GetPadding())
    	{
    		//printf("------------padding len = %d,_wholeLen = %d\n",_buffer[_wholeLen-1],_wholeLen);
			if(_buffer[_wholeLen-1] > 3)
			{
				printf("CRTPPacket::GetPayloadSize() error!!!!!!!!!!!!!!!!!!!!!\n");
			}
			PayloadSize = PayloadSize - _buffer[_wholeLen-1];
    	}
        return PayloadSize;
    }
	//��Ӹ�������, �����ָ�����ݱ��������, Ч��Ҫ��һ��, ������ȫ
    int CRTPPacket::AddPayload(const char* data, int len)
    {
        int hdrlen = GetHeaderSize();
        int padLen = len%4;


        if((hdrlen + len) > _wholeLen)
        {
            printf("===RTPPacket=== Add payload error!\n");
            return -1;
        }

        ///�����ܳ���
        memcpy(_buffer + hdrlen, data, len);
        _wholeLen = hdrlen + len;
        _length = len;

        if (GetPadding() && (0 != padLen))
        {
            //����Padding
            _wholeLen += padLen;
            _length += padLen;
            memset(_buffer + hdrlen + len, 0, padLen);
            _buffer[_wholeLen - 1] = padLen;
            _buffer[0] |= 0x20;
        }
        return 0;
    }

	//��Ӹ�������, �����ָ�����ݱ��������, Ч��Ҫ��һ��, ������ȫ
	int CRTPPacket::AddPayload(char* data, int len)
	{
		if((GetHeaderSize() + len + _interleaved) > _wholeLen)
			return -1;
		memcpy(_buffer + _interleaved + GetHeaderSize(), data, len);
		_length = len;

#if 0
		printf("_length: %d\n", _length);
		for (int i = 0; i < 20; i++)
		{
			if (i%4 == 0)
			{
				printf("\n");
			}
			printf("0x%02x ", (unsigned char)_buffer[i]);
		}
		printf("\n");
#endif

		return 0;
	}


    //��ȡ��������, ������RTPͷ����
    const char* CRTPPacket::ReadPayload(int& len)const
    {
        len = _length;
        return _buffer + _interleaved + GetHeaderSize();
    }

	//��ȡ��������, ������RTPͷ����,����������ֽ�
    const char* CRTPPacket::ReadPayloadNoPadding(int& len)const
    {
		if(GetPadding())
    	{
    		//printf("------------padding len = %d,_wholeLen = %d\n",_buffer[_wholeLen-1],_wholeLen);
			len = _length - _buffer[_wholeLen-1];

		}
		else
		{
			len = _length;
		}
        return _buffer + _interleaved + GetHeaderSize();
    }

	//���ø��ػ��壬���������ڴ�
	//�����Ҫ������绯��������ָ��������ڱ���ȸ������������ڳ���������ȡ�����ڴ�
	//�����Ҫ��ȡ���绯����������Ը��صĿ����ز����٣���Ϊ���봮��ͷ�͸����γɱ�����
    int CRTPPacket::SetPayloadPtr(const char* data, size_t len)
    {
        size_t padLen = len%4;
        assert(data);
        ///ȷ��ԭ�����ڴ滺�壬���һ����㹻�󣬷�ֹ�������������
        assert(!_bCopy);
        assert(_wholeLen > len + GetHeaderSize() + padLen);

        _bExtPayload = true;
        _extPayload = data;
        _length = len;
        _wholeLen = len + GetHeaderSize();

        ///֪̽�Ƿ���Ҫ�����
        if (padLen > 0)
        {
            _wholeLen += padLen;
        }

        return 0;
    }

	//���ø��ػ��壬���������ڴ�
	//�����Ҫ������绯��������ָ��������ڱ���ȸ������������ڳ���������ȡ�����ڴ�
	//�����Ҫ��ȡ���绯����������Ը��صĿ����ز����٣���Ϊ���봮��ͷ�͸����γɱ�����
	int CRTPPacket::SetPayloadPtr(char* data, int len)
	{
		if ((len + GetHeaderSize() + _interleaved) >  _wholeLen)
			return -1;

		if (data != NULL)
		{
			if (_buffer != NULL)
				delete _buffer;

			_buffer = data;
		}

		if (len > 0)
			_length = len;

		return 0;
	}

	//�õ����л�������
    const char* CRTPPacket::GetStream(void)const
    {
        ///����������ⲿָ�룬����Ͳ��ò������ˣ�
        if (_bExtPayload)
        {
            int hdrlen = GetHeaderSize();
            memcpy(_buffer + hdrlen, _extPayload, _length);

            ///����Ƿ�Ҫ���λ
            int padLen = _length%4;
            if (padLen != 0)
            {
                assert(hdrlen + _length + padLen == _wholeLen);
                memset(_buffer + hdrlen + _length, 0, padLen);
                _buffer[_wholeLen - 1] = padLen;
                _buffer[0] |= 0x20;
            }
        }
        return _buffer;
    }

	//�õ����л�������
	char* CRTPPacket::GetStream()
	{
		return _buffer;
	}

	//�õ����л�����������
    int CRTPPacket::GetStreamLen(void)const
    {
        return _length + GetHeaderSize() + _interleaved;
    }

	//�趨RTSP OVER TCP��ʽ��, RTP��ͨ����ʶ
	int CRTPPacket::AddRtspChn(char chn)
	{
		if (_interleaved < 4)
			return -1;
		_buffer[0] = '$';
		_buffer[1] = chn;
		_buffer[2] = ((_length + GetHeaderSize()) & 0x0000FF00) >> 8;
		_buffer[3] = (_length + GetHeaderSize()) & 0xff;
		printf("len: %d, %d, %d\n", _buffer[2], _buffer[3], _length + GetHeaderSize());

		return 0;
	}
}
