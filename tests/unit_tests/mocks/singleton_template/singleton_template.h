#pragma once

#include <stdexcept>

template <typename T>
class SingletonTemplate
{
   public:
    SingletonTemplate()
    {
        instance = static_cast<T*>(this);
    }

    static T& getInstance()
    {
        if (instance == nullptr)
        {
            throw std::logic_error{
                "Null " + std::string(typeid(*instance).name()) + "instance "};
        }

        return *instance;
    }

    SingletonTemplate(const SingletonTemplate&) = delete;
    SingletonTemplate(SingletonTemplate&&) = delete;
    SingletonTemplate& operator=(const SingletonTemplate&) = delete;
    SingletonTemplate& operator=(SingletonTemplate&&) = delete;

   private:
    static T* instance;
};

template <typename T>
T* SingletonTemplate<T>::instance{nullptr};
