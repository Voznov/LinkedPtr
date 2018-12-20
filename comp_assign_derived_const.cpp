#include "linked_ptr.hpp"

struct Base {};
struct Derived : Base {};

int main()
{
    smart_ptr::linked_ptr<Base> p;
    smart_ptr::linked_ptr<Derived> const q;

    p = q;
}
