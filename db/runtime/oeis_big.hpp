// oeis_big.hpp -- the arbitrary-precision seq_t of the OEIS C-code database.
//
// Every sequence function is written once, against `seq_t`, in code that is
// valid C and C++ alike. The fixed-width versions are that code compiled as C
// with seq_t a native integer type; the arbitrary-precision version is the
// same code compiled as C++ with seq_t = oeis_big.
//
// oeis_big wraps GMP's mpz_class so that it mixes with every native integer
// type the way a native integer does. mpz_class alone has no long long or
// __int128 overloads, so `x *= k` with `long long k` would not compile. The
// semantics are C's: / and % truncate toward zero, and converting back to a
// native integer (always explicit, `(long long)x`) keeps the low bits.
//
// Build: g++ -std=gnu++17 ... -lgmpxx -lgmp
#ifndef OEIS_BIG_HPP
#define OEIS_BIG_HPP

#include <gmpxx.h>
#include <cstdint>
#include <ostream>
#include <string>
#include <type_traits>

class oeis_big
{
    mpz_class v;

    template <typename T>
    static constexpr bool native = std::is_integral_v<T> || std::is_same_v<T, __int128> ||
                                   std::is_same_v<T, unsigned __int128>;

    static mpz_class from_u128(unsigned __int128 x)
    {
        mpz_class hi(static_cast<unsigned long>(x >> 64)), lo(static_cast<unsigned long>(x));
        return (hi << 64) + lo;
    }

    template <typename T>
    static mpz_class from(T x)
    {
        if constexpr (sizeof(T) > 8)
        {
            if constexpr (std::is_same_v<T, __int128>)
                return x < 0 ? mpz_class(-from_u128(-(unsigned __int128)x))
                             : from_u128((unsigned __int128)x);
            else
                return from_u128(x);
        }
        else if constexpr (std::is_signed_v<T>)
            return mpz_class(static_cast<long>(x));          // long is 64-bit (LP64)
        else
            return mpz_class(static_cast<unsigned long>(x));
    }

    // the low 128 bits, two's complement
    unsigned __int128 low128() const
    {
        mpz_class m;
        mpz_fdiv_r_2exp(m.get_mpz_t(), v.get_mpz_t(), 128);  // in [0, 2^128)
        mpz_class hi = m >> 64, lo = m - (hi << 64);
        return ((unsigned __int128)mpz_get_ui(hi.get_mpz_t()) << 64) | mpz_get_ui(lo.get_mpz_t());
    }

    static oeis_big wrap(const mpz_class &m)
    {
        oeis_big r;
        r.v = m;
        return r;
    }

public:
    oeis_big() : v(0) {}
    template <typename T, std::enable_if_t<native<T>, int> = 0>
    oeis_big(T x) : v(from(x)) {}

    // back to a native integer: explicit, keeping the low bits like a C cast
    template <typename T, std::enable_if_t<native<T>, int> = 0>
    explicit operator T() const { return static_cast<T>(low128()); }
    explicit operator bool() const { return v != 0; }

    const mpz_class &mpz() const { return v; }
    std::string str() const { return v.get_str(); }

    // arithmetic; a native operand converts implicitly
    friend oeis_big operator+(const oeis_big &a, const oeis_big &b) { return wrap(a.v + b.v); }
    friend oeis_big operator-(const oeis_big &a, const oeis_big &b) { return wrap(a.v - b.v); }
    friend oeis_big operator*(const oeis_big &a, const oeis_big &b) { return wrap(a.v * b.v); }
    friend oeis_big operator/(const oeis_big &a, const oeis_big &b) { return wrap(a.v / b.v); }   // truncates
    friend oeis_big operator%(const oeis_big &a, const oeis_big &b) { return wrap(a.v % b.v); }   // sign of a
    friend oeis_big operator&(const oeis_big &a, const oeis_big &b) { return wrap(a.v & b.v); }
    friend oeis_big operator|(const oeis_big &a, const oeis_big &b) { return wrap(a.v | b.v); }
    friend oeis_big operator^(const oeis_big &a, const oeis_big &b) { return wrap(a.v ^ b.v); }
    template <typename K, std::enable_if_t<native<K>, int> = 0>
    friend oeis_big operator<<(const oeis_big &a, K k) { return wrap(a.v << (mp_bitcnt_t)k); }
    template <typename K, std::enable_if_t<native<K>, int> = 0>
    friend oeis_big operator>>(const oeis_big &a, K k) { return wrap(a.v >> (mp_bitcnt_t)k); }
    oeis_big operator-() const { return wrap(-v); }
    oeis_big operator+() const { return *this; }
    oeis_big operator~() const { return wrap(~v); }
    bool operator!() const { return v == 0; }

    oeis_big &operator+=(const oeis_big &b) { v += b.v; return *this; }
    oeis_big &operator-=(const oeis_big &b) { v -= b.v; return *this; }
    oeis_big &operator*=(const oeis_big &b) { v *= b.v; return *this; }
    oeis_big &operator/=(const oeis_big &b) { v /= b.v; return *this; }
    oeis_big &operator%=(const oeis_big &b) { v %= b.v; return *this; }
    oeis_big &operator&=(const oeis_big &b) { v &= b.v; return *this; }
    oeis_big &operator|=(const oeis_big &b) { v |= b.v; return *this; }
    oeis_big &operator^=(const oeis_big &b) { v ^= b.v; return *this; }
    template <typename K, std::enable_if_t<native<K>, int> = 0>
    oeis_big &operator<<=(K k) { v <<= (mp_bitcnt_t)k; return *this; }
    template <typename K, std::enable_if_t<native<K>, int> = 0>
    oeis_big &operator>>=(K k) { v >>= (mp_bitcnt_t)k; return *this; }
    oeis_big &operator++() { ++v; return *this; }
    oeis_big &operator--() { --v; return *this; }
    oeis_big operator++(int) { oeis_big t = *this; ++v; return t; }
    oeis_big operator--(int) { oeis_big t = *this; --v; return t; }

    friend bool operator==(const oeis_big &a, const oeis_big &b) { return a.v == b.v; }
    friend bool operator!=(const oeis_big &a, const oeis_big &b) { return a.v != b.v; }
    friend bool operator<(const oeis_big &a, const oeis_big &b) { return a.v < b.v; }
    friend bool operator<=(const oeis_big &a, const oeis_big &b) { return a.v <= b.v; }
    friend bool operator>(const oeis_big &a, const oeis_big &b) { return a.v > b.v; }
    friend bool operator>=(const oeis_big &a, const oeis_big &b) { return a.v >= b.v; }

    friend std::ostream &operator<<(std::ostream &os, const oeis_big &a) { return os << a.v; }
};

#endif
