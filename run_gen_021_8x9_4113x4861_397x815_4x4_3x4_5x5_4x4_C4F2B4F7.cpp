#include <cassert>
#include <map>
#include <string>
#include <initializer_list>

#include "linked_ptr.hpp"

struct Cnt
{
public:
    Cnt(char const * name)
        : name_(name)
    {
        add_this();
    }

    virtual ~Cnt()
    {
        remove_this();
    }

    Cnt const * get_this() const
    {
        return this;
    }

    std::string get_name() const
    {
        return name_;
    }

private:
    typedef std::map<std::string, Cnt *> objects_map_t;

    static objects_map_t & objects()
    {
        static objects_map_t objects;
        return objects;
    }

    void add_this()
    {
        objects_map_t & objs = objects();

        assert(objs.find(name_) == objs.end());

        objs.insert(std::make_pair(name_, this));
    }

    void remove_this()
    {
        objects_map_t & objs = objects();

        auto it = objs.find(name_);

        assert(it != objs.end());
        assert(it->second == this);

        objs.erase(it);
    }

public:
    static void verify_state(
        std::initializer_list<char const *> should_be_objects)
    {
        // Get copy of objects map
        objects_map_t stored_objs = objects();
        for (char const * name: should_be_objects)
        {
            auto it = stored_objs.find(name);
            assert(it != stored_objs.end());
            stored_objs.erase(it);
        }

        assert(stored_objs.empty());
    }

private:
    std::string name_;
};

struct CntD : Cnt
{
    CntD(char const * name)
        : Cnt(name)
    {
        (void)fill_;
    }

private:
    int fill_;
};

typedef smart_ptr::linked_ptr<Cnt> cptr_t;
typedef smart_ptr::linked_ptr<CntD> cdptr_t;

