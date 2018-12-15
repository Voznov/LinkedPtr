#include <iostream>
#include <memory>
#include <map>
#include <cassert>
#include "linked_ptr.hpp"

using namespace std;
using namespace smart_ptr;
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

struct A
{
public:
    int i;
    virtual void set(int j)
    {
    	i = j;
    }
    virtual ~A()
	{}
};

struct B : A
{
public:
	void set(int j)
    {
		i = j;
    }
};

struct C
{
public:
	void set(int j)
	{

	}
};

template <typename _Type, typename = typename enable_if<is_convertible<_Type *, A *>::value>::type>
void check(_Type * ptr)
{
	cerr << "kekT " << is_convertible<_Type *, _Type *>::value << endl;
	cerr << "kekA " << is_convertible<_Type *, A *>::value << endl;
	cerr << "kekB " << is_convertible<_Type *, B *>::value << endl;
}

int main()
{
//	int intA = 345, intB = 678;
//	int intC = 345, intD = 678;
//	A aaa = intA, aaa_ = intA;
//	B bbb = intB, bbb_ = intB;

	A aaa, aaa_;
	B bbb, bbb_;
	aaa.set(1); aaa_.set(2);
	bbb.set(3); bbb_.set(4);

	linked_ptr<A> * aL = new linked_ptr<A>(new B);
	linked_ptr<A> * bL = new linked_ptr<A>(*aL);
	shared_ptr<A> * aP = new shared_ptr<A>(new B);
	shared_ptr<A> * bP = new shared_ptr<A>(*aP);

	cerr << (*aP == *bP) << " P " << endl;// << (*aP == nullptr) << " p " << (nullptr == *aP) << endl;
	cerr << (*aL == *bL) << " L " << endl;// << (*aL == nullptr) << " l " << (nullptr == *aL) << endl;

	cerr << is_convertible<B*, A*>::value << endl;
	cerr << is_convertible<A*, B*>::value << endl;

	check(new A);
	check(new B);
//	check(new C);

    cerr << "-----" << endl;

    linked_ptr<Cnt> * cnDL = new linked_ptr<Cnt>(new CntD("cnDL"));
    linked_ptr<Cnt> * cntL = new linked_ptr<Cnt>(*cnDL);
    shared_ptr<Cnt> * cnDP = new shared_ptr<Cnt>(new CntD("cnDP"));
    shared_ptr<Cnt> * cntP = new shared_ptr<Cnt>(*cnDP);

    cerr << &**cntL << ' ' << &**cnDL << endl;
    cerr << &**cntP << ' ' << &**cnDP << endl;

    cntL->reset(new CntD("kek"));
    cntP->reset(new CntD("lol"));

    cerr << &**cntL << ' ' << &**cnDL << endl;
    cerr << &**cntP << ' ' << &**cnDP << endl;

    linked_ptr<CntD> cntd(new CntD("cntd"));
//    linked_ptr<Cnt> cnt = cntd;
    shared_ptr<CntD> cntd_(new CntD("cntd_"));
    shared_ptr<Cnt> cnt_ = cntd_;

    cerr << "-----" << endl;

    string s = "kek";
    shared_ptr<string> D(new string(s));
    cerr << D.unique() << ' ' << D.get() << endl;
    shared_ptr<string> d(D);
    cerr << d.unique() << ' ' << d.get() << endl;
    d = D;
    cerr << d.unique() << ' ' << d.get() << endl;

    linked_ptr<int> b;
    linked_ptr<int> b2(new int);

    b.swap(b2);
    assert(b.unique());

	return 0;
}
