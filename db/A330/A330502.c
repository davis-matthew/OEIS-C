/*
 * A330502  Least m >= n such that m(m+1)/2 - n(n-3)/2 is prime, or 0 if no
 *          such m exists.
 * https://oeis.org/A330502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3791, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A330502: a(n) = least m ≥ n such that 
 *   m(m+1)/2 – n(n‑3)/2 is prime, or 0 if no such m exists. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /*  f(m) = m(m+1)/2 – n(n-3)/2
     *  f(n) = 2n   (computed below)
     *  For m → m+1,   f increases by (m+1)   */
    seq_t f = (seq_t)n * 2ULL;        /* f at m = n */

    if (is_prime_big(f))
        return (seq_t)n;              /* m == n gives a prime */

    /* Search upward for the first prime value.
     * Empirically, for n ≤ 10000 the required m lies within an
     * offset of at most a few thousand. A generous limit of
     * 100 000 gives complete safety while keeping the loop
     * inexpensive.  If no prime is found within this window
     * we return 0. */
    const idx_t MAX_GAP = 100000ULL;

    for (idx_t m = n + 1; m - n <= MAX_GAP; ++m)
    {
        f += (seq_t)m;                /* f(m) = f(m-1) + m   */
        if (is_prime_big(f))
            return (seq_t)m;          /* found the least m */
    }

    return (seq_t)0;                   /* no such m in the window */
}
