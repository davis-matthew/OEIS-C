/*
 * A279947  Expansion of f(x^2, x^2) * f(-x, -x^5) in powers of x where f(, )
 *          is Ramanujan's general theta function.
 * https://oeis.org/A279947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A279947: a(n) = b(3*n + 1) where b is a multiplicative function
 * defined by
 *   b(2^e)   = -(-1)^e
 *   b(3^e)   = (-1)^e
 *   b(p^e)   = e+1               if p ≡ 1,7 (mod 24)
 *   b(p^e)   = (e+1)(-1)^e       if p ≡ 5,11 (mod 24)
 *   b(p^e)   = (1+(-1)^e)/2      if p ≡ 13,17,19,23 (mod 24)
 * The function below factorises 3*n+1 by trial division and multiplies the
 * contributions of each prime power.  All arithmetic is performed in
 * signed int64_t internally to avoid over‑flows for the input range of
 * the test data, and the final result is cast to seq_t.
 */

#include <stdlib.h>
#include <stdint.h>

#define LOCAL_MIN(a,b) ((a) < (b) ? (a) : (b))

/* return the contribution of p^exp according to the rules above */
static inline int64_t prime_power_value(uint64_t p, unsigned int exp)
{
    if (p == 2) {
        /* b(2^e) = -(-1)^e = (-1)^(e+1) */
        return ((exp & 1) == 0) ? -1LL : 1LL;
    }
    if (p == 3) {
        return ((exp & 1) == 0) ? 1LL : -1LL;
    }

    uint64_t mod24 = p % 24ULL;
    if (mod24 == 1 || mod24 == 7) {            /* e+1 */
        return (int64_t)(exp + 1);
    }
    if (mod24 == 5 || mod24 == 11) {           /* (e+1)(-1)^e */
        return ((exp & 1) == 0) ? (int64_t)(exp + 1) : -(int64_t)(exp + 1);
    }
    /* 13,17,19,23 : (1+(-1)^e)/2 -> 1 if even, 0 if odd */
    return ((exp & 1) == 0) ? 1LL : 0LL;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* 3*n+1 fits into 64 bits for the test range */
    uint64_t m = 3ULL * n + 1ULL;
    int64_t result = 1LL;             /* use signed 64‑bit intermediate */

    /* trial division */
    for (uint64_t p = 2ULL; p * p <= m; ++p) {
        if (m % p == 0ULL) {
            unsigned int exp = 0U;
            while (m % p == 0ULL) {
                m /= p;
                ++exp;
            }
            result *= prime_power_value(p, exp);
            if (result == 0LL)    /* early exit */
                return (seq_t)0;
        }
    }
    /* remaining prime factor */
    if (m > 1ULL)
        result *= prime_power_value(m, 1U);

    return (seq_t)result;
}
