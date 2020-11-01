#ifndef PRIORITY_H
#define PRIORITY_H

#include <iostream> // std::cout

// This class encapsulates two values, value_ and priority_, and compares to other Priorities by priority_.
template <class T>
class Priority {

    T value_;
    unsigned int priority_;

public:
    // Default constructor and another constructor.
    Priority()
        : value_()
        , priority_(0)
    {
    }

    Priority(T inValue, uint inPriority)
        : value_(inValue)
        , priority_(inPriority)
    {
    }

    // Copy constructor.
    Priority(const Priority<T>& other)
        : value_(other.value_)
        , priority_(other.priority_)
    {
    }

    // Destructor.
    ~Priority() { }

    // Copy assignment operator.
    Priority& operator=(const Priority<T>& assign)
    {
        // Don't try to assign this to itself.
        if (this != &assign) {
            this->value_ = assign.value_;
            this->priority_ = assign.priority_;
        }
        return *this;
    }

    // Overload ostream operator so we can print out this object.
    friend std::ostream& operator<<(std::ostream& os, const Priority<T>& other)
    {
        os << "{v=" << other.value_ << ", p=" << other.priority_ << "}";
        return os;
    }

    // Getter methods, remember how we preceded variable names with `m` for member.
    T value() { return value_; }
    unsigned int priority() { return priority_; }

    // Overload comparison operators to compare by priority, not value.
    bool operator>(const Priority<T>& other) const { return this->priority_ > other.priority_; }
    bool operator>=(const Priority<T>& other) const { return this->priority_ >= other.priority_; }
    bool operator<(const Priority<T>& other) const { return this->priority_ < other.priority_; }
    bool operator<=(const Priority<T>& other) const { return this->priority_ <= other.priority_; }
    bool operator==(const Priority<T>& other) const { return this->priority_ == other.priority_; }
};

#endif