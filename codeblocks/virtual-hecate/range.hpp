#ifndef RANGE_H
#define RANGE_H

#include "inode.hpp"

#include <cstdint>
#include <cstdlib>

using namespace std;

namespace Hecate
{
    struct RANGE
    {
        uint64_t Begin;
        uint64_t End;
    };

    struct AVL_DATA
    {
        class Inode *   Inode;
        ATTR_RECORD *   Record;
        RANGE *         Range;

        AVL_DATA(class Inode * i, ATTR_RECORD * a, RANGE * r) :
            Inode(i),
            Record(a),
            Range(r)
        { };

        ~AVL_DATA()
        {
            delete Range;
        };
    };

    struct AVL_NODE
    {
        AVL_DATA *  Data;
        int         Balance;
        AVL_NODE *  Left;
        AVL_NODE *  Right;
    };

    class RangeLookupTable
    {
	private :
		AVL_NODE * Root;
    private:
        AVL_NODE * Insert(AVL_DATA * data, int * h);
        void DisplayNode(AVL_NODE * node);
        static AVL_NODE * Buildtree(AVL_NODE * root, AVL_DATA * data, int * h);
        AVL_NODE * RemoveData(AVL_NODE * root, AVL_DATA * data, int * h);
        static AVL_NODE * RemoveNode(AVL_NODE * node, AVL_NODE * root, int * h);
        static AVL_NODE * BalanceRight(AVL_NODE * root, int * h);
		static AVL_NODE * BalanceLeft(AVL_NODE * root, int * h);
		void SetRoot(AVL_NODE * avl);
		static void Delete(AVL_NODE * root);
	public :
		RangeLookupTable();
        ~RangeLookupTable();
        void Display();
        void Insert(class Inode * inode, ATTR_RECORD * rec, RANGE * range)
        {
            int h;

            this->Root = this->Insert(new AVL_DATA(inode, rec, range), &h);
        };
    };
};

#endif // RANGE_H
