#include "linked_ptr.hpp"

struct C {};

int main()
{
    smart_ptr::linked_ptr<C> const p;
    smart_ptr::linked_ptr<C> q;

    bool r;

    r = p == q;

    (void)r;
}
