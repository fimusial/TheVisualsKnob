#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

#include "iaudiodataprovider.h"
#include "visualizerfactory.h"
#include "bufferfifo.h"

using namespace Steinberg;
using namespace Vst;

namespace TVK
{
    class TheVisualsKnobProcessor
        : public AudioEffect,
        public IAudioDataProvider
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

        int getChannelCount() override;
        int getBufferSize() override;
        int getAvailableBufferCount(int channel) override;
        bool tryGetBuffer(int channel, std::vector<double>& buffer) override;
        void setSkipBuffers(bool newValue) override;
        void setBufferSize(int newValue) override;

    private:
        VisualizerFactory visualizerFactory;

        int channelCount = 0;
        int bufferSize = 2048;
        bool skipSilentBuffers = true;

        std::vector<double>* accumulators = nullptr;
        BufferFifo<double>* bufferFifos = nullptr;

        bool transferToOutput(ProcessData& data);
        void publishBuffers(ProcessData& data);
    };
}
