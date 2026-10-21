/*
 * A288419  a(n) = Sum_{d|n} d^3*A000593(n/d).
 * https://oeis.org/A288419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d^3*A000593(n/d) (multiplicative)
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 1469, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow_uint128(unsigned long long base, idx_t exp)
{
    /* compute base^exp using 128‑bit intermediate to avoid overflow   */
    unsigned __int128 r = 1;
    unsigned __int128 b = (unsigned __int128)base;
    for (idx_t i = 0; i < exp; ++i)
        r *= b;
    return (seq_t)r;
}

static inline seq_t prime_power_val(uint64_t p, idx_t e)
{
    if (p == 2) {                       /* a(2^e) = (8^(e+1)-1)/7          */
        unsigned __int128 v = 1;
        for (idx_t i = 0; i < e + 1; ++i)
            v *= 8;
        v = (v - 1) / 7;
        return (seq_t)v;
    }

    /* p > 2 : a(p^e) = (p^(3e+5) - (p^2+p+1)*p^(e+1) + p + 1)
/*(p^3-1)*(p^2-1)  */
    unsigned __int128 p2 = (unsigned __int128)p * p;
    unsigned __int128 p3 = p2 * p;

    unsigned __int128 denom = (p3 - 1) * (p2 - 1);

    unsigned __int128 pow_e1 = 1;
    for (idx_t i = 0; i < e + 1; ++i)
        pow_e1 *= p;

    unsigned __int128 pow_3e5 = 1;
    for (idx_t i = 0; i < 3 * e + 5; ++i)
        pow_3e5 *= p;

    unsigned __int128 term2 = (p2 + p + 1) * pow_e1;
    unsigned __int128 term3 = (unsigned __int128)p + 1;

    unsigned __int128 numer = pow_3e5 - term2 + term3;
    return (seq_t)(numer / denom);
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    for (uint64_t p = 2; p <= m / p; p = (p == 2 ? 3 : p + 2)) {
        if (m % p != 0)
            continue;
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        result *= prime_power_val(p, e);
    }
    if (m > 1) /* remaining prime factor */
        result *= prime_power_val(m, 1);

    return result;
}
