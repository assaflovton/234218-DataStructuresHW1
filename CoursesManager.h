#ifndef DS_H
#define DS_H
#include "Class.h"
#include "Course.h"
#include "AVL.h"

class Boom {
public:
    AVLTree<Course> courses_tree;
    AVLTree<Class> classes_tree;
    Boom() :courses_tree(AVLTree<Course>()), classes_tree(AVLTree<Class>()) {}
    Boom(const Boom& b) :courses_tree(b.courses_tree), classes_tree(b.classes_tree) {}
};



#endif // DS_H
