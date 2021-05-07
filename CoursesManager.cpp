#include "library.h"
#include  "CoursesManager.h"
void* Init()
{
    Boom* DSnew = new Boom();
    return (void*)DSnew;
}
//add the course to the courses tree, still not adding the classes to the classes tree
//log(n)- number of courses in the tree (according to insert in AVL tree's)
StatusType AddCourse(void* DS, int courseID, int numOfClasses)
{
    if (DS == NULL || courseID <= 0 || numOfClasses <= 0)
    {
        return INVALID_INPUT;
    }
    try
    {
        Course new_course = Course(courseID, numOfClasses);
        (((Boom*)DS)->courses_tree).insert(new_course); //log(number of courses)
    }
    catch (AVLTree<Course>::ALLOC_ERROR())
    {
        return ALLOCATION_ERROR;
    }
    catch (Course::ALLOC_ERROR())
    {
        return ALLOCATION_ERROR;
    }
    catch (AVLTree<Course>::ALREADY_EXIST())
    {
        return FAILURE;
    }
    catch (...)
    {
        return FAILURE;
    }
    return SUCCESS;
}
//removing the course from the courses tree and all of his classes from the classes tree
//there are m classes and each class remove is log(M) so the final comlexity is m*log(M) as wanted
StatusType RemoveCourse(void* DS, int courseID)
{
    if (DS == NULL || courseID <= 0)
    {
        return INVALID_INPUT;
    }
    try
    {
        Course temp_course = Course(courseID);
        AVLNode<Course>* tmpNode = ((((Boom*)DS)->courses_tree).find((((Boom*)DS)->courses_tree).getRoot(), temp_course));
        if (tmpNode == NULL)
            return FAILURE;
        else if (tmpNode)
        {
            temp_course = tmpNode->getValue();
            (((Boom*)DS)->courses_tree).remove(temp_course);
            int x = temp_course.num_of_classes;//new line
            Class* arr = temp_course.classes_array;//new line
            for (int i = 0; i < x; i++)                                 //(num_of_classes_deleted)*log(num of classes in DS)
            {
                Class delete_class(arr[i]);
                AVLNode<Class>* tmpcl = (((Boom*)DS)->classes_tree).find(((((Boom*)DS)->classes_tree).getRoot()), delete_class);
                if (tmpcl)
                {
                    (((Boom*)DS)->classes_tree).remove(tmpcl->getValue());
                }
            }
        }
    }
    catch (Course::ALLOC_ERROR())
    {
        return ALLOCATION_ERROR;
    }
    catch (AVLTree<Course>::NOT_EXIST())
    {

        return FAILURE;
    }
    catch (AVLTree<Class>::NOT_EXIST())
    {
        return FAILURE;
    }
    catch (...)
    {
        return FAILURE;
    }
    return SUCCESS;
}
//if the class has already been watchen so removing the class from the classes tree and adding again with 
// the updated time, else just adding the class to the classes tree with time_watched=time
//complexity: at worstCase  (log(M) + log(n)) 
StatusType WatchClass(void* DS, int courseID, int classID, int time)
{
    if (time <= 0 || DS == NULL || courseID <= 0 || classID < 0)
    {
        return INVALID_INPUT;
    }
    try
    {
        Course temp_course(courseID);
        AVLNode<Course>* tmpNode = ((((Boom*)DS)->courses_tree).find((((Boom*)DS)->courses_tree).getRoot(), temp_course));
        if (tmpNode == NULL)
            return FAILURE;//not_exist
        Course find_course = tmpNode->getValue();
        if (find_course.num_of_classes < classID + 1)
            return INVALID_INPUT;
        Class temp_class = tmpNode->getValue().classes_array[classID];//new line
        AVLNode<Class>* tmp = (((Boom*)DS)->classes_tree).find((((Boom*)DS)->classes_tree).getRoot(), temp_class);
        if (tmp)
        {
            Class c = tmp->getValue();
            (((Boom*)DS)->classes_tree).remove(c);
            int new_time = c.time_watched + time;
            (((Boom*)DS)->classes_tree).insert(Class(classID, new_time, courseID));//log(number of courses)
            tmpNode->getValue().classes_array[classID].time_watched = new_time;
        }
        else
        {
            Class temp_class(classID, time, courseID);
            (((Boom*)DS)->classes_tree).insert(temp_class);
            tmpNode->getValue().classes_array[classID].time_watched = time;
        }
    }
    catch (AVLTree<Course>::ALLOC_ERROR())
    {
        return ALLOCATION_ERROR;
    }
    catch (Course::ALLOC_ERROR())
    {
        return ALLOCATION_ERROR;
    }
    catch (AVLTree<Course>::NOT_EXIST())
    {
        return FAILURE;
    }
    catch (...)
    {
        return FAILURE;
    }
    return SUCCESS;
}
//serching for the course in the courses tree ,then get's his time_watched for the class id aked for
//complexity: log(n) n= number of courses
StatusType TimeViewed(void* DS, int courseID, int classID, int* timeViewed)
{
    if (((Boom*)DS) == NULL || courseID <= 0 || classID < 0)
    {
        return INVALID_INPUT;
    }
    try
    {
        Course temp_course(courseID);
        AVLNode<Course>* tmpcr = (((Boom*)DS)->courses_tree).find((((Boom*)DS)->courses_tree).getRoot(), temp_course);
        if (tmpcr == NULL)
        {
            return FAILURE;//not exist in both
        }
        if ((tmpcr->getValue().num_of_classes) < classID + 1) { return INVALID_INPUT; }//new line    
        *timeViewed = tmpcr->getValue().classes_array[classID].time_watched;
        return SUCCESS;
    }
    catch (AVLTree<Course>::ALLOC_ERROR())
    {
        return ALLOCATION_ERROR;
    }
    catch (Course::ALLOC_ERROR())
    {
        return ALLOCATION_ERROR;
    }
    catch (AVLTree<Course>::NOT_EXIST())
    {
        return FAILURE;
    }
    catch (...)
    {
        return FAILURE;
    }
    return SUCCESS;
}
//return's true if the class alrady in the array, else false
bool AlreadyInArray(int* coursesArr, int* classesArr, int size, int courseId, int classId)
{
    for (int i = 0; i < size; i++)
    {
        if (coursesArr[i] == courseId && classesArr[i] == classId)
            return true;
    }
    return false;
}
//first, gets all the "biggest" classes in the classes tree, if there are numOfClasses like that so 
//put them in the array accordingly and end. else, gets the "leftovers" the classes with 0 time watched
// according to the courses id.
//numberOfNotZeroTime= the number of classes that we brought from the classes tree
//numberOfCourses=number of courses we brought from the courses tree
//complexity- o(m) as m is the number of classes we put in the array
StatusType GetMostViewedClasses(void* DS, int numOfClasses, int* courses, int* classes)
{
    if (((Boom*)DS) == NULL || numOfClasses <= 0)
    {
        return INVALID_INPUT;
    }
    Course* courses_array = new Course[numOfClasses];
    if (courses_array == NULL)
    {
        return ALLOCATION_ERROR;
    }
    Class* classes_array = new Class[numOfClasses];
    if (classes_array == NULL)
    {
        return ALLOCATION_ERROR;
    }
    for (int i = 0; i < numOfClasses; i++) { classes[i] = -1; }
    bool stop = false;
    int index = 0;
    int numberOfNotZeroTime = (((Boom*)DS)->classes_tree).getBiggestNodes(classes_array, numOfClasses);
    for (; index < numberOfNotZeroTime; index++)
    {
        courses[index] = (classes_array[index]).course_id;
        classes[index] = (classes_array[index]).class_id;
    }
    if (numberOfNotZeroTime != numOfClasses)
    {
        int numberOfCourses = (((Boom*)DS)->courses_tree).getBiggestNodes(courses_array, numOfClasses - numberOfNotZeroTime);
        for (int i = 0; (i < numberOfCourses) && !stop; i++)
        {
            for (int j = 0; (j < courses_array[i].num_of_classes); j++)
            {

                if (AlreadyInArray(courses, classes, numOfClasses,
                    courses_array[i].classes_array[j].course_id, courses_array[i].classes_array[j].class_id))
                    continue;//not puting twice
                classes[index] = courses_array[i].classes_array[j].class_id;
                courses[index] = courses_array[i].classes_array[j].course_id;
                //std::cout<<courses[index];
                index++;
                if (index == numOfClasses)
                {
                    stop = true;
                    break;
                }
            }
        }
        if (index < numOfClasses)
        {
            return FAILURE;
        }
    }
    delete[] courses_array;
    delete[] classes_array;
    return SUCCESS;
}
//run's through all the tree's and deleting every node
//complexity: o(n+m) as n is the of courses and m is the nnumber of classes 
void Quit(void** DS)
{
    (((Boom*)(*DS))->courses_tree).deleteAVLNode((((Boom*)(*DS))->courses_tree).getRoot()); //(num of courses in DS)
    (((Boom*)(*DS))->classes_tree).deleteAVLNode((((Boom*)(*DS))->classes_tree).getRoot()); //(num of classes in DS)
    *(DS) = NULL;
    (DS) = NULL;
}
//print's the current classes in order(from smallest to biggest)
void Print(void* DS)
{
    (((Boom*)DS)->courses_tree).printInOrder((((Boom*)DS)->courses_tree).getRoot());
    std::cout << "\n";
}
