fndef COMPARABLE_H
#define COMPARABLE_H

// An object that compares by a double and not by its data.
template <typename T>
class Comparable {
    double comp_;
    T data_;
public:
    Comparable() : comp_(0), data_() {}
    Comparable(const double& c, const T& d) : comp_(c), data_(d) {}

    inline bool operator>(const Comparable& oth) const { return comp_ > oth.comp_; }
    inline bool operator<(const Comparable& oth) const { return comp_ < oth.comp_; }
    inline bool operator>=(const Comparable& oth) const { return comp_ >= oth.comp_; }
    inline bool operator<=(const Comparable& oth) const { return comp_ <= oth.comp_; }
    inline bool operator==(const Comparable& oth) const { return comp_ == oth.comp_; }
    inline bool operator!=(const Comparable& oth) const { return not (comp_ == oth.comp_); }

    inline double value() const { return comp_; }
    inline T data() const { return data_; }
};

#endif /* COMPARABLE_H */
