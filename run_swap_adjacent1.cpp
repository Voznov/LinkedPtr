#include "linked_ptr.hpp"

int main()
{
    smart_ptr::linked_ptr<int> p1(new int);
    smart_ptr::linked_ptr<int> p2(p1);

    p1.swap(p2);
}
