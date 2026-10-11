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
        IAudioDataProvider& audio;
        SharedPointer<CVSTGUITimer> timer;
        std::vector<double> buffer;

        void WaveformVisualizer::drawWaveform(CDrawContext* context);
    };
}
