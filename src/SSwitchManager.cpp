#include "SSwitchManager.hpp"
#include "Log.hpp"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/debug/Log.hpp>
#include <vector>

std::vector<CWindow*> SSwitchManager::getReverseCWindowList() {
    hyprswitchr_log(LOG, "Getting reverse CWindow list");
    std::vector<CWindow*> focusHistory = g_pCompositor->m_vWindowFocusHistory;
    CWindow*              urgentWindow = g_pCompositor->getUrgentWindow();
    CWindow*              active;
    std::vector<CWindow*> windowList = {};
    // Iterate through the focus history and add the windows to the list, if the window is active, add it last.
    for (auto w : focusHistory) {
        if (!g_pCompositor->windowExists(w)) {
            continue;
        }
        if (g_pCompositor->isWindowActive(w)) {
            active = w;
        } else {
            windowList.push_back(w);
        }
    }
    // Reverse the list
    std::reverse(windowList.begin(), windowList.end());
    // Add the urgent window to the front of the list.
    if (g_pCompositor->windowExists(urgentWindow)) {
        windowList.insert(windowList.begin(), urgentWindow);
    }
    // Add the active window to the back of the list
    if (g_pCompositor->windowExists(active)) {
        windowList.push_back(active);
    }
    return windowList;
}

std::vector<CWindow*> SSwitchManager::getCWindowList() {
    hyprswitchr_log(LOG, "Getting CWindow list");
    std::vector<CWindow*> focusHistory = g_pCompositor->m_vWindowFocusHistory;
    CWindow*              urgentWindow = g_pCompositor->getUrgentWindow();
    CWindow*              active;
    std::vector<CWindow*> windowList = {};
    // Iterate through the focus history and add the windows to the list, if the window is active, add it last.
    for (auto w : focusHistory) {
        if (!g_pCompositor->windowExists(w)) {
            continue;
        }
        if (g_pCompositor->isWindowActive(w)) {
            active = w;
        } else {
            windowList.push_back(w);
        }
    }
    // Add the urgent window to the front of the list.
    if (g_pCompositor->windowExists(urgentWindow)) {
        windowList.insert(windowList.begin(), urgentWindow);
    }
    // Add the active window to the back of the list
    if (g_pCompositor->windowExists(active)) {
        windowList.push_back(active);
    }
    return windowList;
}

std::vector<SWindow> SSwitchManager::getWindowList() {
    hyprswitchr_log(LOG, "Getting window list");
    std::vector<SWindow> windowList = {};
    for (auto w : SSwitchManager::getCWindowList()) {
        SWindow window = SWindow(w);
        windowList.push_back(window);
    }
    return windowList;
}

std::vector<SWindow> SSwitchManager::getReverseWindowList() {
    hyprswitchr_log(LOG, "Getting reverse window list");
    std::vector<SWindow> windowList = {};
    for (auto w : SSwitchManager::getReverseCWindowList()) {
        SWindow window = SWindow(w);
        windowList.push_back(window);
    }
    return windowList;
}

void SSwitchManager::switchTrackedPrev() {
    if (switchPosition > trackedSwitchList.size() - 1) {
        switchPosition = 0;
    } else {
        switchPosition++;
    }
    if (!g_pCompositor->windowExists(trackedSwitchList[switchPosition])) {
        hyprswitchr_log(LOG, "Window doesn't exist refreshing list");
        SSwitchManager::resetTrackedSwitch();
        SSwitchManager::switchToUrgentOrPrev(true);
    }
    g_pCompositor->focusWindow(trackedSwitchList[switchPosition]);
}

void SSwitchManager::switchTrackedNext() {
    if (switchPosition > trackedSwitchList.size() - 1) {
        switchPosition = 0;
    } else {
        switchPosition++;
    }
    if (!g_pCompositor->windowExists(trackedSwitchList[switchPosition])) {
        hyprswitchr_log(LOG, "Window doesn't exist refreshing list");
        SSwitchManager::resetTrackedSwitch();
        SSwitchManager::switchToUrgentOrNext(true);
    }
    g_pCompositor->focusWindow(trackedSwitchList[switchPosition]);
}

