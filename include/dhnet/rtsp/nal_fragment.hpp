
#ifndef RTSP_NAL_FRAGMENT_H__
#define RTSP_NAL_FRAGMENT_H__


#include <dhfile/dhav/frame_stream.hpp>
#include <dhfile/dhav/nal_stream.hpp>


namespace dhnet {
namespace rtsp {



/// NAL Ƭ������
struct nal_fragment_kind
{
    typedef enum {
        whole,  ///< ����NAL
        head,   ///< �׸�NALƬ��
        mid,    ///< �м�NALƬ��
        tail    ///< ĩβNALƬ��
    } type;
};

/// NAL Ƭ��
struct nal_fragment
{
    typedef dhfile::dhav::frame_raw::data_t::const_iterator raw_iterator;
    typedef nal_fragment_kind::type kind_t;

    kind_t kind;            ///< Ƭ������
    uint8_t      head;      ///< NAL head
    raw_iterator begin;     ///< �������俪ʼ
    raw_iterator end;       ///< �����������

    nal_fragment(kind_t k, uint8_t h, raw_iterator b, raw_iterator e)
        : kind(k)
        , head(h)
        , begin(b)
        , end(e)
    {
    }
};

/// NALƬ������
typedef std::vector<nal_fragment>   nal_fragment_vector;


/// ��һ֡���ݷֽ��nalƬ�Σ���RTP���
/// \param frame_raw [in] ����NAL����
/// \param fragments [out] NALƬ������
void dispart_nal(
        dhfile::dhav::frame_raw const& frame,
        nal_fragment_vector& fragments
)
{
    typedef dhfile::dhav::frame_stream                      frame_stream_t;
    typedef dhfile::dhav::frame_raw                         frame_raw_t;
    typedef frame_raw_t::data_t                             raw_t;
    typedef raw_t::const_iterator                           raw_iterator;
    typedef dhfile::dhav::nal_stream_wrapper<raw_iterator>  nal_stream_t;
    typedef nal_stream_t::raw_range_t                       raw_range_t;

    nal_stream_t nal_stream(frame.data.begin(), frame.data.end());
    nal_fragment_vector nal_fragments;

    raw_range_t nal = nal_stream.read_nal();
    while ( nal.first != nal.second )
    {
        enum { playload_max_size = 1400 };

        /// depart one nal to many fragmets
        uint8_t head = *nal.first;
        ++nal.first;
        if ( std::distance(nal.first,nal.second) <= playload_max_size-1 )
        {
            /// ��������NALƬ��
            nal_fragments.push_back(
                nal_fragment(
                    nal_fragment_kind::whole, head, nal.first, nal.second));
        }
        else
        {
            /// �����׸�NALƬ��
            raw_iterator data_begin = nal.first;
            std::advance(nal.first, playload_max_size-2);
            nal_fragments.push_back(
                nal_fragment(
                    nal_fragment_kind::head, head, data_begin, nal.first));

            /// �����м�NALƬ��
            while (std::distance(nal.first, nal.second) > playload_max_size-2)
            {
                data_begin = nal.first;
                std::advance(nal.first, playload_max_size-2);
                nal_fragments.push_back(
                    nal_fragment(
                        nal_fragment_kind::mid, head, data_begin, nal.first));
            }

            /// ����ĩβNALƬ��
            nal_fragments.push_back(
                nal_fragment(
                    nal_fragment_kind::tail, head, nal.first, nal.second));
        }

        /// ����һ��NAL����
        nal = nal_stream.read_nal();
    }

    /// ����NALƬ������
    std::swap(nal_fragments,fragments);
}









} // namespace rtsp
} // namespace dhnet

#endif // RTSP_NAL_FRAGMENT_H__
