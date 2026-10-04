#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

using namespace Steinberg;
using namespace Vst;

namespace TVK
{
    class TheVisualsKnobController : public EditControllerEx1
    {
    public:
        TheVisualsKnobController() = default;
        ~TheVisualsKnobController() SMTG_OVERRIDE = default;

        static FUnknown* createInstance(void* context)
        {
            return (IEditController*)new TheVisualsKnobController;
        }

        tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
        tresult PLUGIN_API terminate() SMTG_OVERRIDE;
        tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;
        IPlugView* PLUGIN_API createView(FIDString name) SMTG_OVERRIDE;

        DEFINE_INTERFACES
        END_DEFINE_INTERFACES(EditController)
        DELEGATE_REFCOUNT(EditController)
    };
}
