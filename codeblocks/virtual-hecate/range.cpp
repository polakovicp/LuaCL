#include "range.hpp"

#include <iostream>

namespace Hecate
{
    RangeLookupTable::RangeLookupTable()
    {
        Root = NULL;
    };

    AVL_NODE * RangeLookupTable::Insert(AVL_DATA * data, int * h)
    {
        this->Root = this->Buildtree(this->Root, data, h);
        return this->Root;
    };

    AVL_NODE * RangeLookupTable::Buildtree(AVL_NODE * root, AVL_DATA * data, int * h)
    {
        AVL_NODE * node1, * node2;

        if(root == NULL)
        {
            root = new AVL_NODE;
            root->Data = data;
            root->Left = NULL;
            root->Right = NULL;
            root->Balance = 0;
            *h = 1;
            return root;
        }

        if(data->Range->Begin < root->Data->Range->Begin)
        {
            // TODO remove recursion
            root->Left = RangeLookupTable::Buildtree(root->Left, data, h);

            // if left subtree is higher
            if(*h)
            {
                switch(root->Balance)
                {
                case 1 :
                    node1 = root->Left;
                    if(node1->Balance == 1)
                    {
                        // rotate right
                        root->Left = node1->Right;
                        node1->Right = root;
                        root->Balance = 0;
                        root = node1;
                    }
                    else
                    {
                        // double rotation, left, right
                        node2 = node1->Right;
                        node1->Right = node2->Left;
                        node2->Left = node1;
                        root->Left = node2->Right;
                        node2->Right = root;
                        if(node2->Balance == 1)
                            root->Balance = -1;
                        else
                            root->Balance = 0;
                        if (node2->Balance == -1)
                            node1->Balance = 1;
                        else
                            node1->Balance = 0;
                        root = node2;
                    }
                    root->Balance = 0;
                    *h = 0;
                    break;
                case 0 :
                    root->Balance = 1;
                    break ;
                case -1 :
                    root->Balance = 0;
                    *h = 0;
                }
            }
        }

        if(data->Range->Begin > root->Data->Range->Begin)
        {
            root->Right = RangeLookupTable::Buildtree(root->Right, data, h);

            if (*h)
            {
                switch(root->Balance)
                {
                case 1 :
                    root->Balance = 0;
                    *h = 0;
                    break;
                case 0 :
                    root->Balance = -1;
                    break;
                case -1 :
                    node1 = root->Right;
                    if(node1->Balance == -1)
                    {
                        // left rotation
                        root->Right = node1->Left;
                        node1->Left = root;
                        root->Balance = 0;
                        root = node1;
                    }
                    else
                    {
                        // double rotation, right, left
                        node2 = node1->Left;
                        node1->Left = node2->Right;
                        node2->Right = node1;
                        root->Right = node2->Left;
                        node2->Left = root;
                        if(node2->Balance == -1)
                            root->Balance = 1;
                        else
                            root->Balance = 0;
                        if(node2->Balance == 1)
                            node1->Balance = -1;
                        else
                            node1->Balance = 0;
                        root = node2;
                    }
                    root->Balance = 0;
                    *h = 0;
                }
            }
        }

        return root;
    };

    void RangeLookupTable::Display()
    {
        this->DisplayNode(this->Root);
    };

    void RangeLookupTable::DisplayNode(AVL_NODE * node)
    {
        if (node != NULL)
        {
            this->DisplayNode(node->Left);

            std::cout   << node->Data->Inode->Record->MftRecordNumber
                        << " (" << node->Data->Record->Instance << ") "
                        << "["
                        << node->Data->Range->Begin << "-" << node->Data->Range->End
                        << "]"
                        << std::endl;

            this->DisplayNode(node->Right);
        }
    };

