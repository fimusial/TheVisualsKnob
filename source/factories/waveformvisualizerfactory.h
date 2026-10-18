#pragma once

#include "vstgui/vstgui_uidescription.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"

#include "../iaudiodataprovider.h"
#include "../ui/waveformvisualizer.h"

using namespace VSTGUI;

namespace TVK
{
    class WaveformVisualizerFactory : public ViewCreatorAdapter
    {
    public:
        WaveformVisualizerFactory(IAudioDataProvider& provider)
            : provider(provider)
        {
            UIViewFactory::registerViewCreator(*this);
        }

        ~WaveformVisualizerFactory()
        {
            UIViewFactory::unregisterViewCreator(*this);
        }

        IdStringPtr getViewName() const override
        {
            return "WaveformVisualizer";
        }

        IdStringPtr getBaseViewName() const override
        {
            return UIViewCreator::kCView;
        }

        CView* create(const UIAttributes& attributes, const IUIDescription* description) const override
        {
            return new WaveformVisualizer(provider);
        }

    private:
        IAudioDataProvider& provider;
    };
}
