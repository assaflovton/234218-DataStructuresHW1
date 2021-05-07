#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <iostream>
template <class T>
class AVLNode {
public:
    AVLNode(const T& value) : data(value), left_node(NULL), right_node(NULL), parent_node(NULL) {}
    ~AVLNode() {}

    const T& getValue() const { return data; }
    void      setLeft(AVLNode* left) { left_node = left; }
    AVLNode* getLeft() const { return left_node; }
    void      setRight(AVLNode* right) { right_node = right; }
    AVLNode* getRight() const { return right_node; }
    void      setParent(AVLNode* parent) { parent_node = parent; }
    AVLNode* getParent() const { return parent_node; }

    void      print() const { std::cout << data; }

    AVLNode();

    T     data;
    AVLNode* left_node;
    AVLNode* right_node;
    AVLNode* parent_node;
};

template <class T>
class AVLTree {
public:
    AVLTree() : root_node(NULL), biggest_node(NULL){}
    ~AVLTree();
    void insert(const T& value);
    void remove(const T& value);
    AVLNode<T>* getRoot() const { return root_node; }
     AVLNode<T>* getBiggestNode() const { return biggest_node; }
     void setBiggestNode(AVLNode<T>* new_big)  { this->biggest_node=new_big ;}
     void updateBiggestNode();
    AVLNode<T>* find(AVLNode<T>* root, const T& value) const;
    int  getHeight(AVLNode<T>* root) const;
    int  balanceFactor(AVLNode<T>* root) const;
    void fixParent(AVLNode<T>* root) ;
    void fixNodeParent(AVLNode<T>* node, const T& value) const;
    AVLNode<T>* rotateLeft(AVLNode<T>* root);
    AVLNode<T>* rotateRight(AVLNode<T>* root);
    int getBiggestNodes(T* arr,int num_to_bring);
    void printInOrder(AVLNode<T>* root) const; // Left, Parent, Right
    void deleteAVLNode(AVLNode<T>* node);
    AVLNode<T>* getBiggestNodeAux(AVLNode<T>* root) ;
    int getBiggestNodesAux(AVLNode<T>* node, T* arr, int num_to_bring, int index)  ;
    void updateBiggestNodeAux(AVLNode<T>* root) ;
    AVLNode<T>* minValueNode(AVLNode<T>* node);
    void insertAVLNode(AVLNode<T>* root, AVLNode<T>* ins);
    AVLNode<T>* deleteNode(AVLNode<T>* root, const T key);
    AVLNode<T>* root_node;
    AVLNode<T>* biggest_node;
    class ALREADY_EXIST{};
    class ALLOC_ERROR{};
    class NOT_EXIST{};
};

template <class T>
AVLTree<T>::~AVLTree() {
    if (root_node ) {
        deleteAVLNode(root_node);
    }
}
//deletes all nodes in postorder
template <class T>
void AVLTree<T>::deleteAVLNode(AVLNode<T>* node) {
    if (node) {
        if(node->getRight()!=nullptr)
        deleteAVLNode(node->getRight());
        if (node->getLeft() != nullptr)
        deleteAVLNode(node->getLeft());
        
        delete node; // Post Order Deletion
    }
}

//insert new node to the tree
template <class T>
void AVLTree<T>::insert(const T& value) {
    AVLNode<T>* new_node = new (std::nothrow) AVLNode<T>(value);

    if (!new_node)
        throw(AVLTree<T>::ALLOC_ERROR()); // Out of memory
    if (!root_node) //
    {
        root_node = new_node;
    }
    else
    {
        if(find(root_node,new_node->getValue()))
            throw(ALREADY_EXIST());
        insertAVLNode(root_node, new_node);
    }
    fixNodeParent(root_node,value);
    updateBiggestNode();
}


