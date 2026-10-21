/*
 * A285406  Base-2 logarithm of denominator of Sum_{k=0..n^2-1}
 *          (-1)^k*sqrt(Pi)/(Gamma(1/2-k)*Gamma(1+k)*n).
 * https://oeis.org/A285406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 - (popcount(n^2) + v2(n) + 1)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where popcount counts the 1‐bits of n^2 and v2(n) is the exponent of 2 dividing n */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute n^2 safely in 64‑bit */
    uint64_t n64 = (uint64_t)n;
    uint64_t sq64 = n64 * n64;          /* n^2 fits in 64 bits for n <= 10^4 */

    /* popcount of n^2 */
    unsigned int pop = __builtin_popcountll(sq64);

    /* exponent of 2 dividing n (v2(n)) */
    int tz = 0;
    idx_t tmp = n;
    while ((tmp & ((idx_t)1)) == 0) {
        tz++;
        tmp >>= 1;
    }

    /* 2*n^2 - (pop + tz + 1) */
    uint64_t result64 = 2ULL * sq64 - (uint64_t)(pop + tz + 1);

    return (seq_t)result64;
}
