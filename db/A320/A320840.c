/*
 * A320840  Smallest N such that A092391(k) >= n for all k >= N.
 * https://oeis.org/A320840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest N such that A092391(k) = k + popcount(k) >= n
 *            for all k >= N A092391(k) = 2k - v(k!) = k + popcount(k); see
 *            comment in OEIS entry
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* popcount for 64‑bit values */
static inline unsigned int popcount64(idx_t x)
{
#ifdef __GNUC__
    return __builtin_popcountll((unsigned long long)x);
#else
    /* Fallback implementation */
    unsigned int c = 0;
    while (x)
    {
        c += (unsigned int)(x & 1);
        x >>= 1;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* a(0) = 0 */
    idx_t last_bad = 0;          /* last k with f(k) < n */
    /* Scan k from 0 to n-1 inclusive. */
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t f = (seq_t)k + (seq_t)popcount64(k);
        if (f < (seq_t)n)
            last_bad = k;
    }
    return (seq_t)(last_bad + 1);
}
