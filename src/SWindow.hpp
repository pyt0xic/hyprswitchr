#include "Globals.hpp"
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/desktop/Window.hpp>
#include <hyprland/src/managers/AnimationManager.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
using namespace std::chrono;

struct SWindow {
    // private:
    //   CWindow* window;

  public:
    SWindow() : title(std::move("")), initialTitle(std::move("")), initialClass(std::move("")), pid(0), activeTimestamp(0), active(false), urgent(false) {}
    SWindow(CWindow* window) :
        title(window->m_szTitle), initialTitle(window->m_szInitialTitle), initialClass(window->m_szInitialClass), pid(window->getPID()), activeTimestamp(0), active(false),
        urgent(window->m_bIsUrgent) {}
    std::string title;
    std::string initialTitle;
    std::string initialClass;
    int         pid;
    uint64_t    activeTimestamp;
    bool        active;
    bool        urgent;

    // SWindow() : title("") {}

    // CWindow*    getWindow() {
    //     return nullptr;
    // };
    void updateWindow(CWindow* newWindow) {
        // this->window = newWindow;
        title  = newWindow->m_szTitle;
        urgent = newWindow->m_bIsUrgent;
    };
    void focus() {
        active          = true;
        activeTimestamp = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        urgent          = false;
    };
    void unfocus() {
        active          = false;
        activeTimestamp = 0;
    };
};
