/*
 * A309796  Sum of the even parts appearing among the second largest parts of
 *          the partitions of n into 4 parts.
 * https://oeis.org/A309796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=1}^{\lfloor n/4\rfloor} \sum_{j=k}^{\lfloor
 *            (n-k)/3 \rfloor} \sum_{i=j}^{\lfloor (n-j-k)/2 \rfloor} i *
 *            ((i-1)\bmod 2)
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 4)        /* partitions of size 4 require n >= 4 */
        return (seq_t)0;

    seq_t total = 0;

    idx_t max_k = n / 4;
    for (idx_t k = 1; k <= max_k; ++k) {
        idx_t max_j = (n - k) / 3;
        for (idx_t j = k; j <= max_j; ++j) {
            idx_t M = (n - j - k) / 2;
            if (M < j)                       /* no feasible second part */
                continue;

            idx_t first_even = (j & 1u) ? j + 1 : j; /* first even >= j */
            if (first_even > M)
                continue;               /* no even numbers in the interval */

            idx_t last_even = (M & 1u) ? M - 1 : M; /* last even <= M */

            idx_t count = (last_even - first_even) / 2 + 1;

            /* sum of even i in [j, M] */
            seq_t sum_i = (seq_t)count *
                          ((seq_t)first_even + (seq_t)last_even) / 2;

            total += sum_i;
        }
    }
    return total;
}
