#ifndef AVL_CLASS_H_
#define AVL_CLASS_H_
#include <iostream>
class Class {
public:
    int class_id;
    int time_watched;
    int course_id;
    Class(int class_id_, int time_watched_, int course_id_)
    {
        class_id = class_id_;
        time_watched = time_watched_;
        course_id = course_id_;
    }
    Class() :class_id(0), time_watched(0), course_id(0) {}
    ~Class() = default;
    Class(const Class& c)
    {
        class_id = c.class_id;
        time_watched = c.time_watched;
        course_id = c.course_id;
    }

    Class& operator=(const Class& c)
    {
        if (this != &c)
        {
            this->course_id = c.course_id;
            this->class_id = c.class_id;
            this->time_watched = c.time_watched;
        }
        return *this;

    }
    bool operator== (const Class& c) const
    {
        return (this->class_id == c.class_id) && (this->course_id == c.course_id);
    }
    bool operator!= (const Class& c) const
    {
        return !((*this) == c);
    }
    bool operator<= (const Class& c) const
    {
        if (this->time_watched != c.time_watched)
        {
            return this->time_watched <= c.time_watched;
        }
        if (this->course_id != c.course_id)
        {
            return this->course_id >= c.course_id;
        }
        return this->class_id >= c.class_id;
    }

    bool operator>= (const Class& c) const
    {
        return c <= (*this);
    }

    bool operator< (const Class& c) const
    {
        if (this->time_watched != c.time_watched)
        {
            return this->time_watched < c.time_watched;
        }
        if (this->course_id != c.course_id)
        {
            return this->course_id > c.course_id;
        }
        return this->class_id > c.class_id;
    }
    bool operator> (const Class& c) const
    {
        return c < (*this);
    }
    friend std::ostream& operator<<(std::ostream& os, const Class& c)
    {
        os << c.course_id << "," << c.class_id << "," << c.time_watched << "\n";
        return os;
    }
};



#endif //AVL_CLASS_H_