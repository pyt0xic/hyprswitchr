#include "SWindow.hpp"

class SSwitchManager {
  private:
    std::vector<CWindow*> getCWindowList();
    std::vector<CWindow*> getReverseCWindowList();

  public:
    std::vector<SWindow> getWindowList();
    std::vector<SWindow> getReverseWindowList();
    void                 switchToUrgentOrPrevious();
    void                 switchToUrgentOrNext();
    void                 switchToMatchingUrgentOrPrev(std::string title);
    void                 switchToMatchingUrgentOrNext(std::string title);
};
