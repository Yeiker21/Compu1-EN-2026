#pragma once
#include <concepts>
#include <vector>
#include <string>
#include <functional>
#include <tuple>

template <typename t>
struct Eq;

template <typename t>
concept EqImplementation = requires (t x, t y)
  {
    { x == y } -> std::same_as<bool>;
    { x != y } -> std::same_as<bool>;
  };

template<>
struct Eq<int>
  { static bool eq(int x, int y) { return x == y; }
    static bool neq(int x, int y) { return x != y; }
  };

template <>
struct Eq<double>
  { static bool eq(double x, double y) { return x == y; }
    static bool neq(double x, double y) { return x != y; }
  };

template<>
struct Eq<std::string>
  { static bool eq(std::string x, std::string y) { return x == y; }
    static bool neq(std::string x, std::string y) { return x != y; }
  };

template<>
struct Eq<bool>
  { static bool eq(bool x, bool y) { return x == y; }
    static bool neq(bool x, bool y) { return x != y; }
  };


template <typename t>
struct Show;

template <typename t>
concept ShowImplementation = requires (t x)
  {
    { Show<t>::show(x) } -> std::same_as<std::string>;
  };

template<>
struct Show<int>
  { static std::string show(int x) { return std::to_string(x); } };

template <>
struct Show<double>
  { static std::string show(double x) { return std::to_string(x); } };

template<>
struct Show<std::string>
  { static std::string show(std::string x) { return x; } };

template<>
struct Show<bool>
  { static std::string show(bool x) { return x ? "true" : "false"; } };


template <typename t>
class List
  {
    private:
      std::vector<t> _inner;
      List<t> clone() const
      { List<t> xs;
        xs._inner = this->_inner;
        return xs;
      }
    public:
      List() {}
      List(List<t> const& other) : _inner(other._inner)  {}
      List<t> cons (t x) const
      { List<t> xs(*this);
        xs._inner.push_back(x);
        return xs;
      }
      List<t> tail() const
      { List<t> xs(*this);
        if (xs._inner.size() > 0)
          xs._inner.pop_back();
        return xs;
      }
      t head() const
      { return _inner.back(); }
      bool isEmpty() const
      { return _inner.empty(); }
  };

template<typename a, typename b>
  requires ShowImplementation<a> && ShowImplementation<b>
struct Show<std::tuple<a, b>>
  { static std::string show(std::tuple<a, b> t)
    { return "(" + Show<a>::show(std::get<0>(t)) + ", " + Show<b>::show(std::get<1>(t)) + ")"; }
  };


template <typename t>
  requires ShowImplementation<t>
struct Show<List<t>>
  { static std::string show(List<t> xs)
    { std::string s = "[";
      while(!xs.isEmpty())
        { s += Show<t>::show(xs.head());
          xs = xs.tail();
          if (!xs.isEmpty())
            s += ", ";
        }
      s += "]";
      return s;
    } };

template <typename t>
  requires EqImplementation<t>
struct Eq<List<t>>
  { static bool eq(List<t> xs, List<t> ys)
    { if (xs.isEmpty() && ys.isEmpty())
        return true;
      if (xs.isEmpty() || ys.isEmpty())
        return false;
      if (!Eq<t>::eq(xs.head(), ys.head()))
        return false;
      return eq(xs.tail(), ys.tail());
    } };


template <typename t>
  requires ShowImplementation<t>
std::string show(const t& x) {
    return Show<t>::show(x);
}

template <typename t>
  requires EqImplementation<t>
bool operator==(const List<t>& xs, const List<t>& ys) {
    return Eq<List<t>>::eq(xs, ys);
}

template <typename t>
  requires EqImplementation<t>
bool operator!=(const List<t>& xs, const List<t>& ys) {
    return !Eq<List<t>>::eq(xs, ys);
}

template <typename a, typename b, typename f_b_a_b>
  requires std::invocable<f_b_a_b, b, a>
    && std::convertible_to<f_b_a_b,std::function<b(b, a)>>
b reduceLeft(f_b_a_b f, b init, List<a> const& xs ) {
 b acc = init;
    List<a> temp = xs;
    while (!temp.isEmpty()) {
        acc = f(acc, temp.head());
        temp = temp.tail();
    }
    return acc;
 }
template <typename a, typename b, typename f_b_a_b>
  requires std::invocable<f_b_a_b, b, a>
    && std::convertible_to<f_b_a_b,std::function<b(b, a)>>
b reduceLeftRec(f_b_a_b f, b init, List<a> const& xs )
{ if (xs.isEmpty()) return init;
    return reduceLeftRec(f, f(init, xs.head()), xs.tail());}

template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> map(f_a_b f, List<a> const& xs )
  {
    List<b> res;
    List<a> temp = xs;
    while (!temp.isEmpty()) {
        res = res.cons(f(temp.head()));
        temp = temp.tail();
    }
    return res;
}

