/*
 * A276805  a(n) = numerator((n^2 + 3*n + 2)/n^3).
 * https://oeis.org/A276805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator((n^2 + 3*n + 2)/n^3)
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count trailing zeros of x (x > 0). */
static idx_t trailing_zeros(idx_t x)
{
    idx_t c = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t k = trailing_zeros(n);                              /* v2(n) */
    idx_t t_even = 1 + trailing_zeros((n >> 1) + 1);           /* v2(even factor) */
    idx_t t = (3 * k < t_even) ? (3 * k) : t_even;            /* gcd exponent */
    seq_t nseq = (seq_t)n;
    seq_t prod = (nseq + 1) * (nseq + 2);                     /* (n+1)(n+2) */
    return prod >> t;                                         /* divide by 2^t */
}
