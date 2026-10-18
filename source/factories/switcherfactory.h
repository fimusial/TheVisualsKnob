#pragma once

#include "vstgui/vstgui_uidescription.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"

#include "../ui/switcher.h"

using namespace VSTGUI;

namespace TVK
{
    class SwitcherFactory : public ViewCreatorAdapter
    {
    public:
        SwitcherFactory()
        {
            UIViewFactory::registerViewCreator(*this);
        }

        ~SwitcherFactory()
        {
            UIViewFactory::unregisterViewCreator(*this);
        }

        IdStringPtr getViewName() const override
        {
            return "Switcher";
        }

        IdStringPtr getBaseViewName() const override
        {
            return UIViewCreator::kCControl;
        }

        CView* create(const UIAttributes& attributes, const IUIDescription* description) const override
        {
            int stepCount;
            if (!attributes.getIntegerAttribute("stepCount", stepCount) || stepCount < 2)
            {
                return nullptr;
            }

            int parameterId;
            if (!attributes.getIntegerAttribute("parameterId", parameterId) || parameterId < 256)
            {
                return nullptr;
            }

            return new Switcher(stepCount, (ParameterId)parameterId);
        }
    };
}