template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> mapRec(f_a_b f, List<a> const& xs )
  {
    if (xs.isEmpty()) return List<b>();
    return mapRec(f, xs.tail()).cons(f(xs.head()));
}

template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> mapReduce(f_a_b f, List<a> const& xs )
 {
    return reduceLeft([f](List<b> acc, a val) {
        return acc.cons(f(val));
    }, List<b>(), xs);
} 

template <typename t>
List<t> concat(List<List<t>> const& xss) {
  return reduceLeft([](List<t> acc, List<t> sub) {
        return reduceLeft([](List<t> iAcc, t v) { return iAcc.cons(v); }, acc, sub);
    }, List<t>(), xss); }


template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filter(f_a_b f, List<a> const& xs )  {
  List<a> res;
    List<a> temp = xs;
    while (!temp.isEmpty()) {
        if (f(temp.head())) res = res.cons(temp.head());
        temp = temp.tail();
    }
    return res;
 }

template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterRec(f_a_b f, List<a> const& xs )
  {if (xs.isEmpty()) return List<a>();
    List<a> r = filterRec(f, xs.tail());
    return f(xs.head()) ? r.cons(xs.head()) : r;}

template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterMap(f_a_b f, List<a> const& xs )
  {auto m = map([f](a v) { 
        List<a> l; 
        return f(v) ? l.cons(v) : l; 
    }, xs);
    return concat(m);}

template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterReduce(f_a_b f, List<a> const& xs )
  {return reduceLeft([f](List<a> acc, a v) {
        return f(v) ? acc.cons(v) : acc;
    }, List<a>(), xs);}

template <typename t>
  requires EqImplementation<t>
List<List<t>> groupEqual(List<t> const& xs)
  {return reduceLeft([](List<List<t>> acc, t v) {
        if (acc.isEmpty()) return acc.cons(List<t>().cons(v));
        List<t> last = acc.head();
        if (Eq<t>::eq(v, last.head())) return acc.tail().cons(last.cons(v));
        return acc.cons(List<t>().cons(v));
    }, List<List<t>>(), xs);}

template <typename t>
  requires EqImplementation<t>
List<std::tuple<t, int>> encode(List<t> const& xs)
  {return reduceLeft([](List<std::tuple<t, int>> acc, List<t> sub) {
        int n = 0;
        List<t> tmp = sub;
        while(!tmp.isEmpty()) { n++; tmp = tmp.tail(); }
        return acc.cons(std::make_tuple(sub.head(), n));
    }, List<std::tuple<t, int>>(), groupEqual(xs));}

template <typename t>
  requires EqImplementation<t>
List<t> decode(List<std::tuple<t, int>> const& xss)
  {return reduceLeft([](List<t> acc, std::tuple<t, int> p) {
        List<t> tmp = acc;
        for(int i=0; i<std::get<1>(p); i++) tmp = tmp.cons(std::get<0>(p));
        return tmp;
    }, List<t>(), xss);}

template <typename a, typename b>
  List<std::tuple<a,b>> zip(List<a> const& xs, List<b> const& ys)
  {if (xs.isEmpty() || ys.isEmpty()) return List<std::tuple<a,b>>();
    return zip(xs.tail(), ys.tail()).cons(std::make_tuple(xs.head(), ys.head()));}

template <typename t>
  List<t> dropEveryN(int n, List<t> const& xs)
  {List<int> idx; int c = 1; List<t> tmp = xs;
    while(!tmp.isEmpty()) { idx = idx.cons(c++); tmp = tmp.tail(); }
    return map([](auto p){return std::get<0>(p);}, 
           filter([n](auto p){return std::get<1>(p)%n != 0;}, zip(xs, idx)));}


template <typename t, typename f_a_b>
  requires std::invocable<f_a_b, t, t>
    && std::convertible_to<f_a_b,std::function<bool(t, t)>>
  List<t> sortBy(List<t> const& xs, f_a_b f)
  {if (xs.isEmpty()) return xs;
    t p = xs.head(); List<t> r = xs.tail();
    List<t> l = filter([&](t x){return f(x,p);}, r);
    List<t> rg = filter([&](t x){return !f(x,p);}, r);
    return concat(List<List<t>>().cons(sortBy(rg,f)).cons(List<t>().cons(p)).cons(sortBy(l,f)));}

template <typename t>
  List<t> rotateByN(List<t> const& xs, int n)
  {int s = 0; List<t> tmp = xs;
    while(!tmp.isEmpty()) { s++; tmp = tmp.tail(); }
    if(s == 0) return xs;
    int sh = n % s;
    List<int> ids; for(int i=0; i<s; i++) ids = ids.cons(i);
    auto srt = sortBy(zip(xs, ids), [sh, s](auto a, auto b) {
        return ((std::get<1>(a)-sh+s)%s) <= ((std::get<1>(b)-sh+s)%s);
    });
    return map([](auto p){return std::get<0>(p);}, srt);}
