#include "SWindow.hpp"

class SSwitchManager {
  private:
    std::list<SWindow> windows;

  public:
    std::vector<SWindow> getWindowList();
    void                 switchToUrgentOrLast();
    void                 switchToMatchingOrUrgentOrLast(std::string title);

    void                 addWindow(CWindow* window);
    void                 removeWindow(CWindow* window);
    void                 updateWindow(CWindow* window);
    void                 setActiveWindow(CWindow* window);
};
