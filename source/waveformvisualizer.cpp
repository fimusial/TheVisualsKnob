#include "waveformvisualizer.h"

#include "vstgui/lib/platform/iplatformgraphicsdevice.h"
#include "vstgui/lib/platform/iplatformgraphicspath.h"

namespace TVK
{
    WaveformVisualizer::WaveformVisualizer(IAudioDataProvider& audio)
        : CView(CRect()),
        audio(audio)
    {
        timer = makeOwned<CVSTGUITimer>(
            [this](auto*) {
                if (this->audio.tryGetBuffer(0, buffer))
                {
                    invalid();
                }
            },
            1,
            true);
    }

    WaveformVisualizer::~WaveformVisualizer()
    {
    }

    void WaveformVisualizer::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();
        context->setFillColor(CColor(0, 0, 0));
        context->drawRect(viewSize, kDrawFilled);
        drawWaveform(context);
        setDirty(false);
    }

    void WaveformVisualizer::drawWaveform(CDrawContext* context)
    {
        if (buffer.size() < 2)
        {
            return;
        }

        PlatformGraphicsDeviceContextPtr deviceContext = context->getPlatformDeviceContext();
        if (!deviceContext)
        {
            return;
        }

        PlatformGraphicsPathFactoryPtr pathFactory = deviceContext->getGraphicsPathFactory();
        if (!pathFactory)
        {
            return;
        }

        PlatformGraphicsPathPtr path = pathFactory->createPath();
        if (!path)
        {
            return;
        }

        CRect viewSize = getViewSize();
        context->setFrameColor(CColor(0, 255, 0));
        context->setLineWidth(3.0);

        path->beginSubpath(viewSize.getLeftCenter());

        int count = buffer.size();
        double h = viewSize.getHeight();
        double w = viewSize.getWidth();
        int step = 4;
        for (int i = 0; i < count; i += step)
        {
            path->addLine(CPoint(
                (double)i * w / (double)count,
                h / 2.0 + buffer[i] * h / 8.0
            ));
        }

        path->addLine(viewSize.getRightCenter());

        path->finishBuilding();
        deviceContext->drawGraphicsPath(*path, PlatformGraphicsPathDrawMode::Stroked, nullptr);
    }
}
