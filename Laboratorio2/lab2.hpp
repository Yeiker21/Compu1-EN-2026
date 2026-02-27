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
b reduceLeft(f_b_a_b f, b init, List<a> const& xs )
  { b acc = init;
    List<a> actual = xs; // Hacemos una copia porque xs es const
    
    while (!actual.isEmpty()) { // Si isEmpty() te da error luego, prueba con empty()
        acc = f(acc, actual.head());
        actual = actual.tail();
    }
    
    return acc;
  }

template <typename a, typename b, typename f_b_a_b>
  requires std::invocable<f_b_a_b, b, a>
    && std::convertible_to<f_b_a_b,std::function<b(b, a)>>
b reduceLeftRec(f_b_a_b f, b init, List<a> const& xs )
  {if (xs.isEmpty()) {
        return init;
    }
    
    return reduceLeftRec(f, f(init, xs.head()), xs.tail());
  }

template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> map(f_a_b f, List<a> const& xs )
  {
    List<b> res;
    List<a> actual = xs;
    while (!actual.isEmpty()) {
        res = res.cons(f(actual.head()));
        actual = actual.tail();
    }
    return res;
}

template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> mapRec(f_a_b f, List<a> const& xs )
  {
    if (xs.isEmpty()) {
        return List<b>();
    }
    return mapRec(f, xs.tail()).cons(f(xs.head()));

template <typename a, typename b, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<b(a)>>
List<b> mapReduce(f_a_b f, b init, List<a> const& xs )
  {
    return reduceLeft(f, init, map(f, xs));
}
   


template <typename t>
List<t> concat(List<List<t>> const& xss)
  {
    List<t> res;
    List<List<t>> actual = xss;
    while (!actual.isEmpty()) {
        List<t> sublista = actual.head();
        while (!sublista.isEmpty()) {
            res = res.cons(sublista.head());
            sublista = sublista.tail();
        }
        actual = actual.tail();
    }
    return res;
}


template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filter(f_a_b f, List<a> const& xs )
  {
    List<a> res;
    List<a> actual = xs;
    while (!actual.isEmpty()) {
        if (f(actual.head())) {
            res = res.cons(actual.head());
        }
        actual = actual.tail();
    }
    return res;
}


template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterRec(f_a_b f, List<a> const& xs )
  {
    if (xs.isEmpty()) return List<a>();
    if (f(xs.head())) {
        return filterRec(f, xs.tail()).cons(xs.head());
    }
    return filterRec(f, xs.tail());
}

template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterMap(f_a_b f, List<a> const& xs )
  {
    return map(f, filter(f, xs));
}

template <typename a, typename f_a_b>
  requires std::invocable<f_a_b, a>
    && std::convertible_to<f_a_b,std::function<bool(a)>>
List<a> filterReduce(f_a_b f, List<a> const& xs )
  {
    return reduceLeft(f, init, filter(f, xs));
}

template <typename t>
  requires EqImplementation<t>
List<List<t>> groupEqual(List<t> const& xs)
  {
    if (xs.isEmpty()) return List<List<t>>();
    List<List<t>> res;
    List<t> actual = xs;
    while (!actual.isEmpty()) {
        t valor = actual.head();
        List<t> grupo;
        while (!actual.isEmpty() && actual.head() == valor) {
            grupo = grupo.cons(actual.head());
            actual = actual.tail();
        }
        res = res.cons(grupo);
    }
    return res;
}

template <typename t>
  requires EqImplementation<t>
List<std::tuple<t, int>> encode(List<t> const& xs)
  {
    List<List<t>> grupos = groupEqual(xs);
    List<std::tuple<t, int>> res;
    List<List<t>> actual = grupos;
    while (!actual.isEmpty()) {
        List<t> g = actual.head();
        int contador = 0;
        List<t> temp = g;
        while (!temp.isEmpty()) {
            contador++;
            temp = temp.tail();
        }
        res = res.cons(std::make_tuple(g.head(), contador));
        actual = actual.tail();
    }
    return res;
}

template <typename t>
  requires EqImplementation<t>
List<t> decode(List<std::tuple<t, int>> const& xss)
  {
    List<t> res;
    List<std::tuple<t, int>> actual = xss;
    while (!actual.isEmpty()) {
        std::tuple<t, int> pareja = actual.head();
        for (int i = 0; i < std::get<1>(pareja); ++i) {
            res = res.cons(std::get<0>(pareja));
        }
        actual = actual.tail();
    }
    return res;
}

template <typename a, typename b>
  List<std::tuple<a,b>> zip(List<a> const& xs, List<b> const& ys)
 {
    List<std::tuple<a, b>> res;
    List<a> actualX = xs;
    List<b> actualY = ys;
    while (!actualX.isEmpty() && !actualY.isEmpty()) {
        res = res.cons(std::make_tuple(actualX.head(), actualY.head()));
        actualX = actualX.tail();
        actualY = actualY.tail();
    }
    return res;
}

template <typename t>
  List<t> dropEveryN(int n, List<t> const& xs)
  {
    List<t> res;
    List<t> actual = xs;
    int i = 1;
    while (!actual.isEmpty()) {
        if (i % n != 0) {
            res = res.cons(actual.head());
        }
        actual = actual.tail();
        i++;
    }
    return res;
}


template <typename t, typename f_a_b>
  requires std::invocable<f_a_b, t, t>
    && std::convertible_to<f_a_b,std::function<bool(t, t)>>
  List<t> sortBy(List<t> const& xs, f_a_b f)
  {
    if (xs.isEmpty()) return List<t>();
    t pivot = xs.head();
    List<t> restos = xs.tail();
    
    auto menores = filter([&](t x) { return f(x, pivot); }, restos);
    auto mayores = filter([&](t x) { return !f(x, pivot); }, restos);
    
    return concat(List<List<t>>().cons(sortBy(mayores, f)).cons(List<t>().cons(pivot)).cons(sortBy(menores, f)));
}

template <typename t>
  List<t> rotateByN(List<t> const& xs, int n)
  {
    if (xs.isEmpty()) return xs;
    int len = 0;
    List<t> temp = xs;
    while (!temp.isEmpty()) { 
        len++; 
        temp = temp.tail(); 
    }
    
    int realN = n % len;
    if (realN < 0) realN += len;
    
    List<t> res = xs;
    for (int i = 0; i < realN; ++i) {
        t h = res.head();
        res = res.tail().cons(h);
    }
    return res;
}
