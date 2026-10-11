#pragma once

#include "vstgui/vstgui_uidescription.h"
#include "iaudiodataprovider.h"

using namespace VSTGUI;

namespace TVK
{
    class VisualizerFactory : public ViewCreatorAdapter
    {
    public:
        VisualizerFactory(IAudioDataProvider& audio);
        ~VisualizerFactory();
        IdStringPtr getViewName() const override;
        IdStringPtr getBaseViewName() const override;
        CView* create(const UIAttributes& attributes, const IUIDescription* description) const override;

    private:
        IAudioDataProvider& audio;
    };
}
