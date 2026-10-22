/*
 * A372342  Number of noncrossing partitions of [n] that contain exactly one
 *          singleton.
 * https://oeis.org/A372342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{m=1..floor((n+1)/2)} binomial(n,
 *            m-1)*binomial(n-m-1, m-2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special cases: a(0)=0, a(1)=1. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset cases directly. */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t sum = 0;
    idx_t m_max = (n + 1) / 2;          /* floor((n+1)/2) */

    for (idx_t m = 1; m <= m_max; ++m) {
        seq_t t1 = binomial(n, m - 1); /* C(n, m-1) */

        seq_t t2 = 0;
        if (m >= 2) {                  /* binomial for negative k defaults to 0 */
            idx_t k = n - m - 1;       /* remaining elements after choosing a block */
            idx_t r = m - 2;           /* number of additional singletons needed */
            if (r <= k)                /* standard combinatorial check */
                t2 = binomial(k, r);   /* C(k, r) */
        }

        sum += t1 * t2;
    }

    return sum;
}
