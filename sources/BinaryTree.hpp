#pragma once
#include <stack>
#include <unordered_set>
/* Lets Assume Graph G is:

         1
      2__|__3
   4__|__5
1==Root|| 2 == Left Child || 3 == Right Child ||
Parent == Node Above Child
Depth First Traversals: 
Inorder (Left, Root, Right) : 4 2 5 1 3 
Preorder (Root, Left, Right) : 1 2 4 5 3 
Postorder (Left, Right, Root) : 4 5 2 3 1
*/

namespace ariel
{

    template <typename T> // BinaryTree for Any Type -- Templete
    class BinaryTree
    {
    private:
        // Basic Node Struct - ( Data || Left|| Right)
        struct Node
        {
            T data;      // Value
            Node *left;  // Point to left
            Node *right; // Point to right
            Node(T &val) : data(val), right(nullptr), left(nullptr) {}
        };
        Node *root;
        // Private Funtion - Iterate and Delete for Destructor
        void removeTree()
        {
            if (!root)
            {
                for (auto it = begin_inorder(); it != end_inorder(); ++it)
                {
                    Node *tempNode = it.getNode();
                    delete tempNode;
                }
            }
        }
        // Recieve a Node and Iterate Tree to Find it
        // If Found -- Return Specific Node
        Node *searchforNode(T nodedata)
        {
            if (root != nullptr)
            {
                for (auto it = begin_inorder(); it != end_inorder(); ++it)
                {
                    if (*it == nodedata)
                    {
                        return it.getNode();
                    }
                }
            }
            return nullptr;
        }
        // Argument For Func - 2 Nodes - Struct Ubove
        // First We Shall Take Care of one Side Completly and Move to the Other Recursive
        // Check if The Other Node Point Right and Exists -- If so, Add Correctly
        // Finish - Start With Left Side
        void nodeCopy(Node *node, Node *tempBT)
        {
            if (tempBT->left != nullptr)
            {
                node->left = new Node(tempBT->left->data);
                nodeCopy(node->left, tempBT->left);
            }
            if (tempBT->right != nullptr)
            {
                node->right = new Node(tempBT->right->data);
                nodeCopy(node->right, tempBT->right);
            }
        }
        // Now that We have to Above Function - All I need is to begin with our Current Root
        // Then Check and Add vs Argument Tree
        void treeCopy(const BinaryTree &tempBT)
        {
            if (tempBT.root != nullptr)
            {
                root = new Node(tempBT.root->data);
                nodeCopy(root, tempBT.root);
            }
        }

    public:
        // Empty Tree -- Empty Root
        BinaryTree()
        {
            root = nullptr;
        }
        // Destructor
        ~BinaryTree()
        {
            removeTree();
        }
        // As Argument A Refrence to Tree and Copy(DEEP)
        BinaryTree(const BinaryTree &tempBT)
        {
            removeTree();
            treeCopy(tempBT);
        }
        // As Argument A Refrence to Tree and Copy(Shallow)
        BinaryTree(BinaryTree &&tempBT) noexcept
        {
            root = tempBT.root;
            tempBT.root = nullptr;
        }
        //Operator (=): Erase the mem of the previous Tree & Deep Copy Build the new Tree
        BinaryTree &operator=(const BinaryTree &tempBT)
        {
            if (!(this == &tempBT))
            {
                removeTree();
                treeCopy(tempBT);
            }
            return *this;
        }
        //Operator (=): Erase the mem of the previous Tree & SHALLOW Copy Build the new Tree
        BinaryTree &operator=(BinaryTree &&tempBT) noexcept
        {
            root = tempBT.root;
            tempBT.root = nullptr;
            return *this;
        }
        // Checks If a Root Exists - If not then Create and Add
        // Else Overwrite Current Root
        BinaryTree &add_root(T value)
        {
            if (root == nullptr)
            {
                root = new Node(value);
            }
            else
            {
                root->data = value;
            }
            return *this;
        }
        // Additions Of Nodes :
        // Argument - Parent & Node to Add
        // First Find The Parent -- Check if not Nullptr - Then Check Actual Child of Parent and Add Accordingly
        // If No Parent - Exception
        BinaryTree &add_left(T parent, T leftNode)
        {
            Node *parentNode = searchforNode(parent);
            if (parentNode != nullptr)
            {
                if (parentNode->left == nullptr)
                {
                    parentNode->left = new Node(leftNode);
                }
                else
                {
                    (parentNode->left)->data = leftNode;
                }
            }
            else
            {
                throw std::invalid_argument("No Parent Found to Connect");
            }
            return *this;
        }
        // Same Documentation as above -- Difference -- Right Side
        BinaryTree &add_right(T parent, T rightNode)
        {
            Node *parentNode = searchforNode(parent);
            if (parentNode != nullptr)
            {
                if (parentNode->right == nullptr)
                {
                    parentNode->right = new Node(rightNode);
                }
                else
                {
                    (parentNode->right)->data = rightNode;
                }
            }
            else
            {
                throw std::invalid_argument("No Parent Found to Connect");
            }
            return *this;
        }
        // Iterate Tree and Print
        friend std::ostream &operator<<(std::ostream &out, const ariel::BinaryTree<T> &tempBT)
        {
            for (auto it : tempBT)
            {
                out << it << " :TreeNode";
            }
            return out;
        }

