/*
 * A256288  Sum of all the parts in the partitions of 7n into 7 parts.
 * https://oeis.org/A256288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*n*P_{≤7}(7n-7) where P_{≤7}(m) counts partitions of m
 *            into parts of size at most 7 (equivalently, partitions of m into
 *            at most 7 parts). The function computes the value by a simple
 *            dynamic‑programming DP over the partition function, so a(n) can
 *            be evaluated for any n ≥ 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 28, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Argument for the partition table: 7*n-7. */
    idx_t m = 7 * n - 7;

    /* Allocate DP array part[0 … m]. */
    seq_t *part = (seq_t *)malloc((size_t)(m + 1) * sizeof(seq_t));
    if (!part)
        return (seq_t)0;               /* out‑of‑memory fallback */

    /* Initialise */
    for (size_t i = 0; i <= (size_t)m; ++i)
        part[i] = 0;
    part[0] = 1;                       /* one way to partition 0 */

    /* Standard coin‑change DP for parts 1..7. */
    for (idx_t coin = 1; coin <= 7; ++coin) {
        for (idx_t j = coin; j <= m; ++j)
            part[j] += part[j - coin];
    }

    seq_t ways = part[m];
    free(part);

    /* a(n) = 7 * n * ways */
    return (seq_t)7 * (seq_t)n * ways;
}