    AVL_NODE * RangeLookupTable::RemoveData(AVL_NODE * root, AVL_DATA * data, int *h)
    {
        AVL_NODE * node;

        if(root == NULL)
        {
            return root;
        }
        else
        {
            if(data->Range->Begin < root->Data->Range->Begin)
            {
                root->Left = this->RemoveData(root->Left, data, h);
                if(*h)
                    root = this->BalanceRight(root, h);
            }
            else
            {
                if(data->Range->Begin > root->Data->Range->Begin)
                {
                    root->Right = this->RemoveData(root->Right, data, h);
                    if(*h)
                        root = this->BalanceLeft(root, h);
                }
                else
                {
                    node = root;
                    if(node->Right == NULL)
                    {
                        root = node->Left;
                        *h = 1;

                        delete node->Data;
                        delete node;
                    }
                    else
                    {
                        if(node->Left == NULL)
                        {
                            root = node->Right;
                            *h = 1;

                            delete node->Data;
                            delete node;
                        }
                        else
                        {
                            node->Right = this->RemoveNode(node->Right, node, h);
                            if(*h)
                                root = this->BalanceLeft(root, h);
                        }
                    }
                }
            }
        }

        return root;
    };

    AVL_NODE * RangeLookupTable::RemoveNode(AVL_NODE * succ, AVL_NODE * node, int *h)
    {
        AVL_NODE * temp = succ;

        if (succ->Left != NULL)
        {
            succ->Left = RangeLookupTable::RemoveNode(succ->Left, node, h);
            if(*h)
                succ = RangeLookupTable::BalanceRight(succ, h);
        }
        else
        {
            temp = succ;
            node->Data = succ->Data;
            succ = succ->Right;

            delete temp->Data;
            delete temp;
            *h = 1;
        }

        return succ;
    };

    AVL_NODE * RangeLookupTable::BalanceRight(AVL_NODE * root, int * h)
    {
        AVL_NODE * temp1, *temp2;

        switch (root->Balance)
        {
        case 1 :
            root->Balance = 0;
            break ;
        case 0 :
            root->Balance = -1;
            *h = 0;
            break;
        case -1 :
            temp1 = root->Right;
            if(temp1->Balance <= 0)
            {
                // left rotation
                root->Right = temp1->Left;
                temp1->Left = root;
                if(temp1->Balance == 0)
                {
                    root->Balance = -1;
                    temp1->Balance = 1;
                    *h = 0;
                }
                else
                {
                    root->Balance = temp1->Balance = 0;
                }

                root = temp1;
            }
            else
            {
                // double rotation, right, left
                temp2 = temp1->Left;
                temp1->Left = temp2->Right;
                temp2->Right = temp1;
                root->Right = temp2->Left;
                temp2->Left = root;
                if(temp2->Balance == -1)
                    root->Balance = 1;
                else
                    root->Balance = 0;
                if(temp2->Balance == 1)
                    temp1->Balance = -1;
                else
                    temp1->Balance = 0;
                root = temp2;
                temp2->Balance = 0;
            }
        }

        return root;
    };

    AVL_NODE * RangeLookupTable::BalanceLeft(AVL_NODE * root, int * h)
    {
        AVL_NODE * temp1, * temp2;

        switch(root->Balance)
        {
        case -1 :
            root->Balance = 0;
            break;
        case 0 :
            root->Balance = 1;
            *h = 0;
            break;
        case 1 :
            temp1 = root->Left;
            if(temp1->Balance >= 0)
            {
                // right rotation
                root->Left = temp1->Right;
                temp1->Right = root;

                if(temp1->Balance == 0)
                {
                    root->Balance = 1;
                    temp1->Balance = -1;
                    *h = 0;
                }
                else
                {
                    root->Balance = temp1->Balance = 0;
                }
                root = temp1;
            }
            else
            {
                // double rotation, left, right
                temp2 = temp1->Right;
                temp1->Right = temp2->Left;
                temp2->Left = temp1;
                root->Left = temp2->Right;
                temp2->Right = root;
                if(temp2->Balance == 1)
                    root->Balance = -1;
                else
                    root->Balance = 0;
                if(temp2->Balance == -1)
                    temp1->Balance = 1;
                else
                    temp1->Balance = 0;
                root = temp2;
                temp2->Balance = 0;
            }
        }

        return root;
    };

    void RangeLookupTable::SetRoot(AVL_NODE * avl)
    {
        this->Root = avl;
    };

    RangeLookupTable::~RangeLookupTable()
    {
        this->Delete(this->Root);
    };

    void RangeLookupTable::Delete(AVL_NODE * root)
    {
        if(root != NULL)
        {
            RangeLookupTable::Delete(root->Left);
            RangeLookupTable::Delete(root->Right);
        }

        delete root->Data;
        delete root;
    };
};