//recursive funcion to insert a new node to the tree, then balancing the tree to
//kepp him AVL tree
template <class T>
void AVLTree<T>::insertAVLNode(AVLNode<T>* root, AVLNode<T>* node_to_inseret) {
    // Binary Search Tree insertion algorithm
    if (node_to_inseret->getValue() <= root->getValue()) {
        if (root->getLeft()) // If there is a left child, keep searching
            insertAVLNode(root->getLeft(), node_to_inseret);
        else { // Found the right spot
            if(node_to_inseret==root)
            {
                throw(AVLTree<T>::ALREADY_EXIST());
            }
            root->setLeft(node_to_inseret);
            node_to_inseret->setParent(root);
        }
    }
    else {
        if (root->getRight()) // If there is a right child, keep searching
            insertAVLNode(root->getRight(), node_to_inseret);
        else {// Found the right spot
            if(node_to_inseret==root)
            {
                throw(AVLTree<T>::ALREADY_EXIST());
            }
            root->setRight(node_to_inseret);
            node_to_inseret->setParent(root);
        }
    }

    // after we added the new node we might damaged the varient of the tree, so we must balance it again
    //(the varient is that for each node the balance factor is smaller then 2 or and bigger than -2 -1=<BF<=1)
    int balance = balanceFactor(root);
    if (balance > 1) { //unbalanced from the left
        if (balanceFactor(root->getLeft()) < 0) { //we have to much on the right in the left subtree
            rotateLeft(root->getLeft());//preforming another roatioation to fix balance
        }
        rotateRight(root);
    }
    else if (balance < -1) { //  unbalanced from the right
        if (balanceFactor(root->getRight()) > 0) { // we have to much on the left in the right subtree
            rotateRight(root->getRight()); //preforming another roatioation to fix balance
        }
        rotateLeft(root);
    }
}

//prints the tree inorder(smallest to biggest)
template <class T>
void AVLTree<T>::printInOrder(AVLNode<T>* root) const 
{
    if (root) {
        printInOrder(root->getLeft());  // Left
        root->print();                  // Parent
        printInOrder(root->getRight()); // Right
    }
}

//finding the requested value in the tree, recursive function until we get to the value
//log(n)
template <class T>
AVLNode<T>* AVLTree<T>::find(AVLNode<T>* root, const T& value) const 
{
    fixNodeParent(root,value);
    if (root!=NULL) {
        if (root->getValue() == value)
        {
            return root; // Founds
        }
        else if (value < root->getValue()) {
            return find(root->getLeft(), value);
        }
        else {
            return find(root->getRight(), value);
        }
    }
    return NULL;
}

//fixig the parents of the ndoe that contains the value
template <class T>
void AVLTree<T>::fixNodeParent(AVLNode<T>* root, const T& value) const {
    if (root!=NULL) {
        if (value < root->getValue())
        {
            if(root->getLeft())
            {
                if(root->getLeft()->getValue()==value)
                {
                    root->getLeft()->setParent(root);
                }
            }
            fixNodeParent(root->getLeft(), value);
        }
        else
        {
            if(root->getRight())
            {
                if(root->getRight()->getValue()==value)
                {
                    root->getRight()->setParent(root);
                }
            }
            fixNodeParent(root->getRight(), value);
        }
    }
}

//return the height of some node in the tree
template <class T>
int AVLTree<T>::getHeight(AVLNode<T>* root) const {
    int height = 0;
    if (root!=NULL) {
        int right = getHeight(root->getRight());
        int left = getHeight(root->getLeft()); 
        height = 1 + ((left > right) ? left : right);
    }
    return height;
}

//return's the balance factor of some node in the tree
template <class T>
int  AVLTree<T>::balanceFactor(AVLNode<T>* root) const {
    int balance = 0;
    if (root!=NULL) {
        balance = getHeight(root->getLeft()) - getHeight(root->getRight());
    }
    return balance;
}

