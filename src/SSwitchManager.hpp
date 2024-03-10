#include "Log.hpp"
#include "SWindow.hpp"

class SSwitchManager {
  private:
    int                   switchPosition    = -1;
    std::vector<CWindow*> trackedSwitchList = {};
    void                  switchTrackedPrev();
    void                  switchTrackedNext();
    std::vector<CWindow*> getCWindowList();
    std::vector<CWindow*> getReverseCWindowList();

  public:
    std::vector<SWindow> getWindowList();
    std::vector<SWindow> getReverseWindowList();
    void                 switchToUrgentOrPrev(bool track = false);
    void                 switchToUrgentOrNext(bool track = false);
    void                 resetTrackedSwitch();
    void                 switchToMatchingUrgentOrPrev(std::string title);
    void                 switchToMatchingUrgentOrNext(std::string title);
};
