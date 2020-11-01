#ifndef PRIORITY_H
#define PRIORITY_H

#include <iostream> // std::cout

// This class encapsulates two values, mValue and mPriority, and compares to other Priorities by mPriority.
template <class T>
class Priority {

    // Preceded member variables with `m` so that we can use methods with i.e. `value`, `priority`.
    T mValue;
    unsigned int mPriority;

public:
    // Default constructor and another constructor.
    Priority()
        : mValue()
        , mPriority(0)
    {
    }

    Priority(T inValue, uint inPriority)
        : mValue(inValue)
        , mPriority(inPriority)
    {
    }

    // Copy constructor.
    Priority(const Priority<T>& other)
        : mValue(other.mValue)
        , mPriority(other.mPriority)
    {
    }

    // Destructor.
    ~Priority() { }

    // Copy assignment operator.
    Priority& operator=(const Priority<T>& assign)
    {
        // Don't try to assign this to itself.
        if (this != &assign) {
            this->mValue = assign.mValue;
            this->mPriority = assign.mPriority;
        }
        return *this;
    }

    // Overload ostream operator so we can print out this object.
    friend std::ostream& operator<<(std::ostream& os, const Priority<T>& other)
    {
        os << "{v=" << other.mValue << ", p=" << other.mPriority << "}";
        return os;
    }

    // Getter methods, remember how we preceded variable names with `m` for member.
    T value() { return mValue; }
    unsigned int priority() { return mPriority; }

    // Overload comparison operators to compare by priority, not value.
    bool operator>(const Priority<T>& other) const { return this->mPriority > other.mPriority; }
    bool operator>=(const Priority<T>& other) const { return this->mPriority >= other.mPriority; }
    bool operator<(const Priority<T>& other) const { return this->mPriority < other.mPriority; }
    bool operator<=(const Priority<T>& other) const { return this->mPriority <= other.mPriority; }
    bool operator==(const Priority<T>& other) const { return this->mPriority == other.mPriority; }
};

#endif