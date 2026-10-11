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
            8,
            true);
    }

    WaveformVisualizer::~WaveformVisualizer()
    {
    }

    void WaveformVisualizer::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();
        context->setFillColor(CColor(17, 17, 17));
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
        context->setLineWidth(2.0);

        path->beginSubpath(viewSize.getLeftCenter());

        int count = buffer.size();
        double h2 = viewSize.getHeight() / 2.0;
        double w = viewSize.getWidth();
        int step = 4;
        for (int i = 0; i < count; i += step)
        {
            path->addLine(CPoint(
                (double)i * w / (double)count,
                h2 - buffer[i] * h2
            ));
        }

        path->addLine(viewSize.getRightCenter());

        path->finishBuilding();
        deviceContext->drawGraphicsPath(*path, PlatformGraphicsPathDrawMode::Stroked, nullptr);
    }
}
