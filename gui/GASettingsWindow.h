#pragma once
#include "gui/BaseWindow.h"

namespace gui {
    class GASettingsWindow : public BaseWindow
    {
    public:
        GASettingsWindow();
        void render() override;
    };
}