int main()
{
    // Generated tests
    cdptr_t * p0 = new cdptr_t(new CntD("obj0"));
    Cnt::verify_state({"obj0"});
    assert(true && *p0);
    assert(true && (*p0)->get_this() == (*p0).get() && (*(*p0)).get_name() == "obj0");
    assert(true && *p0 == *p0);
    assert(true && (*p0).unique());

    delete p0; p0 = nullptr;
    Cnt::verify_state({});
    assert(true);
    assert(true);
    assert(true);
    assert(true);

    cptr_t * p1 = new cptr_t(new CntD("obj1"));
    Cnt::verify_state({"obj1"});
    assert(true && *p1);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1");
    assert(true && *p1 == *p1);
    assert(true && (*p1).unique());

    cdptr_t * p2 = new cdptr_t(new CntD("obj2"));
    Cnt::verify_state({"obj2", "obj1"});
    assert(true && *p1 && *p2);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj2");
    assert(true && *p2 == *p2 && *p1 == *p1 && *p1 != *p2);
    assert(true && (*p1).unique() && (*p2).unique());

    delete p2; p2 = nullptr;
    Cnt::verify_state({"obj1"});
    assert(true && *p1);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1");
    assert(true && *p1 == *p1);
    assert(true && (*p1).unique());

    *p1 = *p1;
    Cnt::verify_state({"obj1"});
    assert(true && *p1);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1");
    assert(true && *p1 == *p1);
    assert(true && (*p1).unique());

    cptr_t * p3 = new cptr_t(*p1);
    Cnt::verify_state({"obj1"});
    assert(true && *p1 && *p3);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p3)->get_this() == (*p3).get() && (*(*p3)).get_name() == "obj1");
    assert(true && *p3 == *p1);
    assert(true && !(*p1).unique() && !(*p3).unique());

    cptr_t * p4 = new cptr_t(*p1);
    Cnt::verify_state({"obj1"});
    assert(true && *p4 && *p1 && *p3);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj1" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p3)->get_this() == (*p3).get() && (*(*p3)).get_name() == "obj1");
    assert(true && *p3 == *p4);
    assert(true && !(*p4).unique() && !(*p1).unique() && !(*p3).unique());

    delete p4; p4 = nullptr;
    Cnt::verify_state({"obj1"});
    assert(true && *p1 && *p3);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p3)->get_this() == (*p3).get() && (*(*p3)).get_name() == "obj1");
    assert(true && *p3 == *p3);
    assert(true && !(*p1).unique() && !(*p3).unique());

    p1->reset(new CntD("obj3"));
    Cnt::verify_state({"obj3", "obj1"});
    assert(true && *p1 && *p3);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj3" && (*p3)->get_this() == (*p3).get() && (*(*p3)).get_name() == "obj1");
    assert(true && *p1 == *p1 && *p3 == *p3 && *p3 != *p1);
    assert(true && (*p1).unique() && (*p3).unique());

    cptr_t * p5 = new cptr_t(*p1);
    Cnt::verify_state({"obj3", "obj1"});
    assert(true && *p5 && *p1 && *p3);
    assert(true && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj3" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj3" && (*p3)->get_this() == (*p3).get() && (*(*p3)).get_name() == "obj1");
    assert(true && *p5 == *p1 && *p3 == *p3 && *p3 != *p5);
    assert(true && !(*p5).unique() && !(*p1).unique() && (*p3).unique());

    delete p5; p5 = nullptr;
    Cnt::verify_state({"obj3", "obj1"});
    assert(true && *p1 && *p3);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj3" && (*p3)->get_this() == (*p3).get() && (*(*p3)).get_name() == "obj1");
    assert(true && *p1 == *p1 && *p3 == *p3 && *p3 != *p1);
    assert(true && (*p1).unique() && (*p3).unique());

    delete p3; p3 = nullptr;
    Cnt::verify_state({"obj3"});
    assert(true && *p1);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj3");
    assert(true && *p1 == *p1);
    assert(true && (*p1).unique());

    cdptr_t * p6 = new cdptr_t();
    Cnt::verify_state({"obj3"});
    assert(true && *p1 && !*p6);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj3" && (*p6).get() == nullptr);
    assert(true && *p1 == *p1);
    assert(true && (*p1).unique() && !(*p6).unique());

    *p1 = *p6;
    Cnt::verify_state({});
    assert(true && !*p1 && !*p6);
    assert(true && (*p1).get() == nullptr && (*p6).get() == nullptr);
    assert(true);
    assert(true && !(*p1).unique() && !(*p6).unique());

    cptr_t * p7 = new cptr_t(*p6);
    Cnt::verify_state({});
    assert(true && !*p7 && !*p1 && !*p6);
    assert(true && (*p7).get() == nullptr && (*p1).get() == nullptr && (*p6).get() == nullptr);
    assert(true);
    assert(true && !(*p7).unique() && !(*p1).unique() && !(*p6).unique());

    cdptr_t * p8 = new cdptr_t();
    Cnt::verify_state({});
    assert(true && !*p7 && !*p1 && !*p6 && !*p8);
    assert(true && (*p7).get() == nullptr && (*p1).get() == nullptr && (*p6).get() == nullptr && (*p8).get() == nullptr);
    assert(true);
    assert(true && !(*p7).unique() && !(*p1).unique() && !(*p6).unique() && !(*p8).unique());

    delete p8; p8 = nullptr;
    Cnt::verify_state({});
    assert(true && !*p7 && !*p1 && !*p6);
    assert(true && (*p7).get() == nullptr && (*p1).get() == nullptr && (*p6).get() == nullptr);
    assert(true);
    assert(true && !(*p7).unique() && !(*p1).unique() && !(*p6).unique());

    delete p7; p7 = nullptr;
    Cnt::verify_state({});
    assert(true && !*p1 && !*p6);
    assert(true && (*p1).get() == nullptr && (*p6).get() == nullptr);
    assert(true);
    assert(true && !(*p1).unique() && !(*p6).unique());

    delete p6; p6 = nullptr;
    Cnt::verify_state({});
    assert(true && !*p1);
    assert(true && (*p1).get() == nullptr);
    assert(true);
    assert(true && !(*p1).unique());

    delete p1; p1 = nullptr;
    Cnt::verify_state({});
    assert(true);
    assert(true);
    assert(true);
    assert(true);

    return 0;
}
