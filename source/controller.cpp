#include "controller.h"

#include "vstgui/plugin-bindings/vst3editor.h"
#include "vstgui/uidescription/uicontentprovider.h"
#include "resourcemanager.h"

namespace TVK
{
    tresult PLUGIN_API TheVisualsKnobController::initialize(FUnknown* context)
    {
        return EditControllerEx1::initialize(context);
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
        return kResultOk;
    }

    tresult PLUGIN_API TheVisualsKnobController::getState(IBStream* state)
    {
        return kResultOk;
    }

    IPlugView* PLUGIN_API TheVisualsKnobController::createView(FIDString name)
    {
        if (!FIDStringsEqual(name, Vst::ViewType::kEditor))
        {
            return nullptr;
        }

        std::string uiDescriptionContent = ResourceManager::getFileContent("uidesc.json");
        VSTGUI::IContentProvider* contentProvider = new VSTGUI::MemoryContentProvider(
            uiDescriptionContent.c_str(), (unsigned int)uiDescriptionContent.size());

        VSTGUI::UIDescription* uiDescription = new VSTGUI::UIDescription(contentProvider, nullptr);
        return new VSTGUI::VST3Editor(uiDescription, this, "view");
    }
}
