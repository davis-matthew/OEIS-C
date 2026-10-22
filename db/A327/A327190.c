/*
 * A327190  For any n > 0: consider the different ways to split the binary
 *          representation of 2*n+1 into two nonempty parts, say with value x
 *          and y; a(n) is the least possible value of x * y.
 * https://oeis.org/A327190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min_{1 ≤ i < L} ( (2·n+1) >> (L–i) ) * ( (2·n+1) & (
 *            2^(L–i) – 1 ) ) where L is the bit‑length of (2·n+1)
 * Range:     u8 n <= 129, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* size_t */

#ifdef __SIZEOF_INT128__
typedef unsigned __int128 uint128_t;
#else
typedef unsigned long long uint128_t;
#endif

seq_t a(idx_t n)
{
    if (n == 0)          /* value for the (unused) offset 0 */
        return 0;

    /*  m = 2*n + 1  (always odd, binary length ≥ 2) */
    seq_t m = (seq_t)n * 2 + 1;

    /*  compute the bit length L of m  */
    size_t L = 0;
    seq_t t = m;
    while (t) {
        L++;
        t >>= 1;
    }

    /*  initialise best to the largest possible value  */
    uint128_t best = (uint128_t)(-1);

    /*  examine every possible split  */
    for (size_t i = 1; i < L; ++i) {
        size_t right_len = L - i;                 /* length of the low part */
        seq_t  x = m >> right_len;                /* high part               */
        seq_t  y = m & (((seq_t)1 << right_len) - 1); /* low part                */
        uint128_t prod = (uint128_t)x * (uint128_t)y;
        if (prod < best)
            best = prod;
    }

    return (seq_t)best;   /* best fits into seq_t for all terms that are expected to */
}
