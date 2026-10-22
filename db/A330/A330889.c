/*
 * A330889  a(n) is the total number of parts in all partitions of n into
 *          consecutive parts that differ by 3.
 * https://oeis.org/A330889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k>=1} k if n - 3*k*(k-1)/2 > 0 and (n - 3*k*(k-1)/2)
 *            % k == 0 (otherwise the term contributes 0). This is the direct
 *            combinatorial interpretation of the OEIS definition
 * Range:     u8 n <= 1278, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* If n == 0, return 0 (not part of the defined offset). */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t total = (seq_t)0;

    /* For each possible number of parts k, check if a partition exists. */
    for (idx_t k = 1; ; ++k)
    {
        /* Compute 3*k*(k-1)/2 safely using seq_t arithmetic. */
        seq_t term = (seq_t)3 * (seq_t)k * (seq_t)(k - 1) / (seq_t)2;

        /* If the base sum >= n, no further k will be valid. */
        if (term >= (seq_t)n)
            break;

        seq_t rem = (seq_t)n - term;

        /* rem > 0 automatically holds; check divisibility by k. */
        if (rem % (seq_t)k == (seq_t)0)
            total += (seq_t)k;
    }

    return total;
}
