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
        if (accumulators)
        {
            delete[] accumulators;
        }

        if (fifos)
        {
            delete[] fifos;
        }
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
        tresult result = AudioEffect::setupProcessing(newSetup);
        if (result != kResultOk)
        {
            return result;
        }

        if (channelCount < 1)
        {
            return kResultOk;
        }

        if (!accumulators && !fifos)
        {
            accumulators = new std::vector<double>[channelCount];
            fifos = new BufferFifo<double>[channelCount];

            for (int channel = 0; channel < channelCount; channel++)
            {
                accumulators[channel].reserve(dataWindowSize);
            }
        }

        return kResultOk;
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
        if (processSetup.symbolicSampleSize != data.symbolicSampleSize)
        {
            return kInvalidArgument;
        }

        if (processSetup.processMode != kRealtime)
        {
            return kResultOk;
        }

        if (data.numInputs < 1 || data.numOutputs < 1 || data.numSamples < 1 || channelCount < 1)
        {
            return kResultOk;
        }

        bool isSilentOrEmpty = transferToOutput(data);

        if (!skipSilentBlocks || !isSilentOrEmpty)
        {
            publishBlocks(data);
        }

        return kResultOk;
    }

    bool TheVisualsKnobProcessor::transferToOutput(ProcessData& data)
    {
        bool isSilentOrEmpty = true;
        for (int channel = 0; channel < channelCount; channel++)
        {
            for (int index = 0; index < data.numSamples; index++)
            {
                if (processSetup.symbolicSampleSize == kSample32)
                {
                    float sample32 = data.inputs[0].channelBuffers32[channel][index];
                    data.outputs[0].channelBuffers32[channel][index] = sample32;
                    isSilentOrEmpty = sample32 != 0.0f ? false : isSilentOrEmpty;
                }

                if (processSetup.symbolicSampleSize == kSample64)
                {
                    double sample64 = data.inputs[0].channelBuffers64[channel][index];
                    data.outputs[0].channelBuffers64[channel][index] = sample64;
                    isSilentOrEmpty = sample64 != 0.0f ? false : isSilentOrEmpty;
                }
            }
        }

        data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;
        return isSilentOrEmpty;
    }

    void TheVisualsKnobProcessor::publishBlocks(ProcessData& data)
    {
        for (int channel = 0; channel < channelCount; channel++)
        {
            for (int index = 0; index < data.numSamples; index++)
            {
                double sample = 0.0;

                if (processSetup.symbolicSampleSize == kSample32)
                {
                    sample = data.inputs[0].channelBuffers32[channel][index];
                }

                if (processSetup.symbolicSampleSize == kSample64)
                {
                    sample = data.inputs[0].channelBuffers64[channel][index];
                }

                accumulators[channel].push_back(sample);

                if (accumulators[channel].size() > dataWindowSize)
                {
                    accumulators[channel].resize(dataWindowSize);
                }

                if (accumulators[channel].size() == dataWindowSize)
                {
                    fifos[channel].tryPush(accumulators[channel]);
                    accumulators[channel].clear();
                }
            }
        }
    }
}
