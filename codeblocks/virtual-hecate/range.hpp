#ifndef RANGE_H
#define RANGE_H

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

    struct AVL_NODE
    {
        RANGE *     Range;
        int         Balance;
        AVL_NODE *  Left ;
        AVL_NODE *  Right ;
    };

    class RangeLookupTable
    {
	private :
		AVL_NODE * Root;
    private:
        AVL_NODE * Insert(RANGE * data, int * h);
        static AVL_NODE * Buildtree(AVL_NODE * root, RANGE * data, int * h);
        void Display(AVL_NODE * root);
        AVL_NODE * Remove(AVL_NODE * root, RANGE * data, int * h);
        static AVL_NODE * Remove(AVL_NODE * node, AVL_NODE * root, int * h);
        static AVL_NODE * BalanceRight(AVL_NODE * root, int * h);
		static AVL_NODE * BalanceLeft(AVL_NODE * root, int * h);
		void SetRoot(AVL_NODE * avl);
		static void Delete(AVL_NODE * root);
	public :
		RangeLookupTable();
        ~RangeLookupTable();
    };
};

#endif // RANGE_H
