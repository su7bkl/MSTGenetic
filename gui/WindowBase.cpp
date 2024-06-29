#include "WindowBase.h"

void WindowBase::cleanUp()
{
    if (this->disposed)
        return;

    this->disposed = true;
}

WindowBase::~WindowBase()
{
    this->cleanUp();
}
