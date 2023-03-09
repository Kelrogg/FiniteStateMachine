#ifndef UTILITY_HPP 
#define UTILITY_HPP

#include "State.hpp"

template<typename Iterator>
inline unsigned stounsigned(Iterator begin, Iterator end, unsigned radix = 10)
{
    unsigned result = 0;
    for (Iterator it = std::prev(end); it >= begin; --it) {
        result *= radix;
        result += *it - '0';
    }
    return result;
}

struct StateHasher {
    std::size_t operator()(State state) const {
        if (!state.IsOk()) return 0;

        std::hash<unsigned> g;
        return g(state.Index());
    }
    std::size_t operator()(unsigned state) const {
        std::hash<unsigned> g;
        return g(state);
    }
};

template<typename T, typename U>
struct PairHasher {
    std::size_t operator()(std::pair<T*, U> const& value) const {
        std::size_t const& ptr = reinterpret_cast<std::size_t const&>(value.first);
        std::hash<std::size_t> lhs;
        std::hash<unsigned> type;
        return lhs(ptr) ^ type(value.second.Index());
    }
};

#endif