#include <cassert>

#include "linked_ptr.hpp"

int main()
{
    smart_ptr::linked_ptr<int> p(new int);
    assert(p);
    p = p;
    assert(p);
}
