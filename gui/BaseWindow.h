#pragma once

class BaseWindow
{
private:
    bool disposed = false;

    virtual void cleanUpLogic() {};

public:
    virtual void render() = 0;
    void cleanUp();
    virtual ~BaseWindow();
};
