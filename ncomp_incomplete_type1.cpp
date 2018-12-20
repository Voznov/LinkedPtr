#include "linked_ptr.hpp"

struct C;

int main()
{
    smart_ptr::linked_ptr<C> p1(reinterpret_cast<C *>(123));
}
