#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>
#include <thread>

namespace UMSAgent {
template <typename T>
class Singleton {
public:
    static T& getInstance()
    {
        std::lock_guard<std::mutex> _l(sLock);
        T* instance = sInstance;
        if (instance == NULL) {
            instance = new T();
            sInstance = instance;
        }
        return *instance;
    }
    static bool hasInstance()
    {
        std::lock_guard<std::mutex> _l(sLock);
        return sInstance != NULL;
    }

    static void releaseInstance()
    {
        std::lock_guard<std::mutex> _l(sLock);
        if (sInstance) {
            delete sInstance;
            sInstance = NULL;
        }
    }

protected:
    ~Singleton() { }
    Singleton() { }

private:
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
    static std::mutex sLock;
    static T* sInstance;
};

template <typename T>
T* Singleton<T>::sInstance = NULL;
template <typename T>
std::mutex Singleton<T>::sLock;

}

#endif // SINGLETON_H
