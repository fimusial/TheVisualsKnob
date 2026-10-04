#include "public.sdk/source/main/pluginfactory.h"

#include "cids.h"
#include "version.h"
#include "controller.h"
#include "processor.h"

#define stringPluginName "TheVisualsKnob"

using namespace Steinberg;
using namespace TVK;

BEGIN_FACTORY_DEF("fimusial", 
                    "https://github.com/fimusial", 
                    "mailto:fimusial@gmail.com")

    DEF_CLASS2(INLINE_UID_FROM_FUID(kTheVisualsKnobProcessorUID),
                PClassInfo::kManyInstances, // cardinality
                kVstAudioEffectClass, // the component category (do not changed this)
                stringPluginName, // here the Plug-in name (to be changed)
                Vst::kDistributable, // means that component and controller could be distributed on different computers
                TheVisualsKnobVST3Category, // Subcategory for this Plug-in (to be changed)
                "1.0.0", // Plug-in version (to be changed)
                kVstVersionString, // the VST 3 SDK version (do not changed this, use always this define)
                TheVisualsKnobProcessor::createInstance) // function pointer called when this component should be instantiated

    DEF_CLASS2(INLINE_UID_FROM_FUID(kTheVisualsKnobControllerUID),
                PClassInfo::kManyInstances, // cardinality
                kVstComponentControllerClass, // the Controller category (do not changed this)
                stringPluginName "Controller", // controller name (could be the same than component name)
                0, // not used here
                "", // not used here
                "1.0.0", // Plug-in version (to be changed)
                kVstVersionString, // the VST 3 SDK version (do not changed this, use always this define)
                TheVisualsKnobController::createInstance) // function pointer called when this component should be instantiated
END_FACTORY
