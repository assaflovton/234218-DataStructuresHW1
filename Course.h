#ifndef AVL_COURSE_H_
#define AVL_COURSE_H_
#include "Class.h"

class Course
{
public:
    int course_id;
    int num_of_classes;
    Class* classes_array;
    class ALLOC_ERROR {};

    Course(int course_id, int num_of_classes = 0) : course_id(course_id), num_of_classes(num_of_classes)
    {
        classes_array = new Class[num_of_classes];
        if (classes_array == NULL)
            throw(ALLOC_ERROR());
        for (int i = 0; i < num_of_classes; i++)
        {
            classes_array[i] = Class(i, 0, course_id);
        }
    }
    Course() :course_id(0), num_of_classes(0), classes_array(NULL) {}

    Course(const Course& c) :course_id(c.course_id), num_of_classes(c.num_of_classes) {
        if (num_of_classes != 0)
        {
            classes_array = new Class[c.num_of_classes];
            for (int i = 0; i < num_of_classes; i++)
            {
                classes_array[i] = c.classes_array[i];
            }
        }
    }
    ~Course() {}

    Course& operator=(const Course& c)
    {
        if (this != &c)
        {
            this->course_id = c.course_id;
            this->num_of_classes = c.num_of_classes;
            delete[] classes_array;
            classes_array = new Class[c.num_of_classes];
            for (int i = 0; i < num_of_classes; i++)
            {
                classes_array[i] = c.classes_array[i];
            }
        }
        return *this;

    }
    bool operator==(const Course& c) const
    {
        return (this->course_id == c.course_id);
    }
    bool operator!=(const Course& c) const
    {
        return !((*this) == c);
    }
    bool operator<=(const Course& c) const
    {
        return (this->course_id) >= c.course_id;
    }

    bool operator>=(const Course& c) const
    {
        return c <= (*this);
    }

    bool operator<(const Course& c) const
    {
        return (this->course_id) > c.course_id;
    }
    bool operator>(const Course& c) const
    {
        return c < (*this);
    }
    friend std::ostream& operator<<(std::ostream& os, const Course& c)
    {
        for (int i = c.num_of_classes - 1; i >= 0; i--)
        {
            std::cout << c.classes_array[i];
        }
        return os;
    }
};

#endif //AVL_COURSE_H_