void SSwitchManager::switchToUrgentOrPrev(bool track) {
    if (track) {
        trackedSwitchList = switchPosition == -1 ? SSwitchManager::getCWindowList() : trackedSwitchList;
        return SSwitchManager::switchTrackedPrev();
    }
    auto windowList = SSwitchManager::getCWindowList();
    if (!g_pCompositor->windowExists(windowList.front())) {
        hyprswitchr_log(LOG, "No urgent or previous window found");
        return;
    }
    hyprswitchr_log(LOG, "Focusing urgent or previous window: {}", windowList.front()->m_szTitle);
    g_pCompositor->focusWindow(windowList.front());
}

void SSwitchManager::switchToUrgentOrNext(bool track) {
    if (track) {
        trackedSwitchList = switchPosition == -1 ? SSwitchManager::getReverseCWindowList() : trackedSwitchList;
        return SSwitchManager::switchTrackedPrev();
    }
    auto windowList = SSwitchManager::getReverseCWindowList();
    if (!g_pCompositor->windowExists(windowList.front())) {
        hyprswitchr_log(LOG, "No urgent or next window found");
        return;
    }
    hyprswitchr_log(LOG, "Focusing urgent or next window: {}", windowList.front()->m_szTitle);
    g_pCompositor->focusWindow(windowList.front());
}

void SSwitchManager::switchToMatchingUrgentOrPrev(std::string regex) {
    CWindow* matchingWindow = g_pCompositor->getWindowByRegex(regex);
    if (!g_pCompositor->windowExists(matchingWindow)) {
        SSwitchManager::switchToUrgentOrPrev();
    }
    hyprswitchr_log(LOG, "Focusing matching window: {}", matchingWindow->m_szTitle);
    g_pCompositor->focusWindow(matchingWindow);
}

void SSwitchManager::switchToMatchingUrgentOrNext(std::string regex) {
    CWindow* matchingWindow = g_pCompositor->getWindowByRegex(regex);
    if (!g_pCompositor->windowExists(matchingWindow)) {
        SSwitchManager::switchToUrgentOrNext();
    }
    hyprswitchr_log(LOG, "Focusing matching window: {}", matchingWindow->m_szTitle);
    g_pCompositor->focusWindow(matchingWindow);
}

void SSwitchManager::resetTrackedSwitch() {
    hyprswitchr_log(LOG, "Ending switch");
    switchPosition = -1;
    trackedSwitchList.clear();
}

// // Return a vector of SWindow objects in descending order of activeTimestamp, and with urgent windows first.
// std::vector<SWindow> SSwitchManager::getWindowList() {
//     hyprswitchr_log(LOG, "Getting window list");
//     std::vector<SWindow> windowList;
//     std::vector<SWindow> urgentWindows;
//     for (auto w : windows) {
//         if (w.urgent) {
//             urgentWindows.push_back(w);
//         } else {
//             windowList.push_back(w);
//         }
//     }
//     std::sort(windowList.begin(), windowList.end(), [](SWindow a, SWindow b) { return a.activeTimestamp > b.activeTimestamp; });
//     std::sort(urgentWindows.begin(), urgentWindows.end(), [](SWindow a, SWindow b) { return a.activeTimestamp < b.activeTimestamp; });
//     windowList.insert(windowList.begin(), urgentWindows.begin(), urgentWindows.end());
//     return windowList;
// }
//
// void SSwitchManager::switchToUrgentOrLast() {
//     SWindow window         = nullptr;
//     window.activeTimestamp = 0;
//     for (auto w : windows) {
//         if (w.urgent) {
//             hyprswitchr_log(LOG, "Focusing urgent window: {}", w.title);
//             window = w;
//             break;
//         }
//         if (window.activeTimestamp < w.activeTimestamp) {
//             window = w;
//         }
//     }
//     hyprswitchr_log(LOG, "Focusing window: {}", window.title);
//     g_pCompositor->focusWindow(window.getWindow());
// }
//
// void SSwitchManager::switchToMatchingOrUrgentOrLast(std::string title) {
//     SWindow window         = nullptr;
//     window.activeTimestamp = 0;
//     for (auto w : windows) {
//         if (w.title.find(title) != std::string::npos) {
//             hyprswitchr_log(LOG, "Focusing matching window: {}", w.title);
//             window = w;
//             break;
//         }
//         if (w.urgent) {
//             hyprswitchr_log(LOG, "Focusing urgent window: {}", w.title);
//             window = w;
//             break;
//         }
//         if (window.activeTimestamp < w.activeTimestamp) {
//             window = w;
//         }
//     }
//     hyprswitchr_log(LOG, "Focusing window: {}", window.title);
//     g_pCompositor->focusWindow(window.getWindow());
// }
