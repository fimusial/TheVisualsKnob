#pragma once

#include "vstgui/vstgui.h"
#include "iaudiodataprovider.h"

using namespace VSTGUI;

namespace TVK
{
    class WaveformVisualizer : public CView
    {
    public:
        WaveformVisualizer(IAudioDataProvider& audio);
        ~WaveformVisualizer();

        void draw(CDrawContext* context) override;
        CBaseObject* newCopy() const override { return new WaveformVisualizer(*this); }

    private:
        static const int PollDelayMs = 8;
        static const int SampleStep = 4;

        IAudioDataProvider& audio;
        SharedPointer<CVSTGUITimer> timer;
        std::vector<double> newBuffer;
        std::vector<double> oldBuffer;

        CColor backgroundColor = CColor(17, 17, 17);
        CColor waveformColor = CColor(17, 255, 17);
        int waveformThickness = 2.0;

        void startPolling();
        void alignWaveform();
        void drawWaveform(CDrawContext* context);
    };
}
