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

    p0->swap(*p0);
    Cnt::verify_state({"obj0"});
    assert(true && *p0);
    assert(true && (*p0)->get_this() == (*p0).get() && (*(*p0)).get_name() == "obj0");
    assert(true && *p0 == *p0);
    assert(true && (*p0).unique());

    p0->swap(*p0);
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

    cptr_t * p1 = new cptr_t();
    Cnt::verify_state({});
    assert(true && !*p1);
    assert(true && (*p1).get() == nullptr);
    assert(true);
    assert(true && !(*p1).unique());

    p1->reset();
    Cnt::verify_state({});
    assert(true && !*p1);
    assert(true && (*p1).get() == nullptr);
    assert(true);
    assert(true && !(*p1).unique());

    cptr_t * p2 = new cptr_t(new Cnt("obj1"));
    Cnt::verify_state({"obj1"});
    assert(true && !*p1 && *p2);
    assert(true && (*p1).get() == nullptr && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj1");
    assert(true && *p2 == *p2);
    assert(true && !(*p1).unique() && (*p2).unique());

    cptr_t * p3 = new cptr_t(new CntD("obj2"));
    Cnt::verify_state({"obj2", "obj1"});
    assert(true && !*p1 && *p2 && *p3);
    assert(true && (*p1).get() == nullptr && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj1" && (*p3)->get_this() == (*p3).get() && (*(*p3)).get_name() == "obj2");
    assert(true && *p3 == *p3 && *p2 == *p2 && *p2 != *p3);
    assert(true && !(*p1).unique() && (*p2).unique() && (*p3).unique());

    p2->swap(*p1);
    Cnt::verify_state({"obj2", "obj1"});
    assert(true && *p1 && !*p2 && *p3);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2).get() == nullptr && (*p3)->get_this() == (*p3).get() && (*(*p3)).get_name() == "obj2");
    assert(true && *p3 == *p3 && *p1 == *p1 && *p1 != *p3);
    assert(true && (*p1).unique() && !(*p2).unique() && (*p3).unique());

    p2->reset(new Cnt("obj3"));
    Cnt::verify_state({"obj3", "obj2", "obj1"});
    assert(true && *p1 && *p2 && *p3);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p3)->get_this() == (*p3).get() && (*(*p3)).get_name() == "obj2");
    assert(true && *p2 == *p2 && *p3 == *p3 && *p3 != *p2 && *p1 == *p1 && *p1 != *p3);
    assert(true && (*p1).unique() && (*p2).unique() && (*p3).unique());

    delete p3; p3 = nullptr;
    Cnt::verify_state({"obj3", "obj1"});
    assert(true && *p1 && *p2);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3");
    assert(true && *p2 == *p2 && *p1 == *p1 && *p1 != *p2);
    assert(true && (*p1).unique() && (*p2).unique());

    cptr_t * p4 = new cptr_t(*p1);
    Cnt::verify_state({"obj3", "obj1"});
    assert(true && *p4 && *p1 && *p2);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj1" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3");
    assert(true && *p2 == *p2 && *p1 == *p4 && *p4 != *p2);
    assert(true && !(*p4).unique() && !(*p1).unique() && (*p2).unique());

    p4->reset(new CntD("obj4"));
    Cnt::verify_state({"obj4", "obj3", "obj1"});
    assert(true && *p4 && *p1 && *p2);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj4" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3");
    assert(true && *p4 == *p4 && *p2 == *p2 && *p2 != *p4 && *p1 == *p1 && *p1 != *p2);
    assert(true && (*p4).unique() && (*p1).unique() && (*p2).unique());

    p4->swap(*p4);
    Cnt::verify_state({"obj4", "obj3", "obj1"});
    assert(true && *p4 && *p1 && *p2);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj4" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3");
    assert(true && *p4 == *p4 && *p2 == *p2 && *p2 != *p4 && *p1 == *p1 && *p1 != *p2);
    assert(true && (*p4).unique() && (*p1).unique() && (*p2).unique());

    cptr_t * p5 = new cptr_t(new CntD("obj5"));
    Cnt::verify_state({"obj5", "obj4", "obj3", "obj1"});
    assert(true && *p4 && *p5 && *p1 && *p2);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj4" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3");
    assert(true && *p5 == *p5 && *p4 == *p4 && *p4 != *p5 && *p2 == *p2 && *p2 != *p4 && *p1 == *p1 && *p1 != *p2);
    assert(true && (*p4).unique() && (*p5).unique() && (*p1).unique() && (*p2).unique());

    cdptr_t * p6 = new cdptr_t();
    Cnt::verify_state({"obj5", "obj4", "obj3", "obj1"});
    assert(true && *p4 && *p5 && *p1 && *p2 && !*p6);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj4" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p6).get() == nullptr);
    assert(true && *p5 == *p5 && *p4 == *p4 && *p4 != *p5 && *p2 == *p2 && *p2 != *p4 && *p1 == *p1 && *p1 != *p2);
    assert(true && (*p4).unique() && (*p5).unique() && (*p1).unique() && (*p2).unique() && !(*p6).unique());

    cptr_t * p7 = new cptr_t(*p5);
    Cnt::verify_state({"obj5", "obj4", "obj3", "obj1"});
    assert(true && *p4 && *p5 && *p7 && *p1 && *p2 && !*p6);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj4" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p7)->get_this() == (*p7).get() && (*(*p7)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p6).get() == nullptr);
    assert(true && *p5 == *p5 && *p4 == *p4 && *p4 != *p7 && *p2 == *p2 && *p2 != *p4 && *p1 == *p1 && *p1 != *p2);
    assert(true && (*p4).unique() && !(*p5).unique() && !(*p7).unique() && (*p1).unique() && (*p2).unique() && !(*p6).unique());

    p4->swap(*p7);
    Cnt::verify_state({"obj5", "obj4", "obj3", "obj1"});
    assert(true && *p4 && *p5 && *p7 && *p1 && *p2 && !*p6);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj5" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p7)->get_this() == (*p7).get() && (*(*p7)).get_name() == "obj4" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p6).get() == nullptr);
    assert(true && *p5 == *p5 && *p7 == *p7 && *p7 != *p4 && *p2 == *p2 && *p2 != *p7 && *p1 == *p1 && *p1 != *p2);
    assert(true && !(*p4).unique() && !(*p5).unique() && (*p7).unique() && (*p1).unique() && (*p2).unique() && !(*p6).unique());

    delete p7; p7 = nullptr;
    Cnt::verify_state({"obj5", "obj3", "obj1"});
    assert(true && *p4 && *p5 && *p1 && *p2 && !*p6);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj5" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj1" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p6).get() == nullptr);
    assert(true && *p5 == *p5 && *p2 == *p2 && *p2 != *p5 && *p1 == *p1 && *p1 != *p2);
    assert(true && !(*p4).unique() && !(*p5).unique() && (*p1).unique() && (*p2).unique() && !(*p6).unique());

    *p1 = *p6;
    Cnt::verify_state({"obj5", "obj3"});
    assert(true && *p4 && *p5 && !*p1 && *p2 && !*p6);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj5" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1).get() == nullptr && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p6).get() == nullptr);
    assert(true && *p4 == *p4 && *p2 == *p2 && *p2 != *p5);
    assert(true && !(*p4).unique() && !(*p5).unique() && !(*p1).unique() && (*p2).unique() && !(*p6).unique());

    delete p6; p6 = nullptr;
    Cnt::verify_state({"obj5", "obj3"});
    assert(true && *p4 && *p5 && !*p1 && *p2);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj5" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1).get() == nullptr && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3");
    assert(true && *p5 == *p5 && *p2 == *p2 && *p2 != *p5);
    assert(true && !(*p4).unique() && !(*p5).unique() && !(*p1).unique() && (*p2).unique());

    cdptr_t * p8 = new cdptr_t();
    Cnt::verify_state({"obj5", "obj3"});
    assert(true && *p4 && *p5 && !*p1 && *p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj5" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1).get() == nullptr && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p8).get() == nullptr);
    assert(true && *p4 == *p5 && *p2 == *p2 && *p2 != *p4);
    assert(true && !(*p4).unique() && !(*p5).unique() && !(*p1).unique() && (*p2).unique() && !(*p8).unique());

    p1->reset(new Cnt("obj6"));
    Cnt::verify_state({"obj6", "obj5", "obj3"});
    assert(true && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj5" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj6" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p8).get() == nullptr);
    assert(true && *p1 == *p1 && *p5 == *p5 && *p5 != *p1 && *p2 == *p2 && *p2 != *p5);
    assert(true && !(*p4).unique() && !(*p5).unique() && (*p1).unique() && (*p2).unique() && !(*p8).unique());

    cptr_t * p9 = new cptr_t(*p2);
    Cnt::verify_state({"obj6", "obj5", "obj3"});
    assert(true && *p9 && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p9)->get_this() == (*p9).get() && (*(*p9)).get_name() == "obj3" && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj5" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj6" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p8).get() == nullptr);
    assert(true && *p1 == *p1 && *p4 == *p4 && *p5 != *p1 && *p2 == *p2 && *p9 != *p5);
    assert(true && !(*p9).unique() && !(*p4).unique() && !(*p5).unique() && (*p1).unique() && !(*p2).unique() && !(*p8).unique());

    p1->reset(new Cnt("obj7"));
    Cnt::verify_state({"obj7", "obj5", "obj3"});
    assert(true && *p9 && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p9)->get_this() == (*p9).get() && (*(*p9)).get_name() == "obj3" && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj5" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p8).get() == nullptr);
    assert(true && *p1 == *p1 && *p4 == *p5 && *p4 != *p1 && *p2 == *p2 && *p2 != *p4);
    assert(true && !(*p9).unique() && !(*p4).unique() && !(*p5).unique() && (*p1).unique() && !(*p2).unique() && !(*p8).unique());

    cptr_t * p10 = new cptr_t(*p1);
    Cnt::verify_state({"obj7", "obj5", "obj3"});
    assert(true && *p10 && *p9 && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p10)->get_this() == (*p10).get() && (*(*p10)).get_name() == "obj7" && (*p9)->get_this() == (*p9).get() && (*(*p9)).get_name() == "obj3" && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj5" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p8).get() == nullptr);
    assert(true && *p10 == *p1 && *p4 == *p5 && *p4 != *p10 && *p9 == *p2 && *p2 != *p4);
    assert(true && !(*p10).unique() && !(*p9).unique() && !(*p4).unique() && !(*p5).unique() && !(*p1).unique() && !(*p2).unique() && !(*p8).unique());

    *p4 = *p8;
    Cnt::verify_state({"obj7", "obj5", "obj3"});
    assert(true && *p10 && *p9 && !*p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p10)->get_this() == (*p10).get() && (*(*p10)).get_name() == "obj7" && (*p9)->get_this() == (*p9).get() && (*(*p9)).get_name() == "obj3" && (*p4).get() == nullptr && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p8).get() == nullptr);
    assert(true && *p10 == *p1 && *p5 == *p5 && *p5 != *p10 && *p9 == *p9 && *p9 != *p5);
    assert(true && !(*p10).unique() && !(*p9).unique() && !(*p4).unique() && (*p5).unique() && !(*p1).unique() && !(*p2).unique() && !(*p8).unique());

    delete p10; p10 = nullptr;
    Cnt::verify_state({"obj7", "obj5", "obj3"});
    assert(true && *p9 && !*p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p9)->get_this() == (*p9).get() && (*(*p9)).get_name() == "obj3" && (*p4).get() == nullptr && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p8).get() == nullptr);
    assert(true && *p1 == *p1 && *p5 == *p5 && *p5 != *p1 && *p9 == *p9 && *p2 != *p5);
    assert(true && !(*p9).unique() && !(*p4).unique() && (*p5).unique() && (*p1).unique() && !(*p2).unique() && !(*p8).unique());

    delete p9; p9 = nullptr;
    Cnt::verify_state({"obj7", "obj5", "obj3"});
    assert(true && !*p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p4).get() == nullptr && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p8).get() == nullptr);
    assert(true && *p1 == *p1 && *p5 == *p5 && *p5 != *p1 && *p2 == *p2 && *p2 != *p5);
    assert(true && !(*p4).unique() && (*p5).unique() && (*p1).unique() && (*p2).unique() && !(*p8).unique());

    cptr_t * p11 = new cptr_t(*p4);
    Cnt::verify_state({"obj7", "obj5", "obj3"});
    assert(true && !*p11 && !*p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p11).get() == nullptr && (*p4).get() == nullptr && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj3" && (*p8).get() == nullptr);
    assert(true && *p1 == *p1 && *p5 == *p5 && *p5 != *p1 && *p2 == *p2 && *p2 != *p5);
    assert(true && !(*p11).unique() && !(*p4).unique() && (*p5).unique() && (*p1).unique() && (*p2).unique() && !(*p8).unique());

    p4->swap(*p2);
    Cnt::verify_state({"obj7", "obj5", "obj3"});
    assert(true && !*p11 && *p4 && *p5 && *p1 && !*p2 && !*p8);
    assert(true && (*p11).get() == nullptr && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj3" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2).get() == nullptr && (*p8).get() == nullptr);
    assert(true && *p1 == *p1 && *p5 == *p5 && *p5 != *p1 && *p4 == *p4 && *p4 != *p5);
    assert(true && !(*p11).unique() && (*p4).unique() && (*p5).unique() && (*p1).unique() && !(*p2).unique() && !(*p8).unique());

    delete p11; p11 = nullptr;
    Cnt::verify_state({"obj7", "obj5", "obj3"});
    assert(true && *p4 && *p5 && *p1 && !*p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj3" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2).get() == nullptr && (*p8).get() == nullptr);
    assert(true && *p1 == *p1 && *p5 == *p5 && *p5 != *p1 && *p4 == *p4 && *p4 != *p5);
    assert(true && (*p4).unique() && (*p5).unique() && (*p1).unique() && !(*p2).unique() && !(*p8).unique());

    p2->reset(new CntD("obj8"));
    Cnt::verify_state({"obj8", "obj7", "obj5", "obj3"});
    assert(true && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj3" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj8" && (*p8).get() == nullptr);
    assert(true && *p2 == *p2 && *p1 == *p1 && *p1 != *p2 && *p5 == *p5 && *p5 != *p1 && *p4 == *p4 && *p4 != *p5);
    assert(true && (*p4).unique() && (*p5).unique() && (*p1).unique() && (*p2).unique() && !(*p8).unique());

    p5->swap(*p2);
    Cnt::verify_state({"obj8", "obj7", "obj5", "obj3"});
    assert(true && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj3" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj8" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj7" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5" && (*p8).get() == nullptr);
    assert(true && *p5 == *p5 && *p1 == *p1 && *p1 != *p5 && *p2 == *p2 && *p2 != *p1 && *p4 == *p4 && *p4 != *p2);
    assert(true && (*p4).unique() && (*p5).unique() && (*p1).unique() && (*p2).unique() && !(*p8).unique());

    *p1 = *p2;
    Cnt::verify_state({"obj8", "obj5", "obj3"});
    assert(true && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj3" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj8" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5" && (*p8).get() == nullptr);
    assert(true && *p5 == *p5 && *p1 == *p2 && *p1 != *p5 && *p4 == *p4 && *p4 != *p1);
    assert(true && (*p4).unique() && (*p5).unique() && !(*p1).unique() && !(*p2).unique() && !(*p8).unique());

    p1->swap(*p2);
    Cnt::verify_state({"obj8", "obj5", "obj3"});
    assert(true && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj3" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj8" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5" && (*p8).get() == nullptr);
    assert(true && *p5 == *p5 && *p1 == *p1 && *p1 != *p5 && *p4 == *p4 && *p4 != *p1);
    assert(true && (*p4).unique() && (*p5).unique() && !(*p1).unique() && !(*p2).unique() && !(*p8).unique());

    p4->reset(new CntD("obj9"));
    Cnt::verify_state({"obj9", "obj8", "obj5"});
    assert(true && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj9" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj8" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5" && (*p8).get() == nullptr);
    assert(true && *p4 == *p4 && *p5 == *p5 && *p5 != *p4 && *p1 == *p1 && *p2 != *p5);
    assert(true && (*p4).unique() && (*p5).unique() && !(*p1).unique() && !(*p2).unique() && !(*p8).unique());

    cptr_t * p12 = new cptr_t(*p2);
    Cnt::verify_state({"obj9", "obj8", "obj5"});
    assert(true && *p4 && *p5 && *p12 && *p1 && *p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj9" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj8" && (*p12)->get_this() == (*p12).get() && (*(*p12)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5" && (*p8).get() == nullptr);
    assert(true && *p4 == *p4 && *p5 == *p5 && *p5 != *p4 && *p1 == *p12 && *p12 != *p5);
    assert(true && (*p4).unique() && (*p5).unique() && !(*p12).unique() && !(*p1).unique() && !(*p2).unique() && !(*p8).unique());

    delete p12; p12 = nullptr;
    Cnt::verify_state({"obj9", "obj8", "obj5"});
    assert(true && *p4 && *p5 && *p1 && *p2 && !*p8);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj9" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj8" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5" && (*p8).get() == nullptr);
    assert(true && *p4 == *p4 && *p5 == *p5 && *p5 != *p4 && *p2 == *p1 && *p1 != *p5);
    assert(true && (*p4).unique() && (*p5).unique() && !(*p1).unique() && !(*p2).unique() && !(*p8).unique());

    delete p8; p8 = nullptr;
    Cnt::verify_state({"obj9", "obj8", "obj5"});
    assert(true && *p4 && *p5 && *p1 && *p2);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj9" && (*p5)->get_this() == (*p5).get() && (*(*p5)).get_name() == "obj8" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5");
    assert(true && *p4 == *p4 && *p5 == *p5 && *p5 != *p4 && *p1 == *p1 && *p1 != *p5);
    assert(true && (*p4).unique() && (*p5).unique() && !(*p1).unique() && !(*p2).unique());

    delete p5; p5 = nullptr;
    Cnt::verify_state({"obj9", "obj5"});
    assert(true && *p4 && *p1 && *p2);
    assert(true && (*p4)->get_this() == (*p4).get() && (*(*p4)).get_name() == "obj9" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5");
    assert(true && *p4 == *p4 && *p1 == *p1 && *p1 != *p4);
    assert(true && (*p4).unique() && !(*p1).unique() && !(*p2).unique());

    delete p4; p4 = nullptr;
    Cnt::verify_state({"obj5"});
    assert(true && *p1 && *p2);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5");
    assert(true && *p1 == *p1);
    assert(true && !(*p1).unique() && !(*p2).unique());

    p2->reset(new Cnt("obj10"));
    Cnt::verify_state({"obj10", "obj5"});
    assert(true && *p1 && *p2);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj10");
    assert(true && *p2 == *p2 && *p1 == *p1 && *p1 != *p2);
    assert(true && (*p1).unique() && (*p2).unique());

    cptr_t * p13 = new cptr_t(*p1);
    Cnt::verify_state({"obj10", "obj5"});
    assert(true && *p13 && *p1 && *p2);
    assert(true && (*p13)->get_this() == (*p13).get() && (*(*p13)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj10");
    assert(true && *p2 == *p2 && *p1 == *p13 && *p13 != *p2);
    assert(true && !(*p13).unique() && !(*p1).unique() && (*p2).unique());

    *p1 = *p13;
    Cnt::verify_state({"obj10", "obj5"});
    assert(true && *p13 && *p1 && *p2);
    assert(true && (*p13)->get_this() == (*p13).get() && (*(*p13)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj10");
    assert(true && *p2 == *p2 && *p1 == *p1 && *p13 != *p2);
    assert(true && !(*p13).unique() && !(*p1).unique() && (*p2).unique());

    *p2 = *p1;
    Cnt::verify_state({"obj5"});
    assert(true && *p13 && *p1 && *p2);
    assert(true && (*p13)->get_this() == (*p13).get() && (*(*p13)).get_name() == "obj5" && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5");
    assert(true && *p13 == *p13);
    assert(true && !(*p13).unique() && !(*p1).unique() && !(*p2).unique());

    delete p13; p13 = nullptr;
    Cnt::verify_state({"obj5"});
    assert(true && *p1 && *p2);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5" && (*p2)->get_this() == (*p2).get() && (*(*p2)).get_name() == "obj5");
    assert(true && *p2 == *p2);
    assert(true && !(*p1).unique() && !(*p2).unique());

    delete p2; p2 = nullptr;
    Cnt::verify_state({"obj5"});
    assert(true && *p1);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj5");
    assert(true && *p1 == *p1);
    assert(true && (*p1).unique());

    p1->reset(new Cnt("obj11"));
    Cnt::verify_state({"obj11"});
    assert(true && *p1);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj11");
    assert(true && *p1 == *p1);
    assert(true && (*p1).unique());

    cdptr_t * p14 = new cdptr_t(new CntD("obj12"));
    Cnt::verify_state({"obj11", "obj12"});
    assert(true && *p1 && *p14);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj11" && (*p14)->get_this() == (*p14).get() && (*(*p14)).get_name() == "obj12");
    assert(true && *p1 == *p1 && *p14 == *p14 && *p14 != *p1);
    assert(true && (*p1).unique() && (*p14).unique());

    *p1 = *p1;
    Cnt::verify_state({"obj11", "obj12"});
    assert(true && *p1 && *p14);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj11" && (*p14)->get_this() == (*p14).get() && (*(*p14)).get_name() == "obj12");
    assert(true && *p1 == *p1 && *p14 == *p14 && *p14 != *p1);
    assert(true && (*p1).unique() && (*p14).unique());

    delete p14; p14 = nullptr;
    Cnt::verify_state({"obj11"});
    assert(true && *p1);
    assert(true && (*p1)->get_this() == (*p1).get() && (*(*p1)).get_name() == "obj11");
    assert(true && *p1 == *p1);
    assert(true && (*p1).unique());

    delete p1; p1 = nullptr;
    Cnt::verify_state({});
    assert(true);
    assert(true);
    assert(true);
    assert(true);

    cdptr_t * p15 = new cdptr_t(new CntD("obj13"));
    Cnt::verify_state({"obj13"});
    assert(true && *p15);
    assert(true && (*p15)->get_this() == (*p15).get() && (*(*p15)).get_name() == "obj13");
    assert(true && *p15 == *p15);
    assert(true && (*p15).unique());

    delete p15; p15 = nullptr;
    Cnt::verify_state({});
    assert(true);
    assert(true);
    assert(true);
    assert(true);

    return 0;
}
