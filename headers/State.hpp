#ifndef MEALY_STATE_HPP
#define MEALY_STATE_HPP

struct State {
    unsigned m_index;
    bool m_isOk = true;

    State() : m_index() {}
    State(unsigned i) : m_index(i) {}

    State& operator= (unsigned a) {
        m_index = a;
        return *this;
    }

    inline unsigned Index() const { return m_index; }
    inline bool IsOk() const { return m_isOk; }
    void SetFail() { m_isOk = false; }    
    void SetGood() { m_isOk = true; }
    
    static inline char failStateSymbol = '-';
};

struct Signal {
    unsigned m_index;

    Signal() : m_index() {}
    Signal(unsigned i) : m_index(i) {}

    Signal& operator= (unsigned a) {
        m_index = a;
        return *this;
    }

    inline unsigned Index() const {
        return m_index;
    }
};

inline bool operator< (State const& lhs, State const& rhs) {
    return lhs.m_index < rhs.m_index;
}

inline bool operator> (State const& lhs, State const& rhs) {
    return lhs.m_index > rhs.m_index;
}

inline bool operator<= (State const& lhs, State const& rhs) {
    return lhs.m_index <= rhs.m_index;
}

inline bool operator>= (State const& lhs, State const& rhs) {
    return lhs.m_index >= rhs.m_index;
}

inline bool operator== (State const& lhs, State const& rhs) {
    return lhs.m_index == rhs.m_index;
}

inline bool operator< (Signal const& lhs, Signal const& rhs) {
    return lhs.m_index < rhs.m_index;
}

inline bool operator> (Signal const& lhs, Signal const& rhs) {
    return lhs.m_index > rhs.m_index;
}

inline bool operator<= (Signal const& lhs, Signal const& rhs) {
    return lhs.m_index <= rhs.m_index;
}

inline bool operator>= (Signal const& lhs, Signal const& rhs) {
    return lhs.m_index >= rhs.m_index;
}

inline bool operator== (Signal const& lhs, Signal const& rhs) {
    return lhs.m_index == rhs.m_index;
}

inline std::ostream& operator<< (std::ostream& output, State const& state) {
    return output << state.m_index;
}

inline std::ostream& operator<< (std::ostream& output, Signal const& signal) {
    return output << signal.m_index;
}

#endif // MEALY_STATE_HPP