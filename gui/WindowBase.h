#pragma once

class WindowBase
{
private:
    bool disposed = false;

public:
    virtual void render() = 0;
    virtual void cleanUp();
    virtual ~WindowBase();
};
