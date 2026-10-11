#pragma once
#include <vector>

namespace TVK
{
    class IAudioDataProvider
    {
    public:
        virtual int getChannelCount() = 0;
        virtual int getBufferSize() = 0;
        virtual int getAvailableBufferCount(int channel) = 0;

        virtual bool tryGetBuffer(int channel, std::vector<double>& buffer) = 0;

        virtual void setSkipBuffers(bool newValue) = 0;
        virtual void setBufferSize(int newValue) = 0;
    };
}