//rotating the tree left(as RR rotation) while keeping the balance in the nodes
template <class T>
AVLNode<T>* AVLTree<T>::rotateLeft(AVLNode<T>* root) {
    AVLNode<T>* newroot = root->getRight();
    root->setRight(newroot->getLeft());
    newroot->setLeft(root);
    newroot->setParent(root->getParent());//new line 
    if (root->getRight()){root->getRight()->setParent(root);}//new line
    if (root->getParent() == NULL) {
        root_node = newroot;
        newroot->setParent(NULL);
    }
    else {
        if (root->getParent()->getLeft() == root) {
            root->getParent()->setLeft(newroot);
        }
        else {
            root->getParent()->setRight(newroot);
        }
        newroot->setParent(root->getParent());
    }
    root->setParent(newroot);
    return newroot;
}

//rotating the tree right(as LL rotation) while keeping the tree balanced
template <class T>
AVLNode<T>* AVLTree<T>::rotateRight(AVLNode<T>* root) {
    // preforming the rotation of the nodes
    AVLNode<T>* new_root_after_rotation = root->getLeft();
    root->setLeft(new_root_after_rotation->getRight());
    new_root_after_rotation->setRight(root);
    new_root_after_rotation->setParent(root->getParent());//new line
    if (root->getLeft()){root->getLeft()->setParent(root);}//new line
    // orginanizing the tree
    if (root->getParent() == NULL) {
        root_node = new_root_after_rotation;
        new_root_after_rotation->setParent(NULL);
    }
    else {
        if (root->getParent()->getLeft() == root) {
            root->getParent()->setLeft(new_root_after_rotation);
        }
        else {
            root->getParent()->setRight(new_root_after_rotation);
        }
        new_root_after_rotation->setParent(root->getParent());
    }

    root->setParent(new_root_after_rotation);
    return new_root_after_rotation;
}

//sets the first node parent to be NULL
template <class T>
void AVLTree<T>::fixParent(AVLNode<T>* root) {
    if (root!=NULL) {
        if(root->getParent())
        {
            if(root->getParent()->getValue()==root->getValue())
            {
                root->setParent(NULL);
            }
        }
    }
}

//remove's node that contain value from the tree, while keeping the tree balanced
//o(log(n))
template <class T>
void AVLTree<T>::remove(const T& value) {
    fixParent(root_node);
    fixNodeParent(root_node,value);
    AVLNode<T>* node=find(root_node,value);
    if(node==NULL)
    {
        throw(AVLTree<T>::NOT_EXIST());
    }
    if(!deleteNode(root_node, value))
        root_node=NULL;
    updateBiggestNode();
}

//return's the smallest value node in a certain subTree
template <class T>
AVLNode<T>* AVLTree<T>::minValueNode(AVLNode<T>* node)
{
    AVLNode<T>* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left_node != NULL)
        current = current->left_node;

    return current;
}

