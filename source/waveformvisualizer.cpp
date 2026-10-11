#include "waveformvisualizer.h"

#include "vstgui/lib/platform/iplatformgraphicsdevice.h"
#include "vstgui/lib/platform/iplatformgraphicspath.h"

namespace TVK
{
    WaveformVisualizer::WaveformVisualizer(IAudioDataProvider& audio)
        : CView(CRect()),
        audio(audio)
    {
        startPolling();
    }

    WaveformVisualizer::~WaveformVisualizer()
    {
    }

    void WaveformVisualizer::draw(CDrawContext* context)
    {
        CRect viewSize = getViewSize();
        context->setFillColor(backgroundColor);
        context->drawRect(viewSize, kDrawFilled);

        alignWaveform();
        drawWaveform(context);

        setDirty(false);
    }

    void WaveformVisualizer::startPolling()
    {
        auto poll = [this](auto*)
        {
            if (audio.tryGetBuffer(0, newBuffer))
            {
                invalid();
            }
        };

        timer = makeOwned<CVSTGUITimer>(poll, PollDelayMs, true);
    }

    void WaveformVisualizer::alignWaveform()
    {
        if (newBuffer.size() < 2)
        {
            return;
        }

        int size = newBuffer.size();
        int shift = 0;
        double bestError = 4.0 * size;

        oldBuffer.resize(size, 0.0);

        for (int i = 0; i < size; i++)
        {
            double totalError = 0.0;
            for (int j = 0; j < size && totalError < bestError; j += SampleStep)
            {
                double error = newBuffer[(i + j) % size] - oldBuffer[j];
                totalError += error * error;
            }

            if (totalError < bestError)
            {
                bestError = totalError;
                shift = i;
            }
        }

        for (int i = 0; i < size; i++)
        {
            oldBuffer[i] = newBuffer[(shift + i) % size];
        }
    }

    void WaveformVisualizer::drawWaveform(CDrawContext* context)
    {
        if (newBuffer.size() < 2)
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
        context->setFrameColor(waveformColor);
        context->setLineWidth(waveformThickness);

        path->beginSubpath(viewSize.getLeftCenter());

        int size = oldBuffer.size();
        double halfHeight = viewSize.getHeight() / 2.0;
        double width = viewSize.getWidth();

        for (int i = 0; i < size; i += SampleStep)
        {
            path->addLine(CPoint(
                (double)i * width / (double)size,
                halfHeight - oldBuffer[i] * halfHeight
            ));
        }

        path->addLine(viewSize.getRightCenter());

        path->finishBuilding();
        deviceContext->drawGraphicsPath(*path, PlatformGraphicsPathDrawMode::Stroked, nullptr);
    }
}
