#pragma once

#include "vstgui/vstgui_uidescription.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"

#include "../iaudiodataprovider.h"
#include "../ui/spectrogramvisualizer.h"

using namespace VSTGUI;

namespace TVK
{
    class SpectrogramVisualizerFactory : public ViewCreatorAdapter
    {
    public:
        SpectrogramVisualizerFactory(IAudioDataProvider& provider)
            : provider(provider)
        {
            UIViewFactory::registerViewCreator(*this);
        }

        ~SpectrogramVisualizerFactory()
        {
            UIViewFactory::unregisterViewCreator(*this);
        }

        IdStringPtr getViewName() const override
        {
            return "SpectrogramVisualizer";
        }

        IdStringPtr getBaseViewName() const override
        {
            return UIViewCreator::kCView;
        }

        CView* create(const UIAttributes& attributes, const IUIDescription* description) const override
        {
            return new SpectrogramVisualizer(provider);
        }

    private:
        IAudioDataProvider& provider;
    };
}
