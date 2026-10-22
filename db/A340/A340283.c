/*
 * A340283  Number of partitions of n into 3 parts such that the smallest part
 *          is relatively prime to each of the other two parts.
 * https://oeis.org/A340283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n/3⌋} Σ_{i=k..⌊(n-k)/2⌋}
 *            [gcd(k,i)=1]·[gcd(k,n-k-i)=1]
 * Range:     u8 n <= 77, u16 n <= 1169, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where [P] = 1 if the predicate P is true, 0 otherwise. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t count = 0;

    for (idx_t k = 1; k <= n / 3; ++k)
    {
        idx_t max_i = (n - k) / 2;               /* i ≤ j ensures non‑decreasing parts */
        for (idx_t i = k; i <= max_i; ++i)
        {
            idx_t j = n - k - i;                 /* the largest part */

            seq_t g1 = gcd((seq_t)k, (seq_t)i);
            if (g1 != 1)
                continue;

            seq_t g2 = gcd((seq_t)k, (seq_t)j);
            if (g2 != 1)
                continue;

            ++count;                             /* (k,i,j) is a valid partition */
        }
    }

    return count;
}
