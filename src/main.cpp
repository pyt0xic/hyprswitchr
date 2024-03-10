#include "Log.hpp"
#include <string>
#define WLR_USE_UNSTABLE

#include "Globals.hpp"
#include "SSwitchManager.hpp"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/Window.hpp>
#include <hyprland/src/managers/AnimationManager.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>

// inline HANDLE                          PHANDLE = nullptr;

// Map of window PID's to Workspaces
// static std::unordered_map<uint32_t, CWorkspace*> PWORKSPACES;
//
static std::unordered_map<uint32_t, CWorkspace*> PWORKSPACES;

static SSwitchManager                            g_pSwitchManager;

// // fired when a window is mapped
// void onWindowOpen(void* self, std::any data) {
//     hyprswitchr_log(LOG, "Window opened");
//     auto* const PWINDOW = std::any_cast<CWindow*>(data);
//     g_pSwitchManager.addWindow(PWINDOW);
// }
//
// // fired when a window is unmapped
// void onWindowClose(void* self, std::any data) {
//     hyprswitchr_log(LOG, "Window closed");
//     auto* const PWINDOW = std::any_cast<CWindow*>(data);
//     g_pSwitchManager.removeWindow(PWINDOW);
// }
//
// // fired when a window changes workspace
// void onWindowMove(void* self, std::any data) {
//     std::vector<void*> moveData = std::any_cast<std::vector<void*>>(data);
//     CWindow*           window   = static_cast<CWindow*>(moveData.front());
//     CWorkspace*        ws       = static_cast<CWorkspace*>(moveData.back());
// }
//
// // fired on active window change
// void onActiveWindowChange(void* self, std::any data) {
//     hyprswitchr_log(LOG, "Active window changed");
//     auto* const PWINDOW = std::any_cast<CWindow*>(data);
//     g_pSwitchManager.updateWindow(PWINDOW);
//     // if (PWINDOW->m_bIsUrgent) {
//     //     g_pSwitchManager.setWindowResolved(PWINDOW);
//     // }
// }
//
// // fired when a window requests urgent
// void onWindowUrgentRequest(void* self, std::any data) {
//     hyprswitchr_log(LOG, "Window urgent request");
//     auto* const PWINDOW = std::any_cast<CWindow*>(data);
//     g_pSwitchManager.updateWindow(PWINDOW);
// }

// fired when a window title changes
// void onWindowTitleChange(void* self, std::any data) {
//     hyprswitchr_log(LOG, "Window title changed");
//     auto* const PWINDOW = std::any_cast<CWindow*>(data);
//     g_pSwitchManager.updateWindow(PWINDOW);
// }

void switchToUrgentOrPrev(std::string _) {
    hyprswitchr_log(LOG, "Switching to urgent or last");
    g_pSwitchManager.switchToUrgentOrPrevious();
}

void switchToMatchingUrgentOrPrev(std::string regex) {
    hyprswitchr_log(LOG, "Switching to matching or urgent or last");
    g_pSwitchManager.switchToMatchingUrgentOrPrev(regex);
}

void switchToUrgentOrNext(std::string _) {
    hyprswitchr_log(LOG, "Switching to urgent or next");
    g_pSwitchManager.switchToUrgentOrNext();
}

void switchToMatchingUrgentOrNext(std::string regex) {
    hyprswitchr_log(LOG, "Switching to matching or urgent or next");
    g_pSwitchManager.switchToMatchingUrgentOrNext(regex);
}

std::string getPrettyWindowList(std::vector<SWindow> windowList) {
    std::string prettyList = "";
    int         x          = 0;
    // Each window will have the format:
    // Window: <x>
    //     PID: <pid>
    //     Title: <title>
    //     Class: <class>
    //     InitialClass: <initialClass>
    //     InitialTitle: <initialTile>
    //     Active: <true/false>
    //     Urgent: <true/false>
    for (auto& w : windowList) {
        prettyList += "Window: " + std::to_string(x) + "\n";
        prettyList += "    PID: " + std::to_string(w.pid) + "\n";
        prettyList += "    Title: " + w.title + "\n";
        prettyList += "    InitialTitle: " + w.initialTitle + "\n";
        prettyList += "    InitialClass: " + w.initialClass + "\n";
        prettyList += "    Active: " + std::to_string(w.active) + "\n";
        prettyList += "    Urgent: " + std::to_string(w.urgent) + "\n";
        x++;
    }
    return prettyList;
}