        // Preorder:
        class preorder_iterator
        {

        private:
            Node *currentNode; // It NODE
            std::stack<Node *> stackofNodes; //  Obvious way to traverse tree without recursion -- STACK

        public:
        Node *getNode() { return currentNode; } // Needed -- Simply Retuen Current Node Pointed
            preorder_iterator(Node *ptr = nullptr)
            {
                int reg = 0;
                if (ptr != nullptr) // Preorder Begins With ROOT:
                {
                    stackofNodes.push(ptr);
                    currentNode = ptr;
                }
                else // Because This is Pre Then Otherwise Nullptr
                {
                    currentNode = nullptr;
                }
            }
            // Get Data of It
            T &operator*() const
            {
                return currentNode->data;
            }
            // Pointer to Refrence
            T *operator->() const
            {
                return &(currentNode->data);
            }
            // First ++ then Value
            preorder_iterator &operator++()
            {
                // Like Written Above First ++ then Return Value
                // ++ = Connected Node By Order(Pre Post In)
                if (!stackofNodes.empty())
                {
                    bool flag = false;
                    stackofNodes.pop();
                    if (currentNode->right != nullptr)
                    {
                        stackofNodes.push(currentNode->right); // Update Value
                    }
                    if (currentNode->left != nullptr)
                    {
                        stackofNodes.push(currentNode->left); // Update Value
                    }
                    if (!stackofNodes.empty()) // If Something was Pushed Then Return(the ++ happened above!!)
                    {
                        currentNode = stackofNodes.top();
                        return *this;
                    }
                }
                // If Empty Obvioulsy Null
                currentNode = nullptr;
                return *this;
            }
            // First Get Value and Then ++
            preorder_iterator operator++(int)
            {
                preorder_iterator temp = *this;
                operator++();
                return temp;
            }
            // EQ CheCK
            bool operator==(const preorder_iterator &rightsideIT) const
            {
                return (currentNode == rightsideIT.currentNode);
            }
            // NEQ CHECK
            bool operator!=(const preorder_iterator &rightsideIT) const
            {
                return !(currentNode == rightsideIT.currentNode);
            }

        }; 

        // INORDER:
        class inorder_iterator
        {
        private:
            Node *currNodePtr;               // Current Node Pointed Via Iterator
            std::stack<Node *> stackofNodes; //  Obvious way to traverse tree without recursion -- STACK
        public:
            Node *getNode() { return currNodePtr; } // Needed -- Simply Retuen Current Node Pointed
            inorder_iterator(Node *ptr = nullptr)   // Based of GEEKforGEEK IDEA
            {
                bool inor = false;
                while (ptr != nullptr)
                {
                    /* place pointer to a tree node on
               the stack before traversing
               the node's left subtree */
                    stackofNodes.push(ptr);
                    ptr = ptr->left;
                }
                if (stackofNodes.empty()) // Obvious if Stack Is empty Then Current is Nullptr
                {
                    currNodePtr = nullptr;
                }
                else
                {
                    /* Current must be NULL at this point */
                    currNodePtr = stackofNodes.top();
                    stackofNodes.pop();
                }
            }
            // Get Refrence Value of Current It (*it)
            T &operator*() const
            {
                return currNodePtr->data;
            }
            // Get Pointer to Current It Value
            T *operator->() const
            {
                return &(currNodePtr->data);
            }
            // First ++ then send Value
            inorder_iterator &operator++()
            {
                if (!stackofNodes.empty() || currNodePtr != nullptr) // Must Validate So i Can Move Forward
                {
                    if (currNodePtr->right != nullptr)
                    {
                        // Stack is Lifo Hence We Insert Exactly from End to Start so When i Pop -- Data is Accurate
                        Node *temp = currNodePtr->right;
                        stackofNodes.push(temp);
                        while (temp->left != nullptr)
                        {
                            temp = temp->left;
                            stackofNodes.push(temp);
                        }
                        currNodePtr = stackofNodes.top();
                        stackofNodes.pop();
                        return *this;
                    }
                    if (stackofNodes.empty()) // Empty Hence nothing on TOP and NULLPTR
                    {
                        currNodePtr = nullptr;
                        return *this;
                    }
                    currNodePtr = stackofNodes.top(); // If not Empty then After Stack Built Point to Top
                    stackofNodes.pop();
                    return *this;
                }

                currNodePtr = nullptr; // If here then stack was empty or ptr was null hence CURRENTPTR == NULLPTR
                return *this;
            }
            // Simply Executed - First Get Value and only Then ++
            inorder_iterator operator++(int)
            {
                inorder_iterator temp = *this;
                operator++();
                return temp;
            }
            // Compare 2 Iterator Nodes (Equal)
            bool operator==(const inorder_iterator &rightsideIT) const
            {
                return (currNodePtr == rightsideIT.currNodePtr);
            }
            // Compare 2 Iterator Nodes(Not Equal)
            // Already Have == so Just Add Before !
            bool operator!=(const inorder_iterator &rightsideIT) const
            {
                return !(currNodePtr == rightsideIT.currNodePtr);
            }

        }; 

