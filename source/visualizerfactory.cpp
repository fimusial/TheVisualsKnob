#include "visualizerfactory.h"

#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "waveformvisualizer.h"

namespace TVK
{
    VisualizerFactory::VisualizerFactory(IAudioDataProvider& audio)
        : audio(audio)
    {
        UIViewFactory::registerViewCreator(*this);
    }

    VisualizerFactory::~VisualizerFactory()
    {
        UIViewFactory::unregisterViewCreator(*this);
    }

    IdStringPtr VisualizerFactory::getViewName() const
    {
        return "WaveformVisualizer";
    }

    IdStringPtr VisualizerFactory::getBaseViewName() const
    {
        return UIViewCreator::kCView;
    }

    CView* VisualizerFactory::create(const UIAttributes& attributes, const IUIDescription* description) const
    {
        return new WaveformVisualizer(audio);
    }
}
