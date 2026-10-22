/*
 * A381268  a(n) = denominator( [(x*y*z*u)^n] 1/sqrt(1 - (x + y + z + u*(y +
 *          z))) ).
 * https://oeis.org/A381268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(6*n - 4*popcount(n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Portable popcount for idx_t (assumed 64‑bit). */
static inline unsigned popcount_idx(idx_t n)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)n);
#else
    unsigned count = 0;
    while (n) {
        n &= n - 1;  /* remove lowest set bit */
        ++count;
    }
    return count;
#endif
}

seq_t a(idx_t n)
{
    unsigned pc = popcount_idx(n);
    idx_t exponent = 6*n - 4*pc;   /* 6*n - 4*popcount(n) */
    return pow2(exponent);
}