        // Postorder:
        class postorder_iterator
        {
        private:
            std::unordered_set<Node *> isChecked;
            Node *currNodePtr, *root;
            bool hasbeenChecked=true;

        public:
            Node *getNode() // Help Func , Very Simply
            {
                return currNodePtr;
            }
            postorder_iterator(Node *ptr = nullptr)
            {
                if (ptr == nullptr) // Obvious case
                {
                    currNodePtr = nullptr;
                }
                else
                {
                    root = ptr; // Must Update the Root Else Won't Work
                    // PostOrder Insertion
                    while (isChecked.find(ptr) == isChecked.end())
                    {
                        // Take care First of Left Side
                        if (ptr->left != nullptr && isChecked.find(ptr->left) == isChecked.end())
                        {
                            ptr = ptr->left;
                        }
                        else // Done
                        {
                            // Take Care of Right Side
                            if (ptr->right && isChecked.find(ptr->right) == isChecked.end())
                            {
                                ptr = ptr->right;
                            }
                            else // Whats Left is Current || Point then Insert
                            {
                                currNodePtr = ptr;
                                isChecked.insert(ptr);
                                break;
                            }
                        }
                    }
                }
            }
            // Get Refrence Value of Current It (*it)
            T &operator*() const
            {
                return currNodePtr->data;
            }
            // Get Pointer to Current It Value
            T *operator->() const
            {
                return &(currNodePtr->data);
            }

            postorder_iterator &operator++()
            {
                // Set The Pointer So Obviously Like Above Iteration But Just check what is a valid next and Increase
                int help = 0;
                bool flag = false;
                Node *ptr = root;
                while (isChecked.find(ptr) == isChecked.end() && ptr != nullptr)
                {
                    // Based on Post First Check This
                    // Post - Start Bottom And Read Left to Right
                    if (isChecked.find(ptr->left) == isChecked.end() && ptr->left != nullptr)
                    {
                        ptr = ptr->left;
                    }
                    else
                    {
                        // Right
                        if (ptr->right && isChecked.find(ptr->right) == isChecked.end() && ptr->right != nullptr)
                        {
                            ptr = ptr->right;
                        }
                        else
                        {
                            currNodePtr = ptr;
                            isChecked.insert(ptr);
                            return *this;
                        }
                    }
                }
                //When Done Like Func Describes - Return CurrentPTR and ++
                currNodePtr = nullptr;
                return *this;
            }
            // Simply Executed - First Get Value and only Then ++
            postorder_iterator operator++(int)
            {
                postorder_iterator temp = *this; // Get as Temp
                operator++();                    // ++
                return temp;                     // Return
            }
            // Compare 2 Iterator Nodes (Equal)
            bool operator==(const postorder_iterator &rightsideIT) const
            {
                return (currNodePtr == rightsideIT.currNodePtr);
            }
            // Compare 2 Iterator Nodes(Not Equal)
            // Already Have == so Just Add Before !
            bool operator!=(const postorder_iterator &rightsideIT) const
            {
                return !(currNodePtr == rightsideIT.currNodePtr);
            }
        };
        // INORDER AND BASIC BEGIN AND END
        inorder_iterator begin() const
        {
            return inorder_iterator{root};
        }
        inorder_iterator end() const
        {
            return inorder_iterator{};
        }
        inorder_iterator begin_inorder() const
        {
            return inorder_iterator{root};
        }
        inorder_iterator end_inorder() const
        {
            return inorder_iterator{nullptr};
        }
        //PREORDER BEGIN AND END
        preorder_iterator begin_preorder() const
        {
            return preorder_iterator{root};
        }
        preorder_iterator end_preorder() const
        {
            return preorder_iterator{};
        }
        // POSTORDER BEGIN AND END
        postorder_iterator begin_postorder() const
        {
            return postorder_iterator{root};
        }
        postorder_iterator end_postorder() const
        {
            return postorder_iterator{nullptr};
        }

    }; //BINARYTREE CLASS COMPLETE
};     // End