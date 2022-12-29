
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