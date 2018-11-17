#include "serializer.hpp"

namespace rvi
{
    data_t serializer::select_frame(const std::string& fname)
    {
        data_t result;

        // header
        result.push_back(SC_U8(cmd_header::SELECT_FRAME));

        // frame name size
        serialize_integral<uint16_t>(result, SC_U16(fname.size()));

        // frame name
        std::copy(fname.begin(), fname.end(), std::back_inserter(result));

        return result;
    }

    data_t serializer::release_frame()
    {
        data_t result;

        // header
        result.push_back(SC_U8(cmd_header::RELEASE_FRAME));

        return result;
    }

    data_t serializer::delete_frame(const std::string& fname)
    {
        data_t result;

        // header
        result.push_back(SC_U8(cmd_header::DELETE_FRAME));

        // frame name size
        serialize_integral<uint16_t>(result, SC_U16(fname.size()));

        // frame name
        std::copy(fname.begin(), fname.end(), std::back_inserter(result));

        return result;
    }

    data_t serializer::draw_line(const rvi::line& ln)
    {
        data_t result;

        // header
        result.push_back(SC_U8(cmd_header::DRAW_LINE));

        serialize_line_bf(result, ln);

        return result;
    }

    data_t serializer::set_transform(const rvi::transform2& tform)
    {
        data_t result;

        // header
        result.push_back(SC_U8(cmd_header::SET_TRANSFORM));

        serialize_transform2_bf(result, tform);

        return result;
    }   
}