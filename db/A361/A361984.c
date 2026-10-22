/*
 * A361984  a(1) = 1, a(2) = 0; a(n) = Sum_{d|n, d < n} (-1)^(n/d) a(d).
 * https://oeis.org/A361984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = (multiplicative) a(2^e) * ∏ a(p_i) * where a(2) = 0,
 *            a(2^e) = 2^(e-2) (e>1), a(p) = -1 for odd primes, * a(p^e) = 0
 *            for odd primes with e>1. Thus a(n)=0 iff n has a factor 2^1 * or
 *            an odd prime squared; otherwise a(n) = * (2^(k-2) if n contains
 *            2^k, k≥2) * (-1)^(number of distinct odd primes)
 * Range:     i8 n <= 511, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow2(idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) {
        r *= 2;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;

    seq_t result = 1;
    idx_t m = n;
    /* factor 2 */
    idx_t e2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        e2++;
    }
    if (e2 > 0) {
        if (e2 == 1) return 0;                  /* 2^1 gives zero factor */
        result *= pow2(e2 - 2);                 /* 2^(e2-2) for e2>1 */
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t ep = 0;
            while (m % p == 0) {
                m /= p;
                ep++;
                if (ep > 1) return 0;           /* odd prime squared */
            }
            result *= (seq_t)-1;                /* ep == 1 */
        }
    }
    if (m > 1) {
        /* remaining prime > 1, exponent 1 */
        result *= (seq_t)-1;
    }
    return result;
}
