#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

using namespace Steinberg;
using namespace Vst;

namespace TVK
{
    class TheVisualsKnobProcessor : public AudioEffect
    {
    public:
        TheVisualsKnobProcessor();
        ~TheVisualsKnobProcessor() SMTG_OVERRIDE;

        static FUnknown* createInstance(void* context) 
        {
            return (IAudioProcessor*)new TheVisualsKnobProcessor; 
        }

        tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
        tresult PLUGIN_API terminate() SMTG_OVERRIDE;
        tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) SMTG_OVERRIDE;
        tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) SMTG_OVERRIDE;
        tresult PLUGIN_API setupProcessing(ProcessSetup& newSetup) SMTG_OVERRIDE;
        tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
        tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;
        tresult PLUGIN_API process(ProcessData& data) SMTG_OVERRIDE;

    private:
        int channelCount = 0;
    };
}
