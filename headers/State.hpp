#ifndef STATE_HPP
#define STATE_HPP

struct State {
    unsigned m_index;
    bool m_isOk = true;
    bool m_isTerm = false;

    static inline char failStateSymbol = '-';

    State() : m_index() {}
    State(unsigned i) : m_index(i) {}

    State& operator= (unsigned a) noexcept {
        m_index = a;
        return *this;
    }

    inline unsigned Index() const noexcept { return m_index; }
    inline bool IsOk() const noexcept { return m_isOk; }
    inline bool IsTerm() const noexcept { return m_isTerm; }
    void SetFail() noexcept { m_isOk = false; }    
    void SetGood() noexcept { m_isOk = true; }
    

    inline State& operator-= (State const& rhs) {
        m_index -= rhs.m_index;
        return *this;
    }
    
    inline State& operator+= (State const& rhs) {
        m_index += rhs.m_index;
        return *this;
    }
    
    inline unsigned operator- (State const& rhs) {
        return m_index - rhs.m_index;
    }

    inline unsigned operator+ (State const& rhs) {
        return m_index + rhs.m_index;
    }
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

#endif // STATE_HPP