//recursive function to delete a node from the tree
template <class T>
AVLNode<T>* AVLTree<T>::deleteNode(AVLNode<T>* root, const T key)
{
    if (root == NULL)
        return root;
    if (key < root->data) //key is smaller, then search left
        root->left_node = deleteNode(root->left_node, key);
    else if (key > root->data) //key is bigger, then search right
        root->right_node = deleteNode(root->right_node, key);
    else
    {
        // split to options only child, only leftson or only rightson
        if ((root->right_node == NULL)||(root->left_node == NULL))
        {
            AVLNode<T>* node_temp = root->left_node;//if there is no left son the temp will be null 
            if (node_temp == NULL)
            {
                node_temp = root->right_node;//if the there is no right son the temp will be null therefore there will be no sons
            }
            // got here there are no sons
            if (node_temp == NULL)
            {
                node_temp = root;
                root = NULL;
            }
            else { //one of the sons was not null therefore we change the corrent node to be the son and delete the temp
                AVLNode<T>* temp2 = root->getParent();//new line
                *root = *node_temp;
                root->setParent(temp2);//new line
            }
            free(node_temp);
        }
        else
        {
            // the corrent node has two sons, we will replace him, so we take only the data from the minimal value from the right sub tree  
            AVLNode<T>* temp = minValueNode(root->right_node);
            root->data = temp->data;
            root->right_node = deleteNode(root->right_node,temp->data);// remove the node we took the data from   
        }
    }
    //only one node in the tree, therefore we need to remove it and finish
    if (root == NULL)
    {
        return NULL;
    }
    //balance algorythem for AVL
    int balance = balanceFactor(root);
    if ( balanceFactor(root->right_node) > 0 && balance < -1)//need to preform right the left  rotation on the tree
    {
        root->right_node = rotateRight(root->right_node);
        return rotateLeft(root);  
    }
    if ( balanceFactor(root->left_node) >= 0 && balance > 1 ) //need to preform right rotation on the tree
    {
        return rotateRight(root); 
    }
    if (  balanceFactor(root->right_node) <= 0 && balance < -1) //need to preform left rotation on the tree
    {
        return rotateLeft(root);  
    }
    if (  balanceFactor(root->left_node) < 0 && balance > 1)//need to preform left then right rotation on the tree
    {
        root->left_node = rotateLeft(root->left_node);
        return rotateRight(root);  
    }
    return root;
}

//updating the biggest_node going right as u can since it an avl tree
template <class T>
void AVLTree<T>::updateBiggestNode()
{
    if(root_node==NULL )//the tree has no nodes therefore we wont add anything
    {
        setBiggestNode(NULL);
        return;
    } 
    updateBiggestNodeAux(root_node);
}

template <class T>
void AVLTree<T>::updateBiggestNodeAux(AVLNode<T>* root) 
{
    if(root==NULL)
        setBiggestNode(NULL);
    setBiggestNode(getBiggestNodeAux(root));
}


//return the biggestNode for certain subTree
template <class T>
AVLNode<T>* AVLTree<T>::getBiggestNodeAux(AVLNode<T>* root)
{
    if(root==NULL)
        return NULL;
    if(root->getRight()==NULL)
        return root;
    return getBiggestNodeAux(root->getRight());
}


//return's the number of values he found, at max return's num_to_bring
template <class T>
int AVLTree<T>::getBiggestNodes(T* arr,int num_to_bring)
{
    fixParent(root_node);
    fixNodeParent(root_node,biggest_node->getValue());
    if(arr==NULL)
        return -1;
    if(root_node==NULL)
        return 0;
    return getBiggestNodesAux(biggest_node,arr,num_to_bring,0);
}


//we saved the biggest node so we could do this function at O(m) as m is num_to_bring
//each step we put the value in the arr then we have 2 option's:
//1. im right son: so go to my biggest left grandSon first, and then to my Parent
//2. im left son: so go to my biggest left grandSon.
template <class T>
 int AVLTree<T>::getBiggestNodesAux(AVLNode<T>* node, T* arr, int num_to_bring, int index)
 {
     if(node==NULL || index==num_to_bring)
     {
         return 0;
     }
     arr[index]=node->getValue();
     if(node->getParent()==NULL)
     {
         AVLNode<T>* new_big=getBiggestNodeAux(node->getLeft());
         return 1+getBiggestNodesAux(new_big,arr,num_to_bring,index+1);
     }
     if(node->getParent()->getRight())
     {
         if(node->getParent()->getRight()->getValue()==node->getValue())//im right son
        {
            int leftSum= 1+getBiggestNodesAux(getBiggestNodeAux(node->getLeft()),arr,num_to_bring,index+1);
            return leftSum+getBiggestNodesAux(node->getParent(),arr,num_to_bring,index+leftSum);
        }
     }
     //im left son
    if(node->getLeft())
    {
        return 1+getBiggestNodesAux(getBiggestNodeAux(node->getLeft()),arr,num_to_bring,index+1);
    }
    return 1;

 }
#endif // AVL_TREE_H_