std::string getWindowJson(std::vector<SWindow> windowList) {
    std::string json = "[";
    for (auto& w : windowList) {
        json += "{";
        json += "\"pid\": " + std::to_string(w.pid) + ",";
        json += "\"title\": \"" + w.title + "\",";
        json += "\"initialTitle\": \"" + w.initialTitle + "\",";
        json += "\"initialClass\": \"" + w.initialClass + "\",";
        json += "\"active\": " + std::to_string(w.active) + ",";
        json += "\"urgent\": " + std::to_string(w.urgent);
        json += "},";
    }
    json.pop_back();
    json += "]";
    return json;
}

std::string getWindowList(eHyprCtlOutputFormat format, std::string _) {
    hyprswitchr_log(LOG, "Getting window list with format {}", format == eHyprCtlOutputFormat::FORMAT_JSON ? "JSON" : "PRETTY");
    auto windowList = g_pSwitchManager.getWindowList();
    hyprswitchr_log(LOG, "Got window list of length {}", windowList.size());
    auto result = format == eHyprCtlOutputFormat::FORMAT_JSON ? getWindowJson(windowList) : getPrettyWindowList(windowList);
    hyprswitchr_log(LOG, "Window list: {}", result);
    return result;
}

std::string getReverseWindowList(eHyprCtlOutputFormat format, std::string _) {
    hyprswitchr_log(LOG, "Getting window list with format {}", format == eHyprCtlOutputFormat::FORMAT_JSON ? "JSON" : "PRETTY");
    auto windowList = g_pSwitchManager.getReverseWindowList();
    hyprswitchr_log(LOG, "Got window list of length {}", windowList.size());
    auto result = format == eHyprCtlOutputFormat::FORMAT_JSON ? getWindowJson(windowList) : getPrettyWindowList(windowList);
    hyprswitchr_log(LOG, "Window list: {}", result);
    return result;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE          = handle;
    g_pSwitchManager = SSwitchManager();
    // Iterate over existing windows and add them to the switch manager
    // for (auto& w : g_pCompositor->m_vWindows) {
    //     g_pSwitchManager.addWindow(w.get());
    // }

    // Register callbacks
    // HyprlandAPI::registerCallbackDynamic(PHANDLE, "activeWindow", [&](void* self, SCallbackInfo& info, std::any data) { onActiveWindowChange(self, data); });
    // hyprswitchr_log(LOG, "Registered active window change callback");
    // HyprlandAPI::registerCallbackDynamic(PHANDLE, "openWindow", [&](void* self, SCallbackInfo& info, std::any data) { onWindowOpen(self, data); });
    // hyprswitchr_log(LOG, "Registered open window callback");
    // HyprlandAPI::registerCallbackDynamic(PHANDLE, "closeWindow", [&](void* self, SCallbackInfo& info, std::any data) { onWindowClose(self, data); });
    // hyprswitchr_log(LOG, "Registered close window callback");
    // HyprlandAPI::registerCallbackDynamic(PHANDLE, "moveWindow", [&](void* self, SCallbackInfo& info, std::any data) { onWindowMove(self, data); });
    // hyprswitchr_log(LOG, "Registered move window callback");
    // HyprlandAPI::registerCallbackDynamic(PHANDLE, "urgent", [&](void* self, SCallbackInfo& info, std::any data) { onWindowUrgentRequest(self, data); });
    // hyprswitchr_log(LOG, "Registered urgent window callback");
    // HyprlandAPI::registerCallbackDynamic(PHANDLE, "windowtitle", [&](void* self, SCallbackInfo& info, std::any data) { onWindowTitleChange(self, data); });
    // hyprswitchr_log(LOG, "Registered title window callback");

    // Register dispatchers
    HyprlandAPI::addDispatcher(PHANDLE, "switch-to-urgent-or-prev", switchToUrgentOrPrev);
    HyprlandAPI::addDispatcher(PHANDLE, "switch-to-urgent-or-next", switchToUrgentOrNext);
    HyprlandAPI::addDispatcher(PHANDLE, "switch-to-matching-urgent-or-prev", switchToMatchingUrgentOrPrev);
    HyprlandAPI::addDispatcher(PHANDLE, "switch-to-matching-urgent-or-next", switchToMatchingUrgentOrNext);
    HyprlandAPI::registerHyprCtlCommand(PHANDLE, SHyprCtlCommand("windowlist", true, getWindowList));
    HyprlandAPI::registerHyprCtlCommand(PHANDLE, SHyprCtlCommand("windowlist-reverse", true, getReverseWindowList));

    return {"hyprswitchr", "An urgent-first/most-recent window switcher inspired by swayr", "pyt0xic", "0.0.1"};
}

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT void PLUGIN_EXIT() {
    // ...
}
