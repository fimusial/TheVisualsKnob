#include "processor.h"

#include "cids.h"

namespace TVK
{
    TheVisualsKnobProcessor::TheVisualsKnobProcessor()
    {
        setControllerClass(kTheVisualsKnobControllerUID);
    }

    TheVisualsKnobProcessor::~TheVisualsKnobProcessor()
    {
    }

    tresult PLUGIN_API TheVisualsKnobProcessor::initialize(FUnknown* context)
    {
        processContextRequirements = IProcessContextRequirements::kNeedTransportState;

        tresult result = AudioEffect::initialize(context);
        if (result != kResultOk)
        {
            return result;
        }

        addAudioInput(STR16("Stereo In"), SpeakerArr::kStereo);
        addAudioOutput(STR16("Stereo Out"), SpeakerArr::kStereo);
        return kResultOk;
    }

    tresult PLUGIN_API TheVisualsKnobProcessor::terminate()
    {
        return AudioEffect::terminate();
    }

    tresult PLUGIN_API TheVisualsKnobProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
    {
        if (numIns != 1 || numOuts != 1 || inputs[0] != outputs[0])
        {
            return kResultFalse;
        }

        tresult result = AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
        if (result != kResultOk)
        {
            return result;
        }

        int newChannelCount = SpeakerArr::getChannelCount(outputs[0]);
        if (channelCount > 0 && newChannelCount > channelCount)
        {
            return kResultFalse;
        }

        channelCount = newChannelCount;
        return kResultOk;
    }

    tresult PLUGIN_API TheVisualsKnobProcessor::canProcessSampleSize(int32 symbolicSampleSize)
    {
        return symbolicSampleSize == kSample32 || symbolicSampleSize == kSample64
            ? kResultTrue
            : kResultFalse;
    }

    tresult PLUGIN_API TheVisualsKnobProcessor::setupProcessing(ProcessSetup& newSetup)
    {
        return AudioEffect::setupProcessing(newSetup);
    }

    tresult PLUGIN_API TheVisualsKnobProcessor::setActive(TBool state)
    {
        return AudioEffect::setActive(state);
    }

    tresult PLUGIN_API TheVisualsKnobProcessor::setState(IBStream* state)
    {
        return kResultOk;
    }

    tresult PLUGIN_API TheVisualsKnobProcessor::getState(IBStream* state)
    {
        return kResultOk;
    }

    tresult PLUGIN_API TheVisualsKnobProcessor::process(ProcessData& data)
    {
        if (data.numInputs == 0 || data.numOutputs == 0 || data.numSamples == 0)
        {
            return kResultOk;
        }

        for (int channel = 0; channel < channelCount; channel++)
        {
            if (data.symbolicSampleSize == kSample32)
            {
                for (int i = 0; i < data.numSamples; i++)
                {
                    data.outputs[0].channelBuffers32[channel][i] = data.inputs[0].channelBuffers32[channel][i];
                }
            }
            else
            {
                for (int i = 0; i < data.numSamples; i++)
                {
                    data.outputs[0].channelBuffers64[channel][i] = data.inputs[0].channelBuffers64[channel][i];
                }
            }
        }

        data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;
        return kResultOk;
    }
}
