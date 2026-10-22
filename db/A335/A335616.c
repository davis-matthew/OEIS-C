/*
 * A335616  a(n) is twice the number of partitions of n into consecutive
 *          parts, minus the number of partitions of n into consecutive parts
 *          that contain 1 as a part.
 * https://oeis.org/A335616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A001227(n) - A010054(n) (twice the number of partitions
 *            of n into consecutive parts, minus the number of such partitions
 *            that contain the part 1)
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Count partitions of n into consecutive parts.
   A partition corresponds to a pair (m,k) with m≥1 and k≥1 such that
   n = m*k + m*(m-1)/2  (sum of m consecutive integers starting at k).
   This is equivalent to n - m*(m-1)/2 being positive and divisible by m. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset 1; values below 1 are irrelevant */

    seq_t count   = 0;             /* number of partitions into consecutive parts (A001227) */
    seq_t has_one = 0;             /* 1 if there is a partition containing the part 1 (A010054) */

    /* Iterate over the possible lengths m of the consecutive block. */
    for (idx_t m = 1; ; ++m) {
        /* The smallest sum for length m is 1+2+...+m = m*(m+1)/2. */
        seq_t min_sum = (seq_t)m * (m + 1) / 2;
        if (min_sum > n) break;   /* no further partitions possible */

        seq_t rest = n - (seq_t)m * (m - 1) / 2;   /* n - sum of the first m-1 numbers */
        if (rest % m == 0) {      /* there is a valid starting part k = rest/m */
            count += 1;
            if (rest == m)        /* k == 1  →  partition contains 1 */
                has_one = 1;
        }
    }

    return 2 * count - has_one;
}
