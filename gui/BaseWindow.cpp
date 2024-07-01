#include "gui/BaseWindow.h"

namespace gui {
    void BaseWindow::cleanUp()
    {
        if (this->disposed)
            return;

        this->cleanUpLogic();

        this->disposed = true;
    }

    BaseWindow::~BaseWindow()
    {
        this->cleanUp();
    }
}
