#include "linked_ptr.hpp"

struct C1 {};
struct C2 : C1 {};

int main()
{
    smart_ptr::linked_ptr<C1> const p;
    smart_ptr::linked_ptr<C2> q;

    bool r;

    r = p == q;

    (void)r;
}
