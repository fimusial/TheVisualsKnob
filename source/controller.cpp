#include "controller.h"

#include "vstgui/plugin-bindings/vst3editor.h"
#include "vstgui/uidescription/uicontentprovider.h"

#include "cids.h"
#include "resourcemanager.h"

namespace TVK
{
    tresult PLUGIN_API TheVisualsKnobController::initialize(FUnknown* context)
    {
        tresult result = EditControllerEx1::initialize(context);
        if (result != kResultOk)
        {
            return result;
        }

        // title, units, step count, default value normalized, flags, tag

        parameters.addParameter(
            STR16("Visualizer"), nullptr, 0, 0.0, ParameterInfo::kIsHidden, Visualizer);

        return kResultOk;
    }

    tresult PLUGIN_API TheVisualsKnobController::terminate()
    {
        return EditControllerEx1::terminate();
    }

    tresult PLUGIN_API TheVisualsKnobController::setComponentState(IBStream* state)
    {
        return kResultOk;
    }

    tresult PLUGIN_API TheVisualsKnobController::setState(IBStream* state)
    {
        // TODO
        return kResultOk;
    }

    tresult PLUGIN_API TheVisualsKnobController::getState(IBStream* state)
    {
        // TODO
        return kResultOk;
    }

    IPlugView* PLUGIN_API TheVisualsKnobController::createView(FIDString)
    {
        std::string uiDescriptionContent = ResourceManager::getFileContent("uidesc.json");
        VSTGUI::IContentProvider* contentProvider = new VSTGUI::MemoryContentProvider(
            uiDescriptionContent.c_str(), (unsigned int)uiDescriptionContent.size());

        VSTGUI::UIDescription* uiDescription = new VSTGUI::UIDescription(contentProvider, nullptr);
        return new VSTGUI::VST3Editor(uiDescription, this, "view");
    }
}
