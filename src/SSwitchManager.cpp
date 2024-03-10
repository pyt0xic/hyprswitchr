#include "SSwitchManager.hpp"
#include "Log.hpp"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/debug/Log.hpp>
#include <vector>

// Return a vector of SWindow objects in descending order of activeTimestamp, and with urgent windows first.
std::vector<SWindow> SSwitchManager::getWindowList() {
    hyprswitchr_log(LOG, "Getting window list");
    std::vector<CWindow*> focusHistory = g_pCompositor->m_vWindowFocusHistory;
    hyprswitchr_log(LOG, "Got focus history");
    CWindow* urgentWindow = g_pCompositor->getUrgentWindow();
    hyprswitchr_log(LOG, "Got urgent window");
    SWindow              urgent;
    SWindow              active;
    std::vector<SWindow> windowList = {};
    hyprswitchr_log(LOG, "Iterating through focus history");
    // Iterate through the focus history and add the windows to the list, if the window is active, add it last.
    for (auto w : focusHistory) {
        if (!g_pCompositor->windowExists(w)) {
            hyprswitchr_log(LOG, "Window does not exist");
            continue;
        }
        if (g_pCompositor->isWindowActive(w)) {
            hyprswitchr_log(LOG, "Skipping active window: {}", w->m_szTitle);
            active        = SWindow(w);
            active.active = true;
        } else {
            hyprswitchr_log(LOG, "Adding window: {}", w->m_szTitle);
            SWindow window = SWindow(w);
            windowList.push_back(window);
        }
    }
    hyprswitchr_log(LOG, "Got through focus history");
    // Add the urgent window to the front of the list.
    if (g_pCompositor->windowExists(urgentWindow) && urgentWindow->m_bIsUrgent) {
        hyprswitchr_log(LOG, "Adding urgent window: {}", urgentWindow->m_szTitle);
        urgent = SWindow(urgentWindow);
        // urgent.title        = urgentWindow->m_szTitle;
        // urgent.initialTitle = urgentWindow->m_szInitialTitle;
        // urgent.initialClass = urgentWindow->m_szInitialClass;
        // urgent.pid          = urgentWindow->getPID();
        // urgent.urgent       = true;
        windowList.insert(windowList.begin(), urgent);
    }
    hyprswitchr_log(LOG, "Got through urgent window");
    if (active.active) {
        hyprswitchr_log(LOG, "Adding active window: {}", active.title);
        windowList.push_back(active);
    }
    hyprswitchr_log(LOG, "Got through active window");
    return windowList;
}

void SSwitchManager::switchToUrgentOrLast() {
    CWindow* urgentWindow = g_pCompositor->getUrgentWindow();
    if (g_pCompositor->windowExists(urgentWindow) && urgentWindow->m_bIsUrgent) {
        hyprswitchr_log(LOG, "Focusing urgent window: {}", urgentWindow->m_szTitle);
        g_pCompositor->focusWindow(urgentWindow);
    }
    std::vector<CWindow*> focusHistory = g_pCompositor->m_vWindowFocusHistory;
    CWindow*              lastWindow;
    for (auto w : focusHistory) {
        if (w == urgentWindow) {
            continue;
        }
        if (g_pCompositor->isWindowActive(w)) {
            continue;
        }
        lastWindow = w;
        break;
    }
    if (g_pCompositor->windowExists(lastWindow)) {
        hyprswitchr_log(LOG, "Focusing last window: {}", lastWindow->m_szTitle);
        g_pCompositor->focusWindow(lastWindow);
        return;
    }
    hyprswitchr_log(LOG, "No urgent or last window found");
}

void SSwitchManager::switchToMatchingOrUrgentOrLast(std::string regex) {
    CWindow* matchingWindow = g_pCompositor->getWindowByRegex(regex);
    if (g_pCompositor->windowExists(matchingWindow)) {
        hyprswitchr_log(LOG, "Focusing matching window: {}", matchingWindow->m_szTitle);
        g_pCompositor->focusWindow(matchingWindow);
    }
    switchToUrgentOrLast();
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

void SSwitchManager::addWindow(CWindow* window) {
    hyprswitchr_log(LOG, "Adding window: {}", window->m_szTitle);
    SWindow w = SWindow(window);
    // w.title        = window->m_szTitle;
    // w.initialTitle = window->m_szInitialTitle;
    // w.initialClass = window->m_szInitialClass;
    // w.pid          = window->getPID();
    // w.urgent       = window->m_bIsUrgent;
    // windows.push_back(w);
}

void SSwitchManager::removeWindow(CWindow* window) {
    hyprswitchr_log(LOG, "Removing window: {}", window->m_szTitle);
    // windows.remove_if([window](SWindow w) { return w.initialClass == window->m_szInitialClass && w.initialTitle == window->m_szInitialTitle && w.pid == window->getPID(); });
}

void SSwitchManager::updateWindow(CWindow* window) {
    hyprswitchr_log(LOG, "Updating window: {}", window->m_szTitle);
    // bool found = false;
    // for (auto& w : windows) {
    //     if (w.initialClass == window->m_szInitialClass && w.initialTitle == window->m_szInitialTitle && w.pid == window->getPID()) {
    //         w.updateWindow(window);
    //         found = true;
    //         return;
    //     }
    // }
    // if (!found) {
    //     hyprswitchr_log(LOG, "Window not found, adding it");
    //     addWindow(window);
    // }
}

void SSwitchManager::setActiveWindow(CWindow* window) {
    // bool unfocused = false;
    // bool focused   = false;
    hyprswitchr_log(LOG, "Setting active window: {}", window->m_szTitle);
    // for (auto& w : windows) {
    //     if (w.active) {
    //         hyprswitchr_log(LOG, "Unfocusing previous window: {}", w.title);
    //         w.unfocus();
    //         unfocused = true;
    //     }
    //     if (w.initialClass == window->m_szInitialClass && w.initialTitle == window->m_szInitialTitle && w.pid == window->getPID()) {
    //         hyprswitchr_log(LOG, "Focusing window: {}", w.title);
    //         w.focus();
    //         focused = true;
    //     }
    //     if (unfocused && focused) {
    //         return;
    //     }
    // }
    // if (!focused) {
    //     hyprswitchr_log(LOG, "Window not found, adding it");
    //     addWindow(window);
    //     setActiveWindow(window);
    // }
}
