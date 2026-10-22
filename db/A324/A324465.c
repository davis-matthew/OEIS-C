/*
 * A324465  Exponent of highest power of 2 that divides A324152(n).
 * https://oeis.org/A324465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*wt(n) - (v2(n+1) + v2(n+2) + v2(n+3))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where wt(n) is the binary weight of n and v2(x) is the 2‑adic valuation. */

#include <stdint.h>

/* Count trailing zeros of a positive seq_t value. */
static inline unsigned __v2(seq_t x)
{
    unsigned cnt = 0;
    /* x will never be zero for this sequence (n+1,n+2,n+3 > 0). */
    while ((x & 1) == 0) {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

/* Sequence A324465. */
seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is defined as 0 in the OEIS entry. */
        return 0;

    /* Binary weight of n.  __builtin_popcountll works on uint64_t. */
    seq_t w = (seq_t)__builtin_popcountll(n);

    /* Sum of 2‑adic valuations of (n+1)(n+2)(n+3). */
    seq_t v  = (seq_t)__v2((seq_t)n + 1)
             + (seq_t)__v2((seq_t)n + 2)
             + (seq_t)__v2((seq_t)n + 3);

    /* a(n) = 3*wt(n) - v. */
    return 3 * w - v;
}
