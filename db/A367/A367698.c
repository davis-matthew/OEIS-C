/*
 * A367698  The smallest divisor d of n such that n/d is an exponentially
 *          odious number (A270428).
 * https://oeis.org/A367698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, base
 * Formula:   a(n) = the smallest divisor d of n such that n/d is an
 *            exponentially odious number (A270428). This function computes
 *            a(n) multiplicatively using the fact that a(p^e) = p^(e - s(e))
 *            where s(e) is the largest odious integer ≤ e. An integer is
 *            odious if it has an odd number of 1-bits in binary
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration of the utility function provided by the build. */
idx_t _floor_isqrt_u64(idx_t n);

/* Cache for odious exponent lookup: s(e) for e up to 64. */
static bool _odious_pre = false;
static unsigned _smax[65];

/* Initializes the table of s(e). */
static void init_odious_table(void) {
    if (_odious_pre) return;
    for (unsigned i = 0; i <= 64; ++i) {
        _smax[i] = 0;                       /* default for e=0 */
        bool odious = (__builtin_popcount(i) & 1);
        if (i == 0) continue;
        unsigned k = 0;
        for (unsigned t = i; t >= 1; --t) {
            if ((__builtin_popcount(t) & 1)) { k = t; break; }
        }
        _smax[i] = k;                       /* largest odious ≤ i */
    }
    _odious_pre = true;
}

/* Computes the term a(n). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined for n=0, return 0 */

    init_odious_table();

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    /* Naive trial division sufficient for n ≤ 10 000; works for larger n
       up to the 64‑bit limit as well. */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            unsigned exp_out = e - _smax[e];
            if (exp_out > 0) {
                seq_t pow = 1;
                for (unsigned i = 0; i < exp_out; ++i) {
                    pow *= (seq_t)p;
                }
                result *= pow;
            }
        }
    }

    /* If m > 1 then it is a prime factor with exponent 1; s(1)=1 so it
       contributes nothing to a(n). */
    return result;
}
